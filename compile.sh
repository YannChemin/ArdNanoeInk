#!/bin/bash
if [ ! $1 ]
then
	inputxbm=$(ls pnj*.xbm | shuf -n 1)
else
	# Argument is aimed at copying the file of interest to the permanent name of input in the ino file
	inputxbm=$1
fi
# Create the input file from inputxbm filename
cp $inputxbm pnj.xbm
val=$(echo $inputxbm | sed 's/.xbm//')
sed -i "s/"$val"/pnj/g" pnj.xbm

rootdir=$PWD
# THIS IS OPTIONAL: CH340 USB driver issue
## Start by ensuring ttyUSB0 on MacBook Pro
##Enable ttyUSB0
#cd ~/dev/CH341SER
#bash compile.sh
##come back
#cd $rootdir

#Create/Clean tmpdir
tmpdir=/tmp/arduino_build_yann
mkdir -p $tmpdir
cd $tmpdir
rm -Rf *
cd $rootdir

#Build
arduino-builder -dump-prefs -logger=machine -hardware /usr/share/arduino/hardware -hardware /home/yann/.arduino15/packages -tools /usr/share/arduino/hardware/tools/avr -tools /home/yann/.arduino15/packages -libraries /home/yann/Arduino/libraries -fqbn=arduino:avr:nano:cpu=atmega328old -build-path $tmpdir -warnings=none -prefs=build.warn_data_percentage=75 -verbose $rootdir/dnd.ino
arduino-builder -compile -logger=machine -hardware /usr/share/arduino/hardware -hardware /home/yann/.arduino15/packages -tools /usr/share/arduino/hardware/tools/avr -tools /home/yann/.arduino15/packages -libraries /home/yann/Arduino/libraries -fqbn=arduino:avr:nano:cpu=atmega328old -build-path $tmpdir -warnings=none -prefs=build.warn_data_percentage=75 -verbose $rootdir/dnd.ino

#Upload
/usr/bin/avrdude -C/etc/avrdude.conf -v -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:$tmpdir/dnd.ino.hex:i

