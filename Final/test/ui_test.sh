#!/bin/bash
# ==================================================================
# Author: 3blue
# Usage:  GPLv2<
# ==================================================================
# This file creates a way of testing programs using the stdin.  By
# redirecting the file descriptor through a pipe to a linked file.
# Hence one can append or write text into that file, which counts as
# user input.


# =================================== ANSI
RED="\033[0;31m"   # Red
NC="\033[0m"       # No Color
GREEN="\033[0;32m" # Green
BLUE="\033[0;34m"  # Blue
BOLD="\033[1m"     # Nold
BLINK="\033[5m"    # Blinking Text
ITALIC="\033[3m"   # Italics
LINE="\033[4m"     # Underline
# =================================== 
declare -A LOG_ACTIVE_FILE RES_ACTIVE_FILE KEYS_ACTIVE_FILE
declare -A CACHE_DIR TEST_DIR
declare -A PIPE_NAME

# the created pipe (file) will have this name.
PIPE_NAME=test.pipe 
BINARY=$1
DBG=0
# Directories
CACHE_DIR=/home/erik/Repos/ass2/Final/cache
TEST_DIR=/home/erik/Repos/ass2/Final/test
INPUT_DELAY=0.025 # Time to sleep after one input being sent
dofile(){
    # dofile <filename> [<action>  [filename>]]
    case $2 in # File exists
	# Save a File
	save)
	    printf "Saving $1..."
	    cp -r $1 $3  && return 0 || return 1
	    printf "done!\n"
	    return 0
	    ;;

	# Remove a File	
	remove)
	    printf "Removing $1\n...\n"
	    touch $1 && rm -r $1 && return 0 || return 1
	    printf "done!\n"
	    return 0
	    ;;

	# Create a empty File 
	create)
	    printf "Creating $1\n...\n"
	    echo "" > $1 && touch $1 && return 0 || return 1
	    printf "done!\n"

	    return 0
	    ;;
	directory)
	    printf "Creating directory $1\n...\n".

	    if [ ! -e "$1" ]
	    then
		printf "None preexisting found, creating new!\n"
		mkdir  $1 && return 0  || return 1
	    else
		printf "Preexisting found, clearing all content!\n"
		rm -r $1 && mkdir  $1 && return 0 || return 1
	    fi
	    
	    printf "Error processing directory.\n"
	    printf "Invalid Action: $2 \n"
	    printf "Directory: $1\n"
	    return 0
	    ;;

	*)  printf "Error processing any valid action.\n"
	    printf "Invalid Action: $2 \n"
	    printf "File: $1\n"
	    return 1 
    esac


    printf "Error unknown.\n"
    printf "Action: $2 \n"
    printf "File: $1\n"
    
    return 1

}
# Options: create, remove, directory, save
#
# dofile $TEST_DIR/gol directory


gen_name(){
    # The file name will be a "randomly" generated shasum to keep the
    # cache conflict free.
    #
    # Usage: 
    # > gen_name [output|log] <extention> [Optional extra noise for shasum]

    case $1 in
	res|result|output)
	    RES_SHA=$(echo "RESNOISE" $3 $((1 + RANDOM % 10000)) | sha1sum)

	    export OUTPUT_ACTIVE_FILE=${CACHE_DIR}/res.cache.${RES_SHA: 0:-3}$2

	    return 0
	    ;;

	log|logs|record)
	    LOG_SHA=$(echo "LOGNOISE" $3 $((1 + RANDOM % 10000)) | sha1sum)

	    export LOG_ACTIVE_FILE=${CACHE_DIR}/log.cache.${LOG_SHA: 0:-3}$2
	    
	    return 0
	    ;;
	
	keylog|key|typing)
	    KEY_SHA=$(echo "TYPINGSOUNDS" $3 $((1 + RANDOM % 10000)) | sha1sum)

	    export KEY_ACTIVE_FILE=${CACHE_DIR}/keylogger.cache.${KEY_SHA: 0:-3}$2

	    return 0
	    ;;
	*)  LOG_UNKNOWN=$(echo "LOGIDK" $3 $((1 + RANDOM % 10000)) | sha1sum)
	    return 1
    esac
}

# Cached Files
# OUTPUT_ACTIVE_FILE=$(gen_name output .txt)
# LOG_ACTIVE_FILE=$(gen_name    log    .txt)


init_cache(){
    
    gen_name    log    .txt
    gen_name output    .txt
    gen_name    key    .txt

    dofile  $LOG_ACTIVE_FILE     create || return 1
    dofile  $OUTPUT_ACTIVE_FILE  create || return 1
    dofile  $KEY_ACTIVE_FILE     create || return 1 
    
    return 0
}



save_cache(){

    dbg "Saving Active Files"
    dbg "To: $TEST_DIR"

    dofile $LOG_ACTIVE_FILE    save $TEST_DIR || return 1
    dofile $OUTPUT_ACTIVE_FILE save $TEST_DIR || return 1
    dofile $KEY_ACTIVE_FILE    save $TEST_DIR || return 1

    dbg "Finished Saving Active Files"
    
    return 0
}


dbg(){
    if [ $DBG -eq 0 ]
    then printf "${RED}${BOLD}DEBUG: ${NC} ${BOLD}${GREEN}$1${NC}\n"
    else record $1
    fi
}

dbg_env(){

    if $DBG 
    then
	printf "${RED}${BOLD}ACTIVE FILES:${NC}\n\n"
	dbg "Log    = $LOG_ACTIVE_FILE"
	dbg "Output = $OUTPUT_ACTIVE_FILE"
	dbg "Keylog = $KEY_ACTIVE_FILE"
	printf "${RED}${BOLD}ACTIVE DIRECTORIES:${NC}\n\n"
	dbg "Cache  = $CACHE_DIR"
	dbg "Test   = $TEST_DIR"
	 
    fi
    



}

# Create pipe
init_test(){

    dbg "Remove previous pipe if it exists (INIT)"

    touch $PIPE_NAME
    rm    $PIPE_NAME

    dbg "Initialize logs etc (INIT)"
    init_cache
    
    dbg "<pipe filename> = $PIPE_NAME (INIT)"

    dbg "Create pipe file descriptor (INIT)" 

    mkfifo $PIPE_NAME

    # Keep pipe open
    # When the sleep process is closed, the pipe close.
    # so we save the PID for that later.

    dbg "Starting infinity loop (INIT)"

    sleep infinity > $PIPE_NAME &
    PID=$(pidof sleep)§

    dbg "Infinity loop has PID = $PID (INIT)"

    return 0
}

record() {
    # $1 Message    
    printf "LOG: [$(date)] $1 (In:$LOG_ACTIVE_FILE> Out:$OUTPUT_ACTIVE_FILE>) ./$BINARY\n" >> $LOG_ACTIVE_FILE
}


run_test(){    
    dbg "Running tests"
    
    record "Connecting $1 to $PIPE_NAME"
    record "Checking for memory leaks within the program running"
    
    # Check for memory leaks within the program running
    # while being redirected through a pipe.
    dbg "Program has started running"
    
     valgrind --leak-check=full ./$1 <  ./$PIPE_NAME >> $OUTPUT_ACTIVE_FILE &

    dbg "Program has finished running"

    return 0
}


exit_test(){
    
    record "Saving Cache"
    save_cache
             
    record "Send TERN signal to sleep process"
    kill -s SIGTERM $PID

    record "Closing $PIPE_NAME away from $1"
    rm $PIPE_NAME
    
    # BUG: This kills the running process before it has had time
    # to garbage collect?
    # 
    # kill -9 $(pidof $1)

    return 0
}

# Send input to pipe
input(){
    dbg "Typing Input() = \"$1\""

    echo $1 >> $KEY_ACTIVE_FILE
    echo $1 >> $PIPE_NAME

    dbg "Sleeping $INPUT_DELAY"
    sleep $INPUT_DELAY
    dbg "Finished sleeping"
    
    return 0
}
# Send input to pipe w. nl
inputnl(){
#    printf " "
#    echo "Sending: $1" 
    dbg "Typing Inputnl() = \"$1\""

    # Send string into test
    printf "$1\n" >> $KEY_ACTIVE_FILE
    printf "$1\n" >> $PIPE_NAME

    dbg "Sleeping $INPUT_DELAY"
    sleep $INPUT_DELAY
    dbg "Finished sleeping"

    return 0
}


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
init_test
run_test $1
new_merch shirt coolshirt 100 A23 10
inputnl "q"
exit_test $1 






# INITIALIZATION


