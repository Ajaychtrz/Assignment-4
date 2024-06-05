#ifndef ZOORKENGINE_H
#define ZOORKENGINE_H

#include "Player.h"
#include "Location.h"
#include "Passage.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ZOOrkEngine {
public:
    ZOOrkEngine(std::shared_ptr<Room>);

    void run();

private:
    bool gameOver = false;
    bool gameStarted = true; 
    Player *player;

    void handleGoCommand(std::vector<std::string>);
    void handleLookCommand(std::vector<std::string>);
    void handleTakeCommand(std::vector<std::string>);
    void handleQuitCommand(std::vector<std::string>);
    void handleGameOver();

    static std::vector<std::string> tokenizeString(const std::string&);
    static std::string makeLowercase(std::string);
    void displayPrompt();
    void displayRoomDescription();
};

#endif //ZOORKENGINE_H
