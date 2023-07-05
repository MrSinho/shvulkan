function BUILD {
	mkdir linux
    cd linux
    cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
    cmake --build .
}

BUILD > .shci/linux/build-output.txt
