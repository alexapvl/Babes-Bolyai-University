#pragma once

#include "../service/service.h"

typedef struct {
    Service* service;
    Date date;
} UI;

UI* createUI(Service* service);
void destroyUI(UI* ui);
void printMenu();
void start(UI* ui);
void addMaterialUI(UI* ui);
void deleteMaterialUI(UI* ui);
void updateMaterialUI(UI* ui);
int printElementsPastExpirationDateContainingGivenStringUI(UI* ui);
int printElementsNotPastExpirationDateContainingGivenStringUI(UI* ui);
int printElementsUI(UI* ui);
int printMaterialsInShortSupplyFromSpecificSupplierAscendingByQuantityUI(UI* ui);
int printMaterialsInShortSupplyFromSpecificSupplierDescendingByQuantityUI(UI* ui);
Material* getMaterialFromIndexUI(UI* ui, int index);
Repository* getRepoUI(UI* ui);
UndoRedoRepository* getUndoRedoRepoUI(UI* ui);
Service* getService(UI* ui);
void UndoUI(UI* ui);
void RedoUI(UI* ui);
