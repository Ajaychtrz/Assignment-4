#include "Room.h"
#include "NullPassage.h"
#include <utility>
#include <random>

Room::Room(const std::string &n, const std::string &d, const std::string& p, const std::string& s)
    : Location(n, d), description(d), puzzle(p), solution(s) {
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
    if (p == "box puzzle") {
        has_box_puzzle = true;
        initializeBoxPuzzle();
    }
}

Room::Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c, const std::string& p, const std::string& s)
    : Location(n, d, std::move(c)), description(d), puzzle(p), solution(s) {
    if (p == "box puzzle") {
        has_box_puzzle = true;
        initializeBoxPuzzle();
    }
}

void Room::initializeBoxPuzzle() {
    // Randomly assign a box to contain the key (1, 2, or 3)
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 3);
    box_with_key = dist(mt);
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

bool Room::solvePuzzle(const std::string& attempt) {
    std::string lowerAttempt = attempt;
    std::string lowerSolution = solution;
    std::transform(lowerAttempt.begin(), lowerAttempt.end(), lowerAttempt.begin(), ::tolower);
    std::transform(lowerSolution.begin(), lowerSolution.end(), lowerSolution.begin(), ::tolower);
    if (lowerAttempt == lowerSolution) {
        puzzleSolved = true;
        return true;
    }
    return false;
}

bool Room::solveBoxPuzzle(int boxNumber) {
    if (boxNumber == box_with_key) {
        puzzleSolved = true;
        return true;
    }
    return false;
}

void Room::addCharacter(std::shared_ptr<Character> character) {
    characters.push_back(character);
}
