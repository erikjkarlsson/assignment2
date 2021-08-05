# Assignment 2 

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
