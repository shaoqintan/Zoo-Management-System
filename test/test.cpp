#include <gtest/gtest.h>

extern "C" {
#include <zoo.h>
}

#define MAX_LEN 1024

const char* names[5] = {"Cute", "Sea", "Panda", "Penguin", "Starfish"};
Area zoo[2];
Animal animals[4];


class basicDatabaseEnvironment : public testing::Environment {
 public:
  virtual void SetUp() {
    zoo[0].name = (char*)malloc(strlen(names[0]) + 1);
    strcpy(zoo[0].name, names[0]);
    zoo[0].list = NULL;
    zoo[1].name = (char*)malloc(strlen(names[1]) + 1);
    strcpy(zoo[1].name, names[1]);
    zoo[1].list = NULL;

    animals[0].area = (char*)malloc(strlen(names[0]) + 1);
    animals[0].animal_kind = (char*)malloc(strlen(names[2]) + 1);
    strcpy(animals[0].area, names[0]);
    strcpy(animals[0].animal_kind, names[2]);
    animals[1].area = (char*)malloc(strlen(names[0]) + 1);
    animals[1].animal_kind = (char*)malloc(strlen(names[3]) + 1);
    strcpy(animals[1].area, names[0]);
    strcpy(animals[1].animal_kind, names[3]);
    animals[2].area = (char*)malloc(strlen(names[1]) + 1);
    animals[2].animal_kind = (char*)malloc(strlen(names[3]) + 1);
    strcpy(animals[2].area, names[1]);
    strcpy(animals[2].animal_kind, names[3]);
    animals[3].area = (char*)malloc(strlen(names[1]) + 1);
    animals[3].animal_kind = (char*)malloc(strlen(names[4]) + 1);
    strcpy(animals[3].area, names[1]);
    strcpy(animals[3].animal_kind, names[4]);
  }
};

void reset_zoo() {
  zoo[0].list = NULL;
  zoo[1].list = NULL;
}

/**
 * Trying to add all the 'Panda' s in the zoo
 * Should add the Cute Panda and area_add must return 1
 */
TEST(ProjectTests, Area_add_first) {
  reset_zoo();
  int result;
  result = area_add(zoo, 2, animals, 4, "Panda");
  EXPECT_EQ(result, 1) << "The return value of area_add should be 1";
  ASSERT_STREQ((zoo[0].list)->animal->animal_kind, "Panda")
      << "The first animal in Area 'Cute' should be Panda.";
}

/**
 * Trying to add all the 'Apple' s in the zoo
 * No apples exist to nothing added and area_add must return 0
 */
TEST(ProjectTests, Area_add_none) {
  reset_zoo();
  int result;
  result = area_add(zoo, 2, animals, 4, "Apple");
  EXPECT_EQ(result, 0) << "The return value of area_add should be 0";
}

/**
 * Trying to add all the 'Panda' s and 'Penguin' s
 * Should add the Cute Panda, Cute Penguin and Sea Penguin
 * Due to the sorting property the Cute Area linked list must have Panda
 * followed by Penguin
 */
TEST(ProjectTests, Area_add_multiple) {
  reset_zoo();
  int result;
  result = area_add(zoo, 2, animals, 4, "Panda");
  result = area_add(zoo, 2, animals, 4, "Penguin");
  EXPECT_EQ(result, 1) << "The return value of area_add should be 1";
  ASSERT_STREQ((zoo[0].list)->next->animal->animal_kind, "Penguin")
      << "The second animal in Area 'Cute' should be Penguin.";
  ASSERT_STREQ((zoo[1].list)->animal->animal_kind, "Penguin")
      << "The first animal in Area 'Sea' should be Penguin.";
}
   


/**
 * Checks whether removing the second animal from an area works as expected
 * Checks that the correct address is returned and that only that animal from
 * the specified area is removed
 */
TEST(ProjectTests, Area_remove_second_animal) {
  reset_zoo();
  area_add(zoo, 2, animals, 4, "Panda");
  area_add(zoo, 2, animals, 4, "Penguin");
  Animal* result_animal = area_remove(zoo, 2, "Cute", "Penguin");
  EXPECT_TRUE(result_animal == &animals[1])
      << "The returned animal should be the second animal in the animal list.";
  ASSERT_STREQ((zoo[0].list)->animal->animal_kind, "Panda")
      << "The first animal in Area 'Cute' should be Panda.";
  ASSERT_STREQ((zoo[1].list)->animal->animal_kind, "Penguin")
      << "The first animal in Area 'Sea' should be Penguin.";
  EXPECT_TRUE((zoo[0].list)->next == NULL)
      << "The Area 'Cute' should have only one animal";
}

/**
 * Checks whether removing a non-existent animal works as expected
 * i.e. area_remove correctly returns NULL
 */
TEST(ProjectTests, Area_remove_none) {
  reset_zoo();
  area_add(zoo, 2, animals, 4, "Panda");
  area_add(zoo, 2, animals, 4, "Penguin");
  Animal* result_animal = area_remove(zoo, 2, "Cute", "Penguin");
  result_animal = area_remove(zoo, 2, "Cute", "Penguin");
  EXPECT_TRUE(result_animal == NULL) << "The returned result should be NULL";
}

/**
 * Checks whether removing the first node (only node) from a linked list works
 * as expected area_remove returns the correct address and the linked list for
 * the other area remains unchanged
 */
TEST(ProjectTests, Area_remove_first_animal) {
  reset_zoo();
  area_add(zoo, 2, animals, 4, "Panda");
  area_add(zoo, 2, animals, 4, "Penguin");
  Animal* result_animal = area_remove(zoo, 2, "Sea", "Penguin");
  EXPECT_TRUE(result_animal == &animals[2])
      << "The returned animal should be the third animal in the animal list.";
  ASSERT_STREQ((zoo[0].list)->animal->animal_kind, "Panda")
      << "The first animal in Area 'Cute' should be Panda.";
  ASSERT_STREQ((zoo[0].list)->next->animal->animal_kind, "Penguin")
      << "The second animal in Area 'Cute' should be Penguin.";
  EXPECT_TRUE(zoo[1].list == NULL) << "The Area 'Sea' should be empty";
}

/**
 * Checks whether area_change correctly changes the area of animal not in the
 * zoo area_change returns the correct adress and the animal's area is updated
 * correctly
 */
TEST(ProjectTests, Change_animal_not_included) {
  reset_zoo();
  Animal* result_animal =
      area_change(zoo, 2, animals, 4, "Starfish", "Sea", "Cute");
  EXPECT_TRUE(result_animal == &animals[3])
      << "The returned animal should be the forth animal in the animal list.";
  ASSERT_STREQ(animals[3].area, "Cute")
      << "The area of Starfish should be changed to 'Cute'.";
}

/**
 * Checks that changing the area of a non-existene animal does not do anything
 * i.e. area_change returns NULL Checks that changing the area of an existing
 * animal (not in zoo) to a non-existent area does not do anything i.e.
 * area_change returns NULL
 */
TEST(ProjectTests, Change_animal_with_incorrect_area) {
  reset_zoo();
  Animal* result_animal =
      area_change(zoo, 2, animals, 4, "Panda", "Sea", "Cute");
  EXPECT_TRUE(result_animal == NULL)
      << "The returned result of changing Panda from 'Sea' to 'Cute' should be "
         "NULL since there is no Sea Panda.";

  result_animal = area_change(zoo, 2, animals, 4, "Starfish", "Cute", "Wild");
  EXPECT_TRUE(result_animal == NULL)
      << "The returned result of changing Panda from 'Sea' to 'Wild' should be "
         "NULL because there is no area Wild in the zoo.";
}

/**
 * Checks that changing the area of an animal (not in zoo) that would cause
 * break the unique identifier condition fails i.e. area_change returns NULL and
 * area of the animal remains unchanged
 */
TEST(ProjectTests, Change_animal_to_duplicate_area) {
  reset_zoo();
  Animal* result_animal =
      area_change(zoo, 2, animals, 4, "Penguin", "Sea", "Cute");
  EXPECT_TRUE(result_animal == NULL)
      << "The returned result of changing Penguin from 'Sea' to 'Cute' should "
         "be "
         "NULL since there is a Cute Penguin exist in the list.";
  ASSERT_STREQ(animals[2].area, "Sea")
      << "The area of Sea Penguin should not be changed.";
}

/**
 * Checks that area_change correctly changes the area of an animal that is in
 * the zoo
 */
TEST(ProjectTests, Change_animal_included) {
  reset_zoo();
  area_add(zoo, 2, animals, 4, "Panda");
  Animal* result_animal =
      area_change(zoo, 2, animals, 4, "Panda", "Cute", "Sea");
  EXPECT_TRUE(result_animal == &animals[0])
      << "The returned animal should be the first animal in the animal list.";
  ASSERT_STREQ(animals[0].area, "Sea")
      << "The area of Panda should be changed to Sea";
  ASSERT_STREQ((zoo[1].list)->animal->animal_kind, "Panda")
      << "Panda should be the first animal of zoo[1].";
  EXPECT_TRUE(zoo[0].list == NULL)
      << "The zoo[0].list should be NULL because the area Cute has no "
         "animal.";
}


int main(int argc, char** argv) {
testing:
  AddGlobalTestEnvironment(new basicDatabaseEnvironment);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
