#include "ZOOrkEngine.h"
#include "Room.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());
    player->getCurrentRoom()->enter();
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go") {
            handleGoCommand(arguments);
        } else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        } else if ((command == "take") || (command == "get")) {
            handleTakeCommand(arguments);
        } else if (command == "drop") {
            handleDropCommand(arguments);
        } else if (command == "use") {
            handleUseCommand(arguments);
        } else if (command == "inventory") {
            player->showInventory();
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command.\n";
        }
    }
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
        passage->enter();
    } else {
        std::cout << "You can't go that way.\n";
    }
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    if (arguments.empty()) {
        // Look at the current room
        std::cout << currentRoom->getDescription() << std::endl;
        currentRoom->showItems();
        currentRoom->showCharacters();
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
    if (arguments.empty()) {
        std::cout << "Take what? Items in the room: ";
        player->getCurrentRoom()->showItems();
        return;
    }

    std::string itemIdentifier = arguments[0];
    Room* currentRoom = player->getCurrentRoom();
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
        std::cout << "Drop what?\n";
        return;
    }

    std::string itemName = arguments[0];
    std::shared_ptr<Item> item = player->getItem(itemName);

    if (item) {
        player->removeItem(itemName);
        Room* currentRoom = player->getCurrentRoom();
        currentRoom->addItem(item);
        std::cout << "You have dropped the " << itemName << ".\n";
    } else {
        std::cout << "You don't have a " << itemName << ".\n";
    }
}

void ZOOrkEngine::handleUseCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Use what?\n";
        return;
    }

    std::string itemName = arguments[0];
    std::shared_ptr<Item> item = player->getItem(itemName);

    if (item) {
        item->use();
    } else {
        std::cout << "You don't have a " << itemName << ".\n";
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
