CC = gcc
CFLAGS = -g -Wall
LIBS = -lm
EXE = render

$(EXE): main.o utils.o vec.o image.o camera.o sphere.o hittable.o
	$(CC) -o $@ $^ $(LIBS)

run: $(EXE)
	./$(EXE)
	feh -ZF --force-aliasing out.ppm

main.o: defs.h
utils.o: defs.h
vec.o: defs.h
image.o: defs.h
camera.o: defs.h
sphere.o: defs.h
hittable.o: defs.h

clean:
	rm *.o *.ppm $(EXE)

.PHONY: run clean
