
main:

	gcc -g -Wall -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c main.c -o ./bin || echo "ERROR IN COMPILATION"

	@echo "Running Finished"
clean:
	rm ./main
	@echo "Removed Binary"


run: main
	./bin


runl: main
	./bin -l


rund: main
	./bin --log --debug


help:
	@printf "=Commands=\t=Description=\n"
	@printf "make test\tRun with testing\n"
	@printf "make main\tRemove binary and compile\n"
	@printf "make clean\tRemove binary\n"
	@printf "make run\tRun (No Arguments)\n"
	@printf "make runl\tRun (With Logging)\n"
	@printf "make rund\tRun (With Logging and Debugging)\n"
	@printf "make help\tDisplay this message\n"
