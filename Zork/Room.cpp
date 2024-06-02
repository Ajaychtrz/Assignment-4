#include "NullPassage.h"
#include "Room.h"
#include <utility>
#include <iostream>

Room::Room(const std::string &n, const std::string &d, const std::string &puzzle, const std::string &solution)
    : Location(n, d), description(d), puzzle(puzzle), solution(solution) {
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
}

Room::Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c, const std::string &puzzle, const std::string &solution)
    : Location(n, d, std::move(c)), description(d), puzzle(puzzle), solution(solution) {}

void Room::addPassage(const std::string &direction, std::shared_ptr<Passage> p) {
    passageMap[direction] = std::move(p);
}

void Room::removePassage(const std::string &direction) {
    if (passageMap.contains(direction)) {
        passageMap.erase(direction);
    }
}

std::shared_ptr<Passage> Room::getPassage(const std::string &direction) {
    if (passageMap.contains(direction)) {
        return passageMap[direction];
    } else {
        std::cout << "It is impossible to go " << direction << "!\n";
        return std::make_shared<NullPassage>(this);
    }
}

void Room::addItem(std::shared_ptr<Item> item) {
    items.push_back(item);
}

void Room::removeItem(const std::string &itemName) {
    items.erase(std::remove_if(items.begin(), items.end(),
                               [&itemName](std::shared_ptr<Item> item) { return item->getName() == itemName; }),
                items.end());
}

std::shared_ptr<Item> Room::getItem(const std::string &itemName) {
    std::string lowerItemName = itemName;
    std::transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

    for (const auto &item : items) {
        std::string lowerItem = item->getName();
        std::transform(lowerItem.begin(), lowerItem.end(), lowerItem.begin(), ::tolower);
        if (lowerItem == lowerItemName) {
            return item;
        }
    }
    return nullptr;
}

void Room::showItems() const {
    int index = 1;
    for (const auto& item : items) {
        std::cout << index++ << ". " << item->getName() << "\n";
    }
}

const std::vector<std::shared_ptr<Item>>& Room::getItems() const {
    return items;
}

void Room::addCharacter(std::shared_ptr<Character> character) {
    characters.push_back(character);
}

void Room::removeCharacter(const std::string &characterName) {
    characters.erase(std::remove_if(characters.begin(), characters.end(),
                                    [&characterName](std::shared_ptr<Character> character) { return character->getName() == characterName; }),
                     characters.end());
}

std::shared_ptr<Character> Room::getCharacter(const std::string &characterName) {
    for (const auto &character : characters) {
        if (character->getName() == characterName) {
            return character;
        }
    }
    return nullptr;
}

void Room::showCharacters() const {
    for (const auto& character : characters) {
        std::cout << character->getName() << ", ";
    }
    std::cout << "\n";
}

const std::vector<std::shared_ptr<Character>>& Room::getCharacters() const {
    return characters;
}

std::string Room::getTargetDescription(const std::string &target) const {
    // Check for items first
    for (const auto &item : items) {
        if (item->getName() == target) {
            return item->getDescription();
        }
    }

    // Check for characters
    for (const auto &character : characters) {
        if (character->getName() == target) {
            return character->getDescription();
        }
    }

    return "There is nothing special about " + target + ".";
}

bool Room::solvePuzzle(const std::string &attempt) {
    if (attempt == solution) {
        puzzleSolved = true;
        return true;
    }
    return false;
}
