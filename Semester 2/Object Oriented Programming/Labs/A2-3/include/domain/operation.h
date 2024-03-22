#pragma once

#include "material.h"

typedef struct {
    /*
    Operation types: 1 - add, 2 - delete, 3 - update
    */
    int operationType;
    Material* material1;
    Material* material2;
} Operation;

Operation* createOperation(int operationType, Material* material1, Material* material2);
void destroyOperation(Operation* operation);
int getOperationType(Operation* operation);
Material* getOperationMaterial1(Operation* operation);
Material* getOperationMaterial2(Operation* operation);
