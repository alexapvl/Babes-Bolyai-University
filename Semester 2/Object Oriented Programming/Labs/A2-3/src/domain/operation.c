#include "../../include/domain/operation.h"
#include <stdlib.h>
#include <stdio.h>

Operation* createOperation(int operationType, Material* material1, Material* material2) {
    Operation* operation = (Operation*)malloc(sizeof(Operation));
    operation->operationType = operationType;
    operation->material1 = createMaterial(getMaterialName(material1), getMaterialSupplier(material1), getMaterialQuantity(material1), getMaterialDate(material1));
    operation->material2 = NULL;
    if(material2)
        operation->material2 = createMaterial(getMaterialName(material2), getMaterialSupplier(material2), getMaterialQuantity(material2), getMaterialDate(material2));
    return operation;
}

void destroyOperation(Operation* operation) {
    destroyMaterial(getOperationMaterial1(operation));
    if(getOperationMaterial2(operation))
        destroyMaterial(getOperationMaterial2(operation));
    free(operation);
}

int getOperationType(Operation* operation) {
    return operation->operationType;
}

Material* getOperationMaterial1(Operation* operation) {
    return operation->material1;
}

Material* getOperationMaterial2(Operation* operation) {
    return operation->material2;
}