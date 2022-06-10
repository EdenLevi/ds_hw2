//
// Created by edenl on 10/06/2022.
//

#ifndef DS_HW2_CLASSES_H
#define DS_HW2_CLASSES_H

#include "tree.h"

class Elementy;

class Company {

public:
    int value;
    int id;
    int employee_count;
    int salary; //garbage value
    tree<Elementy> *employees_pointers_by_salary;
    Company* parent_company;

    Company(int value, int company_id);

    ~Company() {

        employees_pointers_by_salary = nullptr;
    };

};

class Employee {
public:
    int id;
    int salary;
    int grade;

    Employee(int id, Company *company, int salary, int grade);

    ~Employee() {
        company = nullptr;
    };
    Company* getCompany(){
        Company* cmp= nullptr;
        while(company->parent_company){
            cmp=company->parent_company;
        }
        return cmp;
    }
private:
    Company* company; //only update when head employee
};

class Elementy {
public:
    Employee* employee;
    int id;
    int salary;
    int boys;
    int l_boys;
    int grades_sum_l_boys;

    Elementy(int id, int salary, Employee* employee, int l_boys = 0, int boys = 0, int grades_sum_l_boys = 0);

    ~Elementy() {
        employee = nullptr;
    };
};

#endif //DS_HW2_CLASSES_H
