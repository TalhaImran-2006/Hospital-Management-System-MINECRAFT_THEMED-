#include "Patient.h"
#include<iostream>
using namespace std;

Patient::Patient(int id, const char* nm, int ag, char gen, const char* cont, const char* pass, float bal)
    : patient_id(id), age(ag), gender(gen), balance(bal) {
    name = new char[50];
    password = new char[50];
    contact = new char[12];

    int i = 0;
    for (; nm && nm[i] != '\0' && i < 49; i++) name[i] = nm[i];
    name[i] = '\0';

    i = 0;
    for (; pass && pass[i] != '\0' && i < 49; i++) password[i] = pass[i];
    password[i] = '\0';

    i = 0;
    for (; cont && cont[i] != '\0' && i < 11; i++) contact[i] = cont[i];
    contact[i] = '\0';
}

Patient::~Patient() {
    delete[] name;
    delete[] password;
    delete[] contact;
}

Patient::Patient(const Patient& other) {
    patient_id = other.patient_id;
    age = other.age;
    gender = other.gender;
    balance = other.balance;
    name = new char[50];
    password = new char[50];
    contact = new char[12];

    for (int i = 0; i < 50; i++) name[i] = other.name[i];
    for (int i = 0; i < 50; i++) password[i] = other.password[i];
    for (int i = 0; i < 12; i++) contact[i] = other.contact[i];
}

Patient& Patient::operator=(const Patient& other) {
    if (this != &other) {
        delete[] name; delete[] password; delete[] contact;
        patient_id = other.patient_id; age = other.age;
        gender = other.gender; balance = other.balance;
        name = new char[50]; password = new char[50]; contact = new char[12];
        for (int i = 0; i < 50; i++) name[i] = other.name[i];
        for (int i = 0; i < 50; i++) password[i] = other.password[i];
        for (int i = 0; i < 12; i++) contact[i] = other.contact[i];
    }
    return *this;
}

int Patient::getID() const { return patient_id; }
int Patient::getAge() const { return age; }
const char* Patient::getContact() const { return contact; }
const char* Patient::getName() const { return name; }
const char* Patient::getPassword() const { return password; }
char Patient::getGender() const { return gender; }
float Patient::getBalance() const { return balance; }

Patient& Patient::setID(int id) { patient_id = id; return *this; }
Patient& Patient::setAge(int ag) { age = ag; return *this; }
Patient& Patient::setGender(char gen) { gender = gen; return *this; }
Patient& Patient::setBalance(float bal) { balance = bal; return *this; }

Patient& Patient::setContact(const char* cont) {
    if (!contact) contact = new char[12];
    for (int i = 0; i < 11; i++) contact[i] = cont ? cont[i] : '\0';
    contact[11] = '\0';
    return *this;
}

Patient& Patient::setName(const char* nm) {
    if (!name) name = new char[50];
    for (int i = 0; i < 49; i++) name[i] = nm ? nm[i] : '\0';
    name[49] = '\0';
    return *this;
}

Patient& Patient::setPassword(const char* pass) {
    if (!password) password = new char[50];
    for (int i = 0; i < 49; i++) password[i] = pass ? pass[i] : '\0';
    password[49] = '\0';
    return *this;
}

Patient& Patient::operator+=(const float bal) { balance += bal; return *this; }
Patient& Patient::operator-=(const float bal) { balance -= bal; return *this; }
bool Patient::operator==(const Patient& other) const { return patient_id == other.patient_id; }

void Patient::display() const {
    cout << "Patient: " << name << " (ID: " << patient_id << ")" << endl;
}

ostream& operator<< (ostream& out, const Patient& other) {
    out << "Patient ID: " << other.patient_id << endl;
    out << "Name: " << (other.name ? other.name : "") << endl;
    out << "Age: " << other.age << endl;
    out << "Gender: " << other.gender << endl;
    out << "Contact: " << (other.contact ? other.contact : "") << endl;
    out << "Balance: " << other.balance << endl;
    return out;
}