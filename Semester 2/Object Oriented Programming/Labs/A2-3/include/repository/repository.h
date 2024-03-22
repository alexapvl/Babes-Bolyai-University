#pragma once

#include "../domain/dynamicArray.h"
#include "../domain/material.h"

typedef struct {
    DynamicArray* arr;
} Repository;

Repository* createRepository(int capacity);
DynamicArray* getDynamicArray(Repository* repository);
void destroyRepository(Repository* repository);
void addMaterialToRepository(Repository* repository, Material* material);
void deleteMaterialFromRepository(Repository* repository, int index);
void updateMaterialInRepository(Repository* repository, int index, Material* material);
Material* getMaterialFromRepository(Repository* repository, int index);
int checkIfMaterialInRepository(Repository* repository, Material* material);
int getLengthRepository(Repository* repository);
int getCapacityRepository(Repository* repository);
char* toStringAtIndexRepository(Repository* repository, int index);
void sortRepositoryByQuantityAscending(Repository* repository);
void sortRepositoryByQuantityDescending(Repository* repository);
Repository* copyRepository(Repository* repository);
