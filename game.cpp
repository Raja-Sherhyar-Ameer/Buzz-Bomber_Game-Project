#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<cmath>

using namespace std;
using namespace sf;

// Initializing Dimensions.
const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// Function Prototypes
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, RectangleShape& pellet, int& reducer_count);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void movePlayer(float& player_x, float& player_y, Clock& moveclock, float flowerAttributes[][3], int totalFlower);
void fireBullet(float& bullet_y, float& bullet_x, float& player_x, float& player_y, bool& bullet_exists, int& bulletLeft, int& bulletCan, Clock& bulletClock, int& reducer_count, Sound& bulletSound);
void menuInputs(bool& isinMenu, bool& isinLevel, bool& isinGame, int& selected_option, int& starting_level, int& menuoption, int& leveloption);
void drawMenu(RenderWindow& window, Font& font, bool& isinMenu, char menuOptions[][20], int& selectedOption, int menu_option, int level_option);

// Bee-related functions
void initializeBees(float beeAttributes[][4], int totalBees, int windowWidth);
void drawBees(RenderWindow& window, Sprite& beeSprite, float beeAttributes[][4], int totalBees);
void spawnBee(float beeAttributes[][4], int totalBees, Clock& spawnClock, int& spawnedBeesCount, int windowWidth);
void moveBee(int i, float beeSpeed, int resolutionX, float downtier, float beeAttributes[][4]);
void initializeFastBees(float fastbeeAttributes[][4], int totalFastBees, int resolutionX);
void moveFastBee(int i, float fastBeeSpeed, int resolutionX, float downtier, float fastbeeAttributes[][4]);
void drawFastBees(RenderWindow& window, Sprite& fastbeeSprite, float fastbeeAttributes[][4], int totalFastBees);
void spawn_fastBee(float fastbeeAttributes[][4], int totalFastBees, Clock& spawnClock, int& fastSpawnBeesCount, int windowWidth);

// honey-comb related functions
void drawHoneycombs(RenderWindow& window, Texture& honeycombTexture, Sprite& honeycombSprite, float honeycombAttributes[][3], int totalHoneycombs);
void checkBeeHoneycombCollision(int i, int totalHoneycombs, float beeAttributes[][4], float honeycombAttributes[][3],float downtier, bool &bullet_exists, float bullet_x, float bullet_y);
void checkBeeBulletCollision(int i, bool &bullet_exists, float bullet_x, float bullet_y, float beeAttributes[][4], float honeycombAttributes[][3], int&playerscore, Sound& beesound);
void checkHoneycombBulletCollision(int totalHoneycombs, bool &bullet_exists, float bullet_x, float bullet_y, float honeycombAttributes[][3]);
void checkFastBeeBulletCollision(int i, bool &bullet_exists, float bullet_x, float bullet_y, float beeAttributes[][4], float honeycombAttributes[][3], int&playerscore, Sound& beeSound); 

// flower related functions
void initializeFlowers(float flowerAttributes[][3], int totalFlower);
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, float flowerAttributes[][3], int totalFlower);
void placeFlower(float beeAttributes[][4], float playerX, float flowerAttributes[][3], int totalFlower);

// bird related functions
void handleHummingbird(bool& bird_active, bool& isPaused, float& birdX, float& birdY, int& target_index, int& bird_hit_counter, Clock& movementClock, Clock& pauseClock, float honeycombAttributes[][3], int totalHoneycombs, Sprite& birdSprite, float bullet_x, float bullet_y, int& score);
void handleFastHummingbird(bool& bird_active, bool& isPaused, float& birdX, float& birdY, int& target_index, int& bird_hit_counter, Clock& movementClock, Clock& pauseClock, float honeycombAttributes[][3], int totalHoneycombs, Sprite& birdSprite, float bullet_x, float bullet_y, int& score);
void drawbird(RenderWindow& window, Sprite& birdSprite);
// game-resets
void resetGame(float& player_x, float& player_y, float beeAttributes[][4], int totalBees, float fastbeeAttributes[][4], int totalFastBees, float honeycombAttributes[][3], int totalHoneycombs, float flowerAttributes[][3], int totalFlower, int& bulletLeft, int& bulletCan, int& reducer_count, int& spawnedBeesCount, int& fastSpawnBeesCount, bool& bullet_exists, float redcombAttribute[][3], int redcomb_num, int& starting_level, bool& bird_active, float& birdX, float& birdY, int& target_index);

int main()
{
    srand(time(0));

    // Declaring RenderWindow.
    RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);
    window.setPosition(Vector2i(500, 200));

    // Background Music.
    Music bgMusic;
    if (!bgMusic.openFromFile("Music/Music3.ogg")) {
        cout << "Error: Could not load music file!" << endl;
    }
    bgMusic.setVolume(0);
    bgMusic.setLoop(true);
    bgMusic.play();

    // Player initialization.
    float player_x = (gameRows / 2) * boxPixelsX;
    float player_y = (gameColumns - 4) * boxPixelsY;
    Clock moveClock;
    Texture playerTexture;
    Sprite playerSprite;
    playerTexture.loadFromFile("Textures/spray.png");
    playerSprite.setTexture(playerTexture);

    // Bullet initialization.
    float bullet_x = player_x, bullet_y = player_y;
    bool bullet_exists = false;
    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(3, 3);
    SoundBuffer bulletBuffer;
    if (!bulletBuffer.loadFromFile("Sound Effects/fire.wav")) {
        cout<<"Failed to load sound file!"<<endl;
        return -1;
    }
    Sound bulletSound;
    bulletSound.setBuffer(bulletBuffer);

    // Ground initialization.
    RectangleShape groundRectangle(Vector2f(960, 64));
    groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
    groundRectangle.setFillColor(Color::Green);

    // Spray Can and Lives Initialization.
    RectangleShape fullCan_Rectangle(Vector2f(20.0f, 24.5f));
    fullCan_Rectangle.setFillColor(Color::White);
    Texture bulletCan_design;
    Sprite canSprite;
    bulletCan_design.loadFromFile("Textures/spray.png");
    canSprite.setTexture(bulletCan_design);
    int bulletLeft = 56, reducer = 0, bulletCan = 3;

    RectangleShape sprayleft_rectangle(Vector2f(20.f, 28.5f));
    sprayleft_rectangle.setFillColor(Color::White);

    // Menu initialization.
    int menuitem = 4, sub_level = 4, starting_level = 1, selectedOption = 0;
    bool isIn_menu = true, isLevelmenu = false, isinGame = false;

    char menuOptions[menuitem][20] = {"Start Game", "Select Level", "Highscores", "Exit"};
    char level_Options[sub_level][20] = {"Easy", "Medium", "Difficult", "Boss-Fight"};

    Font menu_font;
    if (!menu_font.loadFromFile("Fonts/PressStart2P-Regular.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }
    
    // Score display
    Text scoreText;
	scoreText.setFont(menu_font);
	scoreText.setCharacterSize(18);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(resolutionX - 220, resolutionY - 30); 
	int playerscore=0;

    // Bee-related initialization
    int totalBees = starting_level==1 || starting_level==3 ? 15: 20;
    float beeSpeed=4.0f;
    float downtier=30.0f;
	float beeAttributes[totalBees][4]; // [beeIndex][0=x, 1=y, 2=direction, 3=active]
    Texture beeTexture;
    if (!beeTexture.loadFromFile("Textures/Regular_bee.png")) {
        cout << "Error loading bee texture!" << endl;
        return -1;
    }
    Sprite beeSprite;
    beeSprite.setTexture(beeTexture);
    beeSprite.setScale(0.8f, 0.8f);
    initializeBees(beeAttributes, totalBees, resolutionX);
    Clock moveClockBees[totalBees], spawnClock;
    int spawnedBeesCount = 0;
    
    int totalFastBees = 20;
	float fastbeeAttributes[totalFastBees][4] = {};
	int fastSpawnBeesCount = 0;
	Clock f_spawnClock;
    float fast_beeSpeed=8.0f;
    Sprite fastbeeSprite;
 	Clock fastbeemove[totalFastBees];
    Texture fastbeeTexture;
    if (!fastbeeTexture.loadFromFile("Textures/Fast_bee.png")) {
        cout << "Error loading bee texture!" << endl;
        return -1;
    }
    fastbeeSprite.setTexture(fastbeeTexture);
    initializeFastBees(fastbeeAttributes, totalFastBees, resolutionX);
    fastbeeSprite.setScale(0.8f, 0.8f);
    SoundBuffer beeBuffer;
    if (!beeBuffer.loadFromFile("Sound Effects/hit.wav")) {
        cout<<"Failed to load sound file!"<<endl;
        return -1;
    }
    Sound beeSound;
    beeSound.setBuffer(beeBuffer);
    
    // honeycomb attributes
	int pregenerated =3;
	int totalHoneycombs = 35; // Adjust as needed
	float honeycombAttributes[totalHoneycombs][3] = {};    // [honeycomb-index][ 0=x 1=y 2=active]
	Texture honeycombTexture;
	if (!honeycombTexture.loadFromFile("Textures/honeycomb.png")) {
		cout << "Error loading honeycomb texture!" << endl;
		return -1;
	}
	Sprite honeycombSprite;
	honeycombSprite.setTexture(honeycombTexture);
	honeycombSprite.setScale(0.5f, 0.5f);
	int redcombs_num=20;
	float redcombAttributes[redcombs_num][3]={};
	 Texture redcombTexture;
	if (!redcombTexture.loadFromFile("Textures/honeycomb_red.png")) {
		cout << "Error loading honeycomb texture!" << endl;
		return -1;
	}
	Sprite redcombSprite;
	redcombSprite.setTexture(redcombTexture);
	redcombSprite.setScale(0.5f, 0.5f);
	

	
	// flower-attributes
	const int totalFlower=20;
	float flowerAttributes[totalFlower][3];
	Texture flowerTexture;
	if(!flowerTexture.loadFromFile("Textures/obstacles.png")){
		cout<<"Error loading the flower textures"<<endl;
	}
	Sprite flowerSprite;
	flowerSprite.setTexture(flowerTexture);
	flowerSprite.setScale(1.5f, 1.5f);
	initializeFlowers(flowerAttributes, totalFlower);
	
	// bird attributes
	bool bird_active = true; 
	bool isPaused = false;   
	float birdX = 100.0f;    
	float birdY = 100.0f;    
	int target_index = -1;   
	int bird_hit_counter = 0; 
	Clock movementClock;     
	Clock pauseClock;
	Sprite birdSprite; 
	Texture birdTexture;
	if(!birdTexture.loadFromFile("Textures/bird.png")){
		cout<<"Error loading the flower textures"<<endl;
	}
	birdSprite.setTexture(birdTexture);
	birdSprite.setScale(1.2f,1.2f);

    // Game loop 
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) {
                window.close();
                return 0;
            }
        }

        window.clear();

        if (isIn_menu || isLevelmenu) {
            menuInputs(isIn_menu, isLevelmenu, isinGame, selectedOption, starting_level, menuitem, sub_level);
            if (isIn_menu) {
                drawMenu(window, menu_font, isIn_menu, menuOptions, selectedOption, menuitem, sub_level);
            } else if (isLevelmenu) {
                drawMenu(window, menu_font, isIn_menu, level_Options, selectedOption, menuitem, sub_level);
            }
        } else if(isinGame){
        		
        		if(starting_level==1 || starting_level==3){
        			totalBees=20;
        		}
        		else
        		totalBees=15;

            movePlayer(player_x, player_y, moveClock, flowerAttributes, totalFlower);
            drawPlayer(window, player_x, player_y, playerSprite, sprayleft_rectangle, reducer);

            // Bee logic
            spawnBee(beeAttributes, totalBees, spawnClock, spawnedBeesCount, resolutionX);
           
           
			for (int i = 0; i < totalBees; ++i) {
				if (beeAttributes[i][3] == 1) { // Only move and check active bees
					// Slow down the bee movement by checking the time elapsed since last update
					if (moveClockBees[i].getElapsedTime().asMilliseconds() > 40) {  // Adjust this value to slow down or speed up
						moveClockBees[i].restart(); // Restart the clock for the next update
						
						moveBee(i, beeSpeed, resolutionX, downtier, beeAttributes);
						checkBeeHoneycombCollision(i, totalHoneycombs, beeAttributes, honeycombAttributes, downtier, bullet_exists, bullet_x, bullet_y);
						checkBeeHoneycombCollision(i, redcombs_num, beeAttributes, redcombAttributes, downtier, bullet_exists, bullet_x, bullet_y);
						checkBeeBulletCollision(i, bullet_exists, bullet_x, bullet_y, beeAttributes, honeycombAttributes,playerscore , beeSound);						
					}
				}
				checkHoneycombBulletCollision(totalHoneycombs, bullet_exists, bullet_x, bullet_y, honeycombAttributes);
			}
			

			
            handleHummingbird(bird_active, isPaused, birdX, birdY, target_index, bird_hit_counter, movementClock, pauseClock,
                honeycombAttributes, totalHoneycombs, birdSprite, bullet_x, bullet_y, playerscore);

			drawbird(window, birdSprite);
			drawBees(window, beeSprite, beeAttributes, totalBees);
			drawHoneycombs(window, honeycombTexture, honeycombSprite, honeycombAttributes, totalHoneycombs);
			

			drawFlowers(window, flowerSprite, flowerAttributes, totalFlower);
			placeFlower(beeAttributes, player_x, flowerAttributes, totalFlower);
			
            fireBullet(bullet_y, bullet_x, player_x, player_y, bullet_exists, bulletLeft, bulletCan, bulletClock, reducer, bulletSound);
            if (bullet_exists) {
                moveBullet(bullet_y, bullet_exists, bulletClock);
                drawBullet(window, bullet_x, bullet_y, bulletSprite);
            } else {
                bullet_x = player_x;
                bullet_y = player_y;
            }
            
            for (static int l = 0; l < pregenerated; l++) {
					honeycombAttributes[l][0] = rand() % resolutionX;
					honeycombAttributes[l][1] = rand() % (resolutionY - 140);
					honeycombAttributes[l][2] = 1;

					if (honeycombAttributes[l][2] == 1) {
						honeycombSprite.setPosition(honeycombAttributes[l][0], honeycombAttributes[l][1]);
						window.draw(honeycombSprite);
					}
				}
			

            window.draw(groundRectangle);

            // Draw spray cans.
            for (int i = 0; i < bulletCan; i++) {
                float xPosition = i * 56;
                float yPosition = resolutionY - 64.0f;
                float can_X = 22.5f + i * 56;
                float can_Y = resolutionY - 26.0f;

                canSprite.setPosition(xPosition, yPosition);
                fullCan_Rectangle.setPosition(can_X, can_Y);

                window.draw(canSprite);
                window.draw(fullCan_Rectangle);
            }
            
            if (starting_level == 2){
            		totalFastBees=5;   redcombs_num=5;  pregenerated=9;
				spawn_fastBee(fastbeeAttributes, totalFastBees, f_spawnClock, fastSpawnBeesCount, resolutionX);

				for (int i = 0; i < totalFastBees; ++i) {
					if (fastbeeAttributes[i][3] == 1) { // Only move active bees
						if (fastbeemove[i].getElapsedTime().asMilliseconds() > 40) {
						    fastbeemove[i].restart();
						    moveFastBee(i, fast_beeSpeed, resolutionX, downtier, fastbeeAttributes);
						    checkFastBeeBulletCollision(i, bullet_exists, bullet_x, bullet_y, fastbeeAttributes, redcombAttributes,playerscore ,beeSound);
						}
					}
					checkHoneycombBulletCollision(redcombs_num, bullet_exists, bullet_x, bullet_y, redcombAttributes);
				}
				
				handleFastHummingbird(bird_active, isPaused, birdX, birdY, target_index, bird_hit_counter, movementClock, pauseClock, redcombAttributes, redcombs_num, birdSprite, bullet_x, bullet_y, playerscore);
				drawFastBees(window, fastbeeSprite, fastbeeAttributes, totalFastBees); // Draw active bees
				drawHoneycombs(window, redcombTexture, redcombSprite, redcombAttributes, redcombs_num);
				
				for (static int m = 0; m < pregenerated; m++) {
					honeycombAttributes[m][0] = rand() % resolutionX;
					honeycombAttributes[m][1] = rand() % (resolutionY - 140);
					honeycombAttributes[m][2] = 1;

					if (honeycombAttributes[m][2] == 1) {
						honeycombSprite.setPosition(honeycombAttributes[m][0], honeycombAttributes[m][1]);
						window.draw(honeycombSprite);
					}
				}
				
				drawFlowers(window, flowerSprite, flowerAttributes, totalFlower);
				placeFlower(fastbeeAttributes, player_x, flowerAttributes, totalFlower);

			}
			
			if (starting_level == 3){
            		totalFastBees=10;     redcombs_num=10;  pregenerated=15;
				spawn_fastBee(fastbeeAttributes, totalFastBees, f_spawnClock, fastSpawnBeesCount, resolutionX);

				for (int i = 0; i < totalFastBees; ++i) {
					if (fastbeeAttributes[i][3] == 1) { // Only move active bees
						if (fastbeemove[i].getElapsedTime().asMilliseconds() > 40) {
						    fastbeemove[i].restart();
						    moveFastBee(i, fast_beeSpeed, resolutionX, downtier, fastbeeAttributes);
						    checkFastBeeBulletCollision(i, bullet_exists, bullet_x, bullet_y, fastbeeAttributes, redcombAttributes,playerscore, beeSound);
						}
					}
					checkHoneycombBulletCollision(redcombs_num, bullet_exists, bullet_x, bullet_y, redcombAttributes);
				}
				
				handleFastHummingbird(bird_active, isPaused, birdX, birdY, target_index, bird_hit_counter, movementClock, pauseClock, redcombAttributes, redcombs_num, birdSprite, bullet_x, bullet_y, playerscore);
				drawFastBees(window, fastbeeSprite, fastbeeAttributes, totalFastBees); // Draw active bees
				drawHoneycombs(window, redcombTexture, redcombSprite, redcombAttributes, redcombs_num);
				
				for (static int n = 0; n < pregenerated; n++) {
					honeycombAttributes[n][0] = rand() % resolutionX;
					honeycombAttributes[n][1] = rand() % (resolutionY - 140);
					honeycombAttributes[n][2] = 1;

					if (honeycombAttributes[n][2] == 1) {
						honeycombSprite.setPosition(honeycombAttributes[n][0], honeycombAttributes[n][1]);
						window.draw(honeycombSprite);
					}
				}
				drawFlowers(window, flowerSprite, flowerAttributes, totalFlower);
				placeFlower(fastbeeAttributes, player_x, flowerAttributes, totalFlower);
				
			}
			
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				// Reset all game-related variables when returning to the menu
				resetGame(player_x, player_y, beeAttributes, totalBees, fastbeeAttributes, totalFastBees,
						  honeycombAttributes, totalHoneycombs, flowerAttributes, totalFlower, bulletLeft,
						  bulletCan, reducer, spawnedBeesCount, fastSpawnBeesCount, bullet_exists,
						  redcombAttributes, redcombs_num, starting_level, bird_active, birdX, birdY, target_index);

				isLevelmenu = false;
				isinGame = false;  
				isIn_menu = true;  
				selectedOption = 0; 
			}

        	
            scoreText.setString("Score: " + to_string(playerscore));
            window.draw(scoreText);
        }


        window.display(); 
    }

    return 0;

}

void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, RectangleShape& pellet, int& reducer_count) {
    playerSprite.setPosition(player_x, player_y);
    // If reducer_count > 0 and less than or equal to 7, reduce pellet size.
    if (reducer_count > 0 && reducer_count <= 7) {
        pellet.setSize(Vector2f(20.f, 28.5f - reducer_count * 4.07f));
        pellet.setPosition(player_x + 21.5, player_y + 33.5 + 4.07f * reducer_count);
    } else {
        pellet.setSize(Vector2f(20.f, 28.5f)); // Default size
        pellet.setPosition(player_x + 21.5, player_y + 33.5);
    }
    window.draw(playerSprite);
    window.draw(pellet);
}


void movePlayer(float& player_x, float& player_y, Clock& moveclock, float flowerAttributes[][3], int totalFlower) {
    float playerSpeed = 6.0f;
    float playerWidth = 64;
    float playerHeight = 64; 

    // Variables to determine dynamic borders
    float rightScreenBorder = resolutionX; 
    float leftScreenBorder = 0;           

    // Check flowers to dynamically adjust screen borders
    for (int i = 0; i < totalFlower; i++) {
        if (flowerAttributes[i][2]) { // If flower is active
            float flowerX = flowerAttributes[i][0];
            float flowerY = flowerAttributes[i][1];
            float flowerWidth = 48; 
            float flowerHeight = 48; 

            if (player_y + playerHeight > flowerY && 
                player_y < flowerY + flowerHeight) {
                
                // Adjust right border
                if (flowerX > player_x && flowerX < rightScreenBorder) {
                    rightScreenBorder = flowerX;
                }
                // Adjust left border
                if (flowerX + flowerWidth < player_x + playerWidth && flowerX + flowerWidth > leftScreenBorder) {
                    leftScreenBorder = flowerX + flowerWidth;
                }
            }
        }
    }

    // Movement with dynamic screen borders
    if (Keyboard::isKeyPressed(Keyboard::Right) &&
        player_x + playerWidth < rightScreenBorder && // Dynamic right border
        moveclock.getElapsedTime().asMilliseconds() > 30) {
        player_x += playerSpeed;
        moveclock.restart();
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) &&
        player_x > leftScreenBorder && // Dynamic left border
        moveclock.getElapsedTime().asMilliseconds() > 30) {
        player_x -= playerSpeed;
        moveclock.restart();
    }
}



void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    bulletClock.restart();
    bullet_y -= 16;
    if (bullet_y < -32)
        bullet_exists = false;
}

void drawBullet(sf::RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet_x, bullet_y);
    window.draw(bulletSprite);
}

void fireBullet(float& bullet_y, float& bullet_x, float& player_x, float& player_y, bool& bullet_exists, int& bulletLeft, int& bulletCan, Clock& bulletClock, int& reducer_count, Sound& bulletSound) {
    static int bulletFireCounter = 0; // Tracks bullets fired since last reduction

    static bool spaceLocked = false; // New static variable to track space key state

    // Fire bullet logic
    if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists && bulletLeft > 0 && !spaceLocked) {
        bullet_x = player_x + 12;
        bullet_y = player_y;
        bullet_exists = true;
        bulletLeft--;
        bulletClock.restart();
        spaceLocked = true; // Lock the space key after firing
        
        // Play the bullet fire sound
        bulletSound.play();
    	
    	// Check if 8 bullets have been fired
        if (bulletFireCounter == 8) {
            bulletFireCounter = 0;  // Reset counter
            reducer_count++;        // Reduce the spray rectangle
        }
        bulletFireCounter++;
    
    }

    // Unlock space key when it's released
    if (!Keyboard::isKeyPressed(Keyboard::Space)) {
        spaceLocked = false;
    }

    // Refill bullets when all bullets are used
    if (bulletLeft <= 0 && bulletCan > 0) {
        bulletLeft = 56;            
        reducer_count = 0;          
        bulletFireCounter = 0;      
        bulletCan--;                
    }

    // No bullets or spray cans left
    if (bulletCan <= 0 && bulletLeft <= 0) {
        bulletLeft = 0;
    }
}


void menuInputs(
    bool& isinMenu, bool& isinLevel, bool& isinGame, int& selected_option,
    int& starting_level, int& menuoption, int& leveloption) {

    static bool keyLock = false;

    if (isinMenu) {
        if (Keyboard::isKeyPressed(Keyboard::Up) && !keyLock) {
            keyLock = true;
            selected_option = (selected_option > 0) ? selected_option - 1 : menuoption - 1;
        } else if (Keyboard::isKeyPressed(Keyboard::Down) && !keyLock) {
            keyLock = true;
            selected_option = (selected_option < menuoption - 1) ? selected_option + 1 : 0;
        } else if (Keyboard::isKeyPressed(Keyboard::Enter) && !keyLock) {
            keyLock = true;
            if (selected_option == 0) { // Start Game
                isinMenu = false;
                isinGame = true;
            } else if (selected_option == 1) { // Select Level
                isinMenu = false;
                isinLevel = true;
                selected_option = 0; // Reset level menu selection
            } else if (selected_option == 3) { // Exit
                exit(0);
            }
        }
    } else if (isinLevel) {
        if (Keyboard::isKeyPressed(Keyboard::Up) && !keyLock) {
            keyLock = true;
            selected_option = (selected_option > 0) ? selected_option - 1 : leveloption - 1;
        } else if (Keyboard::isKeyPressed(Keyboard::Down) && !keyLock) {
            keyLock = true;
            selected_option = (selected_option < leveloption - 1) ? selected_option + 1 : 0;
        } else if (Keyboard::isKeyPressed(Keyboard::Enter) && !keyLock) {
            keyLock = true;
            starting_level = selected_option + 1; // Set selected level
            isinLevel = false;
            isinGame = true; // Transition to gameplay
        } else if (Keyboard::isKeyPressed(Keyboard::Escape) && !keyLock) {
            keyLock = true;
            isinLevel = false;
            isinMenu = true; // Return to the main menu
            selected_option = 0; // Reset selection for main menu
        }
    }

    // Reset key lock when keys are released
    if (!Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down) &&
        !Keyboard::isKeyPressed(Keyboard::Enter) && !Keyboard::isKeyPressed(Keyboard::Escape)) {
        keyLock = false;
    }
}




void drawMenu(RenderWindow& window, Font& font, bool& isinMenu, char options[][20],
              int& selectedOption, int menuOptionCount, int levelOptionCount) {
    int optionCount = isinMenu ? menuOptionCount : levelOptionCount;

    for (int i = 0; i < optionCount; i++) {
        Text text(options[i], font, 20);
        text.setPosition(resolutionX / 3, resolutionY / 4 + i * 40);
        text.setFillColor(i == selectedOption ? Color::Red : Color::White);
        window.draw(text);
    }
}


void initializeBees(float beeAttributes[][4], int totalBees, int windowWidth) {
    for (int i = 0; i < totalBees; ++i) {
        beeAttributes[i][0] = (i % 2 == 0) ? 0 : windowWidth - 50; // x_position: left or right
        beeAttributes[i][1] = 0;                                  // y_position: start at the top
        beeAttributes[i][2] = (i % 2 == 0) ? 1 : -1;              // direction: 1 for right, -1 for left
        beeAttributes[i][3] = 0;                                  // active state: 0 (inactive initially)
    }
}

void drawBees(RenderWindow& window, Sprite& beeSprite, float beeAttributes[][4], int totalBees) {
    for (int i = 0; i < totalBees; ++i) {
        // Skip inactive bees
        if (beeAttributes[i][3] == 0) continue;

        // Set position of the sprite
        beeSprite.setPosition(beeAttributes[i][0], beeAttributes[i][1]);

        // Draw the sprite
        window.draw(beeSprite);
    }
}

void spawnBee(float beeAttributes[][4], int totalBees, Clock& spawnClock, int& spawnedBeesCount, int windowWidth) {
    if (spawnClock.getElapsedTime().asSeconds() < 1.5f) return; // Spawn a new bee every 1.5 seconds
    spawnClock.restart();

    // Spawn a new bee only if the total spawned count is less than the total number of bees
    if (spawnedBeesCount < totalBees) {
        beeAttributes[spawnedBeesCount][3] = 1; // Activate the bee
        beeAttributes[spawnedBeesCount][0] = (spawnedBeesCount % 2 == 0) ? 0 : windowWidth - 50; // Reset x_position
        beeAttributes[spawnedBeesCount][1] = 0; // Reset y_position
        spawnedBeesCount++; // Increment the spawn counter
    }
}

void createHoneycomb(float honeycombX[], float honeycombY[], int honeycombActive[], float bee_x, float bee_y, int totalHoneycombs) {
    for (int i = 0; i < totalHoneycombs; ++i) {
        if (honeycombActive[i] == 0) { // Look for an inactive honeycomb
            honeycombX[i] = bee_x;    // Set x position based on bee's position
            honeycombY[i] = bee_y;    // Set y position based on bee's position
            honeycombActive[i] = 1;   // Mark as active
            break;                    // Exit the loop after placing one honeycomb
        }
    }
}

void drawHoneycombs(RenderWindow& window, Texture& honeycombTexture, Sprite& honeycombSprite, float honeycombAttributes[][3], int totalHoneycombs) {
    for (int i = 0; i < totalHoneycombs; ++i) {
        if (honeycombAttributes[i][2] == 1) { // Check if the honeycomb is active
            honeycombSprite.setPosition(honeycombAttributes[i][0], honeycombAttributes[i][1]);
            window.draw(honeycombSprite);
        }
    }
}

void moveBee(int i, float beeSpeed, int resolutionX, float downtier, float beeAttributes[][4]) {
    if (beeAttributes[i][2] == 1) { // Move right
        beeAttributes[i][0] += beeSpeed;
        if (beeAttributes[i][0] >= resolutionX - 32) { // Reached right edge
            beeAttributes[i][2] = -1; // Change direction
            beeAttributes[i][1] += downtier; // Descend
        }
    }
    else{ // Move left
        beeAttributes[i][0] -= beeSpeed;
        if (beeAttributes[i][0] <= 0) { // Reached left edge
            beeAttributes[i][2] = 1; // Change direction
            beeAttributes[i][1] += downtier; // Descend
        }
    }

}

void checkBeeHoneycombCollision(int i, int totalHoneycombs, float beeAttributes[][4], float honeycombAttributes[][3], float downtier, bool &bullet_exists, float bullet_x, float bullet_y) {
    for (int j = 0; j < totalHoneycombs; ++j) {
        if (honeycombAttributes[j][2] == 1) { // Active honeycomb
            // Bee collides with honeycomb
            if (beeAttributes[i][0] + 32 > honeycombAttributes[j][0] &&
                beeAttributes[i][0] < honeycombAttributes[j][0] + 32 &&
                beeAttributes[i][1] + 32 > honeycombAttributes[j][1] &&
                beeAttributes[i][1] < honeycombAttributes[j][1] + 32) {
                
                beeAttributes[i][2] = -beeAttributes[i][2]; // Change direction
                beeAttributes[i][1] += downtier; // Descend
                break;
            }

            // Bullet collides with honeycomb
            if (bullet_exists &&
                bullet_x + 16 >= honeycombAttributes[j][0] &&
                bullet_x <= honeycombAttributes[j][0] + 32 &&
                bullet_y + 16 >= honeycombAttributes[j][1] &&
                bullet_y <= honeycombAttributes[j][1] + 32) {
                
                honeycombAttributes[j][2] = 0; // Deactivate honeycomb
                bullet_exists = false;
            }
        }
    }
}

void checkBeeBulletCollision(int i, bool &bullet_exists, float bullet_x, float bullet_y, float beeAttributes[][4], float honeycombAttributes[][3], int&playerscore, Sound& beesound) {
    // Only check active bees
    if (beeAttributes[i][3] == 1 && bullet_exists &&
        bullet_x + 16 >= beeAttributes[i][0] &&
        bullet_x <= beeAttributes[i][0] + 32 &&
        bullet_y + 16 >= beeAttributes[i][1] &&
        bullet_y <= beeAttributes[i][1] + 32) {
        
        // Deactivate bee immediately
        beeAttributes[i][3] = 0;
        beesound.play();
        playerscore+=100;
        
        // Find first inactive honeycomb to activate
        for (int j = 0; j < 20; j++) {
            if (honeycombAttributes[j][2] == 0) {
                honeycombAttributes[j][2] = 1; 
                honeycombAttributes[j][0] = beeAttributes[i][0];
                honeycombAttributes[j][1] = beeAttributes[i][1];
                break;
            }
        }
        
        bullet_exists = false; 
    }
}

void checkHoneycombBulletCollision(int totalHoneycombs, bool &bullet_exists, float bullet_x, float bullet_y, float honeycombAttributes[][3]) {
    for (int j = 0; j < totalHoneycombs; ++j) {
        if (honeycombAttributes[j][2] == 1) { // Active honeycomb
            if (bullet_exists &&
                bullet_x + 16 >= honeycombAttributes[j][0] &&
                bullet_x <= honeycombAttributes[j][0] + 32 &&
                bullet_y + 16 >= honeycombAttributes[j][1] &&
                bullet_y <= honeycombAttributes[j][1] + 32) {
                
                // Immediately deactivate honeycomb
                honeycombAttributes[j][2] = 0; 
                bullet_exists = false;
                break; // Exit after first collision
            }
        }
    }
}
void initializeFlowers(float flowerAttributes[][3], int totalFlower) {
    for (int i = 0; i < totalFlower; i++) {
        flowerAttributes[i][0] = 0;                 
        flowerAttributes[i][1] = resolutionY - 110; 
        flowerAttributes[i][2] = 0;                
    }
}
void drawFlowers(RenderWindow& window, Sprite& flowerSprite, float flowerAttributes[][3], int totalFlower) {
    bool firstFlowerDrawn = false;
    
    // Precalculate scaled width (32 * 1.5 = 48)
    const float scaledWidth = 48;
    
    for (int i = 0; i < totalFlower; i++) {
        if (flowerAttributes[i][2]) { 
            // Always draw first flower at both screen borders
            if (!firstFlowerDrawn) {
                // Draw at left border
                flowerSprite.setPosition(0, flowerAttributes[i][1]);
                window.draw(flowerSprite);
                
                // Draw at right border
                flowerSprite.setPosition(resolutionX - scaledWidth, flowerAttributes[i][1]);
                window.draw(flowerSprite);
                
                firstFlowerDrawn = true;
            }
            else{ 
		        // Draw the actual flower at its calculated position
		        flowerSprite.setPosition(flowerAttributes[i][0], flowerAttributes[i][1]);
		        window.draw(flowerSprite);
            }
        }
    }
}
void placeFlower(float beeAttributes[][4], float playerX, float flowerAttributes[][3], int totalFlower) {
   for (int i = 0; i < totalFlower; i++) {
       if (beeAttributes[i][3] && beeAttributes[i][1] > resolutionY - 160) {
           // Find first empty flower slot
           for (int j = 0; j < totalFlower; j++) {
               if (!flowerAttributes[j][2]) {
                   float newFlowerX = beeAttributes[i][0];
                   
                   // Check and adjust for nearby flowers
                   for (int k = 0; k < totalFlower; k++) {
                       if (flowerAttributes[k][2]) {
                           float diff = flowerAttributes[k][0] - newFlowerX;
                           diff = (diff < 0) ? -diff : diff;
                           
                           if (diff < 48) {
                               // Adjust x-position based on player position
                               newFlowerX += (playerX < beeAttributes[i][0]) ? -48 : 48;
                               
                               // Screen wrapping
                               if (newFlowerX < 0) newFlowerX += resolutionX;
                               if (newFlowerX >= resolutionX) newFlowerX -= resolutionX;
                           }
                       }
                   }
                   
                   // Place flower
                   beeAttributes[i][3] = 0;
                   flowerAttributes[j][0] = newFlowerX;
                   flowerAttributes[j][2] = 1;
                   break;
               }
           }
       }
   }
}
void initializeFastBees(float fastbeeAttributes[][4], int totalFastBees, int resolutionX) {
    for (int i = 0; i < totalFastBees; ++i) {
        fastbeeAttributes[i][0] = (rand() % 2 == 0) ? 0 : resolutionX - 50; // x_position: left or right
        fastbeeAttributes[i][1] = 0;                                       // y_position: start at the top
        fastbeeAttributes[i][2] = (fastbeeAttributes[i][0] == 0) ? 1 : -1; // direction: 1 = right, -1 = left
        fastbeeAttributes[i][3] = 1;                                       // active state: 1 (active initially)
    }
}
void drawFastBees(RenderWindow& window, Sprite& fastbeeSprite, float fastbeeAttributes[][4], int totalFastBees) {
    for (int i = 0; i < totalFastBees; ++i) {
        if (fastbeeAttributes[i][3] == 1) { // Draw only active bees
            fastbeeSprite.setPosition(fastbeeAttributes[i][0], fastbeeAttributes[i][1]);
            window.draw(fastbeeSprite);
        }
    }
}
void moveFastBee(int i, float fastBeeSpeed, int resolutionX, float downtier, float fastbeeAttributes[][4]) {
    if (fastbeeAttributes[i][3] == 1) { // Only move active bees
        if (fastbeeAttributes[i][2] == 1) { // Move right
            fastbeeAttributes[i][0] += fastBeeSpeed;
            if (fastbeeAttributes[i][0] >= resolutionX - 32) {
                fastbeeAttributes[i][2] = -1; // Change direction
                fastbeeAttributes[i][1] += downtier; // Descend
            }
        } else { // Move left
            fastbeeAttributes[i][0] -= fastBeeSpeed;
            if (fastbeeAttributes[i][0] <= 0) {
                fastbeeAttributes[i][2] = 1; // Change direction
                fastbeeAttributes[i][1] += downtier; // Descend
            }
        }
    }
}
void spawn_fastBee(float fastbeeAttributes[][4], int totalFastBees, Clock& spawnClock, int& fastSpawnBeesCount, int windowWidth) {
    
    if (spawnClock.getElapsedTime().asSeconds() < 1.5f) return; 
    spawnClock.restart(); 


    if (fastSpawnBeesCount < totalFastBees) {
        fastbeeAttributes[fastSpawnBeesCount][0] = (fastSpawnBeesCount % 2 == 0) ? 0 : windowWidth - 50; 
        fastbeeAttributes[fastSpawnBeesCount][1] = 0;  
        fastbeeAttributes[fastSpawnBeesCount][2] = (fastSpawnBeesCount % 2 == 0) ? 1 : -1; 
        fastbeeAttributes[fastSpawnBeesCount][3] = 1;  

        fastSpawnBeesCount++; 
    }
}
void resetGame(
    float& player_x, float& player_y, float beeAttributes[][4], int totalBees,
    float fastbeeAttributes[][4], int totalFastBees, float honeycombAttributes[][3],
    int totalHoneycombs, float flowerAttributes[][3], int totalFlower, int& bulletLeft,
    int& bulletCan, int& reducer_count, int& spawnedBeesCount, int& fastSpawnBeesCount,
    bool& bullet_exists, float redcombAttribute[][3], int redcomb_num, int& starting_level,
    bool& bird_active, float& birdX, float& birdY, int& target_index) {

    // Reset the game state (player, bullets, bees, etc.)
    starting_level = 1;
    player_x = (gameRows / 2) * boxPixelsX;
    player_y = (gameColumns - 4) * boxPixelsY;

    // Reset entities
    initializeBees(beeAttributes, totalBees, resolutionX);
    spawnedBeesCount = 0;
    
    if (starting_level == 2 || starting_level == 3) {
        initializeFastBees(fastbeeAttributes, totalFastBees, resolutionX);
        fastSpawnBeesCount = 0;
    }

    // Reset honeycombs and red combs
    for (int i = 0; i < totalHoneycombs; i++) {
        honeycombAttributes[i][2] = 0;
    }

    for (int j = 0; j < redcomb_num; j++) {
        redcombAttribute[j][2] = 0;
    }

    // Reset flowers
    initializeFlowers(flowerAttributes, totalFlower);

    // Reset bullets and spray cans
    bulletLeft = 56;
    bulletCan = 3;
    reducer_count = 0;
    bullet_exists = false;

    // Reset bird state (if applicable)
    bird_active = false;
    birdX = -100.0f;  // Move bird off-screen
    birdY = -100.0f;  // Move bird off-screen
    target_index = -1;  // Reset target index for the hummingbird

   
}

void handleHummingbird(bool& bird_active, bool& isPaused, float& birdX, float& birdY, int& target_index, int& bird_hit_counter, Clock& movementClock, Clock& pauseClock, float honeycombAttributes[][3], int totalHoneycombs, Sprite& birdSprite, float bullet_x, float bullet_y,int& score) {
   
    const float hummingbird_speed = 2.5f;  
    const float collection_radius = 16.0f; 
    const float sick_timeout = 3.0f;       
    const float pause_duration = 1.0f;     
    const float interaction_duration = 1.5f; 
    const float movement_interval = 0.025f;
    const float offscreen_distance = 100.0f; 

    // Reactivate bird after a timeout
    if (!bird_active) {
        if (pauseClock.getElapsedTime().asSeconds() >= sick_timeout) {
            bird_active = true;
            birdX = -offscreen_distance; // Move bird off-screen
            birdY = -offscreen_distance;
            pauseClock.restart();
        } else {
            // Move the bird off-screen if inactive
            birdX = -offscreen_distance;
            birdY = -offscreen_distance;
            birdSprite.setPosition(birdX, birdY);
            return;
        }
    }

    // Handle bullet collision
    if (birdX < bullet_x + 48 && birdX + 32 > bullet_x &&
        birdY < bullet_y + 48 && birdY + 32 > bullet_y) {
        bird_hit_counter++;
        if (bird_hit_counter >= 3) { // Deactivate bird on third hit
            bird_active = false; // Deactivate bird
            bird_hit_counter = 0;
            birdX = -offscreen_distance; // Move bird off-screen
            birdY = -offscreen_distance;
            birdSprite.setPosition(birdX, birdY);
            pauseClock.restart();
        }
        return;
    }

    // Handle pause logic
    if (isPaused) {
        if (pauseClock.getElapsedTime().asSeconds() >= pause_duration) {
            isPaused = false; // Resume movement
        } else {
            return; // Stay paused
        }
    }

    // Target selection
    if (target_index == -1) {
        for (int i = 0; i < totalHoneycombs; ++i) {
            if (honeycombAttributes[i][2]) { 
                target_index = i;
                break;
            }
        }
    }

    // Movement logic
    if (target_index != -1 && honeycombAttributes[target_index][2]) {
        if (movementClock.getElapsedTime().asSeconds() > movement_interval) {
            float targetX = honeycombAttributes[target_index][0];
            float targetY = honeycombAttributes[target_index][1];
            // Calculate distance to target
            float dx = targetX - birdX;
            float dy = targetY - birdY;
            float distance = sqrt(dx*dx + dy*dy);
            // Move towards target if not already at it
            if (distance > hummingbird_speed) {
                birdX += hummingbird_speed * (dx/distance); 
                birdY += hummingbird_speed * (dy/distance);
            }
            else{
                birdX = targetX; 
                birdY = targetY;
                if (pauseClock.getElapsedTime().asSeconds() >= interaction_duration) {
                    honeycombAttributes[target_index][2] = 0;
                    if(honeycombAttributes[target_index][1]/30>=0 && honeycombAttributes[target_index][1]/30<=1){
                    	score+=1000;
                    }
                    else if(honeycombAttributes[target_index][1]/30>=2 && honeycombAttributes[target_index][1]/30<=4){
                    	score+=800;
                    }
                    else
                    score+=500; 
                    target_index = -1; 
                    isPaused = true;   
                    pauseClock.restart();
                }else{
                    isPaused = true; 
                }
            }
            movementClock.restart();
        }
    }

    // Update the bird sprite position
    birdSprite.setPosition(birdX, birdY);
}

void handleFastHummingbird(bool& bird_active, bool& isPaused, float& birdX, float& birdY, int& target_index, int& bird_hit_counter, Clock& movementClock, Clock& pauseClock, float honeycombAttributes[][3], int totalHoneycombs, Sprite& birdSprite, float bullet_x, float bullet_y,int& score) {
   
    const float hummingbird_speed = 2.5f;  
    const float collection_radius = 16.0f; 
    const float sick_timeout = 3.0f;       
    const float pause_duration = 1.0f;     
    const float interaction_duration = 1.5f; 
    const float movement_interval = 0.025f;
    const float offscreen_distance = 100.0f; 

    // Reactivate bird after a timeout
    if (!bird_active) {
        if (pauseClock.getElapsedTime().asSeconds() >= sick_timeout) {
            bird_active = true;
            birdX = -offscreen_distance; // Move bird off-screen
            birdY = -offscreen_distance;
            pauseClock.restart();
        } else {
            // Move the bird off-screen if inactive
            birdX = -offscreen_distance;
            birdY = -offscreen_distance;
            birdSprite.setPosition(birdX, birdY);
            return;
        }
    }

    // Handle bullet collision
    if (birdX < bullet_x + 48 && birdX + 32 > bullet_x &&
        birdY < bullet_y + 48 && birdY + 32 > bullet_y) {
        bird_hit_counter++;
        if (bird_hit_counter >= 3) { // Deactivate bird on third hit
            bird_active = false; // Deactivate bird
            bird_hit_counter = 0;
            birdX = -offscreen_distance; // Move bird off-screen
            birdY = -offscreen_distance;
            birdSprite.setPosition(birdX, birdY);
            pauseClock.restart();
        }
        return;
    }

    // Handle pause logic
    if (isPaused) {
        if (pauseClock.getElapsedTime().asSeconds() >= pause_duration) {
            isPaused = false; // Resume movement
        } else {
            return; // Stay paused
        }
    }

    // Target selection
    if (target_index == -1) {
        for (int i = 0; i < totalHoneycombs; ++i) {
            if (honeycombAttributes[i][2]) { 
                target_index = i;
                break;
            }
        }
    }

    // Movement logic
    if (target_index != -1 && honeycombAttributes[target_index][2]) {
        if (movementClock.getElapsedTime().asSeconds() > movement_interval) {
            float targetX = honeycombAttributes[target_index][0];
            float targetY = honeycombAttributes[target_index][1];
            // Calculate distance to target
            float dx = targetX - birdX;
            float dy = targetY - birdY;
            float distance = sqrt(dx*dx + dy*dy);
            // Move towards target if not already at it
            if (distance > hummingbird_speed) {
                birdX += hummingbird_speed * (dx/distance); 
                birdY += hummingbird_speed * (dy/distance);
            }
            else{
                birdX = targetX; 
                birdY = targetY;
                if (pauseClock.getElapsedTime().asSeconds() >= interaction_duration) {
                    honeycombAttributes[target_index][2] = 0;
                    if(honeycombAttributes[target_index][1]/30>=0 && honeycombAttributes[target_index][1]/30<=1){
                    	score+=2000;
                    }
                    else if(honeycombAttributes[target_index][1]/30>=2 && honeycombAttributes[target_index][1]/30<=4){
                    	score+=1800;
                    }
                    else
                    score+=1500; 
                    target_index = -1; 
                    isPaused = true;   
                    pauseClock.restart();
                }else{
                    isPaused = true; 
                }
            }
            movementClock.restart();
        }
    }

    // Update the bird sprite position
    birdSprite.setPosition(birdX, birdY);
}
void drawbird(RenderWindow& window,  Sprite& birdSprite){
    window.draw(birdSprite);
}
void checkFastBeeBulletCollision(int i, bool &bullet_exists, float bullet_x, float bullet_y, float beeAttributes[][4], float honeycombAttributes[][3], int&playerscore, Sound& beeSound) {
    // Only check active bees
    if (beeAttributes[i][3] == 1 && bullet_exists &&
        bullet_x + 16 >= beeAttributes[i][0] &&
        bullet_x <= beeAttributes[i][0] + 32 &&
        bullet_y + 16 >= beeAttributes[i][1] &&
        bullet_y <= beeAttributes[i][1] + 32) {
        
        // Deactivate bee immediately
        beeSound.play();
        beeAttributes[i][3] = 0;
        playerscore+=1000;
        
        // Find first inactive honeycomb to activate
        for (int j = 0; j < 20; j++) {
            if (honeycombAttributes[j][2] == 0) {
                honeycombAttributes[j][2] = 1; 
                honeycombAttributes[j][0] = beeAttributes[i][0];
                honeycombAttributes[j][1] = beeAttributes[i][1];
                break;
            }
        }
        
        bullet_exists = false; 
    }
}
