#include "Room.h"
#include "NullPassage.h"
#include <utility>
#include <algorithm>

Room::Room(const std::string &n, const std::string &d, const std::string& p, const std::string& s)
    : Location(n, d), description(d) {
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
}

Room::Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c, const std::string& p, const std::string& s)
    : Location(n, d, std::move(c)), description(d) {
}


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

std::string Room::getTargetDescription(const std::string &target) const {
    return "There is nothing special about " + target + ".";
}

void Room::addItem(std::shared_ptr<Item> item) {
    items.push_back(item);
}

void Room::removeItem(const std::string& itemName) {
    items.erase(std::remove_if(items.begin(), items.end(),
               [&itemName](const std::shared_ptr<Item>& item) { return item->getName() == itemName; }),
               items.end());
}

const std::vector<std::shared_ptr<Item>>& Room::getItems() const {
    return items;
}

void Room::showItems() const {
    if (items.empty()) {
        std::cout << "None\n";
    } else {
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i + 1 << ". " << items[i]->getName() << "\n";
        }
    }
}


void Room::addCharacter(std::shared_ptr<Character> character) {
    characters.push_back(character);
}

void Room::enter() {
    // Handle any logic needed for entering the room, but do not print the description.
}
