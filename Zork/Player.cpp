#include "Player.h"
#include <iostream>

Player *Player::playerInstance = nullptr;

void Player::setCurrentRoom(Room* room) {
    currentRoom = room;
}

Room* Player::getCurrentRoom() const {
    return currentRoom;
}

void Player::addItem(std::shared_ptr<Item> item) {
    inventory.push_back(item);
}

std::shared_ptr<Item> Player::getItem(const std::string &itemName) {
    for (const auto &item : inventory) {
        if (item->getName() == itemName) {
            return item;
        }
    }
    return nullptr;
}

void Player::removeItem(const std::string &itemName) {
    inventory.erase(std::remove_if(inventory.begin(), inventory.end(),
                                   [&itemName](std::shared_ptr<Item> item) { return item->getName() == itemName; }),
                    inventory.end());
}

void Player::showInventory() const {
    std::cout << "Inventory: ";
    if (inventory.empty()) {
        std::cout << "You are not carrying anything.\n";
    } else {
        for (const auto& item : inventory) {
            std::cout << item->getName() << ", ";
        }
        std::cout << "\n";
    }
}
