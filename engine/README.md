# octo

## Install Dependencies:
- Arch Linux:
    - `sudo pacman -Sy vulkan-devel glfw-x11 glm shaderc`
- MacOS:
    - TBD
- Windows:
    - TBD

## Building:
- Linux:
    - Initialize cmake build dir: `cmake -B build -S .`
    - Build: `cmake --build build -j$(nproc)`
