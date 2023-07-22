#!/bin/bash

tile_test() {
    echo -n "[$1 - $2] "
    ./test "$2" "$3"
    if [ "$?" -eq 0 ]; then
        echo -e "\033[1;32m"Success!"\033[00m"
    else
        echo -e "\033[1;31m"Failed!"\033[00m"
    fi
}

tile_test basic_9 12345678? 9
tile_test basic_5 1234?6789 5
tile_test wrong_1 ?23456789 9
tile_test basic_2 1?3456789 2
tile_test basic_123 0?0456789 123

