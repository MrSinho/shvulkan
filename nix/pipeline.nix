{ pkgs, ... }:

let
  buildInputs = [
    pkgs.vulkan-tools
    pkgs.vulkan-loader
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
in
{
  nativeBuildInputs = [
    pkgs.cmake
    pkgs.ninja
    pkgs.clang
    pkgs.pkg-config
  ];

  buildInputs = [
    pkgs.vulkan-tools
    pkgs.vulkan-loader
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
    "-DSH_GLFW_CMAKE_CONFIG_PATH=${pkgs.glfw}/lib/cmake/glfw3/glfw3Config.cmake"
    
    # If dev outputs have been set correctly, no need to pass manually these libraries and headers

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
    #"-DPKG_CONFIG_PATH=${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.libX11}/lib/pkgconfig:${pkgs.libXi}/lib/pkgconfig:${pkgs.libXrandr}/lib/pkgconfig:${pkgs.libXinerama}/lib/pkgconfig:${pkgs.libXcursor}/lib/pkgconfig"
  ];

  buildPhase = '' # Starts from build directory
    make -j $NIX_BUILD_CORES
    cd ../docs
    doxygen
  '';

  installPhase = '' # Starts from build directory (equal to $PWD)
    # Copy binaries
    cp -r $PWD/../lib $out/lib
    cp -r $PWD/../bin $out/bin

    # Copy source code
    cp -r $PWD/../shvulkan $out/shvulkan
    cp -r $PWD/../examples $out/examples

    # Copy docs
    cp -r $PWD/../docs $out/
  '';

  # Shouldn't be needed anymore if the dev outputs have been set correctly
  LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath (buildInputs); # Allows linking to shared libraries (e.g. through dlopen)

  # Shouldn't be needed anymore if the dev outputs have been set correctly
  environmentSetup = ''
    export Vulkan_INCLUDE_DIR=${pkgs.vulkan-headers}/include
    export Vulkan_LIBRARY=${pkgs.vulkan-loader}/lib/libvulkan.so

    export X11_X11_LIB=${pkgs.libX11}/lib/libX11.so
    export X11_Xrandr_LIB=${pkgs.libXrandr}/lib/libXrandr.so
    export X11_Xinerama_LIB=${pkgs.libXinerama}/lib/libXinerama.so
    export X11_Xkb_LIB=${pkgs.libxkbcommon}/lib/libxkbcommon.so
    export X11_Xcursor_LIB=${pkgs.libXcursor}/lib/libXcursor.so
    export X11_Xi_LIB=${pkgs.libXi}/lib/libXi.so
    export X11_Xshape_LIB=${pkgs.libXext}/lib/libXext.so

    export PKG_CONFIG_PATH=${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.libX11}/lib/pkgconfig:${pkgs.libXi}/lib/pkgconfig:${pkgs.libXrandr}/lib/pkgconfig:${pkgs.libXinerama}/lib/pkgconfig:${pkgs.libXcursor}/lib/pkgconfig

    substitute ./CMakePresets.json.in ./CMakePresets.json \
      --subst-var-by VULKAN_HEADERS ${pkgs.vulkan-headers} \
      --subst-var-by VULKAN_LOADER  ${pkgs.vulkan-loader} \
      --subst-var-by X11            ${pkgs.libX11} \
      --subst-var-by XRANDR         ${pkgs.libXrandr} \
      --subst-var-by XINERAMA       ${pkgs.libXinerama} \
      --subst-var-by XCURSOR        ${pkgs.libXcursor} \
      --subst-var-by XI             ${pkgs.libXi} \
      --subst-var-by XEXT           ${pkgs.libXext} \
      --subst-var-by XKBCOMMON_DEV  ${pkgs.libxkbcommon.dev}\
      \
      --subst-var-by WAYLAND_DEV       ${pkgs.wayland.dev}\
      --subst-var-by WAYLAND_PROTOCOLS ${pkgs.wayland-protocols}\
  '';

  shvulkan = stdenv.mkDerivation {
    pname = "shvulkan";
    version = "1.1.1";

    src = ./.;

    nativeBuildInputs = pipeline.nativeBuildInputs;
    buildInputs = pipeline.buildInputs;

    cmakeFlags = pipeline.cmakeFlags;

    buildPhase = pipeline.buildPhase;
    installPhase = pipeline.installPhase;
  
  };

}