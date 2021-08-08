SOURCED=./src

LIBS= ./src/merch.c ./src/cart2.c \
     ./src/utils.c ./src/common.c ./src/hash_table.c \
     ./src/list_linked.c ./src/iterator.c ./src/webstore.c

UI=./src/ui.c
UI_HEADER=./src/ui.h 
LIBS_HEADERS=./src/merch.h ./src/cart2.h \
     ./src/utils.h ./src/common.h ./src/hash_table.h \
     ./src/list_linked.h ./src/iterator.h ./src/webstore.h


ui: ${LIBS_HEADERS} ${UI_HEADER}
	@printf "Compiling UI Demo to ./bin/ui-demo.out...\n"	
	gcc -g -Wall -pedantic -std=c11 ${LIBS} ${UI} -o ./bin/ui_demo.out
	@printf "\nFinished compiling.\n Run with 'make run_ui'\n"


clean_ui:
	touch ./bin/ui_demo_old.out
	rm ./bin/ui_demo_old.out

	@echo "Removing old backup..."

	touch ./bin/ui_demo.out
	mv ./bin/ui_demo.out ./bin/ui_demo_old.out

	@echo "Backup ==> ./bin/ui-demo-old.out..."

# "¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨" Tests "¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨"


tests: ${LIBS_HEADERS} ./src/test_merch.h
	@echo "> Compiling test binary..."
	gcc -g -Wall -pedantic -std=c11  ${LIBS} ./src/test_merch.c -lcunit -o ./bin/run_test
	@echo "> Done!\n"

clean_tests: 
	@echo "> Removing test binary..."
	touch ./bin/run_test
	rm    ./bin/run_test
	@echo "> Done!\n"

build_and_run_tests: clean_tests tests  
	@echo "#================# Running Tests #================#"
	@echo "> Testing for memory leaks using stdin and out..."
	@echo "> Testing all functionalities in the merch section.."
	./test/ui_test.sh ./bin/ui-demo.out
	@echo "> Finished!\n"
	@echo "> Running Unit Tests...\n"
	valgrind --leak-check=full ./bin/run_test
#	./bin/run_test	

	@echo "> Finished!\n"
	@echo "#================# Tests Complete #================#"
# "¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨" 



run_ui: ui
	./bin/ui_demo.out

clean:
	rm ./main
	@echo "Removed Binary"


run: run_ui

test_cart: 
	gcc -g -Wall -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c cart.c merch.c test_cart_new.c -lcunit  -o ./bin

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

