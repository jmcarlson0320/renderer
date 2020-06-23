CC = gcc
CFLAGS = -g -Wall
LIBS = -lm -lSDL2
EXE = render

$(EXE): main.o renderer.o utils.o vec.o image.o camera.o sphere.o hittable.o
	$(CC) -o $@ $^ $(LIBS)

main.o: defs.h
renderer.o: defs.h
utils.o: defs.h
vec.o: defs.h
image.o: defs.h
camera.o: defs.h
sphere.o: defs.h
hittable.o: defs.h

clean:
	rm *.o $(EXE)

.PHONY: clean
