# Assignment 2 

## Structure

### `src`
The src directory holds all the source files.


The files contained in `src` is:
|-------------|-----------------|-------------------|----------------|--------------|
|  backend.c |  debug.c       |  list_linked.h   |  test_merch.c |  webstore.c |
|  cart.c    |  hash_table.c  |  main.c          |  test_merch.h |  webstore.h |
|  cart.h    |  hash_table.h  |  merch.c         |  ui.c         |              |
|  common.c  |  iterator.c    |  merch.h         |  ui.h         |              |
|  common.h  |  iterator.h    |  test.c          |  utils.c      |              |
|  db.c      |  list_linked.c |  test_cart_new.c |  utils.h      |              |

### `test`
The test directory houses all test scripts and programs source code
and/or binaries.



### `cache`
The cache folder holds temporary trash files or temporary files that
may be generated during script execution etc.

### `bin`
The bin directory holds all compiled binary, linker files being the
outcome of building.

Usually the binary name is either;
	- `main.out`:    Result of running `make main`
	- `run_test`:    Result of running `make test`
    - `ui_demo.out`: Result of running `make ui`
# UI
- Compile the program with UI `make ui`
- Clean program with ui binary with `make clean_ui`
- Run ui with `make run_ui`.

# Tests
- Compile tests with `make tests`
- Clean tests with `make clean_tests`
- Compile and Run tests with `make build_and_run_tests`.


## Preprogrammed input
To test the program with simulated input, the program `ui_test.sh` can
be used. Located in the `./test` directory. This allows for (within
the `ui_test.sh` file) scripts of keypresses to be programmed.

To run the `ui_test.sh` on a program, use the program path as
argument.  Here is how the test binary is tested in the makefile:

- `./test/ui_test.sh ./bin/ui-demo.out`.


## Tools 

### `empty_io.sh`

Clear the contents of the files:
 - `/input.txt` 
 - `/output.txt`
