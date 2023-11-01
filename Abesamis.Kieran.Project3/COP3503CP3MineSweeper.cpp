#include <SFML/Graphics.hpp>
#include <vector>
#include "Tile.h"
#include "Board.h"
#include "TextureManager.h"
#include "Random.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
using namespace sf;

//Function Header for loading config file
vector<int> LoadConfigFile(string file);
vector<int> GetCounterDigits(int countMines);
vector<char> LoadTestFile(string file, int width, int height);

int main()
{
    //Loading config.cfg file
    vector<int> boardData = LoadConfigFile("boards/config.cfg");
    int windowWidth = boardData[0];
    int windowHeight = boardData[1];
    int mines = boardData[2];

    sf::RenderWindow window(sf::VideoMode(windowWidth * 32, (windowHeight * 32) + 100), "Project 3 Minesweeper");
   
    //Creating new Board
    Board mineSweeperBoard = Board(boardData[1], boardData[0], boardData[2]);
    mineSweeperBoard.NewBoard();

    //Loading revealedTile
    sf::Sprite revealedTile(TextureManager::GetTexture("tile_revealed"));

    //Loading buttons + counter
    sf::Sprite smile(TextureManager::GetTexture("face_happy"));
    sf::Sprite testThree(TextureManager::GetTexture("test_3"));
    sf::Sprite testTwo(TextureManager::GetTexture("test_2"));
    sf::Sprite testOne(TextureManager::GetTexture("test_1"));
    sf::Sprite debug(TextureManager::GetTexture("debug"));
    sf::Sprite counter(TextureManager::GetTexture("digits"));
    //Vector to hold each digit's rectangle in the png
    vector<sf::IntRect> rectangleDigits;
    //Vector to hold the sprites that are to be displayed
    vector<sf::Sprite> counterDigits;
    //Vector to hold the digits to determine which sprites are to be displayed
    vector<int> integerDigits;
    //Smiley face goes in the middle of the board
    smile.setPosition((windowWidth/2) * 32, windowHeight * 32);
    //testThree is 8 columns to the right of smiley face
    testThree.setPosition(((windowWidth/2) + 10) * 32, windowHeight * 32);
    //testTwo is 6 columns to the right of smiley face
    testTwo.setPosition(((windowWidth/2) + 8) * 32, windowHeight * 32);
    //testOne is 4 columns to the right of smiley face
    testOne.setPosition(((windowWidth/2) + 6) * 32, windowHeight * 32);
    //debug is 2 columns to the right of smiley face
    debug.setPosition(((windowWidth/2) + 4) * 32, windowHeight * 32);

    //Initializing the counter with the number of mines
    for (int i = 0; i < 211; i += 21) {
        rectangleDigits.push_back(sf::IntRect(i, 0, 21, 32));
   }
   
    integerDigits = GetCounterDigits(mineSweeperBoard.GetMines());
    int digitXCoordinate;
    
    if (integerDigits.size() == 4) {
        digitXCoordinate = 0;
        for (int j = 0; j < 4; j++) {
            counterDigits.push_back(sf::Sprite(counter));
            counterDigits[j].setPosition(digitXCoordinate, windowHeight * 32);
            counterDigits[j].setTextureRect(rectangleDigits[integerDigits[j]]);
            digitXCoordinate += 21;
        }
    }
    else {
        digitXCoordinate = 21;
        for (int k = 0; k < 3; k++) {
            counterDigits.push_back(sf::Sprite(counter));
            counterDigits[k].setPosition(digitXCoordinate, windowHeight * 32);
            counterDigits[k].setTextureRect(rectangleDigits[integerDigits[k]]);
            digitXCoordinate += 21;
        }
     }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                //Left click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i leftClick = sf::Mouse::getPosition(window);
                    //Tiles
                    if (leftClick.y < windowHeight * 32) {
                        Tile* leftClickTile = &mineSweeperBoard.GetTile(windowWidth * (leftClick.y / 32) + (leftClick.x / 32));
                        if ((mineSweeperBoard.gameOver = leftClickTile->RevealTile()) == true) {
                            mineSweeperBoard.CheckGameOver();
                            smile.setTexture(TextureManager::GetTexture("face_lose"));
                        }
                        else {
                            mineSweeperBoard.CheckVictory();
                            if (mineSweeperBoard.victory == true) {
                                smile.setTexture(TextureManager::GetTexture("face_win"));
                                }
                            }
                        }
                    //Smiley Face
                    else if (leftClick.x >= (windowWidth / 2) * 32 && leftClick.x <= (windowWidth / 2 + 2) * 32 
                    && leftClick.y >= windowHeight * 32) {
                        smile.setTexture(TextureManager::GetTexture("face_happy"));
                        bool checkDebug = mineSweeperBoard.inDebug;
                        mineSweeperBoard = Board(boardData[1], boardData[0], boardData[2]);
                        mineSweeperBoard.NewBoard();
                        if (checkDebug = true) {
                            mineSweeperBoard.DebugMode();
                        }
                           
                        //Get Digits
                        integerDigits = GetCounterDigits(mineSweeperBoard.GetMines());
                        counterDigits.clear();
                        //Display Digits
                        if (integerDigits.size() == 4) {
                            digitXCoordinate = 0;
                            for (int i = 0; i < 4; i++) {
                                counterDigits.push_back(sf::Sprite(counter));
                                counterDigits[i].setPosition(digitXCoordinate, windowHeight * 32);
                                counterDigits[i].setTextureRect(rectangleDigits[integerDigits[i]]);
                                digitXCoordinate += 21;
                            }
                        }
                        else {
                            digitXCoordinate = 21;
                            for (int j = 0; j < 3; j++) {
                                counterDigits.push_back(sf::Sprite(counter));
                                counterDigits[j].setPosition(digitXCoordinate, windowHeight * 32);
                                counterDigits[j].setTextureRect(rectangleDigits[integerDigits[j]]);
                                digitXCoordinate += 21;
                            }
                        }
                    }
                    //Debug Button
                    else if (leftClick.x >= ((windowWidth / 2 + 4) * 32) && leftClick.x <= ((windowWidth / 2 + 6) * 32)
                    && leftClick.y >= windowHeight * 32){
                        if (mineSweeperBoard.victory == false && mineSweeperBoard.gameOver == false) {
                            mineSweeperBoard.DebugMode();
                        }
                    }
                    //TestBoard 1 Button
                    else if (leftClick.x >= ((windowWidth / 2 + 6) * 32) && leftClick.x <= ((windowWidth / 2 + 8) * 32)
                        && leftClick.y >= windowHeight * 32) {
                         smile.setTexture(TextureManager::GetTexture("face_happy"));
                          vector<char> testOneTiles = LoadTestFile("boards/testBoard1.brd", windowWidth, windowHeight);
                          mineSweeperBoard.TestBoard(testOneTiles);

                         //Get Digits
                         int count = mineSweeperBoard.GetMines() - mineSweeperBoard.GetFlags();
                         integerDigits = GetCounterDigits(count);
                         counterDigits.clear();
                          //Digit Display
                         if (integerDigits.size() == 4) {
                            digitXCoordinate = 0;
                            for (int i = 0; i < 4; i++) {
                                counterDigits.push_back(sf::Sprite(counter));
                                counterDigits[i].setPosition(digitXCoordinate, windowHeight * 32);
                                counterDigits[i].setTextureRect(rectangleDigits[integerDigits[i]]);
                                digitXCoordinate += 21;
                                }
                            }
                            else {
                                digitXCoordinate = 21;
                                for (int j = 0; j < 3; j++) {
                                    counterDigits.push_back(sf::Sprite(counter));
                                    counterDigits[j].setPosition(digitXCoordinate, windowHeight * 32);
                                    counterDigits[j].setTextureRect(rectangleDigits[integerDigits[j]]);
                                    digitXCoordinate += 21;
                                }
                             }
                     }
                    //TestBoard 2 Button
                    else if (leftClick.x >= ((windowWidth / 2 + 8) * 32) && leftClick.x <= ((windowWidth / 2 + 10) * 32)
                        && leftClick.y >= windowHeight * 32) {
                            smile.setTexture(TextureManager::GetTexture("face_happy"));
                            vector<char> testTwoTiles = LoadTestFile("boards/testboard2.brd", windowWidth, windowHeight);
                            mineSweeperBoard.TestBoard(testTwoTiles);

                            //Get Digits
                            int count = mineSweeperBoard.GetMines() - mineSweeperBoard.GetFlags();
                            integerDigits = GetCounterDigits(count);
                            counterDigits.clear();
                            //Digit Display
                            if (integerDigits.size() == 4) {
                                digitXCoordinate = 0;
                                for (int i = 0; i < 4; i++) {
                                    counterDigits.push_back(sf::Sprite(counter));
                                    counterDigits[i].setPosition(digitXCoordinate, windowHeight * 32);
                                    counterDigits[i].setTextureRect(rectangleDigits[integerDigits[i]]);
                                    digitXCoordinate += 21;
                                }
                            }
                            else {
                                digitXCoordinate = 21;
                                for (int j = 0; j < 3; j++) {
                                    counterDigits.push_back(sf::Sprite(counter));
                                    counterDigits[j].setPosition(digitXCoordinate, windowHeight * 32);
                                    counterDigits[j].setTextureRect(rectangleDigits[integerDigits[j]]);
                                    digitXCoordinate += 21;
                                }
                            }
                    }
                    //TestBoard 3 Button
                    else if (leftClick.x >= ((windowWidth / 2 + 10) * 32) && leftClick.x <= ((windowWidth / 2 + 12) * 32)
                    && leftClick.y >= windowHeight * 32) {
                          smile.setTexture(TextureManager::GetTexture("face_happy"));
                          vector<char> testThreeTiles = LoadTestFile("boards/testboard3.brd", windowWidth, windowHeight);
                          mineSweeperBoard.TestBoard(testThreeTiles);

                        //Get Digits
                          int count = mineSweeperBoard.GetMines() - mineSweeperBoard.GetFlags();
                          integerDigits = GetCounterDigits(count);
                          counterDigits.clear();
                        //Digit Display
                          if (integerDigits.size() == 4) {
                                digitXCoordinate = 0;
                                for (int i = 0; i < 4; i++) {
                                    counterDigits.push_back(sf::Sprite(counter));
                                    counterDigits[i].setPosition(digitXCoordinate, windowHeight * 32);
                                    counterDigits[i].setTextureRect(rectangleDigits[integerDigits[i]]);
                                    digitXCoordinate += 21;
                                }
                           }
                          else {
                                digitXCoordinate = 21;
                                for (int j = 0; j < 3; j++) {
                                    counterDigits.push_back(sf::Sprite(counter));
                                    counterDigits[j].setPosition(digitXCoordinate, windowHeight * 32);
                                    counterDigits[j].setTextureRect(rectangleDigits[integerDigits[j]]);
                                    digitXCoordinate += 21;
                                 }
                          }
                    }
                }
                //Right Click (FLAG)
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i rightClick = sf::Mouse::getPosition(window);

                    if (rightClick.y < windowHeight * 32) {
                        Tile* rightClickTile = &mineSweeperBoard.GetTile(windowWidth * (rightClick.y / 32) + (rightClick.x / 32));
                        rightClickTile->PlaceFlag();
                        mineSweeperBoard.CheckVictory();
                        if (mineSweeperBoard.victory == true) {
                            smile.setTexture(TextureManager::GetTexture("face_win"));
                        }
                        
                        //Get Digits
                        int count = mineSweeperBoard.GetMines() - mineSweeperBoard.GetFlags();
                        integerDigits = GetCounterDigits(count);
                        counterDigits.clear();
                        //Display Digits
                        if (integerDigits.size() == 4) {
                            digitXCoordinate = 0;
                            for (int i = 0; i < 4; i++) {
                                counterDigits.push_back(sf::Sprite(counter));
                                counterDigits[i].setPosition(digitXCoordinate, windowHeight * 32);
                                counterDigits[i].setTextureRect(rectangleDigits[integerDigits[i]]);
                                digitXCoordinate += 21;
                            }
                         }
                         else {
                             digitXCoordinate = 21;
                             for (int j = 0; j < 3; j++) {
                                counterDigits.push_back(sf::Sprite(counter));
                                counterDigits[j].setPosition(digitXCoordinate, windowHeight * 32);
                                counterDigits[j].setTextureRect(rectangleDigits[integerDigits[j]]);
                                digitXCoordinate += 21;
                                
                            }
                        }
                    }
                }
            } 
        }
    

        window.clear();
        window.draw(smile);
        window.draw(testThree);
        window.draw(testTwo);
        window.draw(testOne);
        window.draw(debug);
        for (int i = 0; i < boardData[0] * boardData[1]; i++) {
            window.draw(mineSweeperBoard.GetTile(i).GetRevealedTile());
            window.draw(mineSweeperBoard.GetTile(i).GetDebugFlag());
            window.draw(mineSweeperBoard.GetTile(i).GetImage());
        }

        for (int j = 0; j < counterDigits.size(); j++) {
            window.draw(counterDigits.at(j));
        }

        window.display();
    }

    TextureManager::Clear();
    return 0;
}

vector<int> LoadConfigFile(string file) {
    vector<int> configData;

    ifstream inFile(file);
    string line;
    getline(inFile, line);
    configData.push_back(stoi(line));
    getline(inFile, line);
    configData.push_back(stoi(line));
    getline(inFile, line);
    configData.push_back(stoi(line));
    inFile.close();
    return configData;
}

vector<char> LoadTestFile(string file, int width, int height) {
    vector<char> testTiles;
    ifstream testFile(file);
    if (testFile.is_open()) {
        char charTile;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                testFile.get(charTile);
                testTiles.push_back(charTile);
            }
            testFile.ignore(1);
        }
        testFile.close();
    }
    return testTiles;
}

vector<int> GetCounterDigits(int count) {
    vector<int> returnDigits;
    int firstDigit;
    int secondDigit;
    int thirdDigit;

    if (count <= -100) {
        returnDigits.push_back(10);
        firstDigit = (count * -1) / 100;
        secondDigit = ((count * -1) % 100) / 10;
        thirdDigit = (count * -1) % 10;
        
        returnDigits.push_back(firstDigit);
        returnDigits.push_back(secondDigit);
        returnDigits.push_back(thirdDigit);
    }
    else if (count <= -10 && count >= -99) {
        returnDigits.push_back(10);
        firstDigit = (count * -1) / 10;
        secondDigit = (count * -1) % 10;
        returnDigits.push_back(0);
        returnDigits.push_back(firstDigit);
        returnDigits.push_back(secondDigit);
    }
    else if (count <= -1 && count >= -9) {
        returnDigits.push_back(10);
        returnDigits.push_back(0);
        returnDigits.push_back(0);
        firstDigit = count * -1;

        returnDigits.push_back(firstDigit);
    }
    else if (count <= 9 && count >= 0) {
        returnDigits.push_back(0);
        returnDigits.push_back(0);
        firstDigit = count;

        returnDigits.push_back(count);
    }
    else if (count <= 99 && count >= 10) {
        returnDigits.push_back(0);
        firstDigit = count / 10;
        secondDigit = count % 10;
        
        returnDigits.push_back(firstDigit);
        returnDigits.push_back(secondDigit);
    }
    else if (count >= 100) {
        firstDigit = count / 100;
        secondDigit = (count % 100) / 10;
        thirdDigit = count % 10;

        returnDigits.push_back(firstDigit);
        returnDigits.push_back(secondDigit);
        returnDigits.push_back(thirdDigit);
    }
    return returnDigits;
}
