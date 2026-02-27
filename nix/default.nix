{ pkgs, ... }:

let
  pipeline = import ./pipeline { inherit pkgs; };
in

pipeline.shvulkan