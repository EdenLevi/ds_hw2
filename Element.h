//
// Created by edenl on 01/06/2022.
//

#ifndef DS_HW2_ELEMENT_H
#define DS_HW2_ELEMENT_H

#include "Company.h"


class Company;

class Element {
public:
    Employee* employee;
    int id;
    int salary;
    int boys;
    int l_boys;
    int grades_sum_l_boys;

    Element(int id, int salary, Employee* employee, int l_boys = 0, int boys = 0, int grades_sum_l_boys = 0);

    ~Element() {
        employee = nullptr;
    };
};


#endif //DS_HW2_ELEMENT_H
