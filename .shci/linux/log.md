
# Linux build logs

![](linux-exit-code.svg)

```
system        : Linux
version       : #1 SMP Fri Mar 29 23:14:13 UTC 2024
platform      : Linux-5.15.153.1-microsoft-standard-WSL2-x86_64-with-glibc2.35
processor     : x86_64
min frequency : 0.0 MHz
max frequency : 0.0 MHz
cores         : 4
```

build ran for `85.29 s` and terminated with exit code `0`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y pkg-config libxkbcommon-dev
    sudo apt install -y libvulkan-dev libvulkan1
    sudo apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
    sudo apt install -y libwayland-dev
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt

```

```bash
Hit:1 http://archive.ubuntu.com/ubuntu jammy InRelease
Hit:2 http://archive.ubuntu.com/ubuntu jammy-updates InRelease
Hit:3 http://archive.ubuntu.com/ubuntu jammy-backports InRelease
Hit:4 http://security.ubuntu.com/ubuntu jammy-security InRelease
Reading package lists...
Building dependency tree...
Reading state information...
4 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists...
Building dependency tree...
Reading state information...
libxkbcommon-dev is already the newest version (1.4.0-1).
pkg-config is already the newest version (0.29.2-1ubuntu3).
0 upgraded, 0 newly installed, 0 to remove and 4 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan-dev is already the newest version (1.3.204.1-2).
libvulkan1 is already the newest version (1.3.204.1-2).
0 upgraded, 0 newly installed, 0 to remove and 4 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxcursor-dev is already the newest version (1:1.2.0-2build4).
libxi-dev is already the newest version (2:1.8-1build1).
libxinerama-dev is already the newest version (2:1.1.4-3).
libxrandr-dev is already the newest version (2:1.5.2-1build1).
libx11-dev is already the newest version (2:1.7.5-1ubuntu0.3).
0 upgraded, 0 newly installed, 0 to remove and 4 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libwayland-dev is already the newest version (1.20.0-1ubuntu0.1).
0 upgraded, 0 newly installed, 0 to remove and 4 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
gcc is already the newest version (4:11.2.0-1ubuntu1).
make is already the newest version (4.3-4.1build1).
cmake is already the newest version (3.22.1-1ubuntu1.22.04.2).
libc6-dev is already the newest version (2.35-0ubuntu3.8).
0 upgraded, 0 newly installed, 0 to remove and 4 not upgraded.

```

---
    
```bash
function BUILD {
	mkdir linux
    cd linux
    cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
    cmake --build .
}

BUILD > .shci/linux/build-output.txt

```

```bash
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: /usr/include
-- shvulkan message: Vulkan_LIBRARY:     /usr/lib/x86_64-linux-gnu/libvulkan.so
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including Wayland support
-- Including X11 support
-- Checking for modules 'wayland-client>=0.2.7;wayland-cursor>=0.2.7;wayland-egl>=0.2.7;xkbcommon>=0.5.0'
--   Found wayland-client, version 1.20.0
--   Found wayland-cursor, version 1.20.0
--   Found wayland-egl, version 18.1.0
--   Found xkbcommon, version 1.4.0
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Documentation generation requires Doxygen 1.9.8 or later
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/c/_home/desktop/github/shvulkan/linux
[  1%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVulkan.c.o
[  3%] Linking C static library ../bin/linux/libshvulkan.a
[  3%] Built target shvulkan
[  5%] Building C object CMakeFiles/shvulkan-compute-power-numbers.dir/examples/src/compute/power-numbers.c.o
[  7%] Linking C executable ../bin/linux/shvulkan-compute-power-numbers
[  7%] Built target shvulkan-compute-power-numbers
[  9%] Generating xdg-shell-client-protocol.h
[ 11%] Generating fractional-scale-v1-client-protocol-code.h
[ 13%] Generating fractional-scale-v1-client-protocol.h
[ 15%] Generating idle-inhibit-unstable-v1-client-protocol-code.h
[ 16%] Generating idle-inhibit-unstable-v1-client-protocol.h
[ 18%] Generating pointer-constraints-unstable-v1-client-protocol-code.h
[ 20%] Generating pointer-constraints-unstable-v1-client-protocol.h
[ 22%] Generating relative-pointer-unstable-v1-client-protocol-code.h
[ 24%] Generating relative-pointer-unstable-v1-client-protocol.h
[ 26%] Generating viewporter-client-protocol-code.h
[ 28%] Generating viewporter-client-protocol.h
[ 30%] Generating wayland-client-protocol-code.h
[ 32%] Generating wayland-client-protocol.h
[ 33%] Generating xdg-activation-v1-client-protocol-code.h
[ 35%] Generating xdg-activation-v1-client-protocol.h
[ 37%] Generating xdg-decoration-unstable-v1-client-protocol-code.h
[ 39%] Generating xdg-decoration-unstable-v1-client-protocol.h
[ 41%] Generating xdg-shell-client-protocol-code.h
[ 43%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/context.c.o
[ 45%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/init.c.o
[ 47%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/input.c.o
[ 49%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/monitor.c.o
[ 50%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/platform.c.o
[ 52%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/vulkan.c.o
[ 54%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/window.c.o
[ 56%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/egl_context.c.o
[ 58%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/osmesa_context.c.o
[ 60%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_init.c.o
[ 62%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_monitor.c.o
[ 64%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_window.c.o
[ 66%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_joystick.c.o
[ 67%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/posix_module.c.o
[ 69%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/posix_time.c.o
[ 71%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/posix_thread.c.o
[ 73%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/x11_init.c.o
[ 75%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/x11_monitor.c.o
[ 77%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/x11_window.c.o
[ 79%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/xkb_unicode.c.o
[ 81%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/glx_context.c.o
[ 83%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/wl_init.c.o
[ 84%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/wl_monitor.c.o
[ 86%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/wl_window.c.o
[ 88%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/linux_joystick.c.o
[ 90%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/posix_poll.c.o
[ 92%] Linking C static library libglfw3.a
[ 92%] Built target glfw
[ 94%] Building C object CMakeFiles/shvulkan-clear-color.dir/examples/src/graphics/clear-color.c.o
[ 96%] Linking C executable ../bin/linux/shvulkan-clear-color
[ 96%] Built target shvulkan-clear-color
[ 98%] Building C object CMakeFiles/shvulkan-scene.dir/examples/src/graphics/scene.c.o
[100%] Linking C executable ../bin/linux/shvulkan-scene
[100%] Built target shvulkan-scene

```

---
    
build ran for `85.29 s` and terminated with exit code `0`

---

