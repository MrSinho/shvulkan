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

    in rec {
      
      defaultPackage = pipeline.shvulkan;
      
      defaultApp = flake-utils.lib.mkApp {
        drv = defaultPackage;
      };

      devShell = pkgs.mkShell {
        nativeBuildInputs = pipeline.nativeBuildInputs;
        buildInputs = pipeline.buildInputs;
        #LD_LIBRARY_PATH = pipeline.LD_LIBRARY_PATH;
        #shellHook = pipeline.environmentSetup;
      };

    }

  );

}
