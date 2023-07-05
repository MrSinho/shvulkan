function BUILD {
	mkdir build
    cd build
    cmake -DSH_VULKAN_BUILD_EXAMPLES=ON ..
    cmake --build .
}

BUILD > .shci/linux/build-output.txt
