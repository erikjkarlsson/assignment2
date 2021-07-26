#!/bin/bash
PIPE_NAME=test.pipe
LOG_FILE=log.txt

# Create pipe
init_test(){
    # Remove previous left pipe
    printf " Remove previous pipe\n"
    rm $PIPE_NAME
    printf " Creating pipe '$PIPE_NAME'...\n"
    # Create pipe fd
    mkfifo $PIPE_NAME
    # Keep pipe open
    sleep infinity > $PIPE_NAME &
    PID=$(pidof sleep)
    return 0
}
vg(){
     $1 
}
run_test(){    
    # Run binary with pipe connected
    printf " Connecting $1 to '$PIPE_NAME'...\n"    

    ./$1 < ./$PIPE_NAME &

    return 0
}
exit_test(){
    # Close pipe
    printf " Disconnecting $1 to '$PIPE_NAME'...\n"
    kill -s SIGTERM $PID
    rm $PIPE_NAME
    killall $1
    return 0
}

# Send input to pipe
input(){
    printf " "
    echo Sending $1\n
    
    # Send string into test
    echo $1 > $PIPE_NAME;
    return 0
}

init_test

run_test $1

input "w\n"
input "l\n" 
input "q\nq\n" 

exit_test $1 
