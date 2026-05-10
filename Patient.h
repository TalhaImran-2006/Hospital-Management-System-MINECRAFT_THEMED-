#pragma once
#include "Person.h"
#include <iostream>
using namespace std;

class Patient : public Person {
    int patient_id, age;
    char* contact;
    char* name;
    char* password;
    char gender;
    float balance;
public:
    Patient(int id = 0, const char* nm = "", int ag = 0, char gen = ' ',
        const char* cont = "", const char* pass = "", float bal = 0.0);
    ~Patient();
    Patient(const Patient& other);
    Patient& operator=(const Patient& other);

    int getID() const override;
    int getAge() const;
    const char* getContact() const;
    const char* getName() const override;
    const char* getPassword() const override;
    char getGender() const;
    float getBalance() const;
    void display() const override;

    Patient& setID(int id);
    Patient& setAge(int ag);
    Patient& setContact(const char* cont);
    Patient& setGender(char gen);
    Patient& setBalance(float bal);
    Patient& setName(const char* nm);
    Patient& setPassword(const char* pass);

    Patient& operator+=(float bal);
    Patient& operator-=(float bal);
    bool operator==(const Patient& other) const;
    friend ostream& operator<<(ostream& out, const Patient& other);
};
