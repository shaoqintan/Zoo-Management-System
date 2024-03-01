#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zoo.h>

int area_add(Area zoo[], int number_of_areas, Animal animals[],
             int number_of_animals, const char *animal_kind)
{

  int i;
  int success = 0;
  for (i = 0; i < number_of_animals; ++i)
  {
    if (strcmp(animals[i].animal_kind, animal_kind) == 0)
    {
      int j;
      int breakout = 0;
      for (j = 0; j < number_of_areas; ++j)
      {
        if (strcmp(zoo[j].name, animals[i].area) == 0)
        { // zoo[j].name is each individual area's name, animalarea
          // is the target animal's area
          AnimalNode *addingNode = malloc(sizeof(AnimalNode));
          addingNode->animal = &(animals[i]);
          addingNode->next = NULL;
          AnimalNode *currNode = zoo[j].list;
          AnimalNode *prevNode = NULL;
          if (zoo[j].list == NULL)
          {
            zoo[j].list = addingNode;
            zoo[j].list->next = NULL;
            success = 1;
            break;
          }
          while (currNode != NULL)
          {
            if ((strcmp(currNode->animal->animal_kind, animal_kind) > 0) && (prevNode != NULL) && strcmp(prevNode->animal->animal_kind, animal_kind) < 0) // problem
            {
              prevNode->next = addingNode;
              addingNode->next = currNode;
              success = 1;
            }
            else if ((prevNode == NULL && (strcmp(currNode->animal->animal_kind, animal_kind) > 0))) // if
            {
              addingNode->next = zoo[j].list;
              zoo[j].list = addingNode;
              success = 1;
            }
            else if (strcmp(currNode->animal->animal_kind, animal_kind) == 0) // if same, break
            {
              breakout = 1;
              break;
            }
            else if ((strcmp(currNode->animal->animal_kind, animal_kind) < 0) && (currNode->next == NULL) && (strcmp(currNode->animal->animal_kind, animal_kind) != 0))
            {
              currNode->next = addingNode;
              addingNode->next = NULL;
              success = 1;
            }
            prevNode = currNode;
            currNode = currNode->next;
          }
          if (breakout == 1)
          {
            break;
          }
        }
      }
    }
  }
  return success;
}

Animal *area_remove(Area zoo[], int number_of_areas, const char *area,
                    const char *animal_kind)
{
  // TODO
  int i;
  Animal *result = NULL;
  for (i = 0; i < number_of_areas; ++i)
  {
    if (strcmp(zoo[i].name, area) == 0)
    {
      AnimalNode *currNode = zoo[i].list;
      AnimalNode *prevNode = NULL;
      if (zoo[i].list == NULL)
      {
        return NULL;
      }
      while (currNode != NULL)
      {
        if (strcmp(currNode->animal->animal_kind, animal_kind) == 0)
        {
          if (prevNode == NULL && currNode != NULL)
          {
            Animal *temporary = currNode->animal;
            zoo[i].list = currNode->next;
            result = temporary;
          }
          else if (prevNode != NULL && currNode->next == NULL)
          {
            Animal *temporarily = currNode->animal;
            prevNode->next = NULL;
            result = temporarily;
          }
          else if (prevNode != NULL && currNode != NULL)
          {
            Animal *temp = currNode->animal;
            prevNode->next = currNode->next;
            result = temp;
          }
        }
        prevNode = currNode;
        currNode = currNode->next;
      }
    }
  }
  return result;
}

Animal *area_change(Area zoo[], int number_of_areas, Animal animals[],
                    int number_of_animals, const char *animal_kind,
                    const char *old_area, const char *new_area)
{
  int areaExists = 0;
  for (int x = 0; x < number_of_areas; ++x)
  {
    if (strcmp(zoo[x].name, new_area) == 0)
    {
      areaExists = 1; // see if new area exists
    }
  }
  if (areaExists == 0)
  {
    return NULL;
  }

  /*Check the animal array (and NOT the zoo) to see if there is an animal of the right kind in the new area. If there is, then we abort.
  • Check if the new area is even valid. If it’s not, we abort.
  • Check the animal array (and NOT the zoo) to see if there is an animal of the right kind and area to be
  changed. If there is, change its area */
  for (int i = 0; i < number_of_animals; ++i)
  {
    for (int m = 0; m < number_of_animals; ++m)
    {
      if (strcmp(animals[m].area, new_area) == 0 && strcmp(animals[m].animal_kind, animal_kind) == 0)
      {
        return NULL;
      }
    }
    if (strcmp(old_area, animals[i].area) == 0 && strcmp(animal_kind, animals[i].animal_kind) == 0) // if its the animal i want to move
    {
      for (int k = 0; k < number_of_areas; ++k)
      {
        if (strcmp(old_area, zoo[k].name) == 0) // if i find old area on zoo
        {
          AnimalNode *curr = zoo[k].list; // current node = the list(head) of the old area
          int nodeFound = 0;
          while (curr != NULL)
          {
            if (strcmp(curr->animal->animal_kind, animal_kind) == 0) // if old area and animal kind mathces target animal (found old animal)
            {
              nodeFound = 1;
              Animal *potentialResult = area_remove(zoo, number_of_areas, old_area, animal_kind); // remove from the old zoo area and store in potential result
              char *newnew_area = malloc(sizeof(new_area));
              strcpy(newnew_area, new_area);
              potentialResult->area = newnew_area;
              int result = area_add(zoo, number_of_areas, potentialResult, 1, animal_kind); // see if i can add to new area
              if (result == 1)
              {
                return potentialResult;
              }
              else
              {
                return NULL;
              }
            }
            curr = curr->next;
          }

          if (nodeFound == 0 || zoo[k].list == NULL) //area not included
          {
            char *newnewnewArea = malloc(sizeof(new_area));
            strcpy(newnewnewArea, new_area);
            animals[i].area = newnewnewArea;
            return &(animals[i]);
          }
        }
      }
    }
  }
  return NULL;
}
