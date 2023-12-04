{
  description = "Rust Development Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    fenix.url = "github:nix-community/fenix";
  };

  outputs = { self, nixpkgs, fenix }:
    let

      # -------KNOBS-------
      toolchain = "stable";
      # -------     -------

      overlays = [
        fenix.overlays.default
      ];

      supportedSystems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];

      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system:
        let
          channel = fenix.packages.${system}.${toolchain};
        in
        f rec {
          pkgs = import nixpkgs { inherit overlays system; };

          rustPkg = with fenix.packages.${system};
            if builtins.pathExists ./rust-toolchain.toml then
              fromToolchainFile
                {
                  file = ./rust-toolchain.toml;
                  sha256 = pkgs.lib.fakeSha256;
                }
            else
              combine [
                channel.cargo
                channel.clippy
                channel.rust-src
                channel.rustc
                channel.rust-analyzer
                channel.rustfmt
              ];
        });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs, rustPkg }:
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              rustPkg
              cargo-watch
              cargo-show-asm
              cargo-edit
              cargo-nextest
              pkg-config
              zlib
              lldb_14
            ];
          };
        });
    };
}
