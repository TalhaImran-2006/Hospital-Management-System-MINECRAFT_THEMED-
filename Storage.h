#pragma once
#include <iostream>
using namespace std;

template <class T>
class Storage {
private:
    T data[100];
    int count;

public:
    Storage() : count(0) {}

    T* findByID(int id) {
        for (int i = 0; i < count; i++) {
            if (data[i].getID() == id) {
                return &data[i];
            }
        }
        return nullptr;
    }

    const T* findByID(int id) const {
        for (int i = 0; i < count; i++) {
            if (data[i].getID() == id) {
                return &data[i];
            }
        }
        return nullptr;
    }

    bool removeByID(int id) {
        for (int i = 0; i < count; ++i) {
            if (data[i].getID() == id) {
                for (int j = i; j < count - 1; ++j) {
                    data[j] = data[j + 1];
                }
                --count;
                return true;
            }
        }
        return false;
    }

    int getCount() const {
        return count;
    }

    const T& getAt(int index) const {
        return data[index];
    }

    void setAt(int index, const T& newItem) {
        if (index >= 0 && index < count) {
            data[index] = newItem;
        }
    }

    void add(const T& item) {
        if (count < 100) {
            data[count++] = item;
        }
    }
    T* getAll() {
        return data;
    }

    const T* getAll() const {
        return data;
    }
};