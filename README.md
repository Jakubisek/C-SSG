# C-SSG (C Sudoku Solver & Generator)

## Disclaimer

This project was created for educational purposes and fun, if you are looking for the fastest or most reliable way to solve Sudoku, this is probably not the best choice. Also be aware that it is still work-in-progress and there are likely a lot of undiscovered bugs, feel free to report them if you encounter any.

## How to use

1. Compile all the .c files in the root directory, if you have **gcc** installed you can use:

    ```bash
   gcc -O3 -o cssg tile.c grid.c solver.c main.c
   ```

2. Rewrite the Sudoku you wish to solve from right to left, up to bottom, empty spaces being represented by `0` example:

```
┏━━━┯━━━┯━━━┳━━━┯━━━┯━━━┳━━━┯━━━┯━━━┓
┃   │   │   ┃   │   │   ┃ 6 │ 9 │ 1 ┃
┠───┼───┼───╂───┼───┼───╂───┼───┼───┨
┃   │ 7 │   ┃   │   │   ┃   │   │   ┃
┠───┼───┼───╂───┼───┼───╂───┼───┼───┨
┃   │   │   ┃   │   │ 5 ┃   │   │   ┃
┣━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┫
┃   │ 3 │ 5 ┃ 9 │   │   ┃   │   │   ┃
┠───┼───┼───╂───┼───┼───╂───┼───┼───┨
┃ 4 │ 1 │   ┃   │ 7 │   ┃   │   │   ┃
┠───┼───┼───╂───┼───┼───╂───┼───┼───┨
┃ 6 │   │   ┃   │ 2 │   ┃ 5 │   │   ┃
┣━━━┿━━━┿━━━╋━━━┿━━━┿━━━╋━━━┿━━━┿━━━┫
┃   │   │ 9 ┃ 1 │   │   ┃   │   │ 4 ┃
┠───┼───┼───╂───┼───┼───╂───┼───┼───┨
┃   │ 4 │   ┃   │ 6 │   ┃   │ 7 │   ┃
┠───┼───┼───╂───┼───┼───╂───┼───┼───┨
┃   │   │   ┃   │ 9 │   ┃ 3 │   │ 8 ┃
┗━━━┷━━━┷━━━┻━━━┷━━━┷━━━┻━━━┷━━━┷━━━┛
```

   Would be written as:
   `000000691070000000000005000035900000410070000600020500009100004040060070000090308` <br>
   If there are any zeros at the end of the sequence like so: `...41300000000` they can be safely omitted.
   You can also replace a string of consecutive zeros with *n where n represents the number of zeroes:

   > \*4 will be interpreted as 0000 <br>
   > 1*32 will be interpreted as 10002 (only the first digit after '\*' will be used) <br>
   > *0 will be interpreted as 0000000000 (10 zeros) <br>

   `*669107*7*55*4359*541*27*46*3205*491*4404006007*590308`

3. Use the obtained representation of the table as the last argument for the compiled file: <br>
    
    ```bash
    ./cssg *669107*7*55*4359*541*27*46*3205*491*4404006007*590308
    ```

    Optionally you can also limit the maximum number of solution that should be displayed (if the puzzle has multiple solutions): <br>

    ```bash
   ./cssg 5 *669107*7*55*4359*541*27*46*3205*491*4404006007*590308
   ```

    With this the program will end the solving process after it finds more than 5 valid solutions. <br>

   > if you use `0` as the solution limit, the program will try to count **all the possible solutions**
   > this might take a long time with some inputs, so it will probably be necessary to interrupt the program, also none of the found solution will be displayed when this feature is used.

4. Run the program and look for the message `(n) Solution was found and verified as correct - required n forks.`
    What follows after are the found solutions.

## Limitations and warnings

While this program (even in its incomplete state) is guaranteed to solve any valid Sudoku (with exactly one solution) regardless of difficulty, it is slow to find all the solutions to those that have a very large number of them. If you for example input an empty grid and request all the solutions, the counting is unlikely to finish in the lifetime of the user.

**If you decide to redirect the output of the program into a file, be warned that it can easily generate gigabytes of data in only a few minutes (likely even more when executed on faster machines) if you set the maximum number to be in order of millions.**
> if you still want to do so, it might help to enable the macro for compacting solutions (removes the unnecessary grid symbols like `|,+,-,` leaving just the numbers)

## How it works (oversimplified summary)

Each tile of the Sudoku grid is represented as a set with the capacity to hold 9 elements (the numbers from 1 to 9). Following the rules, all the tiles are continuously updated so that they only contain the numbers that are not present in their respective row/column/square. If a tile has only one number as its element, it is considered solved. Also if a tile contains a number that is unique in that row/column/square, all the other values are removed.
> if you want more details, those techniques are commonly known as finding naked and hidden singles

This is not usually enough to solve harder Sudoku so if the program detects it cannot add any more numbers, it will find the first tile with the smallest number of options for number it can contain and splits the grid into multiple variants where that particular tile is solved. In the code, this operation is referred to as a **fork**. Since all possible configurations are accounted for, it is guaranteed that all possible solution are eventually found (at most this could take 80 forks, but it is unusual for a solution to take more than 40). Because forked grids are evaluated from the highest element in the set to the lowest, solutions will be ordered so that the first solved number is the highest.

> for more information, check the header files in source code

## Planned features for future development

- support for argument parsed options and colored text and unicode (some features are currently in the experimental branch)

- better interface for generating Sudoku puzzles (with better capabilities for evaluating the difficulty than just the number of forks)

- more customizations and support for other solving strategies 
