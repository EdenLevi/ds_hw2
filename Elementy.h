//
// Created by edenl on 01/06/2022.
//

#ifndef DS_HW2_ELEMENTY_H
#define DS_HW2_ELEMENTY_H

#include "Company.h"


class Company;

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


#endif //DS_HW2_ELEMENTY_H
