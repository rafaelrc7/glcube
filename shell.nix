{ pkgs ? import <nixpkgs> { }, ... }:
pkgs.mkShell {
  packages = with pkgs; [
    gcc

    gdb valgrind

    gnumake pkg-config glfw glew cglm
  ];
}

