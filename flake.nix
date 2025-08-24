{
  inputs = {
    nixpkgs = {
      url = "github:nixos/nixpkgs/nixos-unstable";
    };
    flake-utils = {
      url = "github:numtide/flake-utils";
    };
  };
  outputs = { nixpkgs, flake-utils, ... }: flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };
      shvulkan = (with pkgs; stdenv.mkDerivation {
          pname = "shvulkan";
          version = "0.1.0";
          #src = pkgs.lib.cleanSourceWith {
          #  src = ./.;
          #  filter = path: type: true;  # include everything
          #};
          src = pkgs.fetchgit {
            url = "https://github.com/mrsinho/shvulkan";
            rev = "16d8540f134a0da60da8f2f5ab5bfc2e9cdc08b7";
            sha256 = "HwIDSmVNGipxEl8gzvzWuaRpvwBQnXH6lzOLABuTJL8=";# dummy 0000000000000000000000000000000000000000000000000000
            fetchSubmodules = true;
          };

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
            pkgs.glfw-wayland
            pkgs.wayland
            pkgs.wayland-protocols
            pkgs.wayland-scanner
            pkgs.libxkbcommon
            pkgs.xorg.libX11
            pkgs.xorg.libXi
            pkgs.xorg.libXrandr
            pkgs.xorg.libXinerama
            pkgs.xorg.libXcursor
            pkgs.xorg.libXext
            pkgs.doxygen
          ];

          cmakeFlags = [
            "-Wno-dev"
            "-U'*'"
            "-DSH_VULKAN_BUILD_EXAMPLES=ON"############
            "-DSH_VULKAN_BUILD_DOCS=ON"
            
            "-DVulkan_INCLUDE_DIR=${pkgs.vulkan-headers}/include"
            "-DVulkan_LIBRARY=${pkgs.vulkan-loader}/lib/libvulkan.so"

            "-DX11_X11_INCLUDE_PATH=examples/external/libx11/include"
            "-DX11_X11_LIB=${pkgs.xorg.libX11}/lib/libX11.so"

            "-DX11_Xrandr_INCLUDE_PATH=examples/external/libxrandr/include"
            "-DX11_Xrandr_LIB=${pkgs.xorg.libXrandr}/lib/libXrandr.so"

            "-DX11_Xinerama_INCLUDE_PATH=examples/external/libxinerama/include"
            "-DX11_Xinerama_LIB=${pkgs.xorg.libXinerama}/lib/libXinerama.so"

            "-DX11_Xkb_INCLUDE_PATH=examples/external/libxkbcommon/include"

            "-DX11_Xcursor_INCLUDE_PATH=examples/external/libxcursor/include"
            "-DX11_Xcursor_LIB=${pkgs.xorg.libXcursor}/lib/libXcursor.so"

            "-DX11_Xi_INCLUDE_PATH=examples/external/libxi/include"
            "-DX11_Xi_LIB=${pkgs.xorg.libXi}/lib/libXi.so"

            "-DX11_Xshape_INCLUDE_PATH=examples/external/libxext/include"

            "-DPKG_CONFIG_PATH=${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.xorg.libX11}/lib/pkgconfig:${pkgs.xorg.libXi}/lib/pkgconfig:${pkgs.xorg.libXrandr}/lib/pkgconfig:${pkgs.xorg.libXinerama}/lib/pkgconfig:${pkgs.xorg.libXcursor}/lib/pkgconfig"
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
            cp -r $PWD/../lib              $out/
            cp -r $PWD/../shvulkan/include $out/
            cp -r $PWD/../docs/docs        $out/

            # Copy examples binaries and shaders
            mkdir -p $out/examples/bin
            mkdir -p $out/examples/shaders/bin
            cp -r $PWD/../examples/shaders/bin $out/examples/shaders/
            cp -r $PWD/../bin/examples/*       $out/examples/bin/
          '';

        }
      );
    in rec {
      defaultApp = flake-utils.lib.mkApp {
        drv = defaultPackage;
      };

      defaultPackage = shvulkan;

      devShell = pkgs.mkShell {

        nativeBuildInputs = [
          pkgs.cmake
          pkgs.ninja
          pkgs.clang
          pkgs.pkg-config
        ];

        buildInputs = [
          shvulkan
          pkgs.vulkan-tools
          pkgs.vulkan-loader
          pkgs.vulkan-helper
          pkgs.vulkan-headers
          pkgs.glfw-wayland
          pkgs.wayland
          pkgs.wayland-protocols
          pkgs.wayland-scanner
          pkgs.libxkbcommon
          pkgs.xorg.libX11
          pkgs.xorg.libXi
          pkgs.xorg.libXrandr
          pkgs.xorg.libXinerama
          pkgs.xorg.libXcursor
          pkgs.xorg.libXext
          pkgs.doxygen
        ];

        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [ # Allows linking to shared libraries (e.g. through dlopen)
          pkgs.vulkan-loader
          pkgs.glfw-wayland
          pkgs.wayland
          pkgs.wayland-protocols
          pkgs.wayland-scanner
          pkgs.libxkbcommon
          pkgs.xorg.libX11
          pkgs.xorg.libXrandr
          pkgs.xorg.libXinerama
          pkgs.xorg.libXcursor
          pkgs.xorg.libXi
          pkgs.xorg.libXext
        ];

        shellHook = '' # Only for development
          export Vulkan_INCLUDE_DIR=${pkgs.vulkan-headers}/include
          export Vulkan_LIBRARY=${pkgs.vulkan-loader}/lib/libvulkan.so

          export X11_X11_LIB=${pkgs.xorg.libX11}/lib/libX11.so
          export X11_Xrandr_LIB=${pkgs.xorg.libXrandr}/lib/libXrandr.so
          export X11_Xinerama_LIB=${pkgs.xorg.libXinerama}/lib/libXinerama.so
          export X11_Xkb_LIB=${pkgs.libxkbcommon}/lib/libxkbcommon.so
          export X11_Xcursor_LIB=${pkgs.xorg.libXcursor}/lib/libXcursor.so
          export X11_Xi_LIB=${pkgs.xorg.libXi}/lib/libXi.so
          export X11_Xshape_LIB=${pkgs.xorg.libXext}/lib/libXext.so

          export PKG_CONFIG_PATH=${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.xorg.libX11}/lib/pkgconfig:${pkgs.xorg.libXi}/lib/pkgconfig:${pkgs.xorg.libXrandr}/lib/pkgconfig:${pkgs.xorg.libXinerama}/lib/pkgconfig:${pkgs.xorg.libXcursor}/lib/pkgconfig

          substitute ./CMakePresets.json.in ./CMakePresets.json \
            --subst-var-by VULKAN_HEADERS ${pkgs.vulkan-headers} \
            --subst-var-by VULKAN_LOADER  ${pkgs.vulkan-loader} \
            --subst-var-by X11            ${pkgs.xorg.libX11} \
            --subst-var-by XRANDR         ${pkgs.xorg.libXrandr} \
            --subst-var-by XINERAMA       ${pkgs.xorg.libXinerama} \
            --subst-var-by XCURSOR        ${pkgs.xorg.libXcursor} \
            --subst-var-by XI             ${pkgs.xorg.libXi} \
            --subst-var-by XEXT           ${pkgs.xorg.libXext} \
            --subst-var-by XKBCOMMON_DEV  ${pkgs.libxkbcommon.dev}\
            \
            --subst-var-by WAYLAND_DEV       ${pkgs.wayland.dev}\
            --subst-var-by WAYLAND_PROTOCOLS ${pkgs.wayland-protocols}\


        '';

      };
    }
  );
}
