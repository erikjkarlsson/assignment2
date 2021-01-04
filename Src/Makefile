
main:

	gcc -g -Wall -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c main.c -o ./bin || echo "ERROR IN COMPILATION"

	@echo "Running Finished"

cart:
	gcc cart.c utils.c common.c hash_table.c list_linked.c webstore.c iterator.c
	
	
clean:
	rm ./main
	@echo "Removed Binary"


run: main
	./bin


runl: main
	./bin -l


rund: main
	./bin --log --debug

comp_test:
	gcc -ggdb -Wall test.c cart.c utils.c common.c hash_table.c list_linked.c webstore.c iterator.c -o test.out -lcunit
help:
	@printf "=Commands=\t=Description=\n"
	@printf "make test\tRun with testing\n"
	@printf "make main\tRemove binary and compile\n"
	@printf "make clean\tRemove binary\n"
	@printf "make run\tRun (No Arguments)\n"
	@printf "make runl\tRun (With Logging)\n"
	@printf "make rund\tRun (With Logging and Debugging)\n"
	@printf "make help\tDisplay this message\n"
