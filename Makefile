CFLAGS= -g -std=c99
OBJ= camera.o cube.o cylinder.o lighting.o model.o plane.o scanner.o sphere.o tiff.o trace.o vector.o frandom.o
SRC= camera.c cube.c cylinder.c lighting.c model.c plane.o scanner.c sphere.c tiff.c trace.c vector.c frandom.c

art: $(OBJ)
	cc -o art $(CFLAGS) $(OBJ) -lm 

clean:
	rm -f art $(OBJ) core

lint:
	lint -u -x $(SRC)

$(OBJ): art.h artInternal.h
scanner.o: scanner.h
