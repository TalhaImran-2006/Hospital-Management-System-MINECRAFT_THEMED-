#include "Doctor.h"
#include<iostream>
using namespace std;

Doctor::Doctor(int id, const char* nm, const char* spec, const char* cont, const char* pass, float f)
    : doctor_id(id), fee(f) {
    name = new char[50];
    specialization = new char[50];
    contact = new char[12];
    password = new char[50];

    int i = 0;
    for (; nm[i] != '\0'; i++) name[i] = nm[i];
    name[i] = '\0';

    i = 0;
    for (; spec[i] != '\0'; i++) specialization[i] = spec[i];
    specialization[i] = '\0';

    i = 0;
    for (; cont[i] != '\0'; i++) contact[i] = cont[i];
    contact[i] = '\0';

    i = 0;
    for (; pass[i] != '\0'; i++) password[i] = pass[i];
    password[i] = '\0';
}

Doctor::~Doctor() {
    delete[] name;
    delete[] specialization;
    delete[] contact;
    delete[] password;
}

Doctor::Doctor(const Doctor& other) {
    doctor_id = other.doctor_id;
    fee = other.fee;

    name = new char[50];
    int i = 0;
    for (; other.name && other.name[i] != '\0' && i < 49; i++) name[i] = other.name[i];
    name[i] = '\0';

    specialization = new char[50];
    i = 0;
    for (; other.specialization && other.specialization[i] != '\0' && i < 49; i++) specialization[i] = other.specialization[i];
    specialization[i] = '\0';

    contact = new char[12];
    i = 0;
    for (; other.contact && other.contact[i] != '\0' && i < 11; i++) contact[i] = other.contact[i];
    contact[i] = '\0';

    password = new char[50];
    i = 0;
    for (; other.password && other.password[i] != '\0' && i < 49; i++) password[i] = other.password[i];
    password[i] = '\0';
}

Doctor& Doctor::operator=(const Doctor& other) {
    if (this != &other) {
        delete[] name; delete[] specialization; delete[] contact; delete[] password;
        doctor_id = other.doctor_id; fee = other.fee;

        name = new char[50];
        int i = 0;
        for (; other.name && other.name[i] != '\0' && i < 49; i++) name[i] = other.name[i];
        name[i] = '\0';

        specialization = new char[50];
        i = 0;
        for (; other.specialization && other.specialization[i] != '\0' && i < 49; i++) specialization[i] = other.specialization[i];
        specialization[i] = '\0';

        contact = new char[12];
        i = 0;
        for (; other.contact && other.contact[i] != '\0' && i < 11; i++) contact[i] = other.contact[i];
        contact[i] = '\0';

        password = new char[50];
        i = 0;
        for (; other.password && other.password[i] != '\0' && i < 49; i++) password[i] = other.password[i];
        password[i] = '\0';
    }
    return *this;
}

int Doctor::getID() const { return doctor_id; }
const char* Doctor::getName() const { return name; }
const char* Doctor::getSpecialization() const { return specialization; }
const char* Doctor::getContact() const { return contact; }
const char* Doctor::getPassword() const { return password; }
float Doctor::getFee() const { return fee; }

Doctor& Doctor::setID(int id) { doctor_id = id; return *this; }
Doctor& Doctor::setFee(float f) { fee = f; return *this; }

Doctor& Doctor::setName(const char* nm) {
    if (!name) name = new char[50];
    for (int i = 0; nm && nm[i] != '\0' && i < 49; i++) name[i] = nm[i];
    name[49] = '\0'; return *this;
}
Doctor& Doctor::setSpecialization(const char* spec) {
    if (!specialization) specialization = new char[50];
    for (int i = 0; spec && spec[i] != '\0' && i < 49; i++) specialization[i] = spec[i];
    specialization[49] = '\0'; return *this;
}
Doctor& Doctor::setContact(const char* cont) {
    if (!contact) contact = new char[12];
    for (int i = 0; cont && cont[i] != '\0' && i < 11; i++) contact[i] = cont[i];
    contact[11] = '\0'; return *this;
}
Doctor& Doctor::setPassword(const char* pass) {
    if (!password) password = new char[50];
    for (int i = 0; pass && pass[i] != '\0' && i < 49; i++) password[i] = pass[i];
    password[49] = '\0'; return *this;
}

bool Doctor::operator==(const Doctor& other) const { return this->doctor_id == other.doctor_id; }
void Doctor::display() const { cout << *this << endl; }

ostream& operator<< (ostream& out, const Doctor& other) {
    out << "Doctor ID: " << other.doctor_id << endl;
    out << "Name: " << (other.name ? other.name : "") << endl;
    out << "Specialization: " << (other.specialization ? other.specialization : "") << endl;
    out << "Contact: " << (other.contact ? other.contact : "") << endl;
    out << "Fee: " << other.fee << endl;
    return out;
}