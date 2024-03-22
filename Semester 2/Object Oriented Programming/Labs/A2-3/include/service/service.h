#pragma once

#include "../repository/repository.h"
#include "../repository/undoRedoRepository.h"

typedef struct {
   Repository* repo;
   UndoRedoRepository* undoRedoRepo;
} Service;

Service* createService(Repository* repo, UndoRedoRepository* undoRedoRepo);
void destroyService(Service* service);
int addMaterialToService(Service* service, Material* material);
int deleteMaterialFromService(Service* service, Material* mat);
int updateMaterialFromService(Service* service, Material* mat);
Repository* getRepo(Service* service);
Repository* getRepositoryCopy(Service* service);
int getLengthService(Service* service);
char* toStringAtIndexService(Service* service, int index);
void sortRepositoryByQuantityAscendingService(Service* service);
void sortRepositoryByQuantityDescendingService(Service* service);
UndoRedoRepository* getUndoRedoRepo(Service* service);
int undoRedoService(Service* service, DynamicArray* undoFromThisArray, DynamicArray* redoToThisArray);
void hardcodeMaterials(Repository* repo);
