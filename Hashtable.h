//
// Created by edenl on 07/06/2022.
//

#ifndef DS_HW2_HASHTABLE_H
#define DS_HW2_HASHTABLE_H

#include "Employee.h"

#define MAX_MINIMUM 457

class Hashtable {
public:
    int magicNumber;
    tree<Employee> **table;
    int counter;

    Hashtable() : magicNumber(MAX_MINIMUM), counter(0) {
        table = new tree<Employee> *[magicNumber];
        for (int i = 0; i < magicNumber; i++) {
            table[i] = nullptr;
        }
    }

    ~Hashtable() {
        for (int i = 0; i < magicNumber; i++) {
            table[i] = clearAll(table[i]);
        }
        delete[] table;
    }

    StatusType insert(Employee *e) {
        if (e == nullptr || e->id <= 0) {
            return INVALID_INPUT;
        }
        int i = hash(e->id);
        StatusType status = FAILURE;
        if (table[i] == nullptr) { // table is empty
            table[i] = new tree<Employee>(e->id, e);
            counter++;
            return SUCCESS;
        }
        tree<Employee> *temp = addElement(table[i], e, e->id, false, &status);
        if (status != SUCCESS) {
            return status;
        }
        table[i] = temp;
        counter++;
        if (counter >= magicNumber) {
            magicNumber *= 2;
            rehash();
        }
        return SUCCESS;
    }

    StatusType remove(int id) {
        int i = hash(id);
        tree<Employee> *e = findById(table[i], id);
        if (e == nullptr) {
            return INVALID_INPUT;
        }
        StatusType status;
        tree<Employee> *temp = eraseElement(table[i], e->element, false, true, &status);
        if (status != SUCCESS) {
            return status;
        }
        table[i] = temp;
        counter--;
        if (counter <= magicNumber / 2 && magicNumber / 2 >= MAX_MINIMUM) {
            magicNumber /= 2;
            rehash();
        }
        return SUCCESS;
    }

    int hash(int id) {
        return (id % magicNumber);
    }

    void rehash() {
        tree<Employee> **temp = this->table;
        this->table = new tree<Employee> *[this->magicNumber];
        for (int i = 0; i < (this->magicNumber) / 2; i++) {
            inOrderCopy(temp[i]);
        }
    }

    void inOrderCopy(tree<Employee> *oldTree) {
        if (oldTree == nullptr) return;
        inOrderCopy(oldTree->left);
        inOrderCopy(oldTree->right);
        insert(oldTree->element);
    }

};

#endif //DS_HW2_HASHTABLE_H
