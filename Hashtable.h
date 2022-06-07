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
    tree<Employee>** table;
    int counter;
    Hashtable() : magicNumber(MAX_MINIMUM) {
        table = new tree<Employee>*[magicNumber];
    }

    ~Hashtable() {
        for(int i = 0; i < magicNumber; i++) {
            table[i] = clearAll(table[i]);
        }
        delete[] table;
    }

    void insert(Employee* e) {
        int i = hash(e->id);
        StatusType status;
        table[i] = addElement(table[i], e, e->id, false, &status);
        counter++;
        if (counter >= magicNumber) {
            magicNumber *= 2;
            rehash();
        }
    }
    void remove(int id) {
        int i = hash(id);
        tree<Employee>* e = findById(table[i], id);
        StatusType status;
        table[i] = eraseElement(table[i], e->element,false,true,&status);
        counter--;
        if (counter <= magicNumber/2 && magicNumber/2 >= MAX_MINIMUM) {
            magicNumber /= 2;
            rehash();
        }
    }

    int hash(int id) {
        return (id % magicNumber);
    }

    void rehash() {
        tree<Employee>** temp = this->table;
        this->table = new tree<Employee>*[this->magicNumber];
        for(int i = 0; i < (this->magicNumber)/2; i++) {
            inOrderCopy(temp[i]);
        }
    }

    void inOrderCopy(tree<Employee>* oldTree) {
        if(oldTree == nullptr) return;
        inOrderCopy(oldTree->left);
        inOrderCopy(oldTree->right);
        insert(oldTree->element);
    }

};

#endif //DS_HW2_HASHTABLE_H
