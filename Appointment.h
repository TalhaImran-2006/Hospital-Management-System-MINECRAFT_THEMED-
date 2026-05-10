#pragma once
#include <iostream>
using namespace std;

class Appointment {
    int appointment_id;
    int patient_id;
    int doctor_id;
    char* date;
    char* time_slot;
    char* status;
public:
    Appointment(int aid = 0, int pid = 0, int did = 0,
        const char* d = "", const char* t = "", const char* s = "");
    ~Appointment();
    Appointment(const Appointment& other);
    Appointment& operator=(const Appointment& other);

    int getID() const;
    int getPatientID() const;
    int getDoctorID() const;
    const char* getDate() const;
    const char* getTimeSlot() const;
    const char* getStatus() const;

    void setID(int id);
    void setPatientID(int id);
    void setDoctorID(int id);
    void setDate(const char* d);
    void setTimeSlot(const char* t);
    void setStatus(const char* s);

    bool operator==(const Appointment&);
    friend ostream& operator<< (ostream& out, const Appointment& other);
};