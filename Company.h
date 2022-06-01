//
// Created by omerg on 23/04/2022.
//
#ifndef DS_HW1_COMPANY_H
#define DS_HW1_COMPANY_H

#include <memory>
#include "Employee.h"
#include "tree.h"

using std::shared_ptr;

class Employee;

class Company {

public:
    int value;
    int id;
    int employee_count;
    int salary; //garbage value
    Employee *highest_earner_employee;
    tree<Employee>* employees_pointers;
    tree<Employee>* employees_pointers_by_salary;
    Company(int value, int company_id);
    ~Company() {
            highest_earner_employee=nullptr;
            employees_pointers=nullptr;
            employees_pointers_by_salary=nullptr;
    };

};

#endif //DS_HW1_COMPANY_H
