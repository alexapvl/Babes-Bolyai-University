#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/ui/ui.h"

UI* createUI(Service* service) {
    UI* ui = (UI*)malloc(sizeof(UI));
    ui->service = service;
    ui->date = createDate(18, 3, 2024);
    return ui;
}

void destroyUI(UI* ui) {
    destroyService(ui->service);
    free(ui);
}

void printMenu(){
    printf("\n");
    printf("1. Add material\n");
    printf("2. Update material\n");
    printf("3. Delete material\n");
    printf("4. Print all materials past expiration date containing a given string\n");
    printf("5. Print all materials NOT past expiration date containing a given string\n");
    printf("6. Display all materials which are in short supply from a specific supplier in ascending order by quantity\n");
    printf("7. Display all materials which are in short supply from a specific supplier in descending order by quantity\n");
    printf("8. Print all materials\n");
    printf("9. Undo\n");
    printf("10. Redo\n");
    printf("11. Exit\n");
}

void start(UI* ui){
    hardcodeMaterials(getRepoUI(ui));
    int option = -1;
    while(option != 11) {
        printMenu();
        if(scanf("%d", &option) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while(getchar() != '\n'); // clears input buffer and prevents from any unexpected input from the user
            continue;
        }
        switch (option) {
            case 1:
                addMaterialUI(ui);
                break;
            case 2:
                updateMaterialUI(ui);
                break;
            case 3:
                deleteMaterialUI(ui);
                break;
            case 4:
                if(printElementsPastExpirationDateContainingGivenStringUI(ui) == 0) {
                    printf("No materials past expiration date found\n");
                }
                break;
            case 5:
                if(printElementsNotPastExpirationDateContainingGivenStringUI(ui) == 0) {
                    printf("All materials are past their expiration date\n");
                }
                break;
            case 6:
                if(printMaterialsInShortSupplyFromSpecificSupplierAscendingByQuantityUI(ui) == 0) {
                    printf("All materials from given supplier are in stock with the minimum given quantity\n");
                }
                break;
            case 7:
                if(printMaterialsInShortSupplyFromSpecificSupplierDescendingByQuantityUI(ui) == 0) {
                    printf("All materials from given supplier are in stock with the minimum given quantity\n");
                }
                break;
            case 8:
                if(printElementsUI(ui) == 0) {
                    printf("There are no materials in the Bakery\n");
                } 
                break; 
            case 9:
                UndoUI(ui);
                break;
            case 10:
                RedoUI(ui);
                break;
            case 11:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option\n");
                break;
        }

    }
}

void addMaterialUI(UI* ui) {
    char name[1001];
    char supplier[1001];
    int quantity;
    int day, month, year;
    printf("Enter the name of the material: ");
    scanf(" %[^\n]", name);
    printf("Enter the supplier of the material: ");
    scanf(" %[^\n]", supplier);
    printf("Enter the quantity of the material: ");
    scanf("%d", &quantity);
    printf("Enter the expiration date of the material (day month year): ");
    scanf("%d %d %d", &day, &month, &year);
    Date date = createDate(day, month, year);
    Material* mat = createMaterial(name, supplier, quantity, date);
   if(addMaterialToService(getService(ui), mat) == 1)
        printf("Material added successfully\n");
    else {
        printf("Material already exists, quantity updated\n");
        destroyMaterial(mat);
    }
}

void deleteMaterialUI(UI* ui) {
    char name[1001];
    char supplier[1001];
    int day, month, year;
    printf("Enter the name of the material: ");
    scanf(" %[^\n]", name);
    printf("Enter the supplier of the material: ");
    scanf(" %[^\n]", supplier);
    printf("Enter the expiration date of the material (day month year): ");
    scanf("%d %d %d", &day, &month, &year);
    Date date = createDate(day, month, year);
    Material* mat = createMaterial(name, supplier, 0, date);
    Material* materialFromRepo = getMaterialFromIndexUI(ui, checkIfMaterialInRepository(getRepoUI(ui), mat));
    if(deleteMaterialFromService(getService(ui), materialFromRepo) == 1) {
        printf("Material deleted successfully\n");
    } else {
        printf("Material not found\n");
    }
    destroyMaterial(mat);
}

void updateMaterialUI(UI* ui) {
    char name[1001];
    char supplier[1001];
    int quantity;
    int day, month, year;
    printf("Enter the name of the material: ");
    scanf(" %[^\n]", name);
    printf("Enter the supplier of the material: ");
    scanf(" %[^\n]", supplier);
    printf("Enter the quantity of the material: ");
    scanf("%d", &quantity);
    printf("Enter the expiration date of the material (day month year): ");
    scanf("%d %d %d", &day, &month, &year);
    Date date = createDate(day, month, year);
    Material* mat = createMaterial(name, supplier, quantity, date);
    if(updateMaterialFromService(getService(ui), mat) == 1) {
        printf("Material updated successfully\n");
    } else {
        printf("Material not found\n");
        destroyMaterial(mat);
    }
}

int printElementsPastExpirationDateContainingGivenStringUI(UI* ui) {
    int count = 0;
    char str[1001];
    printf("Enter the string(enter \"none\" to simulate an empty string): ");
    scanf(" %[^\n]", str);
    if(strcmp(str, "none") == 0) {
        str[0] = '\0'; // empty string
    }
    for(int i = 0; i < getLengthService(getService(ui)); i++) {
        Material* mat = getMaterialFromRepository(getRepo(getService(ui)), i);
        if(isPastExpirationDate(mat, ui->date) == 1 && checkIfMaterialContainsAGivenString(mat, str) == 1) {
            char* materialString = formatMaterialToString(mat);
            printf("%s\n", materialString);
            count++;
            free(materialString);
        }
    }
    return count;
}
 
 int printElementsNotPastExpirationDateContainingGivenStringUI(UI* ui) {
    int count = 0;
    char str[1001];
    printf("Enter the string(enter \"none\" to simulate an empty string): ");
    scanf(" %[^\n]", str);
    if(strcmp(str, "none") == 0) {
        str[0] = '\0'; // empty string
    }
    for(int i = 0; i < getLengthService(getService(ui)); i++) {
        Material* mat = getMaterialFromRepository(getRepo(getService(ui)), i);
        if(isPastExpirationDate(mat, ui->date) == -1 && checkIfMaterialContainsAGivenString(mat, str) == 1) {
            char* materialString = formatMaterialToString(mat);
            printf("%s\n", materialString);
            count++;
            free(materialString);
        }
    }
    return count;
 }

int printElementsUI(UI* ui) {
    int count = 0;
    for(int i = 0; i < getLengthService(getService(ui)); i++) {
        char* str = toStringAtIndexService(getService(ui), i);
        printf("%s\n", str);
        free(str); // free the memory allocated for the string in the function formatMaterialToString from material.c
        count++;
    }
    return count;
}

int printMaterialsInShortSupplyFromSpecificSupplierAscendingByQuantityUI(UI* ui) {
    int numberOfFoundMaterials = 0;
    char supplier[1001];
    int quantity;
    printf("Enter the supplier: ");
    scanf(" %[^\n]", supplier);
    printf("Enter the quantity: ");
    scanf("%d", &quantity);
    Repository* copyRepo = copyRepository(getRepoUI(ui));
    sortRepositoryByQuantityAscending(copyRepo);
    for(int i = 0; i < getLengthService(getService(ui)); i++) {
        Material* mat = getMaterialFromRepository(copyRepo, i);
        if(strcmp(getMaterialSupplier(mat), supplier) == 0 && getMaterialQuantity(mat) < quantity) {
            char* str = formatMaterialToString(mat);
            printf("%s\n", str);
            numberOfFoundMaterials++;
            free(str);
        }
    }
    destroyRepository(copyRepo);
    return numberOfFoundMaterials;
}

int printMaterialsInShortSupplyFromSpecificSupplierDescendingByQuantityUI(UI* ui) {
    int numberOfFoundMaterials = 0;
    char supplier[1001];
    int quantity;
    printf("Enter the supplier: ");
    scanf(" %[^\n]", supplier);
    printf("Enter the quantity: ");
    scanf("%d", &quantity);
    Repository* copyRepo = copyRepository(getRepoUI(ui));
    sortRepositoryByQuantityDescending(copyRepo);
    for(int i = 0; i < getLengthService(getService(ui)); i++) {
        Material* mat = getMaterialFromRepository(copyRepo, i);
        if(strcmp(getMaterialSupplier(mat), supplier) == 0 && getMaterialQuantity(mat) < quantity) {
            char* str = formatMaterialToString(mat);
            printf("%s\n", str);
            numberOfFoundMaterials++;
            free(str);
        }
    }
    destroyRepository(copyRepo);
    return numberOfFoundMaterials;
}

Material* getMaterialFromIndexUI(UI* ui, int index) {
    return getMaterialFromRepository(getRepoUI(ui), index);
}

Repository* getRepoUI(UI* ui) {
    return getRepo(getService(ui));
}

UndoRedoRepository* getUndoRedoRepoUI(UI* ui) {
    return getUndoRedoRepo(getService(ui));   
}

Service* getService(UI* ui) {
    return ui->service;
}

void UndoUI(UI* ui) {
    if(undoRedoService(getService(ui), getUndoOperations(getUndoRedoRepoUI(ui)), getRedoOperations(getUndoRedoRepoUI(ui))) == 1) {
        printf("Undo successful\n");
    } else {
        printf("No more undos\n");
    }
}

void RedoUI(UI* ui) {
    if(undoRedoService(getService(ui), getRedoOperations(getUndoRedoRepoUI(ui)), getUndoOperations(getUndoRedoRepoUI(ui))) == 1) {
        printf("Redo successful\n");
    } else {
        printf("No more redos\n");
    }
}
