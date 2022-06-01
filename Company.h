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
    Employee* employee_head;
    tree<Employee> *employees_pointers_by_salary;
    Company* parent_company;

    Company(int value, int company_id);

    ~Company() {

        employees_pointers_by_salary = nullptr;
    };

};

#endif //DS_HW1_COMPANY_H
