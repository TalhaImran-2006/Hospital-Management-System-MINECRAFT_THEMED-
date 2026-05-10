#pragma once
#include <iostream>
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "SecurityLog.h"

class FileHandler {
public:

    static int parseInt(char* line, int& i);
    static void parseStr(char* line, int& i, char* out, int size);
    static float parseFloat(char* line, int& i);

    static void loadPatients(Storage<Patient>& s);
    static void savePatients(const Storage<Patient>& s);
    static void addPatient(const Patient& p);
    static void updatePatient(const Storage<Patient>& s);
    static void deletePatient(const Storage<Patient>& s);

    static void loadDoctors(Storage<Doctor>& s);
    static void saveDoctors(const Storage<Doctor>& s);
    static void addDoctor(const Doctor& d);
    static void updateDoctor(const Storage<Doctor>& s);
    static void deleteDoctor(const Storage<Doctor>& s);

    static void loadAdmin(Storage<Admin>& s);
    static void saveAdmin(const Storage<Admin>& s);

    static void loadAppointments(Storage<Appointment>& s);
    static void saveAppointments(const Storage<Appointment>& s);
    static void addAppointment(const Appointment& a);
    static void updateAppointment(const Storage<Appointment>& s);

    static void loadBills(Storage<Bill>& s);
    static void saveBills(const Storage<Bill>& s);
    static void addBill(const Bill& b);
    static void updateBill(const Storage<Bill>& s);

    static void loadPrescription(Storage<Prescription>& s);
    static void savePrescription(const Storage<Prescription>& s);
    static void addPrescription(const Prescription& pr);

    static void loadSecurityLogs(Storage<SecurityLog>& s);
    static void saveSecurityLogs(const Storage<SecurityLog>& s);
    static void addSecurityLog(const SecurityLog& log);

    static bool dischargePatient(int patientID);
};