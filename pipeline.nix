{ pkgs, ... }:

{
  nativeBuildInputs = [
    pkgs.cmake
    pkgs.ninja
    pkgs.clang
    pkgs.pkg-config
  ];

  buildInputs = [
    pkgs.vulkan-tools
    pkgs.vulkan-loader#.dev
    pkgs.vulkan-helper
    pkgs.vulkan-headers
    pkgs.glfw
    pkgs.kdePackages.wayland
    pkgs.kdePackages.wayland-protocols
    pkgs.wayland-scanner
    pkgs.libxkbcommon.dev
    pkgs.libX11.dev
    pkgs.libXi.dev
    pkgs.libXrandr.dev
    pkgs.libXinerama.dev
    pkgs.libXcursor.dev
    pkgs.libXext.dev
    pkgs.doxygen
  ];

  cmakeFlags = [
    "-Wno-dev"
    "-U'*'"
    "-DSH_VULKAN_BUILD_EXAMPLES=ON"
    "-DSH_VULKAN_BUILD_DOCS=ON"
    
    #"-DVulkan_INCLUDE_DIR=${pkgs.vulkan-headers}/include"
    #"-DVulkan_LIBRARY=${pkgs.vulkan-loader}/lib/libvulkan.so"
    #
    #"-DX11_X11_INCLUDE_PATH=${pkgs.libX11.dev}/include"
    #"-DX11_X11_LIB=${pkgs.libX11}/lib/libX11.so"
    #
    #"-DX11_Xrandr_INCLUDE_PATH=${pkgs.libXrandr.dev}/include"
    #"-DX11_Xrandr_LIB=${pkgs.libXrandr}/lib/libXrandr.so"
    #
    #"-DX11_Xinerama_INCLUDE_PATH=${pkgs.libXinerama.dev}/include"
    #"-DX11_Xinerama_LIB=${pkgs.libXinerama}/lib/libXinerama.so"
    #
    #"-DX11_Xkb_INCLUDE_PATH=${pkgs.libxkbcommon.dev}/include"
    #
    #"-DX11_Xcursor_INCLUDE_PATH=${pkgs.libXcursor.dev}/include"
    #"-DX11_Xcursor_LIB=${pkgs.libXcursor}/lib/libXcursor.so"
    #
    #"-DX11_Xi_INCLUDE_PATH=${pkgs.libXi.dev}/include"
    #"-DX11_Xi_LIB=${pkgs.libXi}/lib/libXi.so"
    #
    #"-DX11_Xshape_INCLUDE_PATH=${pkgs.libxext.dev}/include"

    # PKG_CONFIG_PATH should be set automatically
    #"-DPKG_CONFIG_PATH=${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.xorg.libX11}/lib/pkgconfig:${pkgs.xorg.libXi}/lib/pkgconfig:${pkgs.xorg.libXrandr}/lib/pkgconfig:${pkgs.xorg.libXinerama}/lib/pkgconfig:${pkgs.xorg.libXcursor}/lib/pkgconfig"
  ];

  buildPhase = '' # Starts from build directory
    make -j $NIX_BUILD_CORES
    cd ../docs
    doxygen
  '';

  installPhase = '' # Starts from build directory (equal to $PWD)
    mkdir -p $out/lib
    mkdir -p $out/include
    mkdir -p $out/docs

    cp -r $PWD/../lib      $out/
    cp -r $PWD/../shvulkan/include $out/
    cp -r $PWD/../docs/docs        $out/

    # Copy examples binaries and shaders
    mkdir -p $out/examples/bin
    mkdir -p $out/examples/shaders/bin
    cp -r $PWD/../examples/shaders/bin $out/examples/shaders/
    cp -r $PWD/../bin/examples/*       $out/examples/bin/
  '';
}