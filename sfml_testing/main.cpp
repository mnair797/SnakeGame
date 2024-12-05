#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
using namespace sf;

const int tileSize = 40; //Size of each tile
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
 This function places a fruit randomly on the screen passed through reference, and ensures it is not on the snake
 @param fruitX  The x coordinate of the fruit
 @param fruitY  The y coordinate of the fruit
 @param snakeBody A vector of structure indicate the coordinates
 */
void placeFruit(int &fruitX, int &fruitY, const vector<Segment> &snakeBody) {
    //Randomly picks a spot on the board
    fruitX = rand() % gridWidth;
    fruitY = rand() % gridHeight;

    //Ensure the fruit doesn't land on the snake
    for (const Segment& segment : snakeBody) {
        if (segment.x == fruitX && segment.y == fruitY) {
            placeFruit(fruitX, fruitY, snakeBody); //Recurse if the fruit is on the snake
            return;
        }
    }
}

int main() {
    //Create a window
    RenderWindow window(VideoMode(gridWidth * tileSize, gridHeight * tileSize), "Snake Game");

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

    //Game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) { //Checks if a key is pressed
                //Change snake direction based on key press, can not go in opposite direction
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
        if (elapsedTime.asSeconds() >= 0.3) { //Move every 0.3 seconds
            //Determine new head position based on direction
            Segment newHead = snakeBody[0];
            
            if (snakeDirection == Up)
            {
                newHead.y -= 1;
            } else if (snakeDirection == Down)
            {
                newHead.y += 1;
            } else if (snakeDirection == Left)
            {
                newHead.x -= 1;
            } else if (snakeDirection == Right)
            {
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
                    tile.setFillColor(Color(100, 200, 100)); //Light green
                } else {
                    tile.setFillColor(Color(50, 150, 50)); //Darker green
                }
                tile.setPosition(x * tileSize, y * tileSize);
                window.draw(tile);
            }
        }

        //Draw the snake
        for (size_t i = 0; i < snakeBody.size(); ++i) {
            RectangleShape snakeSegment(Vector2f(tileSize, tileSize));

            //Check if it's the head (first segment in the vector)
            if (i == 0) {
                snakeSegment.setFillColor(Color::White);  //Head color (white)
            } else {
                snakeSegment.setFillColor(Color::Black);  //Body color (black)
            }

            snakeSegment.setPosition(snakeBody[i].x * tileSize, snakeBody[i].y * tileSize);
            window.draw(snakeSegment);
        }


        //Draw the fruit
        RectangleShape fruitShape(Vector2f(tileSize, tileSize));
        fruitShape.setFillColor(Color::Red);
        fruitShape.setPosition(fruitX * tileSize, fruitY * tileSize);
        window.draw(fruitShape);


        //Display everything
        window.display();
    }

    return 0;
}
