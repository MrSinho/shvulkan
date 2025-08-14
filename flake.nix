{
  description = "Dev environment using Vulkan + GLFW and Wayland";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: {

    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };

    in {
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = [
          pkgs.glfw-wayland
        ];
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
          pkgs.wayland
          pkgs.vulkan-loader
        ];
      };

    };

  };
}
