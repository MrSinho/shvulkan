# shvulkan
![linux_badge](linux-status.svg)
## [Linux/Unix build logs:](https://github.com/MrSinho/ShCI)
  `Build ran for 8.18s`
```bash $shci call$
apt install -y wget gcc cmake make gdb
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.16.3-1ubuntu1).
gcc is already the newest version (4:9.3.0-1ubuntu2).
make is already the newest version (4.2.1-1.2).
gdb is already the newest version (9.2-0ubuntu1~20.04.1).
wget is already the newest version (1.20.3-1ubuntu2).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.

```

```bash $shci call$
sudo apt update -y

sudo add-apt-repository -y ppa:oibaf/graphics-drivers
sudo apt update -y
sudo apt install -y libvulkan-dev 
sudo apt install -y libvulkan1 
sudo apt install -y mesa-vulkan-drivers 
sudo apt install -y vulkan-utils


Hit:1 http://packages.microsoft.com/repos/code stable InRelease
Hit:2 http://security.ubuntu.com/ubuntu focal-security InRelease
Hit:3 http://archive.canonical.com/ubuntu focal InRelease
Hit:4 https://packages.microsoft.com/repos/edge stable InRelease
Get:5 http://ppa.launchpad.net/obsproject/obs-studio/ubuntu focal InRelease [18,1 kB]
Hit:6 http://archive.ubuntu.com/ubuntu focal InRelease
Ign:7 http://packages.linuxmint.com uma InRelease
Get:8 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease [23,9 kB]
Hit:9 http://archive.ubuntu.com/ubuntu focal-updates InRelease
Hit:10 http://packages.linuxmint.com uma Release
Hit:11 http://archive.ubuntu.com/ubuntu focal-backports InRelease
Err:5 http://ppa.launchpad.net/obsproject/obs-studio/ubuntu focal InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY EFC71127F425E228
Err:8 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 957D2708A03A4626
Reading package lists...
Executing: /tmp/apt-key-gpghome.WSVFaWDgT3/gpg.1.sh --keyserver hkps://keyserver.ubuntu.com:443 --recv-keys 5ABCE68FF4633EA42E219156957D2708A03A4626
You are about to add the following PPA:
 PLEASE READ: don't email me to report bugs, unless you are sure it's a packaging bug. Not only is email not a good tool for tracking bugs, it also excludes anybody else from tracking or working on the issue. Please read the section "Debugging and reporting problems" below.

Also, please don't ask me to include non-free drivers, I won't do it.

Patches and suggestions are welcomed.

=============

All Ubuntu architectures are supported.

Supported Ubuntu versions:
- 20.04 (focal) <- obsolete
- 21.10 (impish) <- obsolete
- 22.04 (jammy) <- supported
- 22.10 (kinetic) <- supported

For forum support see: http://goo.gl/qoUpWK

=== Introduction ===
This PPA provides updated free graphics drivers (from mesa). Updates packages provide:
 * all driver packages are automagically built every day, when there are upstream changes
 * Vulkan 1.3+ and OpenGL 4.6+ : http://mesamatrix.net
 * gallium-nine support. Read the specific section below
 * VDPAU and VAAPI Gallium3D accelerated video drivers (see below)
 * packages built against latest stable LLVM
 * mesa packages built with gcc LTO for smaller binary size (~10-15%) and improved performance

As an added bonus I may include some updated open source games (backported from Debian or newer Ubuntu).

=== Basic usage ===
See "Adding this PPA to your system" later on, or just do:
sudo add-apt-repository ppa:oibaf/graphics-drivers
and update your Ubuntu with the package here.

=== Using gallium-nine ===
Gallium-nine lets you run DirectX 9 games in a faster way, avoiding the inefficient translation of D3D9 calls to OpenGL. For more info see https://wiki.ixit.cz/d3d9
You need:
 * enable this PPA (see previous paragraph)
 * then follow the instructions to install wine from this PPA: https://launchpad.net/~commendsarnex/+archive/ubuntu/winedri3

=== Using alternative drivers - Here be dragons ===
* zink - OpenGL emulated on a Vulkan driver:
  Set MESA_LOADER_DRIVER_OVERRIDE=zink
* zink + lavapipe - OpenGL emulated on a software Vulkan driver:
  Set VK_ICD_FILENAMES=/usr/share/vulkan/icd.d/lvp_icd.x86_64.json MESA_LOADER_DRIVER_OVERRIDE=zink
* llvmpipe OpenGL software render:
  Set MESA_LOADER_DRIVER_OVERRIDE=llvmpipe
* Other options here: https://www.mesa3d.org/envvars.html

=== Using accelerated video ===
Supported VDPAU drivers: r300, r600, radeonsi, nouveau
Supported VAAPI drivers: r600, radeonsi

* First you need to install mesa-vdpau-drivers package:
sudo apt-get install mesa-vdpau-drivers

* Then to test the VDPAU driver with mpv use:
$ mpv --hwdec=vdpau yourvideofile

=== Upgrading to a newer Ubuntu ===
It is *strongly* suggested to remove all packages from this PPA before updating to a newer Ubuntu release. See the section "Revert to original drivers" later on.
Then, after the upgrade, you can add again this PPA.

=== Debugging and reporting problems ===
If you have some problem with the drivers try running the application from the command line and see if there are graphics related warnings. Also type dmesg to see if there are other related information here.

If you get crashes install the relevant -dbg pacakges (libgl1-mesa-dri-dbg or libgl1-mesa-dri-experimental-dbg for 3D drivers and xserver-xorg-video-intel-dbg , xserver-xorg-video-nouveau-dbg or xserver-xorg-video-radeon-dbg for X drivers) and reproduce the crash with gdb. Example:
$ gdb glxgears
[...make your application crash and then get a backtrace with:]
(gdb) bt

If X.org crashes look at /var/log/Xorg.0.log (or /var/log/Xorg.0.log.old when a new X session is started after the crash).

With the gathered information try searching at https://bugs.freedesktop.org/query.cgi (also try with google) if someone already reported a similar bug. If not you may want to report a new bug:
for 3D drivers bugs: https://www.mesa3d.org/bugs.html
for X / 2D driver bugs: https://bugs.freedesktop.org/enter_bug.cgi?product=xorg

=== Revert to original drivers ===
To revert to standard Ubuntu drivers type the following in a prompt shell:
$ sudo apt-get install ppa-purge
$ sudo ppa-purge ppa:oibaf/graphics-drivers

=== Donations ===
Some people asked me if I can accept donations to support this work. I don't, but if you have some spare money I would be happy if you could do a donation to a charity of your choice (for the poors, animals, whatever else you may think it might need it). Then feel free to send me a note about it!
 More info: https://launchpad.net/~oibaf/+archive/ubuntu/graphics-drivers
Hit:1 http://packages.microsoft.com/repos/code stable InRelease
Hit:2 http://archive.ubuntu.com/ubuntu focal InRelease
Hit:3 http://archive.canonical.com/ubuntu focal InRelease
Hit:4 http://archive.ubuntu.com/ubuntu focal-updates InRelease
Hit:5 http://security.ubuntu.com/ubuntu focal-security InRelease
Hit:6 http://archive.ubuntu.com/ubuntu focal-backports InRelease
Hit:7 https://packages.microsoft.com/repos/edge stable InRelease
Get:8 http://ppa.launchpad.net/obsproject/obs-studio/ubuntu focal InRelease [18,1 kB]
Ign:9 http://packages.linuxmint.com uma InRelease
Get:10 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease [23,9 kB]
Hit:11 http://packages.linuxmint.com uma Release
Err:8 http://ppa.launchpad.net/obsproject/obs-studio/ubuntu focal InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY EFC71127F425E228
Err:10 http://ppa.launchpad.net/oibaf/graphics-drivers/ubuntu focal InRelease
  The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 957D2708A03A4626
Reading package lists...
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan-dev is already the newest version (1.2.131.2-1).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
libvulkan1 is already the newest version (1.2.131.2-1).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
mesa-vulkan-drivers is already the newest version (21.2.6-0ubuntu0.1~20.04.2).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.
Reading package lists...
Building dependency tree...
Reading state information...
vulkan-utils is already the newest version (1.2.131.1+dfsg1-1).
0 upgraded, 0 newly installed, 0 to remove and 419 not upgraded.

```

```bash $shci call$
cd _shvulkan && mkdir build && cd build && cmake -DSH_VULKAN_BUILD_EXAMPLES=ON .. && make
-- The C compiler identification is GNU 9.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Found Vulkan: /usr/lib/x86_64-linux-gnu/libvulkan.so  
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Found Doxygen: /usr/bin/doxygen (found version "1.8.17") found components: doxygen 
-- Including X11 support
-- Found X11: /usr/include   
-- Looking for XOpenDisplay in /usr/lib/x86_64-linux-gnu/libX11.so;/usr/lib/x86_64-linux-gnu/libXext.so
-- Looking for XOpenDisplay in /usr/lib/x86_64-linux-gnu/libX11.so;/usr/lib/x86_64-linux-gnu/libXext.so - found
-- Looking for gethostbyname
-- Looking for gethostbyname - found
-- Looking for connect
-- Looking for connect - found
-- Looking for remove
-- Looking for remove - found
-- Looking for shmat
-- Looking for shmat - found
-- Configuring done
-- Generating done
-- Build files have been written to: /home/oem/Desktop/shci-implementation/bin/_shvulkan/build
Scanning dependencies of target glfw
[  2%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/context.c.o
[  5%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/init.c.o
[  8%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/input.c.o
[ 11%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/monitor.c.o
[ 14%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/platform.c.o
[ 17%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/vulkan.c.o
[ 20%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/window.c.o
[ 23%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/egl_context.c.o
[ 26%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/osmesa_context.c.o
[ 29%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_init.c.o
[ 32%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_monitor.c.o
[ 35%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_window.c.o
[ 38%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_joystick.c.o
[ 41%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/posix_module.c.o
[ 44%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/posix_time.c.o
[ 47%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/posix_thread.c.o
[ 50%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/x11_init.c.o
[ 52%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/x11_monitor.c.o
[ 55%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/x11_window.c.o
[ 58%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/xkb_unicode.c.o
[ 61%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/glx_context.c.o
[ 64%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/linux_joystick.c.o
[ 67%] Linking C static library libglfw3.a
[ 67%] Built target glfw
Scanning dependencies of target shvulkan
[ 70%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkCore.c.o
[ 73%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkMemoryInfo.c.o
[ 76%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkPipelineData.c.o
[ 79%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkCheck.c.o
[ 82%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkDrawLoop.c.o
[ 85%] Linking C static library ../bin/libshvulkan.a
[ 85%] Built target shvulkan
Scanning dependencies of target shvulkan-compute-example
[ 88%] Building C object CMakeFiles/shvulkan-compute-example.dir/examples/src/computeExample.c.o
[ 91%] Linking C executable ../bin/shvulkan-compute-example
[ 91%] Built target shvulkan-compute-example
Scanning dependencies of target shvulkan-graphics-example
[ 94%] Building C object CMakeFiles/shvulkan-graphics-example.dir/examples/src/graphicsExample.c.o
[ 97%] Linking C executable ../bin/shvulkan-graphics-example
[ 97%] Built target shvulkan-graphics-example
Scanning dependencies of target docs
[100%] Generating HTML documentation
[100%] Built target docs
