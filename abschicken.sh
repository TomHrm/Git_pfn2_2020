#!/bin/sh

set -e #-x
pfad=`pwd`
echo "Skript zum abschicken eines PfN-Arbeitsblattes"
echo "- Ordner folgendes Format BlattXX.NAME1.NAME2.NAME3"
echo "- unnötige Dateien löschen."
echo "- Bearbeitungen.txt ausgefüllt?!"
echo "- Formatierungsfehler entfernen"
echo "- tar_loesungen.py <N> zum packen des Blattes"
echo "- Per Email an: pfn2@zbh.uni-hamburg.de schicken."
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

for file in ./*/*/*/Makefile; do
	fdir=$(dirname "$file")
	cd $fdir
  make clean
  cd ${pfad}
done     


#Prüft alle *.c Dateien auf richtige Formatierung.
echo " Folgende Formatierungsfehler treten auf:"
for file in ./*/*/*/*.c; do 
  fdir=$(dirname "$file")
	cd $fdir
  fname=$(basename "$file")
  #clang-format.sh $fname
  code_check.py $fname
  cd ${pfad}
done
