#ifndef ENTT_ENTITY_SNAPSHOT_HPP
#define ENTT_ENTITY_SNAPSHOT_HPP

#include <array>
#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include "../config/config.h"
#include "../container/dense_map.hpp"
#include "../core/type_traits.hpp"
#include "entity.hpp"
#include "fwd.hpp"
#include "view.hpp"

namespace entt {

/**
 * @brief Utility class to create snapshots from a registry.
 *
 * A _snapshot_ can be either a dump of the entire registry or a narrower
 * selection of components of interest.<br/>
 * This type can be used in both cases if provided with a correctly configured
 * output archive.
 *
 * @tparam Registry Basic registry type.
 */
template<typename Registry>
class basic_snapshot {
    using traits_type = typename Registry::traits_type;

    template<typename Component, typename Archive, typename It>
    void get(Archive &archive, std::size_t sz, It first, It last) const {
        const auto view = reg->template view<Component>();
        archive(static_cast<typename traits_type::entity_type>(sz));

        for(auto it = first; it != last; ++it) {
            if(view.contains(*it)) {
                std::apply(archive, std::tuple_cat(std::make_tuple(*it), view.get(*it)));
            }
        }
    }

    template<typename... Component, typename Archive, typename It, std::size_t... Index>
    void component(Archive &archive, It first, It last, std::index_sequence<Index...>) const {
        const auto view{std::make_tuple(reg->template view<Component>()...)};
        std::array<std::size_t, sizeof...(Index)> size{};

        for(auto it = first; it != last; ++it) {
            ((std::get<Index>(view).contains(*it) ? ++size[Index] : 0u), ...);
        }

        (get<Component>(archive, size[Index], first, last), ...);
    }

public:
    /*! Basic registry type. */
    using registry_type = Registry;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename registry_type::entity_type;

    /**
     * @brief Constructs an instance that is bound to a given registry.
     * @param source A valid reference to a registry.
     */
    basic_snapshot(const registry_type &source) noexcept
        : reg{&source} {}

    /*! @brief Default move constructor. */
    basic_snapshot(basic_snapshot &&) noexcept = default;

    /*! @brief Default move assignment operator. @return This snapshot. */
    basic_snapshot &operator=(basic_snapshot &&) noexcept = default;

    /**
     * @brief Serializes all identifiers, including those to be recycled.
     * @tparam Archive Type of output archive.
     * @param archive A valid reference to an output archive.
     * @return An object of this type to continue creating the snapshot.
     */
    template<typename Archive>
    const basic_snapshot &entities(Archive &archive) const {
        const auto *storage = reg->template storage<entity_type>();

        archive(static_cast<typename traits_type::entity_type>(storage->size()));
        archive(static_cast<typename traits_type::entity_type>(storage->in_use()));

        for(auto first = storage->data(), last = first + storage->size(); first != last; ++first) {
            archive(*first);
        }

        return *this;
    }

    /**
     * @brief Serializes all required components with associated identifiers.
     * @tparam Component Types of components to serialize.
     * @tparam Archive Type of output archive.
     * @param archive A valid reference to an output archive.
     * @return An object of this type to continue creating the snapshot.
     */
    template<typename... Component, typename Archive>
    const basic_snapshot &component(Archive &archive) const {
        if constexpr(sizeof...(Component) == 1u) {
            const auto view = reg->template view<const Component...>();
            (component<Component>(archive, view.rbegin(), view.rend()), ...);
        } else {
            (component<Component>(archive), ...);
        }

        return *this;
    }

    /**
     * @brief Serializes all required components with associated identifiers for
     * the entities in a range.
     * @tparam Component Types of components to serialize.
     * @tparam Archive Type of output archive.
     * @tparam It Type of input iterator.
     * @param archive A valid reference to an output archive.
     * @param first An iterator to the first element of the range to serialize.
     * @param last An iterator past the last element of the range to serialize.
     * @return An object of this type to continue creating the snapshot.
     */
    template<typename... Component, typename Archive, typename It>
    const basic_snapshot &component(Archive &archive, It first, It last) const {
        component<Component...>(archive, first, last, std::index_sequence_for<Component...>{});
        return *this;
    }

private:
    const registry_type *reg;
};

/**
 * @brief Utility class to restore a snapshot as a whole.
 *
 * A snapshot loader requires that the destination registry be empty and loads
 * all the data at once while keeping intact the identifiers that the entities
 * originally had.<br/>
 * An example of use is the implementation of a save/restore utility.
 *
 * @tparam Registry Basic registry type.
 */
template<typename Registry>
class basic_snapshot_loader {
    using traits_type = typename Registry::traits_type;

    template<typename Component, typename Archive>
    void assign(Archive &archive) {
        auto &storage = reg->template storage<entity_type>();
        auto &elem = reg->template storage<Component>();

        typename traits_type::entity_type length{};
        entity_type entt;

        archive(length);

        if constexpr(std::is_empty_v<Component>) {
            while(length--) {
                archive(entt);
                const auto entity = storage.contains(entt) ? entt : storage.emplace(entt);
                ENTT_ASSERT(entity == entt, "Entity not available for use");
                elem.emplace(entity);
            }
        } else {
            Component instance;

            while(length--) {
                archive(entt, instance);
                const auto entity = storage.contains(entt) ? entt : storage.emplace(entt);
                ENTT_ASSERT(entity == entt, "Entity not available for use");
                elem.emplace(entity, std::move(instance));
            }
        }
    }

public:
    /*! Basic registry type. */
    using registry_type = Registry;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename registry_type::entity_type;

    /**
     * @brief Constructs an instance that is bound to a given registry.
     * @param source A valid reference to a registry.
     */
    basic_snapshot_loader(registry_type &source) noexcept
        : reg{&source} {
        // restoring a snapshot as a whole requires a clean registry
        ENTT_ASSERT(reg->empty(), "Registry must be empty");
    }

    /*! @brief Default move constructor. */
    basic_snapshot_loader(basic_snapshot_loader &&) noexcept = default;

    /*! @brief Default move assignment operator. @return This loader. */
    basic_snapshot_loader &operator=(basic_snapshot_loader &&) noexcept = default;

    /**
     * @brief Restores all identifiers, including those to be recycled.
     * @tparam Archive Type of input archive.
     * @param archive A valid reference to an input archive.
     * @return A valid loader to continue restoring data.
     */
    template<typename Archive>
    basic_snapshot_loader &entities(Archive &archive) {
        auto &storage = reg->template storage<entity_type>();
        typename traits_type::entity_type length{};
        typename traits_type::entity_type in_use{};
        entity_type entity = null;

        archive(length);
        archive(in_use);

        storage.reserve(length);

        for(std::size_t pos{}; pos < length; ++pos) {
            archive(entity);
            storage.emplace(entity);
        }

        storage.in_use(in_use);

        return *this;
    }

    /**
     * @brief Restores all required components with associated identifiers.
     *
     * The template parameter list must be exactly the same used during
     * serialization.
     *
     * @tparam Component Types of components to restore.
     * @tparam Archive Type of input archive.
     * @param archive A valid reference to an input archive.
     * @return A valid loader to continue restoring data.
     */
    template<typename... Component, typename Archive>
    basic_snapshot_loader &component(Archive &archive) {
        (assign<Component>(archive), ...);
        return *this;
    }

    /**
     * @brief Destroys those entities that have no components.
     *
     * In case all the entities were serialized but only part of the components
     * was saved, it could happen that some of the entities have no components
     * once restored.<br/>
     * This function helps to identify and destroy those entities.
     *
     * @return A valid loader to continue restoring data.
     */
    basic_snapshot_loader &orphans() {
        auto &storage = reg->template storage<entity_type>();

        for(auto entt: storage) {
            if(reg->orphan(entt)) {
                storage.erase(entt);
            }
        }

        return *this;
    }

private:
    registry_type *reg;
};

/**
 * @brief Utility class for _continuous loading_.
 *
 * A _continuous loader_ is designed to load data from a source registry to a
 * (possibly) non-empty destination. The loader can accommodate in a registry
 * more than one snapshot in a sort of _continuous loading_ that updates the
 * destination one step at a time.<br/>
 * Identifiers that entities originally had are not transferred to the target.
 * Instead, the loader maps remote identifiers to local ones while restoring a
 * snapshot.<br/>
 * An example of use is the implementation of a client-server application with
 * the requirement of transferring somehow parts of the representation side to
 * side.
 *
 * @tparam Registry Basic registry type.
 */
template<typename Registry>
class basic_continuous_loader {
    using traits_type = typename Registry::traits_type;

    void destroy(typename Registry::entity_type entt) {
        if(const auto it = remloc.find(entt); it == remloc.cend()) {
            const auto local = reg->create();
            remloc.emplace(entt, std::make_pair(local, true));
            reg->destroy(local);
        }
    }

    void restore(typename Registry::entity_type entt) {
        const auto it = remloc.find(entt);

        if(it == remloc.cend()) {
            const auto local = reg->create();
            remloc.emplace(entt, std::make_pair(local, true));
        } else {
            if(!reg->valid(remloc[entt].first)) {
                remloc[entt].first = reg->create();
            }

            // set the dirty flag
            remloc[entt].second = true;
        }
    }

    template<typename Container>
    auto update(int, Container &container) -> decltype(typename Container::mapped_type{}, void()) {
        // map like container
        Container other;

        for(auto &&pair: container) {
            using first_type = std::remove_const_t<typename std::decay_t<decltype(pair)>::first_type>;
            using second_type = typename std::decay_t<decltype(pair)>::second_type;

            if constexpr(std::is_same_v<first_type, entity_type> && std::is_same_v<second_type, entity_type>) {
                other.emplace(map(pair.first), map(pair.second));
            } else if constexpr(std::is_same_v<first_type, entity_type>) {
                other.emplace(map(pair.first), std::move(pair.second));
            } else {
                static_assert(std::is_same_v<second_type, entity_type>, "Neither the key nor the value are of entity type");
                other.emplace(std::move(pair.first), map(pair.second));
            }
        }

        using std::swap;
        swap(container, other);
    }

    template<typename Container>
    auto update(char, Container &container) -> decltype(typename Container::value_type{}, void()) {
        // vector like container
        static_assert(std::is_same_v<typename Container::value_type, entity_type>, "Invalid value type");

        for(auto &&entt: container) {
            entt = map(entt);
        }
    }

    template<typename Component, typename Other, typename Member>
    void update([[maybe_unused]] Component &instance, [[maybe_unused]] Member Other::*member) {
        if constexpr(!std::is_same_v<Component, Other>) {
            return;
        } else if constexpr(std::is_same_v<Member, entity_type>) {
            instance.*member = map(instance.*member);
        } else {
            // maybe a container? let's try...
            update(0, instance.*member);
        }
    }

    template<typename Component>
    void remove_if_exists() {
        auto &storage = reg->template storage<Component>();

        for(auto &&ref: remloc) {
            storage.remove(ref.second.first);
        }
    }

    template<typename Component, typename Archive, typename... Other, typename... Member>
    void assign(Archive &archive, [[maybe_unused]] Member Other::*...member) {
        auto &storage = reg->template storage<Component>();
        typename traits_type::entity_type length{};
        entity_type entt;

        archive(length);

        if constexpr(std::is_empty_v<Component>) {
            while(length--) {
                archive(entt);
                restore(entt);
                storage.emplace(map(entt));
            }
        } else {
            Component instance;

            while(length--) {
                archive(entt, instance);
                (update(instance, member), ...);
                restore(entt);
                storage.emplace(map(entt), std::move(instance));
            }
        }
    }

public:
    /*! Basic registry type. */
    using registry_type = Registry;
    /*! @brief Underlying entity identifier. */
    using entity_type = typename registry_type::entity_type;

    /**
     * @brief Constructs an instance that is bound to a given registry.
     * @param source A valid reference to a registry.
     */
    basic_continuous_loader(registry_type &source) noexcept
        : reg{&source} {}

    /*! @brief Default move constructor. */
    basic_continuous_loader(basic_continuous_loader &&) = default;

    /*! @brief Default move assignment operator. @return This loader. */
    basic_continuous_loader &operator=(basic_continuous_loader &&) = default;

    /**
     * @brief Restores all identifiers, including those to be recycled.
     *
     * It creates local counterparts for remote elements as needed.
     *
     * @tparam Archive Type of input archive.
     * @param archive A valid reference to an input archive.
     * @return A non-const reference to this loader.
     */
    template<typename Archive>
    basic_continuous_loader &entities(Archive &archive) {
        typename traits_type::entity_type length{};
        typename traits_type::entity_type in_use{};

        archive(length);
        archive(in_use);

        entity_type entt{null};
        std::size_t pos{};

        for(; pos < in_use; ++pos) {
            archive(entt);
            restore(entt);
        }

        for(; pos < length; ++pos) {
            archive(entt);
            destroy(entt);
        }

        return *this;
    }

    /**
     * @brief Serializes all required components with associated identifiers.
     *
     * It creates local counterparts for remote elements as needed.<br/>
     * Members are either data members of type entity_type or containers of
     * entities. In both cases, a loader visits them and replaces entities with
     * their local counterpart.
     *
     * @tparam Component Type of component to restore.
     * @tparam Archive Type of input archive.
     * @tparam Other Types of components to update with local counterparts.
     * @tparam Member Types of members to update with their local counterparts.
     * @param archive A valid reference to an input archive.
     * @param member Members to update with their local counterparts.
     * @return A non-const reference to this loader.
     */
    template<typename... Component, typename Archive, typename... Other, typename... Member>
    basic_continuous_loader &component(Archive &archive, Member Other::*...member) {
        (remove_if_exists<Component>(), ...);
        (assign<Component>(archive, member...), ...);
        return *this;
    }

    /**
     * @brief Helps to purge entities that no longer have a conterpart.
     *
     * Users should invoke this member function after restoring each snapshot,
     * unless they know exactly what they are doing.
     *
     * @return A non-const reference to this loader.
     */
    basic_continuous_loader &shrink() {
        const auto &storage = reg->template storage<entity_type>();
        auto it = remloc.begin();

        while(it != remloc.cend()) {
            const auto local = it->second.first;
            bool &dirty = it->second.second;

            if(dirty) {
                dirty = false;
                ++it;
            } else {
                if(storage.contains(local)) {
                    reg->destroy(local);
                }

                it = remloc.erase(it);
            }
        }

        return *this;
    }

    /**
     * @brief Destroys those entities that have no components.
     *
     * In case all the entities were serialized but only part of the components
     * was saved, it could happen that some of the entities have no components
     * once restored.<br/>
     * This function helps to identify and destroy those entities.
     *
     * @return A non-const reference to this loader.
     */
    basic_continuous_loader &orphans() {
        auto &storage = reg->template storage<entity_type>();

        for(auto entt: storage) {
            if(reg->orphan(entt)) {
                storage.erase(entt);
            }
        }

        return *this;
    }

    /**
     * @brief Tests if a loader knows about a given entity.
     * @param entt A valid identifier.
     * @return True if `entity` is managed by the loader, false otherwise.
     */
    [[nodiscard]] bool contains(entity_type entt) const noexcept {
        return (remloc.find(entt) != remloc.cend());
    }

    /**
     * @brief Returns the identifier to which an entity refers.
     * @param entt A valid identifier.
     * @return The local identifier if any, the null entity otherwise.
     */
    [[nodiscard]] entity_type map(entity_type entt) const noexcept {
        if(const auto it = remloc.find(entt); it != remloc.cend()) {
            return it->second.first;
        }

        return null;
    }

private:
    dense_map<entity_type, std::pair<entity_type, bool>> remloc;
    registry_type *reg;
};

} // namespace entt

#endif
