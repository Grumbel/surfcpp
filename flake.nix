{
  description = "Software Surface Library";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
    tinycmmc.url = "gitlab:grumbel/cmake-modules";
    geomcpp.url = "gitlab:grumbel/geomcpp";
    logmich.url = "gitlab:logmich/logmich";
  };

  outputs = { self, nix, nixpkgs, flake-utils, tinycmmc, geomcpp, logmich }:
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
