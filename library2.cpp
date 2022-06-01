//
// Created by edenl on 31/05/2022.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library2.h"
#include <iostream>

void *Init(int k) {
    try {
        int *companyArray = new int[k];
        for (int i = 0; i < k; i++) {
            companyArray[i] = i;
        }
        return companyArray;
    }
    catch (std::bad_alloc const&) {
        return nullptr;
    }
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade);

StatusType RemoveEmployee(void *DS, int employeeID);

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor);

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease);

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade);

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m, void * sumBumpGrade);

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary, void * averageBumpGrade);

StatusType CompanyValue(void *DS, int companyID, void * standing);

/*
 * 20 point Bonus function:
StatusType BumpGradeToEmployees(void *DS, int lowerSalary, int higherSalary, int bumpGrade);
 */

void Quit(void** DS);
