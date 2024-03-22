#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/tests/tests.h"

// test all functions from service using asserts

void TestAll() {
    TestCreateService();
    TestAddMaterialToService();
    TestDeleteMaterialFromService();
    TestUpdateMaterialFromService();
    TestToStringAtIndexService();
    TestSortRepositoryByQuantityAscending();
    TestSortRepositoryByQuantityDescending();
    TestUndoRedoService();
}

void TestCreateService() {
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    // test if service was created correctly
    assert(getCapacityRepository(repo) == 10);
    assert(getLengthService(service) == 0);
    assert(getRepo(service) == repo);
    assert(getCapacity(getUndoOperations(undoRedoRepo)) == 10);
    assert(getSize(getUndoOperations(undoRedoRepo)) == 0);
    assert(getCapacity(getRedoOperations(undoRedoRepo)) == 10);
    assert(getSize(getRedoOperations(undoRedoRepo)) == 0);
    assert(getUndoRedoRepo(service) == undoRedoRepo);
    destroyService(service);
}

void TestAddMaterialToService() {
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    Material* mat = createMaterial("name", "supplier", 10, createDate(1, 1, 2020));
    addMaterialToService(service, mat);
    // test if material was added correctly
    assert(getLengthRepository(repo) == 1);
    assert(getMaterialQuantity(getMaterialFromRepository(repo, 0)) == 10);
    // test if undo operation was added correctly
    assert(getSize(getUndoOperations(undoRedoRepo)) == 1);
    destroyService(service);   
}

void TestDeleteMaterialFromService() {
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    Material* mat = createMaterial("name", "supplier", 10, createDate(1, 1, 2020));
    addMaterialToService(service, mat);
    deleteMaterialFromService(service, mat);
    // test if material was deleted correctly
    assert(getLengthRepository(repo) == 0);
    // test if undo operation was added correctly
    assert(getSize(getUndoOperations(undoRedoRepo)) == 2);
    destroyService(service);
}

void TestUpdateMaterialFromService() {
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    Material* mat = createMaterial("name", "supplier", 10, createDate(1, 1, 2020));
    addMaterialToService(service, mat);
    Material* newMat = createMaterial("name", "supplier", 20, createDate(1, 1, 2020));
    updateMaterialFromService(service, newMat);
    // test if material was updated correctly
    assert(getLengthRepository(repo) == 1);
    assert(getMaterialQuantity(getMaterialFromRepository(repo, 0)) == 20);
    // test if undo operation was added correctly
    assert(getSize(getUndoOperations(undoRedoRepo)) == 2);
    destroyService(service);
}

void TestToStringAtIndexService(){
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    Material* mat = createMaterial("name", "supplier", 10, createDate(1, 1, 2020));
    addMaterialToService(service, mat);
    char* str = toStringAtIndexService(service, 0);
    // test if the string was created correctly
    assert(strcmp(str, "Name: name, Supplier: supplier, Quantity: 10, Expiration date: 1/1/2020") == 0);
    free(str);
    destroyService(service);
}

void TestSortRepositoryByQuantityAscending() {
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    Material* mat1 = createMaterial("name1", "supplier1", 10, createDate(1, 1, 2020));
    Material* mat2 = createMaterial("name2", "supplier2", 20, createDate(1, 1, 2020));
    addMaterialToService(service, mat2);
    addMaterialToService(service, mat1);
    sortRepositoryByQuantityAscendingService(service);
    // test if the repository was sorted correctly
    assert(getMaterialQuantity(getMaterialFromRepository(repo, 0)) == 10);
    assert(getMaterialQuantity(getMaterialFromRepository(repo, 1)) == 20);
    destroyService(service);
}

void TestSortRepositoryByQuantityDescending() {
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    Material* mat1 = createMaterial("name1", "supplier1", 10, createDate(1, 1, 2020));
    Material* mat2 = createMaterial("name2", "supplier2", 20, createDate(1, 1, 2020));
    addMaterialToService(service, mat1);
    addMaterialToService(service, mat2);
    sortRepositoryByQuantityDescendingService(service);
    // test if the repository was sorted correctly
    assert(getMaterialQuantity(getMaterialFromRepository(repo, 0)) == 20);
    assert(getMaterialQuantity(getMaterialFromRepository(repo, 1)) == 10);
    destroyService(service);
}

void TestUndoRedoService() {
    Repository* repo = createRepository(10);
    UndoRedoRepository* undoRedoRepo = createUndoRedoRepository();
    Service* service = createService(repo, undoRedoRepo);
    Material* mat1 = createMaterial("name1", "supplier1", 10, createDate(1, 1, 2020));
    Material* mat2 = createMaterial("name2", "supplier2", 20, createDate(1, 1, 2020));
    addMaterialToService(service, mat1);
    addMaterialToService(service, mat2);
    deleteMaterialFromService(service, mat1);
    undoRedoService(service, getUndoOperations(undoRedoRepo), getRedoOperations(undoRedoRepo));
    // test if the undo operation was done correctly
    assert(getLengthRepository(repo) == 2);
    undoRedoService(service, getRedoOperations(undoRedoRepo), getUndoOperations(undoRedoRepo));
    // test if the redo operation was done correctly
    assert(getLengthRepository(repo) == 1);
    destroyService(service);
}
