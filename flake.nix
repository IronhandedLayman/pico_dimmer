{
  description = "Basic rpi pico development shell";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/master";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system: 
    let
      pico-sdk131 = with pkgs; (pico-sdk.overrideAttrs (o:
        rec {
        pname = "pico-sdk";
        version = "2.1.1";
        src = fetchFromGitHub {
          fetchSubmodules = true;
          owner = "raspberrypi";
          repo = pname;
          rev = version;
          sha256 = "sha256-8ru1uGjs11S2yQ+aRAvzU53K8mreZ+CC3H+ijfctuqg=";
        };
        }));
      pkgs = nixpkgs.legacyPackages.${system};
    in {
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [
            cmake
            gcc-arm-embedded
            libusb1
            openocd
            pico-sdk131
            picotool
            ];
          shellHook = with pkgs; ''
            export PICO_SDK_PATH="${pico-sdk131}/lib/pico-sdk"
            '';
          };
        });
}
