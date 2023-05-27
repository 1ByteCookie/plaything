# Entt plaything

Decided to check out Entt to learn more about component system architecture
and data oriented programming rather than object oriented which turned out to be the foundations of a lot game
engines.


> The movement of the player quad is a bit clunky. I tried to figure what was causing
the problem, and I thought maybe it was because of how SDL
was rendering the rectangles. When player movement is calculated, it factors
speed, and direction, so when it's moving too fast, the change in the X or Y
coordinates can be a little too big that it looks like it's skipping pixels.
Making a custom rendering engine might fix the issue? Or maybe i just suck at things.



###### Requires python installed because I didn't want to use batch to automate build scripts
