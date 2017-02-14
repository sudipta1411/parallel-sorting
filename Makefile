CC = gcc
CFLAGS = -I.
DFLAGS =  -DNUM=1024*128 -DNUM_THREAD=16
FFLAGS = -fopenmp
LFLAGS = -lm
INCLUDE = array.h
OBJ = main.o
EXEC =parallel-sorting

%.o: %.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS)

clean:
	rm -fv *.o
	rm -v $(EXEC)
