# `ECE-469 - Artificial Intelligence`
<span style="color:Gray">Cooper Union (Fall 2023)</span>

## About The Project
The goal of this program is to make an AI to play othello (reversi). The algorithm used to choose the best move is alpha-beta search with itterative deepening.

## Setting Up A Board File (`.txt`)
The board file should contain exactly 10 lines of text.
The first 8 lines store the contents of the board:
- `0 = Empty Space`
- `1 = White Space`
- `2 = Black Space`

The 9th line stores the current turn:
- `0 = White's Turn`
- `1 = Black's Turn`

The 10th line stores the AI's time limit:
- `Any Integer (1-127)`

Example Board File (Starting Board With Black's Turn and a 5 Second Computer Time Limit):
>
> 00000000\
> 00000000\
> 00000000\
> 00012000\
> 00021000\
> 00000000\
> 00000000\
> 00000000\
> 1\
> 5

## Playing the Game
Note that when prompted to enter "Y/N" if anything other than Y or y is entered the program will take that as N.

## How To Use (`Linux`)
Build:
- > <span style="color:lightgreen">$</span><span style="color:silver"> make</span>

Run:
- > <span style="color:lightgreen">$</span><span style="color:silver"> ./othello.exe</span>

Build and Run:
- Default run:
    - > <span style="color:lightgreen">$</span><span style="color:silver"> make run</span>
- Clear the terminal before starting the game:
    - > <span style="color:lightgreen">$</span><span style="color:silver"> make crun</span>

Cleanup:
- Warning: this will delete every file int the current directory with the extensions `[.exe, .o, .stackdump, ~, .out, .gch]`
- > <span style="color:lightgreen">$</span><span style="color:silver"> make clean</span>
