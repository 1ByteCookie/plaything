
import sys
import shutil

SolutionDir = sys.argv[1]
OutDir = sys.argv[2]

shutil.copy(SolutionDir + "Dependencies\SDL2\lib\SDL2.dll", OutDir)
shutil.copy(SolutionDir + "Dependencies\SDL2\lib\SDL2_image.dll", OutDir)
