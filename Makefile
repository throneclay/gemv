cc=icpc
lib=-mkl -lmemkind
target=gemv
source=main.c

$(target): $(source)
	$(cc)  -o $(target) $(source) $(lib)

run: $(target)
	./$(target)
clean:
	rm -rf *.o $(target) sgemv dgemv
