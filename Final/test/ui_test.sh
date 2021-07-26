#!/bin/bash
PIPE_NAME=test.pipe
LOG_FILE=log.txt

# Create pipe
init_test(){
    # Remove previous left pipe
    printf " Remove previous pipe\n"
    rm $PIPE_NAME
    touch $LOG_FILE; rm $LOG_FILE;
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
    valgrind --leak-check=full ./$1 < ./$PIPE_NAME > test-out &

    return 0
}
exit_test(){
    # Close pipe
    printf " Disconnecting $1 to '$PIPE_NAME'...\n"
    kill -s SIGTERM $PID
    rm $PIPE_NAME
    kill -9 $(pidof $1)
    return 0
}

# Send input to pipe
input(){
#    printf " "
#    echo "Sending: $1" 
    
    # Send string into test
    echo $1 >> $LOG_FILE
    echo $1 >> $PIPE_NAME;
    sleep 0.025;
    return 0
}
# Send input to pipe w. nl
inputnl(){
#    printf " "
#    echo "Sending: $1" 
    
    # Send string into test
    printf "$1\n" >> $LOG_FILE
    printf "$1\n" >> $PIPE_NAME
    sleep 0.025
    return 0
}

init_test

run_test $1

list_merch(){

    input "w\n"
    input "l\n"
    input "q\n"
}


new_merch(){

    inputnl "w"
    inputnl "l"
    inputnl "n"
    inputnl $1
 #   input "\n"; # Name
    inputnl $2
#    input "\n" ;# Description
    inputnl $3
  #  input "\n"; # Price
    inputnl $4
 #   input "\n"; # Shelf
    inputnl $5
    #    input "\n"; # Amount
    inputnl "l"
    inputnl "q" 
}

new_merch shirt coolshirt 100 A23 10

inputnl "q"

exit_test $1 








