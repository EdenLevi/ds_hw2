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
    Company *company;
    int salary;
    int grade;
    Employee(int id,Company *company, int salary, int grade);

    ~Employee(){
            company = nullptr;
    };
};


/*Employee* findEmployeeById(Employee* head_employee ,int EmployeeID){
    if(head_employee==nullptr){
        return nullptr;
    }
    if(head_employee->employee_id==EmployeeID){
        return head_employee;
    }
    else if(head_employee->employee_id>EmployeeID && head_employee->right!=nullptr) {
        return findEmployeeById(head_employee->right.get(), EmployeeID);
    }
    else if(head_employee->employee_id<EmployeeID && head_employee->left!=nullptr) {
        return findEmployeeById(head_employee->left.get(), EmployeeID);
    }
    return nullptr;
}

Employee* findMyEmployeeDaddy(Employee* head_employee ,int EmployeeID) {
    if(head_employee == nullptr) { // no head, employee is the new head
        return nullptr;
    }
    else {
        if(head_employee->employee_id > EmployeeID) {
            if(head_employee->left == nullptr) {
                return head_employee;
            }
            return findMyEmployeeDaddy(head_employee->left.get(), EmployeeID);
        }
        else {
            if(head_employee->right == nullptr) {
                return head_employee;
            }
            return findMyEmployeeDaddy(head_employee->right.get(), EmployeeID);
        }
    }
}*/

#endif //DS_HW1_EMPLOYEE_H
