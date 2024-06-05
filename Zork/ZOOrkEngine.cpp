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
    gameStarted = true;
    player->getCurrentRoom()->enter();
    displayRoomDescription(); // Initial room description
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        displayPrompt(); // Display prompt at the start of the loop

        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go") {
            handleGoCommand(arguments);
        } else if (command == "look") {
            handleLookCommand(arguments);
        } else if (command == "take") {
            handleTakeCommand(arguments);
        } else if (command == "inventory") {
            player->showInventory(gameOver);
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command.\n";
        }
    }
}

void ZOOrkEngine::displayPrompt() {
    std::cout << "Select any one of these option \n(go/look/take/inventory/quit)\n";
}

void ZOOrkEngine::displayRoomDescription() {
    Room* currentRoom = player->getCurrentRoom();
    std::cout << "\nCurrent Room: " << currentRoom->getName() << ".\n";
    std::cout << currentRoom->getDescription() << "\n";
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Go where? Specify a direction (north, south, east, west).\n";
        return;
    }

    std::string direction = arguments[0];
    Room* currentRoom = player->getCurrentRoom();

    auto passage = currentRoom->getPassage(direction);

    if (passage) {
        player->setCurrentRoom(passage->getTo());
        displayRoomDescription();
    } else {
        std::cout << "You can't go that way.\n";
    }
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    if (arguments.empty()) {
        const auto& items = currentRoom->getItems();
        std::cout << "\nItems in the room: ";
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

void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::cin >> input;
    std::string quitStr = makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
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
