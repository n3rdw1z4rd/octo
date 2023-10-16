# octo

## Install Dependencies:
- Arch Linux:
    - `sudo pacman -Sy vulkan-devel glfw-x11 glm shaderc`
- MacOS:
    - `brew install glfw3`
- Windows:
    - TBD

## Building:
These steps work for both `engine` and `test`, be sure to `cd` into each directory separately to run these commands:
- Initialize cmake build dir: `cmake -B build -S .`
- Build: `cmake --build build -j$(nproc)`
- Build macOS: `cmake --build build -j$(sysctl -n hw.ncpu)`
