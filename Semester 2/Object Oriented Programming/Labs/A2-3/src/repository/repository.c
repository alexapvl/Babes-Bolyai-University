#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/repository/repository.h"

Repository* createRepository(int capacity) {
    /*
    Creates a repository
    */
    Repository* repo = (Repository*)malloc(sizeof(Repository));
    repo->arr = createDynamicArray(capacity, destroyMaterial);
    return repo;
}

DynamicArray* getDynamicArray(Repository* repo) {
    /*
    Returns the dynamic array from the repository
    */
    return repo->arr;
}

void destroyRepository(Repository* repo) {
    /*
    Destroy the dynamic array and then free the memory allocated for the repository
    */
    destroyDynamicArray(repo->arr);
    free(repo);
}

void addMaterialToRepository(Repository* repo, Material* mat) {
    /*
    Adds a material to the repository
    */
    addElement(getDynamicArray(repo), mat);
}

void deleteMaterialFromRepository(Repository* repo, int index) {
    /*
    Deletes a material from the repository
    */
    deleteElementAtIndex(getDynamicArray(repo), index);
}

void updateMaterialInRepository(Repository* repo, int index, Material* mat) {
    /*
    Updates a material from the repository
    */
    updateElementAtIndex(getDynamicArray(repo), index, mat);
}

Material* getMaterialFromRepository(Repository* repo, int index) {
    /*
    Returns a material from the repository
    */
    return getElementAtIndex(getDynamicArray(repo), index);
}

int checkIfMaterialInRepository(Repository* repo, Material* mat) {
    /*
    Checks if a material is in the repository
    If the material is found, return the index of the material
    Else return -1
    */
    for (int i = 0; i < getSize(getDynamicArray(repo)); i++) {
        Material* m = getMaterialFromRepository(repo, i);
        if(compareMaterials(m, mat) == 1)
            return i;
    }
    return -1;
}

int getLengthRepository(Repository* repo) {
    /*
    Returns the length of the repository
    */
    return getSize(getDynamicArray(repo));
}

int getCapacityRepository(Repository* repo) {
    /*
    Returns the capacity of the repository
    */
    return getCapacity(getDynamicArray(repo));
}

char* toStringAtIndexRepository(Repository* repo, int index) {
    /*
    Returns the string representation of a material from the repository
    */
    return toStringAtIndex(getDynamicArray(repo), index);
}

void sortRepositoryByQuantityAscending(Repository* repo) {
    /*
    Sorts the repository by quantity
    */
    int n = getLengthRepository(repo);
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            Material* m1 = getMaterialFromRepository(repo, i);
            Material* m2 = getMaterialFromRepository(repo, j);
            if (getMaterialQuantity(m1) > getMaterialQuantity(m2)) {
                swapElements(getDynamicArray(repo), i, j);
            }
        }
    }
}

void sortRepositoryByQuantityDescending(Repository* repo) {
    /*
    Sorts the repository by quantity
    */
    int n = getLengthRepository(repo);
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            Material* m1 = getMaterialFromRepository(repo, i);
            Material* m2 = getMaterialFromRepository(repo, j);
            if (getMaterialQuantity(m1) < getMaterialQuantity(m2)) {
                swapElements(getDynamicArray(repo), i, j);
            }
        }
    }
}

Repository* copyRepository(Repository* repo) {
    /*
    Returns a copy of the repository
    */
    Repository* newRepo = createRepository(getCapacityRepository(repo));
    for (int i = 0; i < getLengthRepository(repo); i++) {
        Material* m = getMaterialFromRepository(repo, i);
        Material* copyMaterial = createMaterial(getMaterialName(m), getMaterialSupplier(m), getMaterialQuantity(m), getMaterialDate(m));
        addMaterialToRepository(newRepo, copyMaterial);
    }
    return newRepo;
}
