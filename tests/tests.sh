#!/bin/bash

failed="0"
tile_tester_res="0"
grid_file_res="0"

tile_test() {
    echo -n "[$2] - $1 $(printf '%*s' "$((55-${#1}-${#2}))" | tr ' ' ".")"
    ./test "$2" "$3"
    if [ "$?" -eq 0 ]; then
        echo -e "\033[1;32m"Success"\033[00m"
    else
        echo -e "\033[1;31m"Failed!"\033[00m"
        failed=$(($failed + 1))
    fi
}

grid_test() {
    echo -n "[... $3] - $1 $(printf '%*s' "$((49-${#1}))" | tr ' ' ".")"
    ./test $2 $3 > /dev/null # >> grid_tst_res
    if [ "$?" -eq 0 ]; then
        echo -e "\033[1;32m"Success"\033[00m"
    else
        echo -e "\033[1;31m"Failed!"\033[00m"
        failed=$(($failed + 1))
    fi
}

run_tile_tests() {
    gcc -Wall -o test tile.c tests/tile_tests.c
    ./test
    if [ "$?" -eq 0 ]; then
        echo "ADD / SUB tests done..."
    else
        tile_tester_res="1"
    fi

    gcc -Wall -o test tile.c tests/tile_tester.c
    tile_test "wrong should fail" ?23456789 9
    tile_test "basic 9" 12345678? 9
    tile_test "basic 8" 1234567?9 8
    tile_test "basic 7" 123456?89 7
    tile_test "basic 6" 12345?789 6
    tile_test "basic 5" 1234?6789 5
    tile_test "basic 4" 123?56789 4
    tile_test "basic 3" 12?456789 3
    tile_test "basic 2" 1?3456789 2
    tile_test "basic 1" ?23456789 1
    tile_test "basic 123" 0?0456789 123
    tile_test "basic 456" 1230?0789 456
    tile_test "basic 789" 1234560?0 789
    tile_test "basic TILE_EMPTY" 0000?0000 123456789
    tile_test "advanced 368" 907054021?00000 368
    tile_test "advanced 1" 98765432?23456789 1
    tile_test "advanced even pad 1" ?135790 2468
    tile_test "advanced even pad 2" 135790?0 2468
    tile_test "advanced even pad 9" ?13579000000000 2468
    tile_test "unique 8" ?13579 8
    tile_test "unique 6" 987?123 6
    tile_test "unique 2" 987654?3 2
}

run_grid_tests() {
    gcc -Wall -o test grid.c tile.c ./tests/grid_tester.c
    # touch grid_tst_res

    grid_test "wrong should fail" 551729846496185372278364195367251984845973621912648537754836219689412753123597468 sc
    grid_test "already solved" 531729846496185372278364195367251984845973621912648537754836219689412753123597468 sc
    grid_test "filled but unsolvable" 333329846496185372278364195367251984845973621912648537754836219689412753123597000 sx
    grid_test "simple solvable 1" 501009800090085072008300005360051000045070020902640007004800019000410703020500460 sc
    grid_test "simple solvable 2" 030024000001030705000500800070000000004080057010700900006040300050000020307000060 sc
    grid_test "solvable 2 but compacted input" "030024*5*1030705*3*50080007*9*40800570107009*4*604030005*5*20307*4*60" sc
    grid_test "compacted empty" "*81*" xx
    grid_test "compact easy 123456789" "*6*789456789*5*91234*7*7891567*4*34*6*567345*6*67891*9*5678" sc
    grid_test "compact 123456789" "*6*789056789*5*91234*7*7801567*4*34*6*567345*6*67891*9*5" sc


    # if cmp -s "grid_tst_res" "expected_results"; then
    #     grid_file_res="0"
    # else
    #     grid_file_res="1"
    # fi
    # rm grid_tst_res
}

check_test_reuslts() {
    if [ "$failed" -eq 1 -a "$1" -eq 0 ]; then
        echo -e "\033[1;34m"$2 TESTS DONE - all tests passed"\033[00m"
        failed="0"
    else
       echo -e "\033[1;33m"$2 TESTS DONE - unexpected behaviour"\033[00m"
    fi
}

#---------

run_tile_tests
check_test_reuslts "$tile_tester_res" TILE

# clear

run_grid_tests
check_test_reuslts "$grid_file_res" GRID

echo "more test will go here"