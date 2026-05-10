#include "Admin.h"
#include<iostream>

using namespace std;

Admin::Admin(int id, const char* nm, const char* pass) : admin_id(id) {
    name = new char[50];
    password = new char[50];

    int i = 0;
    for (; nm && nm[i] != '\0' && i < 49; i++) name[i] = nm[i];
    name[i] = '\0';

    i = 0;
    for (; pass && pass[i] != '\0' && i < 49; i++) password[i] = pass[i];
    password[i] = '\0';
}

Admin::~Admin() {
    delete[] name;
    delete[] password;
}

Admin::Admin(const Admin& other) {
    admin_id = other.admin_id;
    name = new char[50];
    password = new char[50];

    for (int i = 0; i < 50; i++) name[i] = other.name[i];
    for (int i = 0; i < 50; i++) password[i] = other.password[i];
}

Admin& Admin::operator=(const Admin& other) {
    if (this != &other) {
        delete[] name; delete[] password;
        admin_id = other.admin_id;
        name = new char[50]; password = new char[50];
        for (int i = 0; i < 50; i++) name[i] = other.name[i];
        for (int i = 0; i < 50; i++) password[i] = other.password[i];
    }
    return *this;
}

int Admin::getID() const { return admin_id; }
const char* Admin::getName() const { return name; }
const char* Admin::getPassword() const { return password; }

void Admin::display() const {
    cout << "Admin ID: " << admin_id << ", Name: " << name << endl;
}

ostream& operator<< (ostream& out, const Admin& other) {
    out << "Admin ID: " << other.admin_id << "\nName: " << other.name << "\nPassword: " << other.password << endl;
    return out;
}