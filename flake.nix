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
      pkgs = import nixpkgs { inherit system; };

      pipeline = import ./pipeline { inherit pkgs; };

      shvulkan = (with pkgs; stdenv.mkDerivation {
          pname = "shvulkan";
          version = "0.1.0";
          #src = pkgs.lib.cleanSourceWith {
          #  src = ./.;
          #  filter = path: type: true;  # include everything
          #};
          src = pkgs.fetchgit {
            url = "https://github.com/mrsinho/shvulkan.git";
            rev = "20e96be9ea829a90f42bed015e86635ba20e227a"; # git rev-parse HEAD
            sha256 = "sha256-smJvvA2U1fCESp1wNPA/ShUurOZ4z8s/GUAh00FEUV4="; # nix hash path --type sha256 .    or paste dummy 0000000000000000000000000000000000000000000000000000
            fetchSubmodules = true;
          };

          nativeBuildInputs = pipeline.nativeBuildInputs;

          buildInputs = pipeline.buildInputs;

          cmakeFlags = pipeline.cmakeFlags;

          buildPhase = pipeline.buildPhase;

          installPhase = pipeline.installPhase;

        }
      );
    in rec {

      defaultApp = flake-utils.lib.mkApp {
        drv = defaultPackage;
      };

      defaultPackage = shvulkan;

      devShell = pkgs.mkShell {

        nativeBuildInputs = pipeline.nativeBuildInputs;

        buildInputs = pipeline.buildInputs;

        #LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [ # Allows linking to shared libraries (e.g. through dlopen)
        #  pkgs.vulkan-loader
        #  pkgs.glfw-wayland
        #  pkgs.wayland
        #  pkgs.wayland-protocols
        #  pkgs.wayland-scanner
        #  pkgs.libxkbcommon
        #  pkgs.xorg.libX11
        #  pkgs.xorg.libXrandr
        #  pkgs.xorg.libXinerama
        #  pkgs.xorg.libXcursor
        #  pkgs.xorg.libXi
        #  pkgs.xorg.libXext
        #];

        #shellHook = '' # Only for development
        #  export Vulkan_INCLUDE_DIR=${pkgs.vulkan-headers}/include
        #  export Vulkan_LIBRARY=${pkgs.vulkan-loader}/lib/libvulkan.so
#
        #  export X11_X11_LIB=${pkgs.xorg.libX11}/lib/libX11.so
        #  export X11_Xrandr_LIB=${pkgs.xorg.libXrandr}/lib/libXrandr.so
        #  export X11_Xinerama_LIB=${pkgs.xorg.libXinerama}/lib/libXinerama.so
        #  export X11_Xkb_LIB=${pkgs.libxkbcommon}/lib/libxkbcommon.so
        #  export X11_Xcursor_LIB=${pkgs.xorg.libXcursor}/lib/libXcursor.so
        #  export X11_Xi_LIB=${pkgs.xorg.libXi}/lib/libXi.so
        #  export X11_Xshape_LIB=${pkgs.xorg.libXext}/lib/libXext.so
#
        #  export PKG_CONFIG_PATH=${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.xorg.libX11}/lib/pkgconfig:${pkgs.xorg.libXi}/lib/pkgconfig:${pkgs.xorg.libXrandr}/lib/pkgconfig:${pkgs.xorg.libXinerama}/lib/pkgconfig:${pkgs.xorg.libXcursor}/lib/pkgconfig
#
        #  substitute ./CMakePresets.json.in ./CMakePresets.json \
        #    --subst-var-by VULKAN_HEADERS ${pkgs.vulkan-headers} \
        #    --subst-var-by VULKAN_LOADER  ${pkgs.vulkan-loader} \
        #    --subst-var-by X11            ${pkgs.xorg.libX11} \
        #    --subst-var-by XRANDR         ${pkgs.xorg.libXrandr} \
        #    --subst-var-by XINERAMA       ${pkgs.xorg.libXinerama} \
        #    --subst-var-by XCURSOR        ${pkgs.xorg.libXcursor} \
        #    --subst-var-by XI             ${pkgs.xorg.libXi} \
        #    --subst-var-by XEXT           ${pkgs.xorg.libXext} \
        #    --subst-var-by XKBCOMMON_DEV  ${pkgs.libxkbcommon.dev}\
        #    \
        #    --subst-var-by WAYLAND_DEV       ${pkgs.wayland.dev}\
        #    --subst-var-by WAYLAND_PROTOCOLS ${pkgs.wayland-protocols}\
#
#
        #'';

      };
    }
  );
}
