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

      pipeline = import ./nix/pipeline.nix { inherit pkgs; };

      shvulkanApp = flake-utils.lib.mkApp {
        drv = pipeline.shvulkan;
      };

    in rec {

      packages = {
        shvulkan = pipeline.shvulkan;
        default = pipeline.shvulkan;
      };

      apps = {
        shvulkan = shvulkanApp;
        default = shvulkanApp;
      };

      devShells.default = pkgs.mkShell {
        nativeBuildInputs = pipeline.nativeBuildInputs;
        buildInputs = pipeline.buildInputs;
      };

    }

  );

}
