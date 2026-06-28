#!/bin/bash

BOARD=$1

if [ "$BOARD" = "pi3" ]
then
KERNEL=kernel8-32.img
elif [ "$BOARD" = "pi0" ]
then
KERNEL=kernel.img
elif [ "$BOARD" = "pi2" ]
then
KERNEL=kernel7.img
elif [ "$BOARD" = "pi4" ]
then
KERNEL=kernel7l.img
elif [ "$BOARD" = "pizero2w" ]
then
KERNEL=kernel8-32.img
else
echo "Need arg [pi0|pi2|pi3|pi4|pizero2w]"
exit
fi

cd third_party/common
make
if [ "$?" != "0" ]
then
	exit
fi
cd ../..

cd third_party/vice-3.9

# Prevent make from trying to regenerate autoconf/automake outputs.
# Touch Makefile last so it appears newer than Makefile.in (prevents
# config.status re-running configure, which would fail without the
# special LEXLIB/LEX env vars that were set during make_all.sh).
find . \( -name 'aclocal.m4' -o -name 'configure' -o -name 'config.status' -o -name 'Makefile.in' -o -name 'Makefile' \) -not -path '*/\.git/*' | xargs touch

# --keep-going lets make continue past rawnet sub-configure failure
# (rawnet requires dos2unix which may not be installed) so the
# top-level src/*.o objects still get compiled.
make --keep-going x64
make --keep-going x128
make --keep-going xvic
make --keep-going xplus4
make --keep-going xpet

# Rebuild libraries cleaned by make x64.
# Use subshells so a make failure doesn't strand us in a subdirectory.
(cd src && make)
(cd src/resid && make)
(cd src/c64 && make)
(cd src/userport && make)
(cd src/imagecontents && make)
(cd src/lib/libzmbv && make)
(cd src/sounddrv && make)
(cd src/mididrv && make)
(cd src/socketdrv && make)
(cd src/hwsiddrv && make)
(cd src/iodrv && make)
(cd src/rtc && make)
(cd src/arch/raspi && make)
(cd src/arch/raspi/c64 && make)
(cd src/arch/raspi/vic20 && make)

cd ../..

MACHINES="C64:c64 VIC20:vic20"

for m in $MACHINES
do
   P1=`echo $m | sed 's/:.*$//'`
   P2=`echo $m | sed 's/^.*://'`
   make clean
   BOARD=$BOARD make -f Makefile-$P1
   cp $KERNEL ${KERNEL}.$P2
done
