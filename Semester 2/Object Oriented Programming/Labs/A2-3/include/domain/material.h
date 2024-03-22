#pragma once

#include "date.h"

typedef struct {
    char* name;
    char* supplier;
    int quantity;
    Date expiration_date;
} Material;

/*
Material methods
*/

Material* createMaterial(char* name, char* supplier, int quantity, Date expiration_date);
void destroyMaterial(Material* mat);
char* formatMaterialToString(Material* mat);
int compareMaterials(Material* mat1, Material* mat2);
int isPastExpirationDate(Material* material, Date date);
int checkIfMaterialContainsAGivenString(Material* material, char* str);
char* getMaterialName(Material* mat);
char* getMaterialSupplier(Material* mat);
int getMaterialQuantity(Material* mat);
Date getMaterialDate(Material* mat);
void setMaterialName(Material* mat, char* name);
void setMaterialSupplier(Material* mat, char* supplier);
void setMaterialDate(Material* mat, Date date);
void setMaterialQuantity(Material* mat, int quantity);