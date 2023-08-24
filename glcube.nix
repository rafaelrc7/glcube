{ pkgs ? import <nixpkgs> { }, ... }:
pkgs.stdenv.mkDerivation rec {
  pname = "glcube";
  version = "1.0.0";

  src = ./.;

  nativeBuildInputs = with pkgs; [
    gcc gnumake pkg-config
    glfw glew cglm
  ];

  installPhase = ''
    mkdir -p $out
    mv ./bin $out/bin
  '';
}

