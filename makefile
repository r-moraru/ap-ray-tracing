# Makefile

MPICXX = mpic++

SRCS = main.cpp

EXEC = render.out

PPMFILE = test.ppm

PNGFILE = test.png

all: render run convert

render:
	$(MPICXX) $(SRCS) -o $(EXEC)

run:
	@mpirun --oversubscribe -np 4 ./$(EXEC)

convert:
	@convert $(PPMFILE) $(PNGFILE)

clean:
	rm -f $(OBJS) $(EXEC) $(PPMFILE) $(PNGFILE)

.PHONY: all run convert clean

