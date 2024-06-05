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
    bool gameStarted = false; // Add a flag to check if the game has started
    Player *player;

    void handleGoCommand(std::vector<std::string>);
    void handleLookCommand(std::vector<std::string>);
    void handleTakeCommand(std::vector<std::string>);
    void handleDropCommand(std::vector<std::string>);
    void handleSolveCommand(std::vector<std::string>);
    void handleCraftCommand(std::vector<std::string>);
    void handleQuitCommand(std::vector<std::string>);
    void handleStartCommand();
    void handleGameOver();

    static std::vector<std::string> tokenizeString(const std::string&);
    static std::string makeLowercase(std::string);
    void displayPrompt();
    void displayRoomDescription();
    void displayWelcomeMessage(); 
};

#endif //ZOORKENGINE_H
