#pragma once

#include "../domain/operation.h"
#include "../domain/dynamicArray.h"

typedef struct {
    DynamicArray* undoOperations;
    DynamicArray* redoOperations;
} UndoRedoRepository;

UndoRedoRepository* createUndoRedoRepository();
void destroyUndoRedoRepository(UndoRedoRepository* undoRedoRepository);
DynamicArray* getUndoOperations(UndoRedoRepository* undoRedoRepository);
DynamicArray* getRedoOperations(UndoRedoRepository* undoRedoRepository);
void resetRedoOperations(UndoRedoRepository* undoRedoRepository);
void addUndoOperation(UndoRedoRepository* undoRedoRepository, Operation* operation);
void addRedoOperation(UndoRedoRepository* undoRedoRepository, Operation* operation);
Operation* getLastUndoOperation(UndoRedoRepository* undoRedoRepository);
Operation* getLastRedoOperation(UndoRedoRepository* undoRedoRepository);
void removeLastUndoOperation(UndoRedoRepository* undoRedoRepository);
void removeLastRedoOperation(UndoRedoRepository* undoRedoRepository);
