{ stdenv, avrgcclibc }:

stdenv.mkDerivation {
  name = "control-firmware";
  src = ./.;
  buildInputs = [ avrgcclibc ];
  installPhase = ''
    install -D -m 0644 control-firmware.{elf,hex} -t $out
  '';
}
