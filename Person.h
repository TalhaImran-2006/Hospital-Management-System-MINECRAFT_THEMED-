#pragma once
#include <iostream>

class Person {
public:
    virtual int getID() const = 0;
    virtual const char* getName() const = 0;
    virtual const char* getPassword() const = 0;
    virtual void display() const = 0;
    virtual ~Person() {}
};