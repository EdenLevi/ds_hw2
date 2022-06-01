//
// Created by omerg on 23/04/2022.
//

#ifndef DS_HW1_EMPLOYEE_H
#define DS_HW1_EMPLOYEE_H

#include "Company.h"


class Company;

class Employee {
public:
    int id;
    Company* company; //only update when head employee
    int salary;
    int grade;

    Employee(int id, Company *company, int salary, int grade);

    ~Employee() {
        company = nullptr;
    };
};


#endif //DS_HW1_EMPLOYEE_H
