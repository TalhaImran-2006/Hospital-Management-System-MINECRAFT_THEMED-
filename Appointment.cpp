#include "Appointment.h"
#include<iostream>
using namespace std;

Appointment::Appointment(int aid, int pid, int did, const char* d, const char* t, const char* s)
    : appointment_id(aid), patient_id(pid), doctor_id(did) {
    date = new char[11];
    time_slot = new char[6];
    status = new char[20];

    int i = 0;
    for (; d[i] != '\0' && i < 10; i++) date[i] = d[i];
    date[i] = '\0';

    i = 0;
    for (; t[i] != '\0' && i < 5; i++) time_slot[i] = t[i];
    time_slot[i] = '\0';

    i = 0;
    for (; s[i] != '\0' && i < 19; i++) status[i] = s[i];
    status[i] = '\0';
}

Appointment::~Appointment() {
    delete[] date;
    delete[] time_slot;
    delete[] status;
}

Appointment::Appointment(const Appointment& other)
    : appointment_id(other.appointment_id), patient_id(other.patient_id), doctor_id(other.doctor_id) {
    date = new char[11];
    time_slot = new char[6];
    status = new char[20];

    for (int i = 0; i < 11; i++) date[i] = other.date[i];
    for (int i = 0; i < 6; i++) time_slot[i] = other.time_slot[i];
    for (int i = 0; i < 20; i++) status[i] = other.status[i];
}

Appointment& Appointment::operator=(const Appointment& other) {
    if (this != &other) {
        delete[] date; delete[] time_slot; delete[] status;

        appointment_id = other.appointment_id;
        patient_id = other.patient_id;
        doctor_id = other.doctor_id;

        date = new char[11];
        time_slot = new char[6];
        status = new char[20];

        for (int i = 0; i < 11; i++) date[i] = other.date[i];
        for (int i = 0; i < 6; i++) time_slot[i] = other.time_slot[i];
        for (int i = 0; i < 20; i++) status[i] = other.status[i];
    }
    return *this;
}

int Appointment::getID() const { return appointment_id; }
int Appointment::getPatientID() const { return patient_id; }
int Appointment::getDoctorID() const { return doctor_id; }
const char* Appointment::getDate() const { return date; }
const char* Appointment::getTimeSlot() const { return time_slot; }
const char* Appointment::getStatus() const { return status; }

void Appointment::setID(int id) { appointment_id = id; }
void Appointment::setPatientID(int id) { patient_id = id; }
void Appointment::setDoctorID(int id) { doctor_id = id; }

void Appointment::setDate(const char* d) {
    for (int i = 0; d && d[i] != '\0' && i < 10; i++) date[i] = d[i];
    date[10] = '\0';
}
void Appointment::setTimeSlot(const char* t) {
    for (int i = 0; t && t[i] != '\0' && i < 5; i++) time_slot[i] = t[i];
    time_slot[5] = '\0';
}
void Appointment::setStatus(const char* s) {
    for (int i = 0; s && s[i] != '\0' && i < 19; i++) status[i] = s[i];
    status[19] = '\0';
}

bool Appointment::operator==(const Appointment& other) {

    if (this->doctor_id != other.doctor_id) {
        return false;
    }

    int i = 0;
    while (this->date[i] != '\0' && other.date[i] != '\0') {
        if (this->date[i] != other.date[i]) return false;
        i++;
    }
    if (this->date[i] != other.date[i]) return false;
    
    i = 0;
    while (this->time_slot[i] != '\0' && other.time_slot[i] != '\0') {
        if (this->time_slot[i] != other.time_slot[i]) return false;
        i++;
    }
    if (this->time_slot[i] != other.time_slot[i]) return false;

    const char* canc = "cancelled";

    bool thisIsCancelled = true;
    int j = 0;
    while (this->status[j] != '\0' && canc[j] != '\0') {
        if (this->status[j] != canc[j]) { thisIsCancelled = false; break; }
        j++;
    }
    if (this->status[j] != canc[j]) thisIsCancelled = false;

    bool otherIsCancelled = true;
    j = 0;
    while (other.status[j] != '\0' && canc[j] != '\0') {
        if (other.status[j] != canc[j]) { otherIsCancelled = false; break; }
        j++;
    }
    if (other.status[j] != canc[j]) otherIsCancelled = false;

    if (thisIsCancelled || otherIsCancelled) {
        return false;
    }
    return true;
}

ostream& operator<< (ostream& out, const Appointment& other) {
    out << "Appointment ID: " << other.appointment_id << endl;
    out << "Patient ID: " << other.patient_id << endl;
    out << "Doctor ID: " << other.doctor_id << endl;
    out << "Date: " << (other.date ? other.date : "") << endl;
    out << "Time Slot: " << (other.time_slot ? other.time_slot : "") << endl;
    out << "Status: " << (other.status ? other.status : "") << endl;
    return out;
}