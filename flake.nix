{
  description = "Software Surface Library";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
    flake-utils.url = "github:numtide/flake-utils";

    tinycmmc.url = "gitlab:grumbel/cmake-modules";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";
    tinycmmc.inputs.flake-utils.follows = "flake-utils";

    logmich.url = "gitlab:logmich/logmich";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.flake-utils.follows = "flake-utils";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    geomcpp.url = "gitlab:grumbel/geomcpp";
    geomcpp.inputs.nixpkgs.follows = "nixpkgs";
    geomcpp.inputs.flake-utils.follows = "flake-utils";
    geomcpp.inputs.tinycmmc.follows = "tinycmmc";
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc, geomcpp, logmich }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          surfcpp = pkgs.stdenv.mkDerivation {
            pname = "surfcpp";
            version = "0.3.0";
            src = nixpkgs.lib.cleanSource ./.;
            cmakeFlags = [ "-DBUILD_EXTRA=ON" ];
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.gcc
              pkgs.pkgconfig
              tinycmmc.defaultPackage.${system}
            ];
            buildInputs = [
              geomcpp.defaultPackage.${system}
              logmich.defaultPackage.${system}
              pkgs.gtest
              pkgs.gbenchmark
              pkgs.fmt
              pkgs.glm
              pkgs.imagemagick6
            ];
            propagatedBuildInputs = [
              pkgs.libexif
              pkgs.libjpeg
              pkgs.libpng
              pkgs.SDL2
            ];
          };
        };
        defaultPackage = packages.surfcpp;
      }
    );
}