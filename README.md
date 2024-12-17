# Buzz Bomber
 This is basically an arcade shooting game having some uniquesness in its game level such that in this game user which is a spray shoots the bees, then bee converts into honeycomb and his companion bird eats the comb. Resultantly player get the points... and level ups with more difficulty and harder challenges... However , if player fails to fire the bee and bee reaches at bottom position , which then pollinate at that position; which ultimately restricts the player left right movement.

# Steps to run the Game
For Linux OS system
1. Requirements For Compiling (Only need to be run once):
	a. Install GNU G++ Compiler:
	sudo apt-get install g++

	b. Install SFML:
	sudo apt-get install libsfml-dev

2. Compilation Commands (In Order):	
	a. g++ -c buzz.cpp
	b. g++ buzz.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

3. Running The Game:
 a. ./sfml-app



