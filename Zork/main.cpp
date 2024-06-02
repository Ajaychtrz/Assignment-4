#include "RoomDefaultEnterCommand.h"
#include "Passage.h"
#include "Player.h"
#include "Room.h"
#include "ZOOrkEngine.h"
#include "Character.h"
#include "Item.h"

int main() {
    // Create the rooms
    std::shared_ptr<Room> entrance_hall = std::make_shared<Room>("Entrance Hall",
                           "A grand hall with stone walls covered in moss. There's an inscription on the wall.\n");

    std::shared_ptr<Room> mirror_chamber = std::make_shared<Room>("Mirror Chamber",
                                    "A room filled with mirrors of various sizes and shapes.\n");

    std::shared_ptr<Room> library_of_whispers = std::make_shared<Room>("Library of Whispers",
                                  "Shelves filled with dusty old books, some of which seem to whisper as you pass.\n");

    std::shared_ptr<Room> clockwork_room = std::make_shared<Room>("Clockwork Room",
                                 "A room dominated by a giant, intricate clockwork mechanism.\n");

    std::shared_ptr<Room> garden_of_illusions = std::make_shared<Room>("Garden of Illusions",
                                    "An underground garden with plants that change shape and color.\n");

    std::shared_ptr<Room> hall_of_echoes = std::make_shared<Room>("Hall of Echoes",
                                "A long, narrow hallway where every sound echoes infinitely.\n");

    std::shared_ptr<Room> frozen_chamber = std::make_shared<Room>("Frozen Chamber",
                                  "A room encased in ice, with a pedestal at its center.\n");

    std::shared_ptr<Room> shadow_room = std::make_shared<Room>("Shadow Room",
                                "A dark room where shadows move independently of their objects.\n");

    std::shared_ptr<Room> hall_of_champions = std::make_shared<Room>("Hall of Champions",
                                      "Statues of legendary Zork heroes line the walls.\n");

    std::shared_ptr<Room> treasure_vault = std::make_shared<Room>("Treasure Vault",
                                    "A room filled with gold, jewels, and a large, ornate chest.\n");

    // Create passages between the rooms
    Passage::createBasicPassage(entrance_hall.get(), mirror_chamber.get(), "north", true);
    Passage::createBasicPassage(mirror_chamber.get(), entrance_hall.get(), "south", true);
    Passage::createBasicPassage(mirror_chamber.get(), library_of_whispers.get(), "east", true);
    Passage::createBasicPassage(library_of_whispers.get(), mirror_chamber.get(), "west", true);
    Passage::createBasicPassage(library_of_whispers.get(), clockwork_room.get(), "north", true);
    Passage::createBasicPassage(clockwork_room.get(), library_of_whispers.get(), "south", true);
    Passage::createBasicPassage(clockwork_room.get(), garden_of_illusions.get(), "east", true);
    Passage::createBasicPassage(garden_of_illusions.get(), clockwork_room.get(), "west", true);
    Passage::createBasicPassage(garden_of_illusions.get(), hall_of_echoes.get(), "north", true);
    Passage::createBasicPassage(hall_of_echoes.get(), garden_of_illusions.get(), "south", true);
    Passage::createBasicPassage(hall_of_echoes.get(), frozen_chamber.get(), "east", true);
    Passage::createBasicPassage(frozen_chamber.get(), hall_of_echoes.get(), "west", true);
    Passage::createBasicPassage(frozen_chamber.get(), shadow_room.get(), "north", true);
    Passage::createBasicPassage(shadow_room.get(), frozen_chamber.get(), "south", true);
    Passage::createBasicPassage(shadow_room.get(), hall_of_champions.get(), "east", true);
    Passage::createBasicPassage(hall_of_champions.get(), shadow_room.get(), "west", true);
    Passage::createBasicPassage(hall_of_champions.get(), treasure_vault.get(), "north", true);
    Passage::createBasicPassage(treasure_vault.get(), hall_of_champions.get(), "south", true);

    // Create a character and add it to a room
    std::shared_ptr<Character> rodie_jaz = std::make_shared<Character>("Rodie Jaz", "A brave adventurer with a keen sense of direction.");
    entrance_hall->addCharacter(rodie_jaz);

    // Create items and add them to rooms
    std::shared_ptr<Item> translators_amulet = std::make_shared<Item>("Translator's Amulet", "A magical amulet that helps you understand ancient languages.");
    entrance_hall->addItem(translators_amulet);

    std::shared_ptr<Item> silver_key = std::make_shared<Item>("Silver Key", "A shiny silver key that unlocks mysterious doors.");
    mirror_chamber->addItem(silver_key);

    std::shared_ptr<Item> mysterious_book = std::make_shared<Item>("Mysterious Book", "An ancient book filled with strange writings.");
    library_of_whispers->addItem(mysterious_book);

    std::shared_ptr<Item> clockwork_key = std::make_shared<Item>("Clockwork Key", "A key that fits into the clockwork mechanism.");
    clockwork_room->addItem(clockwork_key);

    std::shared_ptr<Item> enchanted_flower = std::make_shared<Item>("Enchanted Flower", "A flower with magical properties.");
    garden_of_illusions->addItem(enchanted_flower);

    std::shared_ptr<Item> echo_stone = std::make_shared<Item>("Echo Stone", "A stone that resonates with echoing sounds.");
    hall_of_echoes->addItem(echo_stone);

    std::shared_ptr<Item> fire_amulet = std::make_shared<Item>("Fire Amulet", "An amulet that radiates warmth and can melt ice.");
    frozen_chamber->addItem(fire_amulet);

    std::shared_ptr<Item> shadow_lantern = std::make_shared<Item>("Shadow Lantern", "A lantern that casts eerie shadows.");
    shadow_room->addItem(shadow_lantern);

    std::shared_ptr<Item> heros_medallion = std::make_shared<Item>("Hero's Medallion", "A medallion awarded to legendary heroes.");
    hall_of_champions->addItem(heros_medallion);

    std::shared_ptr<Item> master_key = std::make_shared<Item>("Master Key", "A key that unlocks the final treasure chest.");
    treasure_vault->addItem(master_key);

    // Initialize the game engine with the starting room
    ZOOrkEngine zoork(entrance_hall);

    zoork.run();

    return 0;
}
