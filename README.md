# shvulkan

[![linux_badge](.shci/linux/exit_code.svg)](.shci/linux/log.md)
[![windows_badge](.shci/windows/exit_code.svg)](.shci/windows/log.md)

`shvulkan` is a lightweight and flexible wrapper around the Vulkan API written completely in C, that makes it easier to work with graphics efficiently without writing thousands of lines of code.

![alpha-blend](examples/capture/alpha-blending.png)

---

## [Build status](https://github.com/MrSinho/ShVulkan/tree/main/.shci) 

_See the linux and windows build logs_

[![linux_badge](.shci/linux/exit_code.svg)](.shci/linux/log.md)
[![windows_badge](.shci/windows/exit_code.svg)](.shci/windows/log.md)

The examples are frequently being tested on Windows 11, Linux Mint (virtual machine and pc) with different compilers (MSVC, gcc), and hardware configurations (RX580 4GB GDDR5, Radeon V Carrizo 500MB).

---

# To do
 * texture examples (graphics and plain data)
 * Error/debug/info logging to file
 * read spirv assembly (not sure about this)
 * ray tracing example

---

# Index
* [Clone and build](#clone-and-build)
* [CMake targets](#cmake-targets)
* [Graphics example overview](#graphics-example-overview)
* [Compute example overview](#compute-example-overview)

---

## Clone and Build
Open the terminal and run the following commands:
```bash
git clone --recursive https://github.com/mrsinho/shvulkan.git
cd shvulkan
mkdir build
cd build
cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
cmake --build .
```

If you are having some trouble with building the targets and compiling the code, check the [Linux](https://github.com/MrSinho/shvulkan/blob/main/.shci/linux-log.md) and [Windows](https://github.com/MrSinho/shvulkan/blob/main/.shci/windows-log.md) build [logs](https://github.com/MrSinho/shvulkan/blob/main/.shci).

[top](#shvulkan)

---

## CMake targets

| CMake target                                                | type       |
|-------------------------------------------------------------|------------|
| [shvulkan](../ShVulkan/index)                       	      | library    |
| [shvulkan-clear-color](examples/src/graphics/clear-color.c) | executable |
| [shvulkan-scene](#graphics-example-overview)                | executable |
| [shvulkan-compute-example](#compute-example-overview)       | executable |

If the cmake option `SH_VULKAN_BUILD_EXAMPLES` is enabled, the additional [`glfw`](https://github.com/glfw/glfw) target will be generated as a static library.

[top](#shvulkan)

---

## [Graphics example overview](examples/src/graphics/scene.c)

This example demonstrates how to setup a vulkan environment, allocate, write and bind memory to the GPU, enable multisampled anti aliasing, write a vertex and a fragment shader, build a graphics pipeline, bind inputs with a command buffer, query draw calls (with indexing and without indexing, with instancing and without instancing) to a graphics queue and synchronize data between host and graphics card.

The first draw call will require some quad vertices, which are going to be combined with two `per instance` model matrices located on an instance buffer, which was bound with and offset of 0 bytes, and produce different vertex positions (see `gl_PerVertex_var.gl_Position`):

quad input vertex positions:
![](examples/capture/quads%20in.PNG)
1st quad instance input matrix and output fragment positions:
![](examples/capture/1st%20quad%20inout.PNG)
![](examples/capture/1st%20quad%20out.PNG)
2nd quad instance input matrix and output fragment position:
![](examples/capture/2nd%20quad%20inout.PNG)
![](examples/capture/2nd%20quad%20out.PNG)

Through the renderpass, the first draw call will produce the following framebuffers on the swapchain (the fragment color is calculated by determining the distance between the fragment position and the light source, which properties were defined on the descriptor set 0):
![](examples/capture/1st%20draw%20call.PNG)

The second draw call requires three triangle vertices, which are going to be combined with the last raw matrix on the instance buffer, that was bound with an offset of 128 bytes, and return the following vertex positions (see `gl_PerVertex_var.gl_Position`):

triangle input vertex positions: 
![](examples/capture/triangle%20in.PNG)
triangle input matrix and output fragment positions:
![](examples/capture/triangle%20inout.PNG)
![](examples/capture/triangle%20out.PNG)

Through the renderpass, the second draw call will produce the following framebuffers on the swapchain (as you can see, all the fragments are located at the same `z` distance from the origin):

![](examples/capture/2nd%20draw%20call.PNG)

### What could the graphics example be improved with

* textures
* read framebuffer attachments memory
* offline rendering
* raytracing using compute shader
* update graphics examples maps for a quick look 

[top](#shvulkan)

---

## [Compute example overview](examples/src/compute/power-numbers.c)

The compute examples shows how setup a vulkan environment, allocate, write and bind memory to the GPU, setup a compute shader, write a compute shader and build a compute pipeline (the program takes 64 decimal numbers as inputs and square their values), bind and dispatch inputs to compute workgroups, manage parallel invocations, submit to a compute queue, synchronize data between host and graphics card and read the output values. The following diagram briefly shows what happens in this example program:

[![](examples/diagrams/shvulkan%20compute.drawio.svg)](examples/diagrams/shvulkan%20compute.drawio.svg)

[top](#shvulkan)
 
---
