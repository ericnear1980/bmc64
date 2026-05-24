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
make x64
make x128
make xvic
make xplus4
make xpet

# Rebuild libraries cleaned by make x64
cd src/resid && make && cd ../..
cd src/c64 && make && cd ../..
cd src/userport && make && cd ../..
cd src/imagecontents && make && cd ../..
cd src/lib/libzmbv && make && cd ../../..
cd src/arch/raspi && make && cd ../../..
cd src/arch/raspi/c64 && make && cd ../../../..

cd ../..

MACHINES="C64:c64"

for m in $MACHINES
do
   P1=`echo $m | sed 's/:.*$//'`
   P2=`echo $m | sed 's/^.*://'`
   make clean
   BOARD=$BOARD make -f Makefile-$P1
   cp $KERNEL ${KERNEL}.$P2
done
