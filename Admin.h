#pragma once
#include "Person.h"
#include <iostream>
using namespace std;

class Admin : public Person {
    int admin_id;
    char* name;
    char* password;
public:
    Admin(int id = 0, const char* nm = "", const char* pass = "");
    ~Admin();
    Admin(const Admin& other);
    Admin& operator=(const Admin& other);

    int getID() const override;
    const char* getName() const override;
    const char* getPassword() const override;
    void display() const override;

    friend ostream& operator<<(ostream& out, const Admin& other);
};