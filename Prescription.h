#pragma once
#include <iostream>
using namespace std;

class Prescription {
    int prescription_id, appointment_id, patient_id, doctor_id;
    char* date;
    char* medicines;
    char* notes;
public:
    Prescription(int pid = 0, int aid = 0, int patid = 0, int docid = 0,
        const char* d = "", const char* med = "", const char* nt = "");
    ~Prescription();
    Prescription(const Prescription& other);
    Prescription& operator=(const Prescription& other);

    int getID() const;
    int getAppointmentID() const;
    int getPatientID() const;
    int getDoctorID() const;
    const char* getDate() const;
    const char* getMedicines() const;
    const char* getNotes() const;

    Prescription& setID(int id);
    Prescription& setAppointmentID(int id);
    Prescription& setPatientID(int id);
    Prescription& setDoctorID(int id);
    Prescription& setDate(const char* d);
    Prescription& setMedicines(const char* med);
    Prescription& setNotes(const char* nt);

    bool operator==(const Prescription& other) const;
    friend ostream& operator<<(ostream& out, const Prescription& other);
};