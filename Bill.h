#pragma once
#include <iostream>

class Bill {
    int bill_id;
    int patient_id;
    int appointment_id;
    float amount;
    char* status;
    char* date;
public:
    Bill(int b = 0, int p = 0, int a = 0, float amt = 0.0,
        const char* s = "", const char* d = "");
    ~Bill();
    Bill(const Bill& other);
    Bill& operator=(const Bill& other);

    int getID() const;
    int getPatientID() const;
    int getAppointmentID() const;
    float getAmount() const;
    const char* getStatus() const;
    const char* getDate() const;

    void setID(int id);
    void setPatientID(int id);
    void setAppointmentID(int id);
    void setAmount(float amt);
    void setStatus(const char* s);
    void setDate(const char* d);
};