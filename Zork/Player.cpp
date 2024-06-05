#include "Player.h"

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

void Player::removeItem(const std::string& itemName) {
    inventory.erase(std::remove_if(inventory.begin(), inventory.end(),
                                   [&itemName](std::shared_ptr<Item> item) { return item->getName() == itemName; }),
                    inventory.end());
}

std::shared_ptr<Item> Player::getItem(const std::string& itemName) {
    for (const auto& item : inventory) {
        if (item->getName() == itemName) {
            return item;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Item>> Player::getInventory() const {
    return inventory;
}

void Player::showInventory(bool gameOver) const {
    std::cout << "\nInventory:\n";
    if (inventory.empty()) {
        std::cout << "You are not carrying anything.";
    } else {
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i + 1 << ". " << inventory[i]->getName() << ",\n";
        }
    }
    std::cout << "\n";
}


bool Player::hasItem(const std::string& itemName) const {
    for (const auto& item : inventory) {
        if (item->getName() == itemName) {
            return true;
        }
    }
    return false;
}
