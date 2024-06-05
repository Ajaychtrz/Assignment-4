#ifndef ZOOrk_PLAYER_H
#define ZOOrk_PLAYER_H

#include "Character.h"
#include "Location.h"
#include "NullRoom.h"
#include "Item.h"
#include <vector>
#include <memory>
#include <iostream>

class Player : public Character {
public:
    static Player *instance() {
        // Note: lazy instantiation of the singleton Player object
        if (!playerInstance) {
            playerInstance = new Player();
        }
        return Player::playerInstance;
    }

    void setCurrentRoom(Room*);

    Room* getCurrentRoom() const;

    void addItem(std::shared_ptr<Item> item);
    void removeItem(const std::string& itemName);
    std::shared_ptr<Item> getItem(const std::string& itemName);
    std::vector<std::shared_ptr<Item>> getInventory() const; // Add this line
    void showInventory(bool gameOver = false) const; // Add gameOver parameter
    bool hasItem(const std::string& itemName) const;

    Player(const Player &) = delete;

    Player &operator=(const Player &) = delete;

private:
    static Player *playerInstance;
    Room* currentRoom;
    std::vector<std::shared_ptr<Item>> inventory;
    

    Player() : Character("You", "You are a person, alike in dignity to any other, but uniquely you."),
               currentRoom(new NullRoom()) {}
};

#endif //ZOORK_PLAYER_H
