#pragma once

typedef struct {
    int day;
    int month;
    int year;
} Date;

/*
Date methods
*/

Date createDate(int day, int month, int year);
int compareDates(Date date1, Date date2);
void setDateDay(Date* date, int day);
void setDateMonth(Date* date, int month);
void setDateYear(Date* date, int year);
int getDateDay(Date* date);
int getDateMonth(Date* date);
int getDateYear(Date* date);
