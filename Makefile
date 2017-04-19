CC = gcc
CFLAGS = -I.
DFLAGS = -DNUM=1024 -DNUM_THREAD=8  -DOMP -DMAX=1000
FFLAGS = -fopenmp
LFLAGS = -lm
INCLUDE = array.h merge_sort.h comp.h partition_sort.h bin_search_range.h stack.c
OBJ = main.o merge_sort.o partition_sort.o bin_search_range.o stack.o
EXEC =parallel-sorting

%.o: %.c $(INCLUDE)
	$(CC) -c -o $@ $< $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS) -g

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DFLAGS) $(FFLAGS) $(LFLAGS) -g

clean:
	rm -fv *.o
	rm -v $(EXEC)
