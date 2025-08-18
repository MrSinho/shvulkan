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
          src = pkgs.fetchgit {
            url = "https://github.com/mrsinho/shvulkan";
            rev = "fd4f90196fa95bd960274acac349435aaeb4b66a";
            sha256 = "d7rU3sbr6/u9cSiqPqMZAZAY5vWjR6nQdmU46Qjy8tA=";
            fetchSubmodules = true;
          };

          nativeBuildInputs = [
            pkgs.pkg-config
            clang
            cmake
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

            "-DSH_VULKAN_BUILD_EXAMPLES=ON"
            
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
          ];
          buildPhase = "
            make -j $NIX_BUILD_CORES
          ";
          installPhase = ''
            mkdir -p $out/bin
            mkdir -p $out/lib
            cp $src/lib/shvulkan.a $out/lib/shvulkan.a
          '';
        }
      );
    in rec {
      defaultApp = flake-utils.lib.mkApp {
        drv = defaultPackage;
      };
      defaultPackage = shvulkan;
      devShell = pkgs.mkShell {
        buildInputs = [
          shvulkan
        ];
      };
    }
  );
}
