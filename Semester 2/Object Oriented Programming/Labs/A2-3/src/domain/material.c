#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../../include/domain/material.h"

Material* createMaterial(char* name, char* supplier, int quantity, Date expiration_date) {
    /*
    Creates a new material
    */
    Material* mat = (Material*)malloc(sizeof(Material));
    mat->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(mat->name, name);
    mat->supplier = (char*)malloc(sizeof(char) * (strlen(supplier) + 1));
    strcpy(mat->supplier, supplier);
    mat->quantity = quantity;
    mat->expiration_date = expiration_date; 
    return mat;
} 

void destroyMaterial(Material* mat) {
    /*
    Frees the memory allocated for the material
    */
    free(mat->name);
    free(mat->supplier);
    free(mat);
}

char* formatMaterialToString(Material* mat) {
    /*
    Returns a string representation of the material
    */
    char* str = (char*)malloc(sizeof(char) * 1001);
    sprintf(str, "Name: %s, Supplier: %s, Quantity: %d, Expiration date: %d/%d/%d", mat->name, mat->supplier, mat->quantity, mat->expiration_date.day, mat->expiration_date.month, mat->expiration_date.year);
    return str;
}

int compareMaterials(Material* mat1, Material* mat2) {
    /*
    Compares two materials
    Two materials are equal if they have the same name, supplier and expiration date
    */
    if (strcmp(getMaterialName(mat1), getMaterialName(mat2)) == 0 && strcmp(getMaterialSupplier(mat1), getMaterialSupplier(mat2)) == 0 && compareDates(getMaterialDate(mat1), getMaterialDate(mat2)) == 1)
        return 1;
    else
        return 0;
}

int isPastExpirationDate(Material* m, Date d) {
    /*
    Checks if a material is expired
    The material is expired if the current date is greater than the expiration date
    */
    Date date = getMaterialDate(m);
    if(date.year < d.year)
        return 1;
    if(date.year == d.year && date.month < d.month)
        return 1;
    if(date.year == d.year && date.month == d.month && date.day < d.day)
        return 1;
    return -1;
}

int checkIfMaterialContainsAGivenString(Material* m, char* str) {
    /*
    Checks if a material name contains a given string
    */
    if(strlen(str) == 0)
        return 1;
    if(strstr(getMaterialName(m), str) != NULL || strstr(getMaterialSupplier(m), str) != NULL)
        return 1;
    return -1;
}

char* getMaterialName(Material* mat) {
    /*
    Returns the name of the material
    */
    return mat->name;
}

char* getMaterialSupplier(Material* mat) {
    /*
    Returns the supplier of the material
    */
    return mat->supplier;
}

int getMaterialQuantity(Material* mat) {
    /*
    Returns the quantity of the material
    */
    return mat->quantity;
}

Date getMaterialDate(Material* mat) {
    /*
    Returns the expiration date of the material
    */
    return mat->expiration_date;
}

void setMaterialName(Material* mat, char* name) {
    /*
    Sets the name of the material
    */   
    free(mat->name);
    mat->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(mat->name, name);
}

void setMaterialSupplier(Material* mat, char* supplier) {
    /*
    Sets the supplier of the material
    */
    free(mat->supplier);
    mat->supplier = (char*)malloc(sizeof(char) * (strlen(supplier) + 1));
    strcpy(mat->supplier, supplier);
}

void setMaterialQuantity(Material* mat, int quantity) {
    /*
    Sets the quantity of the material
    */
    mat->quantity = quantity;
}

void setMaterialDate(Material* mat, Date date) {
    /*
    Sets the expiration date of the material
    */
    mat->expiration_date = date;
}
