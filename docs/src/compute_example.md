# Compute Example

![](https://img.shields.io/badge/shvulkan%20|%20SINHO%20SOFTWORKS-3CFADC?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
[![](https://img.shields.io/badge/GitHub_repository-3CFADC?style=for-the-badge&logo=github&logoColor=black)](https://github.com/mrsinho/shvulkan)

[TOC]

The compute example demonstrates the setup and execution of a Vulkan-based compute shader application. The application starts by initializing Vulkan components, including creating an instance, selecting a physical device, and setting up a logical device with a compute queue. A command pool and command buffer are also created to handle command submissions.

## Memory Allocation

The primary objective of this application is to perform computations using a compute shader. The application first prepares memory by setting up staging and device-local buffers. The staging buffer is used for host-visible memory, allowing the CPU to write data to it. This data includes an array of input values and a factor. This buffer is then copied to a device-local buffer, which is optimized for GPU access.

## Compute Pipeline

Next, the compute pipeline is set up. This involves defining descriptor sets and layout bindings, creating and setting up shader modules, and configuring the compute pipeline. The compute shader is responsible for performing operations on the input data. In this example, the shader is designed to square the input values.

## Compute Queue Submission

Once the pipeline is ready, the application records commands into the command buffer. This includes binding the compute pipeline and descriptor sets, and dispatching compute work to the GPU. After recording, the command buffer is submitted to the compute queue, and the application waits for the operations to complete.

## GPU Memory reading and cleanup

After the computation, the application copies the results from the device-local buffer back to the staging buffer. This allows the CPU to access and read the results of the computation. The final step involves reading the output values from the staging buffer and printing them to the console.

The application concludes by cleaning up resources, including destroying Vulkan objects and freeing allocated memory.

## Terminal output

```bash
Compute shader inputs:
0.000000
1.000000
2.000000
3.000000
4.000000
5.000000
6.000000
7.000000
8.000000
9.000000
10.000000
11.000000
12.000000
13.000000
14.000000
15.000000
16.000000
17.000000
18.000000
19.000000
20.000000
21.000000
22.000000
23.000000
24.000000
25.000000
26.000000
27.000000
28.000000
29.000000
30.000000
31.000000
32.000000
33.000000
34.000000
35.000000
36.000000
37.000000
38.000000
39.000000
40.000000
41.000000
42.000000
43.000000
44.000000
45.000000
46.000000
47.000000
48.000000
49.000000
50.000000
51.000000
52.000000
53.000000
54.000000
55.000000
56.000000
57.000000
58.000000
59.000000
60.000000
61.000000
62.000000
63.000000

Squaring the numbers...

Compute shader output values:
0.000000
1.000000
4.000000
9.000000
16.000000
25.000000
36.000000
49.000000
64.000000
81.000000
100.000000
121.000000
144.000000
169.000000
196.000000
225.000000
256.000000
289.000000
324.000000
361.000000
400.000000
441.000000
484.000000
529.000000
576.000000
625.000000
676.000000
729.000000
784.000000
841.000000
900.000000
961.000000
1024.000000
1089.000000
1156.000000
1225.000000
1296.000000
1369.000000
1444.000000
1521.000000
1600.000000
1681.000000
1764.000000
1849.000000
1936.000000
2025.000000
2116.000000
2209.000000
2304.000000
2401.000000
2500.000000
2601.000000
2704.000000
2809.000000
2916.000000
3025.000000
3136.000000
3249.000000
3364.000000
3481.000000
3600.000000
3721.000000
3844.000000
3969.000000
```