#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>   // For rand() function
#include <ctime>     //For seeding rand()
using namespace std;
using namespace sf;

const int tileSize = 40; // Size of each tile
const int gridWidth = 20; //Number of tiles in width
const int gridHeight = 15; // Number of tiles in height

//Direction constants
const int Up = 0;
const int Down = 1;
const int Left = 2;
const int Right = 3;

/**
 This function places a fruit randomly on the screen, and ensures it is not on the snake
 @param fruitX  The x coordinate of the fruit
 @param fruitY  The y coordinate of the fruit
 @param snakeX The x coordinate of the snake
 @param snakeY The y coordinate of the snake
 */
void placeFruit(int &fruitX, int &fruitY, int snakeX, int snakeY) { //Pass fruit position through reference
    fruitX = rand() % gridWidth;
    fruitY = rand() % gridHeight;

    // Make sure the fruit doesn't land on the snake
    while (fruitX == snakeX && fruitY == snakeY) {
        fruitX = rand() % gridWidth;
        fruitY = rand() % gridHeight;
    }
}

int main() {
    // Create a window
    RenderWindow window(VideoMode(gridWidth * tileSize, gridHeight * tileSize), "Snake Game");

    // Initialize random seed
    srand(time(0));

    //Create snake
    RectangleShape snake(Vector2f(tileSize, tileSize));
    snake.setFillColor(Color::Black);
    int snakeX = gridWidth / 2;  // Snake starts in the center of the grid
    int snakeY = gridHeight / 2;
    snake.setPosition(snakeX * tileSize, snakeY * tileSize);

    //Create fruit
    RectangleShape fruit(Vector2f(tileSize, tileSize));
    fruit.setFillColor(Color::Red);

    //Inital random placement of  fruit
    int fruitX, fruitY;
    placeFruit(fruitX, fruitY, snakeX, snakeY);
    fruit.setPosition(fruitX * tileSize, fruitY * tileSize);

    // Directional variable
    int snakeDirection = Right; // Snake starts moving to the right initally

    bool gameOver = false;
    bool gameWon = false;

    // SFML Clock for automatic movement
    Clock clock;
    Time elapsedTime;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (gameOver !=true && gameWon !=true && event.type == Event::KeyPressed) { //Checks if a key is pressed
                // Change snake direction
                if (event.key.code == Keyboard::Up) {
                    snakeDirection = Up;
                }
                if (event.key.code == Keyboard::Down) {
                    snakeDirection = Down;
                }
                if (event.key.code == Keyboard::Left) {
                    snakeDirection = Left;
                }
                if (event.key.code == Keyboard::Right) {
                    snakeDirection = Right;
                }
            }
        }

        // Check if the snake hits the edge (Game Over)
        if (snakeX < 0 || snakeX >= gridWidth || snakeY < 0 || snakeY >= gridHeight) {
            if (gameOver == false) {
                gameOver = true;
                cout << "Game Over!" << endl;
            }
        }

        // Check if snake eat fruit (Game Won)
        if (gameOver != true && gameWon !=true && snakeX == fruitX && snakeY == fruitY) {
            gameWon = true;
            cout << "Game Won!" << endl;  // Debug message
        }

        //Move the snake automatically every 1/3 second
        elapsedTime = clock.getElapsedTime();
        if (gameOver !=true && gameWon !=true && elapsedTime.asSeconds() >= 0.33) {
            // Update the snake's position by one square depending on current direction of snake
            if (snakeDirection == Up) {
                snakeY -= 1;
            }
            if (snakeDirection == Down) {
                snakeY += 1;
            }
            if (snakeDirection == Left) {
                snakeX -= 1;
            }
            if (snakeDirection == Right) {
                snakeX += 1;
            }

            //Reset the clock after moving the snake
            clock.restart();

            snake.setPosition(snakeX * tileSize, snakeY * tileSize);
        }

        // Clear window
        window.clear();

        if (gameWon) {
            Font font;
            if (!font.loadFromFile("Roboto-Regular.ttf")) {
                return -1;
            }

            Text gameWonText;
            gameWonText.setFont(font);
            gameWonText.setString("Game Won!");
            gameWonText.setCharacterSize(50);
            gameWonText.setFillColor(Color::White);
            gameWonText.setPosition(gridWidth * tileSize / 4, gridHeight * tileSize / 2);

            window.draw(gameWonText); // Draw the "Game Won" text
        } else if (gameOver) {
            Font font;
            if (!font.loadFromFile("Roboto-Regular.ttf")) {
                return -1;
            }

            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over!");
            gameOverText.setCharacterSize(50);
            gameOverText.setFillColor(Color::White);
            gameOverText.setPosition(gridWidth * tileSize / 4, gridHeight * tileSize / 2);

            window.draw(gameOverText); // Draw the "Game Over" text
        } else {
            // Draw the checkerboard pattern
            for (int y = 0; y < gridHeight; ++y) {
                for (int x = 0; x < gridWidth; ++x) {
                    RectangleShape tile(Vector2f(tileSize, tileSize));
                    // Alternate colors based on position
                    if ((x + y) % 2 == 0) {
                        tile.setFillColor(Color(100, 200, 100)); //Light green
                    } else {
                        tile.setFillColor(Color(50, 150, 50)); //Darker green
                    }
                    tile.setPosition(x * tileSize, y * tileSize);
                    window.draw(tile);
                }
            }

            // Draw the snake
            window.draw(snake);

            // Draw the fruit
            window.draw(fruit);
        }

        window.display();
    }

    return 0;
}
