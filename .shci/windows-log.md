# shvulkan
![windows_badge](windows-status.svg)
## [Windows build logs:](https://github.com/MrSinho/ShCI)
  `Build ran for 23.00s`
```bash $shci call$
choco install -y curl mingw cmake
Chocolatey v0.11.2
Chocolatey detected you are not running from an elevated command shell
 (cmd/powershell).

 You may experience errors - many functions/packages
 require admin rights. Only advanced users should run choco w/out an
 elevated shell. When you open the command shell, you should ensure
 that you do so with "Run as Administrator" selected. If you are
 attempting to use Chocolatey in a non-administrator setting, you
 must select a different location other than the default install
 location. See
 https://docs.chocolatey.org/en-us/choco/setup#non-administrative-install
 for details.

For the question below, you have 20 seconds to make a selection.

 Do you want to continue?([Y]es/[N]o): 
Timeout or your choice of '' is not a valid selection.
Installing the following packages:
curl;mingw;cmake
By installing, you accept licenses for the packages.
curl v7.80.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
mingw v11.2.0.07112021 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.22.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/3 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.22.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - curl - curl v7.80.0 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v11.2.0.07112021 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

```

```bash $shci call$
mingw-get install gcc mingw32-make gdb

```

```bash $shci call$
cd _shvulkan && mkdir build && cd build && cmake -G"MinGW Makefiles" -DSH_VULKAN_BUILD_EXAMPLES=ON .. && mingw32-make
-- The C compiler identification is GNU 6.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/MinGW/bin/gcc.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Found Vulkan: C:/VulkanSDK/1.2.198.1/Lib32/vulkan-1.lib  
-- Looking for pthread.h
-- Looking for pthread.h - not found
-- Found Threads: TRUE  
-- Found Doxygen: C:/ProgramData/chocolatey/bin/doxygen.exe (found version "1.9.1") found components: doxygen 
-- Including Win32 support
-- Looking for dinput.h
-- Looking for dinput.h - not found
-- Looking for xinput.h
-- Looking for xinput.h - not found
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/sino3/Desktop/GitHub/ShCI-implementation/build/_shvulkan/build
[  3%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkCore.c.obj
[  6%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkMemoryInfo.c.obj
[  9%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkPipelineData.c.obj
[ 12%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkCheck.c.obj
[ 15%] Building C object CMakeFiles/shvulkan.dir/shvulkan/src/shvulkan/shVkDrawLoop.c.obj
[ 18%] Linking C static library ..\bin\libshvulkan.a
[ 18%] Built target shvulkan
[ 21%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/context.c.obj
[ 24%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/init.c.obj
[ 27%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/input.c.obj
[ 30%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/monitor.c.obj
[ 33%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/platform.c.obj
[ 36%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/vulkan.c.obj
[ 39%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/window.c.obj
[ 42%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/egl_context.c.obj
[ 45%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/osmesa_context.c.obj
[ 48%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_init.c.obj
[ 51%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_monitor.c.obj
[ 54%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_window.c.obj
[ 57%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/null_joystick.c.obj
[ 60%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/win32_module.c.obj
[ 63%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/win32_time.c.obj
[ 66%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/win32_thread.c.obj
[ 69%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/win32_init.c.obj
[ 72%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/win32_joystick.c.obj
[ 75%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/win32_monitor.c.obj
[ 78%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/win32_window.c.obj
[ 81%] Building C object examples/external/glfw/src/CMakeFiles/glfw.dir/wgl_context.c.obj
[ 84%] Linking C static library libglfw3.a
[ 84%] Built target glfw
[ 87%] Building C object CMakeFiles/shvulkan-graphics-example.dir/examples/src/graphicsExample.c.obj
[ 90%] Linking C executable ..\bin\shvulkan-graphics-example.exe
[ 90%] Built target shvulkan-graphics-example
[ 93%] Building C object CMakeFiles/shvulkan-compute-example.dir/examples/src/computeExample.c.obj
[ 96%] Linking C executable ..\bin\shvulkan-compute-example.exe
[ 96%] Built target shvulkan-compute-example
[100%] Generating HTML documentation
[100%] Built target docs
