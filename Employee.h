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


#endif //DS_HW1_EMPLOYEE_H
