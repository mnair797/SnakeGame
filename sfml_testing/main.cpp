//  main.cpp
//  sfml_testing
//  Created by Meenakshi Nair on 10/27/24.

#include <SFML/Graphics.hpp>
#include <iostream>  // For debugging output
#include <cstdlib>   // For rand() function
#include <ctime>     // For seeding rand()

const int tileSize = 40; // Size of each tile
const int gridWidth = 20; // Number of tiles in width
const int gridHeight = 15; // Number of tiles in height

/**
 This function places a fruit randomly on the screen, and ensures it is not on the snake
 @param fruitX  The x coordinate of the fruit
 @param fruitY  The y coordinate of the fruit
 @param snakeX The x coordinate of the snake
 @param snakeY The y coordinate of the snake
 */
void placeFruit(int &fruitX, int &fruitY, int snakeX, int snakeY) {
    fruitX = std::rand() % gridWidth;
    fruitY = std::rand() % gridHeight;

    // Make sure the fruit doesn't land on the snake
    while (fruitX == snakeX && fruitY == snakeY) {
        fruitX = std::rand() % gridWidth;
        fruitY = std::rand() % gridHeight;
    }
}

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(gridWidth * tileSize, gridHeight * tileSize), "Snake Game Layout");

    // Initialize random seed
    std::srand(std::time(0));

    // Initialize snake
    sf::RectangleShape snake(sf::Vector2f(tileSize, tileSize));
    snake.setFillColor(sf::Color::Black);
    int snakeX = gridWidth / 2;  // Snake starts in the center of the grid
    int snakeY = gridHeight / 2;
    snake.setPosition(snakeX * tileSize, snakeY * tileSize);

    // Initialize fruit
    sf::RectangleShape fruit(sf::Vector2f(tileSize, tileSize));
    fruit.setFillColor(sf::Color::Red);

    // Initial random placement of the fruit
    int fruitX, fruitY;
    placeFruit(fruitX, fruitY, snakeX, snakeY);
    fruit.setPosition(fruitX * tileSize, fruitY * tileSize);

    // Directional control variables
    enum Direction {Up, Down, Left, Right};
    Direction snakeDirection = Right; // Snake starts moving to the right

    bool gameOver = false;
    bool gameWon = false;

    // SFML Clock for automatic movement
    sf::Clock clock;
    sf::Time elapsedTime;

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameOver && !gameWon && event.type == sf::Event::KeyPressed) {
                // Handle snake direction change with key presses
                if (event.key.code == sf::Keyboard::Up) {
                    snakeDirection = Up;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    snakeDirection = Down;
                }
                if (event.key.code == sf::Keyboard::Left) {
                    snakeDirection = Left;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    snakeDirection = Right;
                }
            }
        }

        // Check if the snake hits the edge (Game Over condition)
        if (snakeX < 0 || snakeX >= gridWidth || snakeY < 0 || snakeY >= gridHeight) {
            if (!gameOver) {
                gameOver = true;
                std::cout << "Game Over triggered!" << std::endl;  // Debug message
            }
        }

        // Check for collision with fruit (Game Won condition)
        if (!gameOver && !gameWon && snakeX == fruitX && snakeY == fruitY) {
            gameWon = true;
            std::cout << "Game Won!" << std::endl;  // Debug message
        }

        // If game isn't over, move the snake automatically every 1/3 second
        elapsedTime = clock.getElapsedTime();
        if (!gameOver && !gameWon && elapsedTime.asSeconds() >= 0.33) {
            // Update the snake's position based on its direction
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

            // Reset the clock after moving the snake
            clock.restart();

            snake.setPosition(snakeX * tileSize, snakeY * tileSize);
        }

        // Clear window
        window.clear();

        if (gameWon) {
            // Draw "Game Won" screen
            sf::Font font;
            if (!font.loadFromFile("Roboto-Regular.ttf")) {  // Path relative to your project directory
                std::cerr << "Error: Could not load font!" << std::endl;
                return -1;  // Exit if font loading fails
            }

            sf::Text gameWonText;
            gameWonText.setFont(font);
            gameWonText.setString("Game Won!");
            gameWonText.setCharacterSize(50);
            gameWonText.setFillColor(sf::Color::White);
            gameWonText.setPosition(gridWidth * tileSize / 4, gridHeight * tileSize / 2); // Centered

            window.draw(gameWonText); // Draw the "Game Won" text
        } else if (gameOver) {
            // Draw "Game Over" screen
            sf::Font font;
            if (!font.loadFromFile("Roboto-Regular.ttf")) {  // Path relative to your project directory
                std::cerr << "Error: Could not load font!" << std::endl;
                return -1;  // Exit if font loading fails
            }

            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over!");
            gameOverText.setCharacterSize(50);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setPosition(gridWidth * tileSize / 4, gridHeight * tileSize / 2); // Centered

            window.draw(gameOverText); // Draw the "Game Over" text
        } else {
            // Draw the checkerboard pattern
            for (int y = 0; y < gridHeight; ++y) {
                for (int x = 0; x < gridWidth; ++x) {
                    sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                    // Alternate colors based on position
                    if ((x + y) % 2 == 0) {
                        tile.setFillColor(sf::Color(100, 200, 100)); // Light green
                    } else {
                        tile.setFillColor(sf::Color(50, 150, 50)); // Darker green
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

        // Display everything
        window.display();
    }

    return 0;
}
