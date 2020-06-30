#!/bin/sh

set -e #-x
pfad=`pwd`

for file in ./*/*/*/Makefile; do
	fdir=$(dirname "$file")
	cd $fdir
  make clean
  cd ${pfad}
done  
for file in ./*/*/*/*.c; do 
  fdir=$(dirname "$file")
	cd $fdir
  fname=$(basename "$file")
  clang-format -style=file -i $fname
  cd ${pfad}
done
for file in ./*/*/*/*.cpp; do 
  fdir=$(dirname "$file")
	cd $fdir
  fname=$(basename "$file")
  clang-format -style=file -i $fname
  cd ${pfad}
done
for file in ./*/*/*/*.h; do 
  fdir=$(dirname "$file")
	cd $fdir
  fname=$(basename "$file")
  clang-format -style=file -i $fname
  cd ${pfad}
done
for file in ./*/*/*/*.hpp; do 
  fdir=$(dirname "$file")
	cd $fdir
  fname=$(basename "$file")
  clang-format -style=file -i $fname
  cd ${pfad}
done
clear
echo " Formatierung abgeschlossen..."
echo " "
echo "----------------------------------------------------------------------"
echo " Skript zum abschicken eines PfN-Arbeitsblattes"
echo " - Ordner folgendes Format BlattXX.NAME1.NAME2.NAME3"
echo " - Bearbeitungen.txt ausgefüllt?!"
echo " - tar_loesungen.py <N> zum packen des Blattes"
echo " - Per Email an: pfn2@zbh.uni-hamburg.de schicken."
echo "----------------------------------------------------------------------"

##############################################################################
# Um alle Tests einmal aufzurufen muss der folgende Codeabschnitt            #
# ein- bzw wieder auskommentiert werden:                                     #
#                                                                            #
#for file in ./*/*/*/Makefile; do                                            #
#	fdir=$(dirname "$file")                                                    #
#	cd $fdir                                                                   #
#	echo "\n\n$fdir:"                                                          #
#	make                                                                       #
#  make test                                                                 #
#  make clean                                                                #
#  echo "\n"                                                                 #
#  cd ${pfad}                                                                #
#done                                                                        #
#echo "---------------------------------------------------------------------"#
##############################################################################
