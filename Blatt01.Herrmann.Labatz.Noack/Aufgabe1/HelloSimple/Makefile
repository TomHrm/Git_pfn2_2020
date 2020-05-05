# Hier wird die Standartausführung der make Datei angegeben und die Startparameter in die entsprechenden Variablen gespeichert.
.PHONY:test
CC?=clang
CFLAGS=-g -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=hello

# Hier wird festgelegt wie die Zieldatei enden soll, also welches Dateiformat sie ist.
all:${FILE_BASE}.x

# Hier wird es dann schließlich ausgeführt. Compiler, Optionenen, Zeieldatei, Quelldatei
%.x:%.c
	${CC} ${CFLAGS} -o $@ $<

# Eine Readme Datei wird angelegt.
README:Makefile
	@echo "Die Loesung zu dieser Aufgabe muss in der Datei ${FILE_BASE}.c implementiert werden" > $@

# Beim dem Aufruf von "make test" wir dieser Teil aktiviert . Und er prüft ob sich die Ausgabe des eigenen Programms, von denen der vorgefertigten Lösungen unterscheiden.
# Und gibt jeweils die Differenzen wieder aus. Falls alles korrekt abläuft gibt er die letzte Zeile aus.
test:${FILE_BASE}.x
	@./${FILE_BASE}.x | diff - hello_1.txt
	@./${FILE_BASE}.x Tick Trick Track | diff - hello_Tick_Trick_Track.txt
	@./${FILE_BASE}.x 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 | diff - hello_1_26.txt
	@echo "Congratulations. Tests passed"

# Wenn man make clean aufruft werden alle *.o oder *.x Datein gelöscht. Ausserdem werden falls Chache Dateien vorhanden sind werden sie ebenfalls gelöscht.
.PHONY:clean
clean:
	${RM} ${FILE_BASE}.[ox]
	${RM} -r ${FILE_BASE}.x.dSYM __pycache__
