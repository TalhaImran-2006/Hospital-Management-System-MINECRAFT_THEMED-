#include "FileHandler.h"
#include <fstream>
#include <iostream>
using namespace std;

int FileHandler::parseInt(char* line, int& i) {
    int val = 0;
    bool neg = false;
    if (line[i] == '-') { neg = true; i++; }
    while (line[i] != '\0' && line[i] != ',') {
        val = val * 10 + (line[i] - '0');
        i++;
    }
    if (line[i] == ',') i++;
    return neg ? -val : val;
}

void FileHandler::parseStr(char* line, int& i, char* out, int size) {
    int pos = 0;
    while (line[i] != '\0' && line[i] != ',' && pos < size - 1) {
        out[pos++] = line[i++];
    }
    out[pos] = '\0';
    if (line[i] == ',') i++;
    int end = pos - 1;
    while (end >= 0 && (out[end] == ' ' || out[end] == '.' ||
        out[end] == '\r' || out[end] == '\n' ||
        (int)out[end] < 32)) {
        out[end] = '\0';
        end--;
    }
    int start = 0;
    while (out[start] == ' ') start++;

    if (start > 0) {
        int k = 0;
        while (out[start + k] != '\0') {
            out[k] = out[start + k];
            k++;
        }
        out[k] = '\0';
    }
}

float FileHandler::parseFloat(char* line, int& i) {
    float w = 0.0f, f = 0.0f, div = 1.0f;
    bool frac = false;
    while (line[i] != '\0' && line[i] != ',') {
        if (line[i] == '.') frac = true;
        else if (!frac) w = w * 10 + (line[i] - '0');
        else { f = f * 10 + (line[i] - '0'); div *= 10.0f; }
        i++;
    }
    if (line[i] == ',') i++;
    return w + (f / div);
}

void FileHandler::loadPatients(Storage<Patient>& s) {
    ifstream file("patients.txt");
    if (!file.is_open()) return;
    char line[500];
    while (file.getline(line, 500)) {
        int i = 0;
        int id = parseInt(line, i);
        char name[50]; parseStr(line, i, name, 50);
        int age = parseInt(line, i);
        char genderBuf[2]; parseStr(line, i, genderBuf, 2);
        char contact[12]; parseStr(line, i, contact, 12);
        char pass[50]; parseStr(line, i, pass, 50);
        float bal = parseFloat(line, i);
        s.add(Patient(id, name, age, genderBuf[0], contact, pass, bal));
    }
    file.close();
}

void FileHandler::savePatients(const Storage<Patient>& s) {
    ofstream file("patients.txt");
    if (!file.is_open()) return;
    for (int i = 0; i < s.getCount(); i++) {
        const Patient& p = s.getAt(i);
        file << p.getID() << ',' << p.getName() << ',' << p.getAge() << ','
            << p.getGender() << ',' << p.getContact() << ','
            << p.getPassword() << ',' << p.getBalance() << '\n';
    }
    file.close();
}

void FileHandler::addPatient(const Patient& p) {
    ofstream file("patients.txt", ios::app);
    if (!file.is_open()) return;
    file << p.getID() << ',' << p.getName() << ',' << p.getAge() << ','
        << p.getGender() << ',' << p.getContact() << ','
        << p.getPassword() << ',' << p.getBalance() << '\n';
    file.close();
}

void FileHandler::updatePatient(const Storage<Patient>& s) {
    savePatients(s);
}

void FileHandler::deletePatient(const Storage<Patient>& s) {
    savePatients(s);
}

void FileHandler::loadDoctors(Storage<Doctor>& s) {
    ifstream file("doctors.txt");
    if (!file.is_open()) return;
    char line[500];
    while (file.getline(line, 500)) {
        int i = 0;
        int id = parseInt(line, i);
        char name[50], spec[50], cont[12], pass[50];
        parseStr(line, i, name, 50);
        parseStr(line, i, spec, 50);
        parseStr(line, i, cont, 12);
        parseStr(line, i, pass, 50);
        float fee = parseFloat(line, i);
        s.add(Doctor(id, name, spec, cont, pass, fee));
    }
    file.close();
}

void FileHandler::saveDoctors(const Storage<Doctor>& s) {
    ofstream file("doctors.txt");
    if (!file.is_open()) return;
    for (int i = 0; i < s.getCount(); i++) {
        const Doctor& d = s.getAt(i);
        file << d.getID() << ',' << d.getName() << ',' << d.getSpecialization()
            << ',' << d.getContact() << ',' << d.getPassword()
            << ',' << d.getFee() << '\n';
    }
    file.close();
}

void FileHandler::addDoctor(const Doctor& d) {
    ofstream file("doctors.txt", ios::app);
    if (!file.is_open()) return;
    file << d.getID() << ',' << d.getName() << ',' << d.getSpecialization()
        << ',' << d.getContact() << ',' << d.getPassword()
        << ',' << d.getFee() << '\n';
    file.close();
}

void FileHandler::updateDoctor(const Storage<Doctor>& s) {
    saveDoctors(s);
}

void FileHandler::deleteDoctor(const Storage<Doctor>& s) {
    saveDoctors(s);
}

void FileHandler::loadAdmin(Storage<Admin>& s) {
    ifstream file("admin.txt");
    if (!file.is_open()) return;
    char line[300];
    while (file.getline(line, 300)) {
        int i = 0;
        int id = parseInt(line, i);
        char name[50], pass[50];
        parseStr(line, i, name, 50);
        parseStr(line, i, pass, 50);
        s.add(Admin(id, name, pass));
    }
    file.close();
}

void FileHandler::saveAdmin(const Storage<Admin>& s) {
    ofstream file("admin.txt");
    if (!file.is_open()) return;
    for (int i = 0; i < s.getCount(); i++) {
        const Admin& a = s.getAt(i);
        file << a.getID() << ',' << a.getName() << ',' << a.getPassword() << '\n';
    }
    file.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& s) {
    ifstream file("appointments.txt");
    if (!file.is_open()) return;
    char line[300];
    while (file.getline(line, 300)) {
        int i = 0;
        int id = parseInt(line, i);
        int pid = parseInt(line, i);
        int did = parseInt(line, i);
        char date[11], time[6], stat[20];
        parseStr(line, i, date, 11);
        parseStr(line, i, time, 6);
        parseStr(line, i, stat, 20);
        s.add(Appointment(id, pid, did, date, time, stat));
    }
    file.close();
}

void FileHandler::saveAppointments(const Storage<Appointment>& s) {
    ofstream file("appointments.txt");
    if (!file.is_open()) return;
    for (int i = 0; i < s.getCount(); i++) {
        const Appointment& a = s.getAt(i);
        file << a.getID() << ',' << a.getPatientID() << ',' << a.getDoctorID()
            << ',' << a.getDate() << ',' << a.getTimeSlot()
            << ',' << a.getStatus() << '\n';
    }
    file.close();
}

void FileHandler::addAppointment(const Appointment& a) {
    ofstream file("appointments.txt", ios::app);
    if (!file.is_open()) return;
    file << a.getID() << ',' << a.getPatientID() << ',' << a.getDoctorID()
        << ',' << a.getDate() << ',' << a.getTimeSlot()
        << ',' << a.getStatus() << '\n';
    file.close();
}

void FileHandler::updateAppointment(const Storage<Appointment>& s) {
    saveAppointments(s);
}

void FileHandler::loadBills(Storage<Bill>& s) {
    ifstream file("bills.txt");
    if (!file.is_open()) return;
    char line[300];
    while (file.getline(line, 300)) {
        int i = 0;
        int id = parseInt(line, i);
        int pid = parseInt(line, i);
        int aid = parseInt(line, i);
        float amt = parseFloat(line, i);
        char stat[20], date[11];
        parseStr(line, i, stat, 20);
        parseStr(line, i, date, 11);
        s.add(Bill(id, pid, aid, amt, stat, date));
    }
    file.close();
}

void FileHandler::saveBills(const Storage<Bill>& s) {
    ofstream file("bills.txt");
    if (!file.is_open()) return;
    for (int i = 0; i < s.getCount(); i++) {
        const Bill& b = s.getAt(i);
        file << b.getID() << ',' << b.getPatientID() << ','
            << b.getAppointmentID() << ',' << b.getAmount() << ','
            << b.getStatus() << ',' << b.getDate() << '\n';
    }
    file.close();
}

void FileHandler::addBill(const Bill& b) {
    ofstream file("bills.txt", ios::app);
    if (!file.is_open()) return;
    file << b.getID() << ',' << b.getPatientID() << ','
        << b.getAppointmentID() << ',' << b.getAmount() << ','
        << b.getStatus() << ',' << b.getDate() << '\n';
    file.close();
}

void FileHandler::updateBill(const Storage<Bill>& s) {
    saveBills(s);
}

void FileHandler::loadPrescription(Storage<Prescription>& s) {
    ifstream file("prescriptions.txt");
    if (!file.is_open()) return;
    char line[1200];
    while (file.getline(line, 1200)) {
        int i = 0;
        int pid = parseInt(line, i);
        int aid = parseInt(line, i);
        int patid = parseInt(line, i);
        int docid = parseInt(line, i);
        char date[20], meds[500], notes[300];
        parseStr(line, i, date, 20);
        parseStr(line, i, meds, 500);
        parseStr(line, i, notes, 300);
        s.add(Prescription(pid, aid, patid, docid, date, meds, notes));
    }
    file.close();
}

void FileHandler::savePrescription(const Storage<Prescription>& s) {
    ofstream file("prescriptions.txt");
    if (!file.is_open()) return;
    for (int i = 0; i < s.getCount(); i++) {
        const Prescription& pr = s.getAt(i);
        file << pr.getID() << ',' << pr.getAppointmentID() << ','
            << pr.getPatientID() << ',' << pr.getDoctorID() << ','
            << pr.getDate() << ',' << pr.getMedicines() << ','
            << pr.getNotes() << '\n';
    }
    file.close();
}

void FileHandler::addPrescription(const Prescription& pr) {
    ofstream file("prescriptions.txt", ios::app);
    if (!file.is_open()) return;
    file << pr.getID() << ',' << pr.getAppointmentID() << ','
        << pr.getPatientID() << ',' << pr.getDoctorID() << ','
        << pr.getDate() << ',' << pr.getMedicines() << ','
        << pr.getNotes() << '\n';
    file.close();
}

void FileHandler::loadSecurityLogs(Storage<SecurityLog>& s) {
    ifstream file("security_log.txt");
    if (!file.is_open()) return;
    char line[200];
    while (file.getline(line, 200)) {
        int i = 0;
        char ts[32], role[15], res[10];
        parseStr(line, i, ts, 32);
        parseStr(line, i, role, 15);
        int id = parseInt(line, i);
        parseStr(line, i, res, 10);
        s.add(SecurityLog(ts, role, id, res));
    }
    file.close();
}

void FileHandler::saveSecurityLogs(const Storage<SecurityLog>& s) {
    ofstream file("security_log.txt");
    if (!file.is_open()) return;
    for (int i = 0; i < s.getCount(); i++) {
        const SecurityLog& log = s.getAt(i);
        file << log.getTimestamp() << ',' << log.getRole() << ','
            << log.getID() << ',' << log.getResult() << '\n';
    }
    file.close();
}

void FileHandler::addSecurityLog(const SecurityLog& log) {
    ofstream file("security_log.txt", ios::app);
    if (!file.is_open()) return;
    file << log.getTimestamp() << ',' << log.getRole() << ','
        << log.getID() << ',' << log.getResult() << '\n';
    file.close();
}

bool FileHandler::dischargePatient(int patientID) {

    Storage<Patient> patStore; loadPatients(patStore);
    Storage<Bill> billStore; loadBills(billStore);
    Storage<Appointment> appStore; loadAppointments(appStore);
    Storage<Prescription> preStore; loadPrescription(preStore);

    for (int i = 0; i < billStore.getCount(); i++) {
        if (billStore.getAt(i).getPatientID() == patientID) {
            string stat = billStore.getAt(i).getStatus();
            if (stat == "unpaid") return false;
        }
    }

    for (int i = 0; i < appStore.getCount(); i++) {
        if (appStore.getAt(i).getPatientID() == patientID) {
            string stat = appStore.getAt(i).getStatus();
            if (stat == "pending") return false;
        }
    }
    int patientIdx = -1;
    for (int i = 0; i < patStore.getCount(); i++) {
        if (patStore.getAt(i).getID() == patientID) {
            patientIdx = i;
            break;
        }
    }

    if (patientIdx != -1) {
        ofstream archive("discharged.txt", ios::app);
        if (archive.is_open()) {
            const Patient& p = patStore.getAt(patientIdx);
            archive << p.getID() << ',' << p.getName() << ',' << p.getAge() << ','
                << p.getGender() << ',' << p.getContact() << ','
                << p.getPassword() << ',' << p.getBalance() << '\n';
            archive.close();
        }
    }
    else {
        return false;
    }

    patStore.removeByID(patientID);
    savePatients(patStore);

    Storage<Bill> remainingBills;
    for (int i = 0; i < billStore.getCount(); i++) {
        if (billStore.getAt(i).getPatientID() != patientID) {
            remainingBills.add(billStore.getAt(i));
        }
    }
    saveBills(remainingBills);

    Storage<Appointment> remainingApps;
    for (int i = 0; i < appStore.getCount(); i++) {
        if (appStore.getAt(i).getPatientID() != patientID) {
            remainingApps.add(appStore.getAt(i));
        }
    }
    saveAppointments(remainingApps);
    Storage<Prescription> remainingPres;
    for (int i = 0; i < preStore.getCount(); i++) {
        if (preStore.getAt(i).getPatientID() != patientID) {
            remainingPres.add(preStore.getAt(i));
        }
    }
    savePrescription(remainingPres);

    return true;
}