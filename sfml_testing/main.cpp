#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
using namespace sf;

const int tileSize = 40;  //Size of each tile
const int gridWidth = 20; //Number of tiles in width
const int gridHeight = 15; //Number of tiles in height

//Direction constants
const int Up = 0;
const int Down = 1;
const int Left = 2;
const int Right = 3;

//Snake segment structure
struct Segment {
    int x;
    int y;
};

/**
 This function places a fruit randomly on the screen passed through reference, and ensures it is not on the snake.
 @param fruitX The x-coordinate of the fruit.
 @param fruitY The y-coordinate of the fruit.
 @param snakeBody A vector of structures indicating the coordinates.
 */
void placeFruit(int &fruitX, int &fruitY, const vector<Segment> &snakeBody) {
    //Randomly pick a spot on the board
    fruitX = rand() % gridWidth;
    fruitY = rand() % gridHeight;

    //Ensure the fruit doesn't land on the snake
    for (const Segment &segment : snakeBody) {
        if (segment.x == fruitX && segment.y == fruitY) {
            placeFruit(fruitX, fruitY, snakeBody);  //Recurse if the fruit is on the snake
            return;
        }
    }
}
/**
 Displays the start screen, allowing players to choose the snake's color and speed, and start the game
 @param window The SFML RenderWindow for the game
 @param snakeColor The snake's color reference,  updated based on user input
 @param speedLevel The speed level reference, updated based on user input
 */
void displayStartScreen(RenderWindow &window, Color &snakeColor, int &speedLevel) {
    Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        cout << "Error loading font!" << endl;
        return;
    }

    //Title text
    Text title("Snake Game", font, 50);
    title.setFillColor(Color::White);
    title.setPosition(gridWidth * tileSize / 2 - title.getGlobalBounds().width / 2, 50);

    //Instructions text
    Text instructions("Press UP/DOWN to select snake color\nPress LEFT/RIGHT to adjust speed\nPress ENTER to start", font, 20);
    instructions.setFillColor(Color::White);
    instructions.setPosition(gridWidth * tileSize / 2 - instructions.getGlobalBounds().width / 2, 200);

    //Options for snake colors
    vector<Color> colors = {Color::Black, Color::Blue, Color::Magenta, Color::Yellow};
    vector<string> colorNames = {"Black", "Blue", "Magenta", "Yellow"};
    int selectedColor = 0;

    speedLevel = 5; //Default speed level (medium)

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up) {
                    selectedColor = (int) (selectedColor - 1 + colors.size()) % colors.size();
                }
                if (event.key.code == Keyboard::Down) {
                    selectedColor = (int) (selectedColor + 1) % colors.size();
                }
                if (event.key.code == Keyboard::Left) {
                    speedLevel = max(1, speedLevel - 1); //Decrease speed level, minimum 1
                }
                if (event.key.code == Keyboard::Right) {
                    speedLevel = min(10, speedLevel + 1); //Increase speed level, maximum 10
                }
                if (event.key.code == Keyboard::Enter) {
                    snakeColor = colors[selectedColor];
                    return; //Start the game
                }
            }
        }

        //Clear the window
        window.clear(Color(30, 30, 30));

        //Draw the title
        window.draw(title);

        //Draw the instructions
        window.draw(instructions);

        //Display color options
        for (size_t i = 0; i < colors.size(); ++i) {
            Text colorOption(colorNames[i], font, 25);
            if (i == selectedColor) {
                colorOption.setFillColor(Color::White); //Highlight selected color
            } else {
                colorOption.setFillColor(Color(150, 150, 150)); //Set to gray
            }
            
            colorOption.setPosition(gridWidth * tileSize / 2 - colorOption.getGlobalBounds().width / 2, 300 + i * 40);
            window.draw(colorOption);
        }

        //Display speed level
        Text speedText("Speed Level: " + to_string(speedLevel), font, 25);
        speedText.setFillColor(Color::White);
        speedText.setPosition(gridWidth * tileSize / 2 - speedText.getGlobalBounds().width / 2, 500);
        window.draw(speedText);

        //Display everything
        window.display();
    }
}


int main() {
    //Create a window
    RenderWindow window(VideoMode(gridWidth * tileSize, gridHeight * tileSize), "Snake Game");

    //Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    
    Color snakeColor = Color::Black; //Default to black body color
    int speedLevel = 5; //Default to medium speed

    //Display start screen
    displayStartScreen(window, snakeColor, speedLevel);

    //Create snake (using a vector for the snake body)
    vector<Segment> snakeBody;
    snakeBody.push_back({gridWidth / 2, gridHeight / 2});  //Initial position of the snake's head

    //Create fruit
    RectangleShape fruit(Vector2f(tileSize, tileSize));
    fruit.setFillColor(Color::Red);
    int fruitX, fruitY;
    placeFruit(fruitX, fruitY, snakeBody);
    fruit.setPosition(fruitX * tileSize, fruitY * tileSize);

    //Direction variable
    int snakeDirection = Right; //Snake starts moving to the right initially
    bool ateFruit = false; //If snake ate the fruit

    //SFML clock to calculate time for automatic movement
    Clock clock;
    Time elapsedTime;
    
    float moveDelay = 0.46f - ((speedLevel - 1) * 0.04f); //Maps speed level 1-10 to delay

    //Game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {  //Checks if a key is pressed
                //Change snake direction based on key press, cannot go in opposite direction
                if (event.key.code == Keyboard::Up && snakeDirection != Down) {
                    snakeDirection = Up;
                }
                if (event.key.code == Keyboard::Down && snakeDirection != Up) {
                    snakeDirection = Down;
                }
                if (event.key.code == Keyboard::Left && snakeDirection != Right) {
                    snakeDirection = Left;
                }
                if (event.key.code == Keyboard::Right && snakeDirection != Left) {
                    snakeDirection = Right;
                }
            }
        }

        //Move the snake
        elapsedTime = clock.getElapsedTime();
        if (elapsedTime.asSeconds() >= moveDelay) {
            //Determine new head position based on direction
            Segment newHead = snakeBody[0];
            if (snakeDirection == Up) {
                newHead.y -= 1;
            } else if (snakeDirection == Down) {
                newHead.y += 1;
            } else if (snakeDirection == Left) {
                newHead.x -= 1;
            } else if (snakeDirection == Right) {
                newHead.x += 1;
            }

            //Check if the snake eats the fruit
            if (newHead.x == fruitX && newHead.y == fruitY) {
                ateFruit = true; //Snake will grow
                placeFruit(fruitX, fruitY, snakeBody); //Place new fruit
            }

            snakeBody.insert(snakeBody.begin(), newHead);

            if (!ateFruit) {
                snakeBody.pop_back(); //Remove the tail if no fruit eaten
            } else {
                ateFruit = false; //Reset after growing
            }

            //Check if the snake collides with itself (game over)
            for (size_t i = 1; i < snakeBody.size(); ++i) {  //Start at 1, since head is already checked
                if (snakeBody[i].x == newHead.x && snakeBody[i].y == newHead.y) {
                    cout << "Game Over!" << endl;
                    window.close();
                }
            }

            //Check if the snake hits the wall (game over)
            if (newHead.x < 0 || newHead.x >= gridWidth || newHead.y < 0 || newHead.y >= gridHeight) {
                cout << "Game Over!" << endl;
                window.close();
            }

            //Reset clock after moving the snake
            clock.restart();
        }

        //Clear window
        window.clear();

        //Draw the checkerboard pattern
        for (int y = 0; y < gridHeight; ++y) {
            for (int x = 0; x < gridWidth; ++x) {
                RectangleShape tile(Vector2f(tileSize, tileSize));
                //Alternate colors based on position
                if ((x + y) % 2 == 0) {
                    tile.setFillColor(Color(100, 200, 100));  //Light green
                } else {
                    tile.setFillColor(Color(50, 150, 50));  //Darker green
                }
                tile.setPosition(x * tileSize, y * tileSize);
                window.draw(tile);
            }
        }

        
        //Draw the snake
        for (size_t i = 0; i < snakeBody.size(); ++i) {
            RectangleShape snakeSegment(Vector2f(tileSize, tileSize));
            if (i == 0) {
                snakeSegment.setFillColor(Color::White); //Make the head white
            } else {
                snakeSegment.setFillColor(snakeColor); //Use chosen snake color for the rest of the body
            }
            snakeSegment.setPosition(snakeBody[i].x * tileSize, snakeBody[i].y * tileSize);
            window.draw(snakeSegment);
        }


        //Draw the fruit
        fruit.setPosition(fruitX * tileSize, fruitY * tileSize);
        window.draw(fruit);

        //Display the window
        window.display();
    }

    return 0;
}
