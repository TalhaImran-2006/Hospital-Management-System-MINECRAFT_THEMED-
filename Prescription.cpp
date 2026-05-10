#include "Prescription.h"
#include<iostream>
using namespace std;

Prescription::Prescription(int pid, int aid, int patid, int docid, const char* d, const char* med, const char* nt)
    : prescription_id(pid), appointment_id(aid), patient_id(patid), doctor_id(docid) {
    date = new char[20];
    medicines = new char[500];
    notes = new char[300];

    int i = 0;
    for (; d && d[i] != '\0' && i < 19; i++) date[i] = d[i];
    date[i] = '\0';

    i = 0;
    for (; med && med[i] != '\0' && i < 499; i++) medicines[i] = med[i];
    medicines[i] = '\0';

    i = 0;
    for (; nt && nt[i] != '\0' && i < 299; i++) notes[i] = nt[i];
    notes[i] = '\0';
}

Prescription::~Prescription() {
    delete[] date;
    delete[] medicines;
    delete[] notes;
}

Prescription::Prescription(const Prescription& other)
    : prescription_id(other.prescription_id), appointment_id(other.appointment_id),
    patient_id(other.patient_id), doctor_id(other.doctor_id) {
    date = new char[20];
    medicines = new char[500];
    notes = new char[300];

    for (int i = 0; i < 20; i++) date[i] = other.date[i];
    for (int i = 0; i < 500; i++) medicines[i] = other.medicines[i];
    for (int i = 0; i < 300; i++) notes[i] = other.notes[i];
}

Prescription& Prescription::operator=(const Prescription& other) {
    if (this != &other) {
        delete[] date; delete[] medicines; delete[] notes;

        prescription_id = other.prescription_id;
        appointment_id = other.appointment_id;
        patient_id = other.patient_id;
        doctor_id = other.doctor_id;

        date = new char[20]; medicines = new char[500]; notes = new char[300];
        for (int i = 0; i < 20; i++) date[i] = other.date[i];
        for (int i = 0; i < 500; i++) medicines[i] = other.medicines[i];
        for (int i = 0; i < 300; i++) notes[i] = other.notes[i];
    }
    return *this;
}

int Prescription::getID() const { return prescription_id; }
int Prescription::getAppointmentID() const { return appointment_id; }
int Prescription::getPatientID() const { return patient_id; }
int Prescription::getDoctorID() const { return doctor_id; }
const char* Prescription::getDate() const { return date; }
const char* Prescription::getMedicines() const { return medicines; }
const char* Prescription::getNotes() const { return notes; }

Prescription& Prescription::setID(int id) { prescription_id = id; return *this; }
Prescription& Prescription::setAppointmentID(int id) { appointment_id = id; return *this; }
Prescription& Prescription::setPatientID(int id) { patient_id = id; return *this; }
Prescription& Prescription::setDoctorID(int id) { doctor_id = id; return *this; }

Prescription& Prescription::setDate(const char* d) {
    if (!date) date = new char[20];
    for (int i = 0; d && d[i] != '\0' && i < 19; i++) date[i] = d[i];
    date[19] = '\0'; return *this;
}

Prescription& Prescription::setMedicines(const char* med) {
    if (!medicines) medicines = new char[500];
    for (int i = 0; med && med[i] != '\0' && i < 499; i++) medicines[i] = med[i];
    medicines[499] = '\0'; return *this;
}

Prescription& Prescription::setNotes(const char* nt) {
    if (!notes) notes = new char[300];
    for (int i = 0; nt && nt[i] != '\0' && i < 299; i++) notes[i] = nt[i];
    notes[299] = '\0'; return *this;
}

bool Prescription::operator==(const Prescription& other) const { return this->prescription_id == other.prescription_id; }
ostream& operator<< (ostream& out, const Prescription& other) {
    out << "Prescription ID: " << other.prescription_id << endl;
    out << "Appointment ID: " << other.appointment_id << endl;
    out << "Patient ID: " << other.patient_id << endl;
    out << "Doctor ID: " << other.doctor_id << endl;
    out << "Date: " << (other.date ? other.date : "") << endl;
    out << "Medicines: " << (other.medicines ? other.medicines : "") << endl;
    out << "Notes: " << (other.notes ? other.notes : "") << endl;
    return out;
}