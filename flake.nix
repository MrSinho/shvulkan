{
  description = "Dev environment using Vulkan + GLFW and Wayland";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:

    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in

    {
      devShells.${system}.default = pkgs.mkShell {

        buildInputs = [
          pkgs.pkg-config # needed for CMake to find packages
          pkgs.glfw-wayland
          pkgs.wayland
          pkgs.wayland-protocols
          pkgs.libxkbcommon
          pkgs.xorg.libX11
          pkgs.xorg.libXi
          pkgs.xorg.libXrandr
          pkgs.xorg.libXinerama
          pkgs.xorg.libXcursor
          pkgs.xorg.libXext
          #pkgs.xorg.libxcb
        ];

        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
          pkgs.wayland
          pkgs.vulkan-loader
        ];

        # Set environment variables when entering the shell
        # To find the path of a package: nix-instantiate --eval-only --expr '(import <nixpkgs> {}).glfw-wayland.outPath'
        # To find a package name based on the path: basename /nix/store/b2aj478jxny0yvwqr6zglcmp99r3563b-libX11-1.8.12-dev | cut -d'-' -f2-
        shellHook = ''
          export Vulkan_INCLUDE_DIR=${pkgs.vulkan-headers}/include
          export Vulkan_LIBRARY=${pkgs.vulkan-loader}/lib/libvulkan.so

          export X11_X11_INCLUDE_PATH=${pkgs.xorg.libX11}/include
          export X11_X11_LIB=${pkgs.xorg.libX11}/lib

          export X11_Xrandr_INCLUDE_PATH=${pkgs.xorg.libXrandr}/include
          export X11_Xrandr_LIB=${pkgs.xorg.libXrandr}/lib

          export X11_Xinerama_INCLUDE_PATH=${pkgs.xorg.libXinerama}/include
          export X11_Xinerama_LIB=${pkgs.xorg.libXinerama}/lib

          export X11_Xkb_INCLUDE_PATH=${pkgs.libxkbcommon}/include
          export X11_Xkb_LIB=${pkgs.libxkbcommon}/lib

          export X11_Xcursor_INCLUDE_PATH=${pkgs.xorg.libXcursor}/include
          export X11_Xcursor_LIB=${pkgs.xorg.libXcursor}/lib

          export X11_Xi_INCLUDE_PATH=${pkgs.xorg.libXi}/include
          export X11_Xi_LIB=${pkgs.xorg.libXi}/lib

          export X11_Xshape_INCLUDE_PATH=${pkgs.xorg.libXext}/include
          export X11_Xshape_LIB=${pkgs.xorg.libXext}/lib


          export PKG_CONFIG_PATH=${pkgs.wayland}/lib/pkgconfig:${pkgs.wayland-protocols}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.xorg.libX11}/lib/pkgconfig:${pkgs.xorg.libXi}/lib/pkgconfig:${pkgs.xorg.libXrandr}/lib/pkgconfig:${pkgs.xorg.libXinerama}/lib/pkgconfig:${pkgs.xorg.libXcursor}/lib/pkgconfig


          mkdir linux
          cd linux
          cmake .. \
            -DSH_VULKAN_BUILD_EXAMPLES=ON \
            -DVulkan_LIBRARY=$Vulkan_LIBRARY -DVulkan_INCLUDE_DIR=$Vulkan_INCLUDE_DIR \
            -DX11_X11_INCLUDE_PATH=$X11_X11_INCLUDE_PATH -DX11_X11_LIB=$X11_X11_LIB \
            -DX11_Xrandr_INCLUDE_PATH=$X11_Xrandr_INCLUDE_PATH -DX11_Xrandr_LIB=$X11_Xrandr_LIB \
            -DX11_Xinerama_INCLUDE_PATH=$X11_Xinerama_INCLUDE_PATH -DX11_Xinerama_LIB=$X11_Xinerama_LIB \
            -DX11_Xkb_INCLUDE_PATH=$X11_Xkb_INCLUDE_PATH -DX11_Xkb_LIB=$X11_Xkb_LIB \
            -DX11_Xcursor_INCLUDE_PATH=$X11_Xcursor_INCLUDE_PATH -DX11_Xcursor_LIB=$X11_Xcursor_LIB \
            -DX11_Xi_INCLUDE_PATH=$X11_Xi_INCLUDE_PATH -DX11_Xi_LIB=$X11_Xi_LIB \
            -DX11_Xshape_INCLUDE_PATH=$X11_Xshape_INCLUDE_PATH -DX11_Xshape_LIB=$X11_Xshape_LIB

          make

          cd ..
        '';


      };

    };

}

