# Buzz Bomber
 This is basically an arcade shooting game having some uniquesness in its game level such that in this game user which is a spray shoots the bees, then bee converts into honeycomb and his companion bird eats the comb. Resultantly player get the points... and level ups with more difficulty and harder challenges... However , if player fails to fire the bee and bee reaches at bottom position , which then pollinate at that position; which ultimately restricts the player left right movement.

## Steps to run the Game
For Linux OS system
1. Requirements For Compiling (Only need to be run once):
   <br>
	a. Install GNU G++ Compiler: <br>
	sudo apt-get install g++ <br>
	b. Install SFML: <br>
	sudo apt-get install libsfml-dev
<br> <br>
2. Compilation Commands (In Order):<br>
	a. g++ -c buzz.cpp <br>
	b. g++ buzz.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system  <br>
<br>
4. Running The Game:
   ```bash
   ./sfml-app
```
   





