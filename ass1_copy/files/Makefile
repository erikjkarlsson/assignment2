RED=\033[0;31m # Red
NC=\033[0m     # No Color
GRE=\033[0;32m # Green
BLU=\033[0;34m # Blue
# These should not be run manually

iterator.o:
	gcc -c -pedantic -g -Wall ./iterator.c -o ./iterator.o
	@printf "${RED}Finished Compiling ./iterator.o${NC}\n"

common.o:
	gcc -c -pedantic -g -Wall ./common.c -o ./common.o
	@printf "${RED}Finished Compiling ./common.o${NC}\n"

list_linked.o:
	gcc -c -pedantic -g -Wall ./list_linked.c -o ./list_linked.o
	@printf "${RED}Finished Compiling ./list_linked.o${NC}\n"

main:  main.o #common.o iterator.o main.o  list_linked.o
	gcc -g -lcunit -Wall -pedantic ./main.o ./common.o ./iterator.o  \
	./list_linked.o -o ./iterator
	@printf "${RED}Finished Compiling ./Bin/iterator${NC}\n"	

list_test: common.o iterator.o list_linked.o 
	gcc -pedantic -g -Wall test.c  ./list_linked.o ./common.o ./iterator.o  -lcunit -o ./list_test

hash_test: common.o  iterator.o list_linked.o
	gcc hash_table_tests.c hash_table.1.c common_list.c common.o list_linked.o iterator.o -lcunit -o ./hash_test

tests: hash_test list_test
	./hash_test
	./list_test -t -e 1 -e 2 -e 3 -e 4 -e 5 -e 6 -e 7 	


## BUILD
# This will compile hash_table.h, hash_table.c and main.c
# into Bin/ as hash_table.o main.o and the binary hash_table
# after compiling it changes the permission of the binary to
# allow execution with (chmod +x).

build: main
	chmod +x ./Bin/iterator
	@echo "Enabled Exec Permission for hash_table"

## CLEAN
# This will remove ALL files in Bin/ which
# should be all previously compiled files.

clean:
	rm -f  ./Bin/*
	@echo "Finished cleaning!"

## RUN
# This will run the binary file "hash_table"
run:
	@printf "${RED}#### Executing ... ####${NC}\n\n";	
	time ./Bin/iterator
	@printf "${RED}#### Finished      ####${NC}\n\n";	
git_all:
	git add iterator.c iterator.h main.c
	git commit -m "Lazy Makefile commit (something has been changed)"
	git push


## DEBUG
# This will run gdb on the binary "hash_table" in Bin/
debug:
	gdb ./Bin/iterator


mem1:
	valgrind --leak-check=full -s ./Bin/iterator -t -d
mem2:
	valgrind --leak-check=full -s ./Bin/iterator -t -d -D
mem3:
	valgrind --leak-check=full -s ./Bin/iterator --run-tests --debug --deep-debug --set span -1000 1000
mem4:
	valgrind --leak-check=full -s ./Bin/iterator -t -d -D --set list 10 1 2 3 4 5 -1000 -2 53456 1 

## TEST
# This compiles and runs the binary

test1: main
	@printf "${RED}#### iterator --run-tests --debug                ${NC}\n\n";
	time ./Bin/iterator -t -d
	@printf "${RED}#### Finished                                    ${NC}\n\n";

test2: main
	@printf "${RED}#### Iterator --run-tests --debug --deep-debug ${NC}\n\n";	
	time ./Bin/iterator -t -d -D
	@printf "${RED}#### Finished                                    ${NC}\n\n";	

test3: main
	@printf "${RED}#### iterator --run-tests --debug                ${NC}\n\n";
	./Bin/iterator --run-tests --debug --deep-debug --set span -1000 1000

	@printf "${RED}#### Finished                                    ${NC}\n\n";

test4: main
	@printf "${RED}#### iterator --run-tests --debug                ${NC}\n\n";
	./Bin/iterator -t -d -D --set list 10 1 2 3 4 5 -1000 -2 53456 1 
	@printf "${RED}#### Finished                                    ${NC}\n\n";



help: 
	@printf "${RED}Usage:${NC}\n"
	@printf "  ${RED}make build${NC}  - Compile the source files.\n"
	@printf "  ${RED}make clean${NC}  - Remove all compiled binary files.\n"
	@printf "  ${RED}make run${NC}    - Run the binary file.\n"
	@printf "  ${RED}make debug${NC}  - Run gdb on the binary file.\n"
	@printf "  ${RED}make test${NC}   - Compile and Run the source files.\n"
	@printf "  ${RED}make mem${NC}    -  Run valgrind on the binary file.\n"
	@printf "  ${GRE}make help${NC}   - Show this message.\n\n\n"
