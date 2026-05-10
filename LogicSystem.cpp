#include "LogicSystem.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int LogicSystem::tries = 0;

LogicSystem::LogicSystem(int id, const char* pass) : id(id) {
    int len = str_len(pass);
    password = new char[len + 1];
    for (int i = 0; i < len; i++) {
        password[i] = pass[i];
    }
    password[len] = '\0';
}

LogicSystem::~LogicSystem() {
    delete[] password;
}

bool LogicSystem::isExist(const char* fileName, const char* role) {
    ifstream file(fileName);
    if (!file.is_open()) return false;

    char line[1000];
    while (file.getline(line, 1000)) {
        int i = 0;
        int currentID = FileHandler::parseInt(line, i);
        char dummy[100];
        char passFromFile[50];

        if (currentID == this->id) {
            FileHandler::parseStr(line, i, dummy, 100);
            if (str_len(role) == 7) {
                FileHandler::parseInt(line, i);
                FileHandler::parseStr(line, i, dummy, 100);
                FileHandler::parseStr(line, i, dummy, 100);
            }
            else if (str_len(role) == 6) {
                FileHandler::parseStr(line, i, dummy, 100);
                FileHandler::parseStr(line, i, dummy, 100);
            }

            FileHandler::parseStr(line, i, passFromFile, 50);

            bool match = true;
            int j = 0;
            while (this->password[j] != '\0' || passFromFile[j] != '\0') {
                if (this->password[j] != passFromFile[j]) {
                    match = false;
                    break;
                }
                j++;
            }

            file.close();
            if (match) {
                tries = 0;
                return true;
            }
            else {
                tries++;
                return false;
            }
        }
    }
    file.close();
    tries++;
    return false;
}

bool LogicSystem::canCheckAgain() {
    return (tries < 3);
}

void LogicSystem::failedAttempt() {
    SecurityLog s;
    s.setID(id);
    s.setRole("User");
    s.setResult("FAILED");
    char buffer[80];
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &localTime);

    s.setTimestamp(buffer);
    FileHandler::addSecurityLog(s);
}

int LogicSystem::str_len(const char* arr) {
    int len = 0;
    while (arr && arr[len] != '\0') len++;
    return len;
}

char LogicSystem::to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (c + 32) : c;
}

bool LogicSystem::compareInsensitive(const char* c1, const char* c2) {
    if (str_len(c1) != str_len(c2)) return false;
    for (int i = 0; c1[i] != '\0'; i++) {
        if (to_lower(c1[i]) != to_lower(c2[i])) return false;
    }
    return true;
}

void LogicSystem::sortPrescriptions() {
    Storage<Prescription> tempStorage;
    FileHandler::loadPrescription(tempStorage);

    int n = tempStorage.getCount();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            const char* d1 = tempStorage.getAt(j).getDate();
            const char* d2 = tempStorage.getAt(j + 1).getDate();

            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int mon1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int yr1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 + (d1[8] - '0') * 10 + (d1[9] - '0');

            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');
            int mon2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int yr2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 + (d2[8] - '0') * 10 + (d2[9] - '0');

            bool swapNeeded = false;
            if (yr1 > yr2) swapNeeded = true;
            else if (yr1 == yr2 && mon1 > mon2) swapNeeded = true;
            else if (yr1 == yr2 && mon1 == mon2 && day1 > day2) swapNeeded = true;

            if (swapNeeded) {
                Prescription temp = tempStorage.getAt(j);
                tempStorage.setAt(j, tempStorage.getAt(j + 1));
                tempStorage.setAt(j + 1, temp);
            }
        }
    }
}

void LogicSystem::sortAppointments() {
    Storage<Appointment> tempStorage;
    FileHandler::loadAppointments(tempStorage);

    int n = tempStorage.getCount();
    if (n <= 1) return;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            const char* d1 = tempStorage.getAt(j).getDate();
            const char* d2 = tempStorage.getAt(j + 1).getDate();

            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int mon1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int yr1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 + (d1[8] - '0') * 10 + (d1[9] - '0');

            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');
            int mon2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int yr2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 + (d2[8] - '0') * 10 + (d2[9] - '0');

            bool swapNeeded = false;
            if (yr1 > yr2) swapNeeded = true;
            else if (yr1 == yr2 && mon1 > mon2) swapNeeded = true;
            else if (yr1 == yr2 && mon1 == mon2 && day1 > day2) swapNeeded = true;

            if (swapNeeded) {
                Appointment temp = tempStorage.getAt(j);
                tempStorage.setAt(j, tempStorage.getAt(j + 1));
                tempStorage.setAt(j + 1, temp);
            }
        }
    }
    FileHandler::saveAppointments(tempStorage);
}

void LogicSystem::sortBills() {
    Storage<Bill> tempStorage;
    FileHandler::loadBills(tempStorage);

    int n = tempStorage.getCount();
    if (n <= 1) return;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            const char* d1 = tempStorage.getAt(j).getDate();
            const char* d2 = tempStorage.getAt(j + 1).getDate();

            int day1 = (d1[0] - '0') * 10 + (d1[1] - '0');
            int mon1 = (d1[3] - '0') * 10 + (d1[4] - '0');
            int yr1 = (d1[6] - '0') * 1000 + (d1[7] - '0') * 100 + (d1[8] - '0') * 10 + (d1[9] - '0');

            int day2 = (d2[0] - '0') * 10 + (d2[1] - '0');
            int mon2 = (d2[3] - '0') * 10 + (d2[4] - '0');
            int yr2 = (d2[6] - '0') * 1000 + (d2[7] - '0') * 100 + (d2[8] - '0') * 10 + (d2[9] - '0');

            bool swapNeeded = false;
            if (yr1 > yr2) swapNeeded = true;
            else if (yr1 == yr2 && mon1 > mon2) swapNeeded = true;
            else if (yr1 == yr2 && mon1 == mon2 && day1 > day2) swapNeeded = true;

            if (swapNeeded) {
                Bill temp = tempStorage.getAt(j);
                tempStorage.setAt(j, tempStorage.getAt(j + 1));
                tempStorage.setAt(j + 1, temp);
            }
        }
    }
    FileHandler::saveBills(tempStorage);
}

void LogicSystem::sortSecurityLogs() {
    Storage<SecurityLog> tempStorage;
    FileHandler::loadSecurityLogs(tempStorage);

    int n = tempStorage.getCount();
    if (n <= 1) return;

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            const char* t1 = tempStorage.getAt(j).getTimestamp();
            const char* t2 = tempStorage.getAt(j + 1).getTimestamp();

            int yr1 = (t1[6] - '0') * 1000 + (t1[7] - '0') * 100 + (t1[8] - '0') * 10 + (t1[9] - '0');
            int yr2 = (t2[6] - '0') * 1000 + (t2[7] - '0') * 100 + (t2[8] - '0') * 10 + (t2[9] - '0');

            int mon1 = (t1[3] - '0') * 10 + (t1[4] - '0');
            int mon2 = (t2[3] - '0') * 10 + (t2[4] - '0');

            int day1 = (t1[0] - '0') * 10 + (t1[1] - '0');
            int day2 = (t2[0] - '0') * 10 + (t2[1] - '0');

            int hr1 = (t1[11] - '0') * 10 + (t1[12] - '0');
            int hr2 = (t2[11] - '0') * 10 + (t2[12] - '0');

            int min1 = (t1[14] - '0') * 10 + (t1[15] - '0');
            int min2 = (t2[14] - '0') * 10 + (t2[15] - '0');

            int sec1 = (t1[17] - '0') * 10 + (t1[18] - '0');
            int sec2 = (t2[17] - '0') * 10 + (t2[18] - '0');

            bool swapNeeded = false;
            if (yr1 < yr2) swapNeeded = true;
            else if (yr1 == yr2) {
                if (mon1 < mon2) swapNeeded = true;
                else if (mon1 == mon2) {
                    if (day1 < day2) swapNeeded = true;
                    else if (day1 == day2) {
                        if (hr1 < hr2) swapNeeded = true;
                        else if (hr1 == hr2) {
                            if (min1 < min2) swapNeeded = true;
                            else if (min1 == min2) {
                                if (sec1 < sec2) swapNeeded = true;
                            }
                        }
                    }
                }
            }

            if (swapNeeded) {
                SecurityLog temp = tempStorage.getAt(j);
                tempStorage.setAt(j, tempStorage.getAt(j + 1));
                tempStorage.setAt(j + 1, temp);
            }
        }
    }
    FileHandler::saveSecurityLogs(tempStorage);
}