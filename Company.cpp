//
// Created by omerg on 23/04/2022.
//

#include "Company.h"

Company::Company(int value, int company_id) : value(value), id(company_id), employee_count(0), salary(0), employee_head(
        nullptr), employees_pointers_by_salary(nullptr), parent_company(nullptr) {};

