#pragma once
#include <iostream>
using namespace std;

class SecurityLog {
    char timestamp[32];
    char role[15];
    int entered_id;
    char result[10];
public:
    SecurityLog(const char* ts = "", const char* r = "",
        int id = 0, const char* res = "");
    ~SecurityLog();
    SecurityLog(const SecurityLog& other);
    SecurityLog& operator=(const SecurityLog& other);

    int getID() const;
    const char* getTimestamp() const;
    const char* getRole() const;
    const char* getResult() const;

    void setTimestamp(const char* ts);
    void setRole(const char* r);
    void setID(int id);
    void setResult(const char* res);

    bool operator==(const SecurityLog& other) const;
    friend ostream& operator<<(ostream& out, const SecurityLog& other);
};