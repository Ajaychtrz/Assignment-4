#include "ZOOrkEngine.h"
#include "Room.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype> // Include cctype for tolower

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());
    displayWelcomeMessage(); // Display the welcome message
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (!gameStarted) {
            if (command == "start") {
                handleStartCommand();
            } else {
                std::cout << "Please type 'start' to begin the game.\n";
            }
        } else {
            if (command == "go" || command == "move") {
                handleGoCommand(arguments);
            } else if ((command == "look") || (command == "inspect")) {
                handleLookCommand(arguments);
            } else if ((command == "take") || (command == "get") || (command == "pick up")) {
                handleTakeCommand(arguments);
            } else if (command == "drop") {
                handleDropCommand(arguments);
            } else if (command == "solve") {
                handleSolveCommand(arguments);
            } else if (command == "craft") {
                handleCraftCommand(arguments);
            } else if (command == "inventory") {
                player->showInventory(gameOver);
            } else if (command == "quit") {
                handleQuitCommand(arguments);
            } else {
                std::cout << "I don't understand that command.\n";
            }
        }

        if (gameStarted && !gameOver) {
            displayPrompt();
        }
    }
}

void ZOOrkEngine::displayWelcomeMessage() {
    std::cout << "Hi Rodie, Welcome to Dungeon and Dragon. If you want to play the game, type 'start'.\n";
}

void ZOOrkEngine::handleStartCommand() {
    gameStarted = true;
    player->getCurrentRoom()->enter();
    displayRoomDescription(); // Initial room description
}

void ZOOrkEngine::displayPrompt() {
    std::cout << "\nWhat would you like to do? (look/take/solve/move/craft/drop/inventory/quit) ";
}

void ZOOrkEngine::displayRoomDescription() {
    Room* currentRoom = player->getCurrentRoom();
    std::cout << "\nHi Rodie, You are in the " << currentRoom->getName() << ".\n";
    std::cout << currentRoom->getDescription() << "\n";
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Go where? Specify a direction (north, south, east, west).\n";
        return;
    }

    std::string direction = arguments[0];
    Room* currentRoom = player->getCurrentRoom();

    // Check if player is in the Treasure Vault and trying to move east
    if (currentRoom->getName() == "Treasure Vault" && direction == "east") {
        if (player->hasItem("Master Key")) {
            handleGameOver();
            return;
        } else {
            std::cout << "You cannot end the game without the Master Key.\n";
            return;
        }
    }

    // Other movement conditions
    if (currentRoom->getName() == "Clockwork Room" && direction == "east" && !currentRoom->isPuzzleSolved()) {
        std::cout << "You must solve the puzzle in the Clockwork Room before moving east to the Garden of Illusions.\n";
        return;
    }

    auto passage = currentRoom->getPassage(direction);

    if (passage) {
        if (passage->getTo()->getName() == "Shadow Room" && !player->hasItem("Torch")) {
            std::cout << "It's too dark to enter the Shadow Room without a Torch.\n";
        } else {
            player->setCurrentRoom(passage->getTo());
            displayRoomDescription();
        }
    } else {
        std::cout << "You can't go that way.\n";
    }
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    if (arguments.empty()) {
        const auto& items = currentRoom->getItems();
        std::cout << "Items in the room: ";
        if (items.empty()) {
            std::cout << "None\n";
        } else {
            std::cout << "\n";
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << i + 1 << ". " << items[i]->getName() << "\n";
            }
            std::cout << "To take an item, use the command 'take' followed by the item number.\n";
        }
    } else {
        // Look at a specific object in the room
        std::string target = arguments[0];
        std::string targetDescription = currentRoom->getTargetDescription(target);
        if (!targetDescription.empty()) {
            std::cout << targetDescription << std::endl;
        } else {
            std::cout << "You don't see anything special about " << target << "." << std::endl;
        }
    }
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    if (!currentRoom->isPuzzleSolved()) {
        std::cout << "You cannot pick item without solving the puzzle.\n";
        return;
    }

    if (arguments.empty()) {
        std::cout << "Take what? Items in the room: ";
        currentRoom->showItems();
        return;
    }

    std::string itemIdentifier = arguments[0];
    const auto& items = currentRoom->getItems(); // Use the getter method to access items

    // Check if the identifier is a number
    try {
        int itemIndex = std::stoi(itemIdentifier);
        if (itemIndex > 0 && itemIndex <= items.size()) {
            std::shared_ptr<Item> item = items[itemIndex - 1];
            player->addItem(item);
            currentRoom->removeItem(item->getName());
            std::cout << "You have picked up the " << item->getName() << ".\n";
        } else {
            std::cout << "There is no item with number " << itemIdentifier << " here.\n";
        }
    } catch (std::invalid_argument&) {
        std::cout << "Invalid item number.\n";
    }
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Drop what? To drop an item, use the command 'drop' followed by the item number.\n";
        player->showInventory(gameOver);
        return;
    }

    // Join the arguments to handle item names with spaces
    std::string itemIdentifier = arguments[0];
    for (size_t i = 1; i < arguments.size(); ++i) {
        itemIdentifier += " " + arguments[i];
    }

    std::shared_ptr<Item> item;

    // Check if the identifier is a number (item index)
    try {
        int itemIndex = std::stoi(itemIdentifier);
        const auto& inventory = player->getInventory();
        if (itemIndex > 0 && itemIndex <= inventory.size()) {
            item = inventory[itemIndex - 1];
        } else {
            std::cout << "Invalid item number.\n";
            return;
        }
    } catch (std::invalid_argument&) {
        // If it's not a number, treat it as an item name
        item = player->getItem(itemIdentifier);
    }

    if (item) {
        player->removeItem(item->getName());
        Room* currentRoom = player->getCurrentRoom();
        currentRoom->addItem(item);
        std::cout << "You have dropped the " << item->getName() << ".\n";
    } else {
        std::cout << "You don't have a " << itemIdentifier << ".\n";
    }
}

void ZOOrkEngine::handleSolveCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    if (currentRoom->isPuzzleSolved()) {
        std::cout << "The puzzle in this room has already been solved.\n";
        return;
    }
    
    if (currentRoom->hasBoxPuzzle()) {
        int choice;
        std::cout << "There are three boxes here. One of them contains the key to the next room, the other two have traps that reduce your HP by 15%. Choose a box (1, 2, or 3): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the input buffer
        if (currentRoom->solveBoxPuzzle(choice)) {
            std::cout << "Correct! You have found the key in the box.\n";
        } else {
            player->reduceHealth(15);
            std::cout << "Incorrect! The box contained a trap. Your health is now " << player->getHealth() << "%.\n";
        }
    } else if (currentRoom->hasPuzzle()) {
        std::cout << "Solve the puzzle: " << currentRoom->getPuzzle() << "\n";
        std::string attempt;
        std::getline(std::cin, attempt); // Read the user input
        // Make the comparison case-insensitive
        std::transform(attempt.begin(), attempt.end(), attempt.begin(), ::tolower);
        std::string solution = currentRoom->getSolution();
        std::transform(solution.begin(), solution.end(), solution.begin(), ::tolower);
//        std::cout << "Attempt: " << attempt << ", Solution: " << solution << "\n";
        if (attempt == solution) {
            currentRoom->solvePuzzle(attempt);
            std::cout << "Correct! You have solved the puzzle in the " << currentRoom->getName() << ". Now you can pick the item as a reward.\n";
        } else {
            std::cout << "Incorrect. Try again.\n";
        }
    } else {
        std::cout << "There is no puzzle to solve in this room.\n";
    }
}

void ZOOrkEngine::handleCraftCommand(std::vector<std::string> arguments) {
    if (player->hasItem("Stick") && player->hasItem("Fire Amulet")) {
        player->removeItem("Stick");
        player->removeItem("Fire Amulet");
        std::shared_ptr<Item> torch = std::make_shared<Item>("Torch", "A torch made from a stick and a fire amulet, it can light up dark places.");
        player->addItem(torch);
        std::cout << "You have crafted a Torch from the Stick and Fire Amulet.\n";
    } else {
        std::cout << "You don't have the necessary items to craft anything.\n";
    }
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::cin >> input;
    std::string quitStr = makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
}

void ZOOrkEngine::handleGameOver() {
    std::cout << "\nCongratulations, Rodie! You have reached the end of the game.\n";
    std::cout << "Your final inventory: \n";
    player->showInventory(true); // Pass true to indicate game over
    std::cout << "Your final health: " << player->getHealth() << "\n";
    gameOver = true;
}

std::vector<std::string> ZOOrkEngine::tokenizeString(const std::string &input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(makeLowercase(token));
    }

    return tokens;
}

std::string ZOOrkEngine::makeLowercase(std::string input) {
    std::string output = std::move(input);
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);

    return output;
}
