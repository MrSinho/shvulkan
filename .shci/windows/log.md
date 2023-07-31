
# shvulkan

![windows-badge](exit_code.svg)

## [windows build logs:](https://github.com/mrsinho/shci)

```
system        : Windows
version       : 10.0.22621
platform      : Windows-10-10.0.22621-SP0
processor     : AMD64 Family 23 Model 17 Stepping 0, AuthenticAMD
min frequency : 0.0 MHz
max frequency : 3500.0 MHz
cores         : 4
```

build ran for `35.91 s` and terminated with exit code `0`

---

```bash
> .shci/windows/prerequisites-output.txt (
    choco install -y curl mingw cmake
    mingw-get install gcc mingw32-make gdb
)
```

```bash
Chocolatey v2.0.0
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
curl v8.1.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/3 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - curl - curl v8.1.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

```

---
    
```bash
> .shci/windows/build-output.txt (
    mkdir windows
    cd windows
    cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
    cmake --build .
)

```

```bash
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- shvulkan message: found Vulkan
-- shvulkan message: Vulkan_INCLUDE_DIR: D:/lib/VulkanSDK/1.3.250.0/Include
-- shvulkan message: Vulkan_LIBRARY:     D:/lib/VulkanSDK/1.3.250.0/Lib/vulkan-1.lib
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Including Win32 support
-- Configuring done (1.1s)
-- Generating done (1.2s)
-- Build files have been written to: D:/home/desktop/GitHub/shvulkan/windows
MSBuild version 17.6.3+07e294721 for .NET Framework

  glfw.vcxproj -> D:\home\desktop\GitHub\shvulkan\windows\examples\external\glfw\src\Debug\glfw3.lib
  shvulkan.vcxproj -> D:\home\desktop\GitHub\shvulkan\bin\Debug\shvulkan.lib
  shvulkan-clear-color.vcxproj -> D:\home\desktop\GitHub\shvulkan\bin\Debug\shvulkan-clear-color.exe
  shvulkan-compute-power-numbers.vcxproj -> D:\home\desktop\GitHub\shvulkan\bin\Debug\shvulkan-compute-power-numbers.exe
  shvulkan-scene.vcxproj -> D:\home\desktop\GitHub\shvulkan\bin\Debug\shvulkan-scene.exe

```

---
    
build ran for `35.91 s` and terminated with exit code `0`

---

