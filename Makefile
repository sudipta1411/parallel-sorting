CC = gcc
CFLAGS = -I.
DFLAGS = -DNUM=4096 -DNUM_THREAD=8 -DOMP -DMAX=10000
FFLAGS = -fopenmp
LFLAGS = -lm
INCLUDE = array.h merge_sort.h
OBJ = main.o merge_sort.o
EXEC =parallel-sorting

%.o: %.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS) -g

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS) -g

clean:
	rm -fv *.o
	rm -v $(EXEC)
