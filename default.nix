{ stdenv, avrdude, avrgcclibc }:

stdenv.mkDerivation {
  name = "control-firmware";
  src = ./.;
  buildInputs = [ avrdude avrgcclibc ];
  installPhase = ''
    install -D -m 0644 control-firmware.{elf,hex} -t $out
  '';
  dontStrip = true;
  dontPatchELF = true;
}
