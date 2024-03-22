#include "../../include/domain/date.h"

Date createDate(int day, int month, int year) {
    /*
    Creates a date
    */
    Date date;
    date.day = day;
    date.month = month;
    date.year = year;
    return date;
}

int compareDates(Date date1, Date date2) {
    /*
    Compares two dates
    Two dates are equal if they have the same day, month and year
    */
    if (date1.day == date2.day && date1.month == date2.month && date1.year == date2.year) {
        return 1;
    }
    return 0;
}

void setDateDay(Date* date, int day) {
    /*
    Sets the day of a date
    */
    date->day = day;
}

void setDateMonth(Date* date, int month) {
    /*
    Sets the month of a date
    */
    date->month = month;
}

void setDateYear(Date* date, int year) {
    /*
    Sets the year of a date
    */
    date->year = year;
}

int getDateDay(Date* date) {
    /*
    Returns the day of a date
    */
    return date->day;
}

int getDateMonth(Date* date) {
    /*
    Returns the month of a date
    */
    return date->month;
}

int getDateYear(Date* date) {
    /*
    Returns the year of a date
    */
    return date->year;
}
