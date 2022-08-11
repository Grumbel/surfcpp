{
  description = "Software Surface Library";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.05";

    tinycmmc.url = "github:grumbel/tinycmmc";
    tinycmmc.inputs.nixpkgs.follows = "nixpkgs";

    logmich.url = "github:logmich/logmich";
    logmich.inputs.nixpkgs.follows = "nixpkgs";
    logmich.inputs.tinycmmc.follows = "tinycmmc";

    geomcpp.url = "github:grumbel/geomcpp";
    geomcpp.inputs.nixpkgs.follows = "nixpkgs";
    geomcpp.inputs.tinycmmc.follows = "tinycmmc";

    SDL2_src.url = "https://libsdl.org/release/SDL2-devel-2.0.22-mingw.tar.gz";
    SDL2_src.flake = false;
  };

  outputs = { self, nixpkgs, flake-utils, tinycmmc, geomcpp, logmich, SDL2_src }:
    tinycmmc.lib.eachSystemWithPkgs (pkgs:
      {
        packages = rec {
          default = surfcpp;

          SDL2 = if pkgs.targetPlatform.isWindows then
            pkgs.stdenv.mkDerivation {
            pname = "SDL2";
            version = "2.0.22";
            src = SDL2_src;
            installPhase = ''
              mkdir $out
            '' +
            (if pkgs.system == "i686-w64-mingw32" then
              "cp -vr i686-w64-mingw32/. $out/"
            else
              "cp -vr x86_64-w64-mingw32/. $out/");
          }
          else null;

          surfcpp = pkgs.callPackage ./surfcpp.nix {
            SDL2 = if pkgs.targetPlatform.isWindows then SDL2 else pkgs.SDL2;
            libjpeg = if pkgs.targetPlatform.isWindows then pkgs.libjpeg_original else pkgs.libjpeg;
            geomcpp = geomcpp.packages.${pkgs.targetPlatform.system}.default;
            logmich = logmich.packages.${pkgs.targetPlatform.system}.default;
            tinycmmc = tinycmmc.packages.${pkgs.targetPlatform.system}.default;
          };
        };
      }
    );
}
