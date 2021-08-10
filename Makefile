SOURCED=./src

LIBS= ./src/merch.c ./src/cart.c \
     ./src/utils.c ./src/common.c ./src/hash_table.c \
     ./src/list_linked.c ./src/iterator.c ./src/webstore.c

UI=./src/ui.c
UI_HEADER=./src/ui.h 
LIBS_HEADERS=./src/merch.h ./src/cart.h \
     ./src/utils.h ./src/common.h ./src/hash_table.h \
     ./src/list_linked.h ./src/iterator.h ./src/webstore.h


main: ${LIBS_HEADERS} 
	@printf "Compiling UI Demo to ./bin/ui-demo.out...\n"	
	gcc -g -Wall -pedantic -std=c11 ${LIBS} ./src/main.c -o ./bin/main.out
	@printf "\nFinished compiling.\n Run with 'make run_ui'\n"

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


# Shorthand
bart: build_and_run_tests

build_and_run_tests: clean_tests tests ui
	@echo "#================# Running Tests #================#"
	@echo "> Testing for memory leaks using stdin and out..."
	@echo "> Testing all functionalities in the merch section.."
	./test/ui_test.sh ./bin/ui_demo.out
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
	@printf "Running main.out... \n"	
	rm ./bin/main.out

run: main

	@printf "Running main.out... \n"	
	./bin/main.out

test_cart:
	@printf "Compiling tests for the cart... \n"	
	gcc -g -Wall -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c cart.c merch.c test_cart_new.c -lcunit  -o ./bin

runl: main
	@printf "Running main.out with logging and debugging flag...\n"		
	./bin --log --debug

help:
	@printf "=Commands=\t=Description=\n"
	@printf "make [Option]            \n"
	@printf "== Options [Shortcut name] ==            \n"
	@printf "build_and_tests[bart] -- Build and run tests\n"
	@printf	"main                  -- Build without UI   \n
	@printf	"clean                 -- Clean the binary without UI\n
	@printf "ui                    -- Build with UI      \n
	@printf "clean_ui              -- Clean UI binaries  \n
	@printf	"run_ui                -- Run UI binary      \n
	@printf "tests                 -- Build CUnit Tests  \n
	@printf "clean_tests           -- Clean Test binaries\n
	@printf "
	@printf	"
	@printf "make help\tDisplay this message\n"

