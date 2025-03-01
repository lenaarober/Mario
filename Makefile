
window_mario: main.o window.o
	g++ -g3 -o window_mario main.o window.o -lX11 

main.o: main.cc window.hh
	g++ -g3 -c main.cc

window.o: window.cc window.hh
	g++ -g3 -c window.cc

clean:
	rm -f window_mario *.o

.PHONY: clean all