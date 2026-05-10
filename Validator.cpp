#include "Validator.h"
#include <iostream>

using namespace std;

bool Validator::containsDelimiter(const char* str) {
    if (!str) return false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') return true;
    }
    return false;
}

bool Validator::isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool Validator::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool Validator::isValidName(const char* name) {
    if (!name || name[0] == '\0') return false;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isAlpha(name[i]) && name[i] != ' ') return false;
    }
    return true;
}

bool Validator::isValidContact(const char* contact) {
    if (!contact || contact[0] == '\0') return false;
    int count = 0;
    for (int i = 0; contact[i] != '\0'; i++) {
        if (!isDigit(contact[i])) return false;
        count++;
    }
    return count == 11;
}

bool Validator::isPositive(int val) {
    return val > 0;
}

bool Validator::isValidDate(const char* date) {
    if (!date) return false;

    int len = 0;
    while (date[len] != '\0') len++;
    if (len != 10) return false;

    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isDigit(date[i])) return false;
    }

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 2025) return false;

    return true;
}

bool Validator::isValidTimeSlot(const char* slot) {
    if (!slot) return false;

    const char* validSlots[8] = {
        "09:00", "10:00", "11:00", "12:00",
        "13:00", "14:00", "15:00", "16:00"
    };

    for (int i = 0; i < 8; i++) {
        bool match = true;
        for (int j = 0; j < 5; j++) {
            if (slot[j] != validSlots[i][j]) {
                match = false;
                break;
            }
        }
        if (match && slot[5] == '\0') {
            return true;
        }
    }
    return false;
}

bool Validator::isValidPassword(const char* pass) {
    if (!pass) return false;
    int count = 0;
    while (pass[count] != '\0') {
        count++;
    }
    return count >= 6;
}

bool Validator::isPositiveFloat(float val) {
    return val > 0.0f;
}