CC = gcc
CFLAGS = -I.
DFLAGS = -DNUM=64 -DNUM_THREAD=8  -DOMP -DMAX=1000
FFLAGS = -fopenmp
LFLAGS = -lm
INCLUDE = array.h merge_sort.h comp.h partition_sort.h
OBJ = main.o merge_sort.o partition_sort.o
EXEC =parallel-sorting

%.o: %.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS)

clean:
	rm -fv *.o
	rm -v $(EXEC)
