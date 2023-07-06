
# shvulkan

![linux-badge](exit_code.svg)

## [linux build logs:](https://github.com/mrsinho/shci)

```
system        : Linux
version       : #1 SMP Fri Jan 27 02:56:13 UTC 2023
platform      : Linux-5.15.90.1-microsoft-standard-WSL2-x86_64-with-glibc2.31
processor     : 
min frequency : 0.0 MHz
max frequency : 0.0 MHz
cores         : 4
```

build ran for `88.92 s` and terminated with exit code `0`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y libvulkan-dev libvulkan1
    sudo apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev
    sudo apt install -y libwayland-dev
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt

```

```bash
Hit:1 http://deb.debian.org/debian bullseye InRelease
Get:2 http://deb.debian.org/debian bullseye-updates InRelease [44.1 kB]
Get:3 http://security.debian.org/debian-security bullseye-security InRelease [48.4 kB]
Get:4 http://ftp.debian.org/debian bullseye-backports InRelease [49.0 kB]
Get:5 http://security.debian.org/debian-security bullseye-security/main amd64 Packages [252 kB]
Fetched 393 kB in 4s (94.6 kB/s)
Reading package lists...
Building dependency tree...
Reading state information...
All packages are up to date.
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan-dev is already the newest version (1.2.162.0-1).
libvulkan1 is already the newest version (1.2.162.0-1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libxcursor-dev is already the newest version (1:1.2.0-2).
libxinerama-dev is already the newest version (2:1.1.4-2).
libxrandr-dev is already the newest version (2:1.5.1-1).
libx11-dev is already the newest version (2:1.7.2-1+deb11u1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libwayland-dev is already the newest version (1.18.0-2~exp1.1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
libc6-dev is already the newest version (2.31-13+deb11u6).
make is already the newest version (4.3-4.1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.

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
-- Including X11 support
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/GitHub/shvulkan/linux
Scanning dependencies of target shvulkan
[  3%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVulkan.c.o
[  6%] Linking C static library ../bin/libshvulkan.a
[  6%] Built target shvulkan
[ 81%] Built target glfw
Scanning dependencies of target shvulkan-scene
[ 84%] Building C object CMakeFiles/shvulkan-scene.dir/examples/src/graphics/scene.c.o
[ 87%] Linking C executable ../bin/shvulkan-scene
[ 87%] Built target shvulkan-scene
Scanning dependencies of target shvulkan-clear-color
[ 90%] Building C object CMakeFiles/shvulkan-clear-color.dir/examples/src/graphics/clear-color.c.o
[ 93%] Linking C executable ../bin/shvulkan-clear-color
[ 93%] Built target shvulkan-clear-color
Scanning dependencies of target shvulkan-compute-power-numbers
[ 96%] Building C object CMakeFiles/shvulkan-compute-power-numbers.dir/examples/src/compute/power-numbers.c.o
[100%] Linking C executable ../bin/shvulkan-compute-power-numbers
[100%] Built target shvulkan-compute-power-numbers

```

---
    
build ran for `88.92 s` and terminated with exit code `0`

---

