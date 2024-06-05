#ifndef ZOORK_ROOM_H
#define ZOORK_ROOM_H

#include "Character.h"
#include "RoomDefaultEnterCommand.h"
#include "Item.h"
#include "Location.h"
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>

class Passage;

class Room : public Location {
public:
    Room(const std::string &, const std::string &, const std::string& puzzle = "", const std::string& solution = "");
    Room(const std::string &, const std::string &, std::shared_ptr<Command>, const std::string& puzzle = "", const std::string& solution = "");

    void addPassage(const std::string &, std::shared_ptr<Passage>);
    void removePassage(const std::string &);
    std::shared_ptr<Passage> getPassage(const std::string &);
    std::string getDescription() const { return description; }
    std::string getTargetDescription(const std::string &) const;
    

    void addItem(std::shared_ptr<Item> item);
    void removeItem(const std::string& itemName);
    const std::vector<std::shared_ptr<Item>>& getItems() const;
    void showItems() const;



    void addCharacter(std::shared_ptr<Character> character);

    // Add the declaration of the enter method
    void enter();

private:
    std::map<std::string, std::shared_ptr<Passage>> passageMap;
    std::string description;
    std::string puzzle;
    std::string solution;
    bool puzzleSolved = false;
    std::vector<std::shared_ptr<Item>> items;
    bool has_box_puzzle = false;
    int box_with_key;
    std::vector<std::shared_ptr<Character>> characters;

    void initializeBoxPuzzle();
};

#endif //ZOORK_ROOM_H
