



main: merch.h main.c webstore.c webstore.h merch.c merch.h
	gcc -g -Wall -pedantic -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c cart2.c merch.c main.c -o ./bin
	@echo "::::::::::: Finished :::::::::::"



dbg: debug.c webstore.c webstore.h merch.h merch.c
	gcc -g -Wall -pedantic -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c debug.c merch.c -o ./dbg 
	./dbg
	@echo "::::::::::: Finished :::::::::::"

dbgt: dbg
	valgrind --leak-check=full ./dbg

cart:
	gcc cart.c utils.c common.c hash_table.c list_linked.c webstore.c iterator.c

ui: ui.h merch.h cart2.h utils.h common.h hash_table.h list_linked.h iterator.h webstore.h
	@printf "Compiling UI Demo to ./bin/ui-demo.out...\n"	
	gcc -g -Wall -pedantic -std=c11 \
		ui.c \
		merch.c \
		cart2.c \
		utils.c \
		common.c \
		hash_table.c list_linked.c iterator.c \
		webstore.c \
		-o ./bin/ui_demo.out
	@printf "\nFinished compiling.\n Run with 'make run_ui'\n"


clean_ui:
	touch ./bin/ui_demo_old.out
	rm ./bin/ui_demo_old.out

	@echo "Removing old backup..."

	touch ./bin/ui_demo.out
	mv ./bin/ui_demo.out ./bin/ui_demo_old.out

	@echo "Backup ==> ./bin/ui-demo-old.out..."

# "¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨" Tests "¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨"


make_tests:
	@echo "> Compiling test binary..."
	gcc -g -Wall -pedantic -std=c11           \
		list_linked.c common.c iterator.c \
		hash_table.c webstore.c utils.c   \
		cart2.c merch.c test_merch.c      \
		-lcunit  -o ./bin/run_test
	@echo "> Done!\n"

clean_tests: 
	@echo "> Removing test binary..."
	touch ./bin/run_test
	rm    ./bin/run_test
	@echo "> Done!\n"

build_and_run_tests: clean_tests make_tests  
	@echo "#================# Running Tests #================#"
	@echo "> Testing for memory leaks using stdin and out..."
	@echo "> Testing all functionalities in the merch section.."
	./test/ui_test.sh ./bin/ui-demo.out
	@echo "> Finished!\n"
	@echo "> Running Unit Tests...\n"
#	valgrind --leak-check=full ./bin/run_test
	./bin/run_test	

	@echo "> Finished!\n"


	@echo "#================# Tests Complete #================#"
# "¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨" 



run_ui: ui
	./bin/ui_demo.out

clean:
	rm ./main
	@echo "Removed Binary"


run: main
	./bin

test: 
	gcc -g -Wall -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c  test.c -lcunit  -o ./bin

memtest: test
	valgrind --leak-check=full ./bin

test_cart: 
	gcc -g -Wall -std=c11 list_linked.c common.c iterator.c \
	hash_table.c webstore.c utils.c cart.c merch.c test_cart_new.c -lcunit  -o ./bin

memtest_cart: test_cart
	valgrind --leak-check=full ./bin

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
