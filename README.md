## Primitive Snake game using Ncurses
Small snake game written in C++ using ncurses.
I was more interested in challenging myself to build an OOP game project along with a gameplay loop and frame update loop, which is why the graphical aspects are a bit rough.

## Gameplay Loop
Use ZQSD to control the snake. Eat the eggs 'O' to get bigger. If the snake bites its body the game ends.
An egg 'O' appears at the tip of the snake's tail (not very healthy) everytime it eats an egg. The snake speeds when it eats.

## How to play
Linux only, requires ncurses.
```
cd src
g++ main.cpp -lncurses -o snake
snake
```
