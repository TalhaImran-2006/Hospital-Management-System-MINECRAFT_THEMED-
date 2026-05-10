#include "Bill.h"
#include<iostream>
using namespace std;

Bill::Bill(int b, int p, int a, float amt, const char* s, const char* d)
    : bill_id(b), patient_id(p), appointment_id(a), amount(amt) {
    status = new char[20];
    date = new char[11];

    int i = 0;
    for (; s && s[i] != '\0' && i < 19; i++) status[i] = s[i];
    status[i] = '\0';

    i = 0;
    for (; d && d[i] != '\0' && i < 10; i++) date[i] = d[i];
    date[i] = '\0';
}

Bill::~Bill() {
    delete[] status;
    delete[] date;
}

Bill::Bill(const Bill& other)
    : bill_id(other.bill_id), patient_id(other.patient_id), appointment_id(other.appointment_id), amount(other.amount) {
    status = new char[20];
    date = new char[11];

    for (int i = 0; i < 20; i++) status[i] = other.status[i];
    for (int i = 0; i < 11; i++) date[i] = other.date[i];
}

Bill& Bill::operator=(const Bill& other) {
    if (this != &other) {
        delete[] status;
        delete[] date;

        bill_id = other.bill_id;
        patient_id = other.patient_id;
        appointment_id = other.appointment_id;
        amount = other.amount;

        status = new char[20];
        date = new char[11];

        for (int i = 0; i < 20; i++) status[i] = other.status[i];
        for (int i = 0; i < 11; i++) date[i] = other.date[i];
    }
    return *this;
}

int Bill::getID() const { return bill_id; }
int Bill::getPatientID() const { return patient_id; }
int Bill::getAppointmentID() const { return appointment_id; }
float Bill::getAmount() const { return amount; }
const char* Bill::getStatus() const { return status; }
const char* Bill::getDate() const { return date; }

void Bill::setID(int id) { bill_id = id; }
void Bill::setPatientID(int id) { patient_id = id; }
void Bill::setAppointmentID(int id) { appointment_id = id; }
void Bill::setAmount(float amt) { amount = amt; }

void Bill::setStatus(const char* s) {
    for (int i = 0; s && s[i] != '\0' && i < 19; i++) status[i] = s[i];
    status[19] = '\0';
}

void Bill::setDate(const char* d) {
    for (int i = 0; d && d[i] != '\0' && i < 10; i++) date[i] = d[i];
    date[10] = '\0';
}