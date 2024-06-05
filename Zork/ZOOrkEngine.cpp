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
    displayRoomDescription(); // Initial room description
    displayPrompt();
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go" || command == "move") {
            handleGoCommand(arguments);
        } else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        } else if ((command == "take") || (command == "get") || (command == "pick up")) {
            handleTakeCommand(arguments);
        } else if (command == "drop") {
            handleDropCommand(arguments);
        } else if (command == "use") {
            handleUseCommand(arguments);
        } else if (command == "solve") {
            handleSolveCommand(arguments);
        } else if (command == "craft") {
            handleCraftCommand(arguments);
        } else if (command == "inventory") {
            player->showInventory();
            displayPrompt();
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command.\n";
            displayPrompt();
        }
    }
}

void ZOOrkEngine::displayPrompt() {
    std::cout << "\nWhat would you like to do? (look/take/solve/move/craft/use/inventory/quit) ";
}

void ZOOrkEngine::displayRoomDescription() {
    Room* currentRoom = player->getCurrentRoom();
    std::cout << "\nHi Rodie, You are in the " << currentRoom->getName() << ".\n";
    std::cout << currentRoom->getDescription() << "\n";
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Go where? Specify a direction (north, south, east, west).\n";
        displayPrompt();
        return;
    }

    std::string direction = arguments[0];
    Room* currentRoom = player->getCurrentRoom();
    auto passage = currentRoom->getPassage(direction);

    if (passage) {
        if (passage->getTo()->getName() == "Shadow Room" && !player->hasItem("Torch")) {
            std::cout << "It's too dark to enter the Shadow Room without a Torch.\n";
        } else if (currentRoom->getName() == "Treasure Vault" && direction == "east") {
            handleGameOver();
            return;
        } else {
            player->setCurrentRoom(passage->getTo());
            passage->enter();
            displayRoomDescription();
        }
    } else {
        std::cout << "You can't go that way.\n";
    }
    displayPrompt();
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

    displayPrompt();
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    if (!currentRoom->isPuzzleSolved()) {
        std::cout << "You cannot pick item without solving the puzzle.\n";
        displayPrompt();
        return;
    }

    if (arguments.empty()) {
        std::cout << "Take what? Items in the room: ";
        currentRoom->showItems();
        displayPrompt();
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

    displayPrompt();
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Drop what?\n";
        displayPrompt();
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

    displayPrompt();
}

void ZOOrkEngine::handleUseCommand(std::vector<std::string> arguments) {
    if (arguments.empty()) {
        std::cout << "Use what?\n";
        displayPrompt();
        return;
    }

    std::string itemName = arguments[0];
    std::shared_ptr<Item> item = player->getItem(itemName);

    if (item) {
        item->use();
    } else {
        std::cout << "You don't have a " << itemName << ".\n";
    }

    displayPrompt();
}

void ZOOrkEngine::handleSolveCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();
    
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
        std::cout << "Attempt: " << attempt << ", Solution: " << solution << "\n";  // Debug print
        if (attempt == solution) {
            currentRoom->solvePuzzle(attempt);
            std::cout << "Correct! You have solved the puzzle in the " << currentRoom->getName() << ". Now you can pick the item as a reward.\n";
        } else {
            std::cout << "Incorrect. Try again.\n";
        }
    } else {
        std::cout << "There is no puzzle to solve in this room.\n";
    }
    displayPrompt();
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
    displayPrompt();
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::cin >> input;
    std::string quitStr = makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    } else {
        displayPrompt();
    }
}

void ZOOrkEngine::handleGameOver() {
    std::cout << "\nCongratulations, Rodie! You have reached the end of the game.\n";
    std::cout << "Your final inventory: \n";
    player->showInventory();
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
