#include <stdlib.h>
#include "../../include/repository/undoRedoRepository.h"

UndoRedoRepository* createUndoRedoRepository() {
    UndoRedoRepository* undoRedoRepository = (UndoRedoRepository*)malloc(sizeof(UndoRedoRepository));
    undoRedoRepository->undoOperations = createDynamicArray(10, destroyOperation);
    undoRedoRepository->redoOperations = createDynamicArray(10, destroyOperation);
    return undoRedoRepository;
}

void destroyUndoRedoRepository(UndoRedoRepository* undoRedoRepository) {
    destroyDynamicArray(getUndoOperations(undoRedoRepository));
    destroyDynamicArray(getRedoOperations(undoRedoRepository));
    free(undoRedoRepository);
}

DynamicArray* getUndoOperations(UndoRedoRepository* undoRedoRepository) {
    return undoRedoRepository->undoOperations;
}

DynamicArray* getRedoOperations(UndoRedoRepository* undoRedoRepository) {
    return undoRedoRepository->redoOperations;
}

void resetRedoOperations(UndoRedoRepository* undoRedoRepository) {
    destroyDynamicArray(getRedoOperations(undoRedoRepository));
    undoRedoRepository->redoOperations = createDynamicArray(10, destroyOperation);
}

void addUndoOperation(UndoRedoRepository* undoRedoRepository, Operation* operation) {
    addElement(getUndoOperations(undoRedoRepository), operation);
}

void addRedoOperation(UndoRedoRepository* undoRedoRepository, Operation* operation) {
    addElement(getRedoOperations(undoRedoRepository), operation);
}

Operation* getLastUndoOperation(UndoRedoRepository* undoRedoRepository) {
    if(getSize(getUndoOperations(undoRedoRepository)) == 0)
        return NULL;
    return getElementAtIndex(getUndoOperations(undoRedoRepository), getSize(getUndoOperations(undoRedoRepository)) - 1);
}

Operation* getLastRedoOperation(UndoRedoRepository* undoRedoRepository) {
    if(getSize(getRedoOperations(undoRedoRepository)) == 0)
        return NULL;
    return getElementAtIndex(getRedoOperations(undoRedoRepository), getSize(getRedoOperations(undoRedoRepository)) - 1);
}

void removeLastUndoOperation(UndoRedoRepository* undoRedoRepository) {
    deleteElementAtIndex(getUndoOperations(undoRedoRepository), getSize(getUndoOperations(undoRedoRepository)) - 1);
}

void removeLastRedoOperation(UndoRedoRepository* undoRedoRepository) {
    deleteElementAtIndex(getRedoOperations(undoRedoRepository), getSize(getRedoOperations(undoRedoRepository)) - 1);
}
