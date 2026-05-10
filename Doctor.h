#pragma once
#include "Person.h"
#include <iostream>
using namespace std;

class Doctor : public Person {
    int doctor_id;
    char* name;
    char* specialization;
    char* contact;
    char* password;
    float fee;
public:
    Doctor(int id = 0, const char* nm = "", const char* spec = "",
        const char* cont = "", const char* pass = "", float f = 0.0);
    ~Doctor();
    Doctor(const Doctor& other);
    Doctor& operator=(const Doctor& other);

    int getID() const override;
    const char* getName() const override;
    const char* getSpecialization() const;
    const char* getContact() const;
    const char* getPassword() const override;
    float getFee() const;
    void display() const override;

    Doctor& setID(int id);
    Doctor& setName(const char* nm);
    Doctor& setSpecialization(const char* spec);
    Doctor& setContact(const char* cont);
    Doctor& setPassword(const char* pass);
    Doctor& setFee(float f);

    bool operator==(const Doctor& other) const;
    friend ostream& operator<<(ostream& out, const Doctor& other);
};