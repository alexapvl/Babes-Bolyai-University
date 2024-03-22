#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/service/service.h"

Service* createService(Repository* repo, UndoRedoRepository* undoRedoRepo) {
    /*
    Creates a service with a given repository
    */
    Service* service = (Service*)malloc(sizeof(Service));
    service->repo = repo;
    service->undoRedoRepo = undoRedoRepo;
    return service;
}

void destroyService(Service* service) {
    /*
    Destroy the repository and then free the memory allocated for the service
    */
    destroyRepository(getRepo(service));
    destroyUndoRedoRepository(getUndoRedoRepo(service));
    free(service);
}

int addMaterialToService(Service* service, Material* material) {
    /*
    Adds a material to the repository
    */
    int indexOfExistingMaterial = checkIfMaterialInRepository(getRepo(service), material);
    if(indexOfExistingMaterial == -1) {
        // if material does not exist in repo, add it
        resetRedoOperations(getUndoRedoRepo(service)); // if a new operation is added, then the redo stack should be reset
        Operation* operation = createOperation(2, material, NULL); // created a delete operation to add to the undo stack
        addUndoOperation(getUndoRedoRepo(service), operation);
        addMaterialToRepository(getRepo(service), material);
        return 1;
    }
    // if the material already exists, then update the quantity
    Material* existingMaterial = getMaterialFromRepository(getRepo(service), indexOfExistingMaterial); 
    Material* newMaterial = createMaterial(getMaterialName(existingMaterial), getMaterialSupplier(existingMaterial), getMaterialQuantity(existingMaterial) + getMaterialQuantity(material), getMaterialDate(existingMaterial));
    updateMaterialFromService(service, newMaterial); // this will trigger the undo stack to be updated
    return 2;
}

int deleteMaterialFromService(Service* service, Material* mat) {
    /*
    Deletes a specific material from the repository
    If the material is not found, return -1 and a message will be displayed in the ui
    */
    int indexOfMaterialInRepo = checkIfMaterialInRepository(getRepo(service), mat); 
    if(indexOfMaterialInRepo == -1)
        return -1; // if material was not found in repository, return -1
    resetRedoOperations(getUndoRedoRepo(service)); // if a new operation is added, then the redo stack should be reset
    Operation* operation = createOperation(1, mat, NULL); // created an add operation to add to the undo stack
    addUndoOperation(getUndoRedoRepo(service), operation);
    deleteMaterialFromRepository(getRepo(service), indexOfMaterialInRepo);
    return 1;
}

int updateMaterialFromService(Service* service, Material* mat) {
    /*
    Updates a specific material from the repository
    If the material is not found, return -1 and a message will be displayed in the ui
    */
    int indexOfMaterialInRepo = checkIfMaterialInRepository(getRepo(service), mat);
    if(indexOfMaterialInRepo == -1)
        return -1; // if material was not found in repo, return -1
    resetRedoOperations(getUndoRedoRepo(service)); // if a new operation is added, then the redo stack should be reset
    Material* existingMaterial = getMaterialFromRepository(getRepo(service), indexOfMaterialInRepo); 
    Operation* operation = createOperation(3, mat, existingMaterial); // created an update operation to add to the undo stack
    addUndoOperation(getUndoRedoRepo(service), operation);
    updateMaterialInRepository(getRepo(service), indexOfMaterialInRepo, mat);
    return 1;
}

Repository* getRepo(Service* service) {
    /*
    Returns the repository from the service
    */
    return service->repo;
}

Repository* getRepositoryCopy(Service* service) {
    /*
    Returns a copy of the repository from the service
    */
    return copyRepository(getRepo(service));
}

int getLengthService(Service* service) {
    /*
    Returns the length of the repository
    */
    return getLengthRepository(getRepo(service));
}

char* toStringAtIndexService(Service* service, int index) {
    /*
    Returns the string representation of a material from the repository
    */
    return toStringAtIndexRepository(getRepo(service), index);
}

void sortRepositoryByQuantityAscendingService(Service* service) {
    /*
    Sorts the repository by quantity
    */
    sortRepositoryByQuantityAscending(getRepo(service));
}

void sortRepositoryByQuantityDescendingService(Service* service) {
    /*
    Sorts the repository by quantity
    */
    sortRepositoryByQuantityDescending(getRepo(service));
}

UndoRedoRepository* getUndoRedoRepo(Service* service) {
    /*
    Returns the undoRedoRepository from the service
    */
    return service->undoRedoRepo;
}

int undoRedoService(Service* service, DynamicArray* undoFromThisArray, DynamicArray* redoToThisArray) {
    /*  
    Undo or redo the last operation
    */
    if(getSize(undoFromThisArray) == 0)
        return 0;

    Operation* operation = (Operation*)getElementAtIndex(undoFromThisArray, getSize(undoFromThisArray) - 1);
    Operation* operationToBeAddedToRedo = NULL;

    if(getOperationType(operation) == 1) {
        Material* materialFromRepository = getOperationMaterial1(operation);
        Material* copyMaterial = createMaterial(getMaterialName(materialFromRepository), getMaterialSupplier(materialFromRepository), getMaterialQuantity(materialFromRepository), getMaterialDate(materialFromRepository));
        addMaterialToRepository(getRepo(service), copyMaterial);
        operationToBeAddedToRedo = createOperation(2, materialFromRepository, NULL);
    } else if(getOperationType(operation) == 2) {
        deleteMaterialFromRepository(getRepo(service), checkIfMaterialInRepository(getRepo(service), getOperationMaterial1(operation)));
        operationToBeAddedToRedo = createOperation(1, getOperationMaterial1(operation), NULL);
    } else if(getOperationType(operation) == 3) {
        Material* material2FromRepository = getOperationMaterial2(operation);
        Material* copyMaterial = createMaterial(getMaterialName(material2FromRepository), getMaterialSupplier(material2FromRepository), getMaterialQuantity(material2FromRepository), getMaterialDate(material2FromRepository));
        updateMaterialInRepository(getRepo(service), checkIfMaterialInRepository(getRepo(service), getOperationMaterial1(operation)), copyMaterial);
        operationToBeAddedToRedo = createOperation(3, getOperationMaterial2(operation), getOperationMaterial1(operation));
    }
    addElement(redoToThisArray, operationToBeAddedToRedo);
    deleteElementAtIndex(undoFromThisArray, getSize(undoFromThisArray) - 1);
    return 1;
}

void hardcodeMaterials(Repository* repo) {
    Material* m1 = createMaterial("Material1", "Supplier1", 100, createDate(1, 1, 2020));
    Material* m2 = createMaterial("Material2", "Supplier2", 20, createDate(2, 2, 2020));
    Material* m3 = createMaterial("Material3", "Supplier3", 30, createDate(3, 3, 2020));
    Material* m4 = createMaterial("Material4", "Supplier4", 40, createDate(4, 4, 2020));
    Material* m5 = createMaterial("Material5", "Supplier5", 90, createDate(5, 5, 2020));
    Material* m6 = createMaterial("Material6", "Supplier6", 60, createDate(6, 6, 2020));
    Material* m7 = createMaterial("Material7", "Supplier7", 70, createDate(7, 7, 2020));
    Material* m8 = createMaterial("Material8", "Supplier8", 80, createDate(8, 8, 2020));
    Material* m9 = createMaterial("Material9", "Supplier9", 50, createDate(9, 9, 2020));
    Material* m10 = createMaterial("Material10", "Supplier10", 10, createDate(10, 10, 2020));
    Material* m11 = createMaterial("Material11", "Supplier3", 10, createDate(11, 11, 2020));
    Material* m12 = createMaterial("Material12", "Supplier3", 5, createDate(12, 12, 2020));
    addMaterialToRepository(repo, m1);
    addMaterialToRepository(repo, m2);
    addMaterialToRepository(repo, m3);
    addMaterialToRepository(repo, m4);
    addMaterialToRepository(repo, m5);
    addMaterialToRepository(repo, m6);
    addMaterialToRepository(repo, m7);
    addMaterialToRepository(repo, m8);
    addMaterialToRepository(repo, m9);
    addMaterialToRepository(repo, m10);
    addMaterialToRepository(repo, m11);
    addMaterialToRepository(repo, m12);
}
