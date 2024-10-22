## Primitive Snake game using Ncurses
The following project is a snake game written in C++ using the ncurses library. 
I was more interested in challenging myself to build an OOP game project along with a gameplay loop and frame update loop, which is why the graphical aspects are a bit rough.

## Gameplay Loop
Use ZQSD to control the snake. Eat the eggs 'O' to get bigger. If the snake bites its body the game ends.
An egg 'O' appears at the tip of the snake's tail (not very healthy) everytime it eats an egg. The snake doesn't (yet) go faster each time it eats.

## How to play (Linux only - Windows support coming)
```
g++ main.cpp -lncurses -o snake
./snake
```
