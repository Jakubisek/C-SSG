# C-SSG (C Sudoku Solver & Generator)

## Disclaimer

This project was created for educational purposes and fun, if you are looking for the fastest or most reliable way to solve Sudoku, this is probably not the best choice. Also, be aware that it is still work-in-progress and there are likely a lot of undiscovered bugs, feel free to report them if you encounter any.

## How to use this

1. Compile all the .c files in the root directory, if you have **gcc** installed, you can use:

    ```bash
   gcc -O3 -o cssg tile.c grid.c solver.c main.c
   ```

2. Rewrite the Sudoku you wish to solve from left to right, top to bottom, with empty spaces represented by `0`. <br>
   For example, the following Sudoku:

   ![image](https://github.com/Jakubisek/C-SSG/assets/109464241/3e253891-02cc-464a-8de3-925af412cc84)
   
   would be written as: <br>
   `740000060020000801000000002008000027090400030000016500900070005500600000000320000` <br>
   If the resulting sequence ends with zeroes, they can be all safely omitted,
   additionally, any consecutive group of zeros can be replaced with `*n` where n represents the number of zeroes: <br>
   `74*56002*4801*82008*4270904*33*5165009*37*355006*832`

   > Some specific examples:<br>
   > `*2` will be interpreted as 00 (this is unnecessary, the number of characters remains the same) <br>
   > `1*32` will be interpreted as 10002 (only the first digit after the \* will be used) <br>
   > `*abc56` will be interpreted as 000006 (non-numeric characters will be ignored until the \* can be resolved) <br>
   > `*0` will be interpreted as 0000000000 (10 zeros) <br>

4. Use the obtained representation of the table as the last argument for the compiled binary file: <br>
    
    ```bash
    ./cssg 74*56002*4801*82008*4270904*33*5165009*37*355006*832
    ```

    Optionally, you can also limit the maximum number of solutions that should be displayed (if the puzzle has multiple solutions): <br>

    ```bash
   ./cssg 5 74*56002*4801*82008*4270904*33*5165009*37*355006*832
   ```

    With this option, the program will end the solving process if it finds more than 5 valid solutions. <br>

   > if you use `0` as the solution limit, no solutions will be displayed, but the solving will not terminate until **all the valid solutions** are found, verified, and counted,
   > this could take a long time with some inputs, so it might be necessary to interrupt the program

5. Run the program and look for the message(s) <br> `(n) Solution was found and verified as correct - required m forks.` <br>
    What follows after this are the found solutions.

## Limitations and warnings

While this program (even in its incomplete state) is guaranteed to solve any valid Sudoku (with exactly one solution) regardless of difficulty, it is slow to find all the solutions to those that have a very large number of them. If you for example input an empty grid and request all the solutions, the counting is unlikely to finish in the lifetime of the user.

**If you decide to redirect the output of the program into a file, be warned that it can easily generate gigabytes of data in only a few minutes (likely even more when executed on faster machines) if you set the maximum number to be in the order of millions.**
> if you still want to do so, it might help to enable the macro for compacting solutions (removes the unnecessary grid symbols like `|,+,-,` leaving just the numbers)

## How it works (oversimplified summary)

Each tile of the Sudoku grid is represented as a set with the capacity to hold 9 elements (the numbers from 1 to 9). Following the rules, all the tiles are continuously updated so that they only contain the numbers that are not present in their respective row/column/square. If a tile has only one number it can contain, it is considered solved. Also, if a tile contains a number that is unique in that row/column/square, all the other values are removed, which solves that tile.
> if you want to search for more details, those techniques are commonly known as finding naked and hidden singles (they are considered as the most basic Sudoku solving techniques)

This is not usually enough to solve harder Sudoku so if the program detects it cannot add any more numbers, it will find the first tile with the smallest number of options for the number it can contain and splits the grid into multiple variants where that particular tile is solved (with alle the possible numbers it could contain). In the code, this operation is referred to as a **fork**. Since all possible configurations are accounted for, it is guaranteed that all possible solutions are eventually found (at most, this could take 80 forks, but it is unusual for a solution to take more than 40). Because forked grids are evaluated from the highest element in the tile set to the lowest, solutions will be ordered so that the first solved number is the highest.
> for more information, check the documentation in the header files

## Planned features for future development

_I would like to add those features when I eventually return to this project..._

- support for argument parsed options, colored text and unicode <br> (some features are currently in the experimental branch)

- better interface for generating Sudoku puzzles <br> (with better capabilities for evaluating the difficulty than just the number of forks)

- parallelization \- making the first few forks actually fork the program into independent threads

- more customization options and support for other solving strategies 
