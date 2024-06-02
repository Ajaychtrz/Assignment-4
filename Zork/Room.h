#ifndef ZOORK_ROOM_H
#define ZOORK_ROOM_H

#include "Character.h"
#include "RoomDefaultEnterCommand.h"
#include "Item.h"
#include "Location.h"
#include <map>
#include <vector>
#include <memory>

class Passage;

class Room : public Location {
public:
    Room(const std::string &, const std::string &);
    Room(const std::string &, const std::string &, std::shared_ptr<Command>);

    void addPassage(const std::string &, std::shared_ptr<Passage>);
    void removePassage(const std::string &);
    std::shared_ptr<Passage> getPassage(const std::string &);
    std::string getDescription() const { return description; }
    std::string getTargetDescription(const std::string &) const;
    void addItem(std::shared_ptr<Item>);
    void removeItem(const std::string &);
    std::shared_ptr<Item> getItem(const std::string &);
    void showItems() const;
    const std::vector<std::shared_ptr<Item>>& getItems() const; // Add this line

    // Add methods for managing characters
    void addCharacter(std::shared_ptr<Character>);
    void removeCharacter(const std::string &);
    std::shared_ptr<Character> getCharacter(const std::string &);
    void showCharacters() const;

protected:
    std::map<std::string, std::shared_ptr<Passage>> passageMap;
    std::string description;
    std::vector<std::shared_ptr<Item>> items;
    std::vector<std::shared_ptr<Character>> characters;
};

#endif //ZOORK_ROOM_H
