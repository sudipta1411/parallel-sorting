CC = gcc
CFLAGS = -I.
DFLAGS = -DNUM=8 -DNUM_THREAD=2 -DOMP
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
