{
  description = "Simple openGL cube example";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/23.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
    let pkgs = import nixpkgs { inherit system; };
    in {
      devShell       = import ./shell.nix  { inherit pkgs; };
      defaultPackage = import ./glcube.nix { inherit pkgs; };
    }
  );
}

