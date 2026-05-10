#pragma once
#include <iostream>

class Validator {
public:
    static bool containsDelimiter(const char* str);
    static bool isAlpha(char c);
    static bool isDigit(char c);
    static bool isValidName(const char* name);
    static bool isValidContact(const char* contact);
    static bool isPositive(int val);
    static bool isValidDate(const char* date);
    static bool isValidTimeSlot(const char* slot);
    static bool isValidPassword(const char* pass);
    static bool isPositiveFloat(float val);
};