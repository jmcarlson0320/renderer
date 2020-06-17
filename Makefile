CC = gcc
CFLAGS = -g -Wall
LIBS = -lm
EXE = render

$(EXE): main.o utils.o vec.o image.o
	$(CC) -o $@ $^ $(LIBS)

run: $(EXE)
	./image

main.o: defs.h
utils.o: defs.h
vec.o: defs.h
image.o: defs.h

clean:
	rm *.o *.ppm $(EXE)

.PHONY: run clean
