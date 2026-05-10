#include "SecurityLog.h"
#include<iostream>
using namespace std;

SecurityLog::SecurityLog(const char* ts, const char* r, int id, const char* res)
    : entered_id(id) {
    int i = 0;
    for (; ts && ts[i] != '\0' && i < 31; i++) timestamp[i] = ts[i];
    timestamp[i] = '\0';

    i = 0;
    for (; r && r[i] != '\0' && i < 14; i++) role[i] = r[i];
    role[i] = '\0';

    i = 0;
    for (; res && res[i] != '\0' && i < 9; i++) result[i] = res[i];
    result[i] = '\0';
}

SecurityLog::~SecurityLog() {}

SecurityLog::SecurityLog(const SecurityLog& other) : entered_id(other.entered_id) {
    for (int i = 0; i < 32; i++) timestamp[i] = other.timestamp[i];
    for (int i = 0; i < 15; i++) role[i] = other.role[i];
    for (int i = 0; i < 10; i++) result[i] = other.result[i];
}

SecurityLog& SecurityLog::operator=(const SecurityLog& other) {
    if (this != &other) {
        entered_id = other.entered_id;
        for (int i = 0; i < 32; i++) timestamp[i] = other.timestamp[i];
        for (int i = 0; i < 15; i++) role[i] = other.role[i];
        for (int i = 0; i < 10; i++) result[i] = other.result[i];
    }
    return *this;
}

int SecurityLog::getID() const { return entered_id; }
const char* SecurityLog::getTimestamp() const { return timestamp; }
const char* SecurityLog::getRole() const { return role; }
const char* SecurityLog::getResult() const { return result; }

// Setters
void SecurityLog::setTimestamp(const char* ts) {
    if (!ts) { timestamp[0] = '\0'; return; }
    int i = 0;
    for (; ts[i] != '\0' && i < 31; i++) timestamp[i] = ts[i];
    timestamp[i] = '\0';
}

void SecurityLog::setRole(const char* r) {
    if (!r) { role[0] = '\0'; return; }
    int i = 0;
    for (; r[i] != '\0' && i < 14; i++) role[i] = r[i];
    role[i] = '\0';
}

void SecurityLog::setID(int id) { entered_id = id; }

void SecurityLog::setResult(const char* res) {
    if (!res) { result[0] = '\0'; return; }
    int i = 0;
    for (; res[i] != '\0' && i < 9; i++) result[i] = res[i];
    result[i] = '\0';
}

bool SecurityLog::operator==(const SecurityLog& other) const {
    return this->entered_id == other.entered_id;
}

ostream& operator<< (ostream& out, const SecurityLog& other) {
    out << "Timestamp: " << other.timestamp << endl;
    out << "Role: " << other.role << endl;
    out << "User ID: " << other.entered_id << endl;
    out << "Result: " << other.result << endl;
    return out;
}