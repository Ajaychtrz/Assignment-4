#include "RoomDefaultEnterCommand.h"
#include "Passage.h"
#include "Player.h"
#include "Room.h"
#include "ZOOrkEngine.h"
#include "Character.h"
#include "Item.h"

int main() {
    // Create the rooms with puzzles
    std::shared_ptr<Room> catacomb_entrance = std::make_shared<Room>("Catacomb Entrance",
                           "A dark and foreboding entrance to the ancient catacombs. The air is musty, and you can hear faint echoes from within.\n",
                           "Find the hidden lever to open the way further in.", "lever");

    std::shared_ptr<Room> hall_of_whispers = std::make_shared<Room>("Hall of Whispers",
                                    "A long hallway where every sound seems to be amplified. The walls are lined with ancient carvings that seem to whisper secrets of the past.\n",
                                    "Decipher the whispers to reveal a hidden message.", "whispers");

    std::shared_ptr<Room> hidden_passageway = std::make_shared<Room>("Hidden Passageway",
                                  "A narrow, secret passage leading deeper into the catacombs. The walls are covered in ancient runes that glow faintly.\n",
                                  "Use the shadows to find a hidden key.", "glow");

    std::shared_ptr<Room> archive_of_elders = std::make_shared<Room>("Archive of Elders",
                                 "A dusty archive filled with ancient scrolls and tomes. The knowledge of Eldoria’s past is stored here, but some scrolls are written in cryptic languages.\n",
                                 "Find the scroll that translates the ancient text.", "scroll");

    std::shared_ptr<Room> sanctuary_of_light = std::make_shared<Room>("Sanctuary of Light",
                                    "A bright, radiant room that seems out of place in the dark catacombs. A glowing crystal in the center illuminates the space.\n",
                                    "Use the crystal's light to reveal a hidden path.", "crystal");

    std::shared_ptr<Room> armory_of_valor = std::make_shared<Room>("Armory of Valor",
                                "A room filled with ancient weapons and armor, some of which still shimmer with magical enchantments.\n",
                                "Equip the right armor to gain access to the next area.", "armor");

    std::shared_ptr<Room> hall_of_mirrors = std::make_shared<Room>("Hall of Mirrors",
                                  "A disorienting hallway lined with mirrors that reflect not only your image but also strange, otherworldly scenes.\n",
                                  "Align the mirrors to reveal a hidden door.", "mirrors");

    std::shared_ptr<Room> throne_of_kings = std::make_shared<Room>("Throne of Kings",
                                "A grand room with a majestic throne at the far end, once the seat of Eldoria’s rulers. The air is thick with a sense of lost glory.\n",
                                "Sit on the throne to unlock a hidden compartment.", "throne");

    std::shared_ptr<Room> guardians_keep = std::make_shared<Room>("Guardian’s Keep",
                                  "A heavily fortified room where the Guardian of the Orb resides. The Guardian’s presence is imposing, and the air crackles with magical energy.\n",
                                  "Defeat the Guardian to proceed.", "guardian");

    std::shared_ptr<Room> chamber_of_eternity = std::make_shared<Room>("Chamber of Eternity",
                                    "The final room where the Orb of Eternity rests on a pedestal, radiating with immense power. The room is filled with an eerie, otherworldly light.\n",
                                    "Solve the final puzzle to claim the Orb.", "puzzle");

    // Create passages between the rooms
    Passage::createBasicPassage(catacomb_entrance.get(), hall_of_whispers.get(), "north", true);
    Passage::createBasicPassage(hall_of_whispers.get(), catacomb_entrance.get(), "south", true);
    Passage::createBasicPassage(hall_of_whispers.get(), hidden_passageway.get(), "east", true);
    Passage::createBasicPassage(hidden_passageway.get(), hall_of_whispers.get(), "west", true);
    Passage::createBasicPassage(hall_of_whispers.get(), archive_of_elders.get(), "west", true);
    Passage::createBasicPassage(archive_of_elders.get(), hall_of_whispers.get(), "east", true);
    Passage::createBasicPassage(hidden_passageway.get(), sanctuary_of_light.get(), "north", true);
    Passage::createBasicPassage(sanctuary_of_light.get(), hidden_passageway.get(), "south", true);
    Passage::createBasicPassage(archive_of_elders.get(), armory_of_valor.get(), "north", true);
    Passage::createBasicPassage(armory_of_valor.get(), archive_of_elders.get(), "south", true);
    Passage::createBasicPassage(sanctuary_of_light.get(), hall_of_mirrors.get(), "east", true);
    Passage::createBasicPassage(hall_of_mirrors.get(), sanctuary_of_light.get(), "west", true);
    Passage::createBasicPassage(armory_of_valor.get(), throne_of_kings.get(), "north", true);
    Passage::createBasicPassage(throne_of_kings.get(), armory_of_valor.get(), "south", true);
    Passage::createBasicPassage(hall_of_mirrors.get(), guardians_keep.get(), "north", true);
    Passage::createBasicPassage(guardians_keep.get(), hall_of_mirrors.get(), "south", true);
    Passage::createBasicPassage(guardians_keep.get(), chamber_of_eternity.get(), "north", true);
    Passage::createBasicPassage(chamber_of_eternity.get(), guardians_keep.get(), "south", true);


    // Create items and add them to rooms
    std::shared_ptr<Item> lever = std::make_shared<Item>("Hidden Lever", "A lever hidden in the wall.");
    catacomb_entrance->addItem(lever);

    std::shared_ptr<Item> whispers_book = std::make_shared<Item>("Whispers Book", "A book that helps decipher the whispers.");
    hall_of_whispers->addItem(whispers_book);

    std::shared_ptr<Item> shadow_key = std::make_shared<Item>("Hidden key", "A key found by aligning the shadows.");
    hidden_passageway->addItem(shadow_key);

    std::shared_ptr<Item> translation_scroll = std::make_shared<Item>("Translation Scroll", "A scroll that translates ancient text.");
    archive_of_elders->addItem(translation_scroll);

    std::shared_ptr<Item> crystal_light = std::make_shared<Item>("Crystal Light", "A crystal that reveals hidden paths.");
    sanctuary_of_light->addItem(crystal_light);

    std::shared_ptr<Item> valor_armor = std::make_shared<Item>("Valor Armor", "A piece of armor that grants access to the next area.");
    armory_of_valor->addItem(valor_armor);

    std::shared_ptr<Item> mirror_shard = std::make_shared<Item>("Mirror Shard", "A shard that can be used to align mirrors.");
    hall_of_mirrors->addItem(mirror_shard);

    std::shared_ptr<Item> kings_compartment = std::make_shared<Item>("King's Compartment Key", "A key to the hidden compartment in the throne.");
    throne_of_kings->addItem(kings_compartment);

    std::shared_ptr<Item> guardian_token = std::make_shared<Item>("Guardian Token", "A token that signifies the defeat of the Guardian.");
    guardians_keep->addItem(guardian_token);

    std::shared_ptr<Item> orb_of_eternity = std::make_shared<Item>("Orb of Eternity", "The powerful Orb of Eternity.");
    chamber_of_eternity->addItem(orb_of_eternity);

    // Initialize the game engine with the starting room
    ZOOrkEngine zoork(catacomb_entrance);

    zoork.run();

    return 0;
}
