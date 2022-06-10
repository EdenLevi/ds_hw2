//
// Created by edenl on 31/05/2022.
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library2.h"
#include "Company.h"
#include "Hashtable.h"
#include "rankTree.h"
#include <iostream>


class DataStructure {
public:
    DataStructure(int k) : k(k) {
        companyArray = new Company *[k + 1];
        for (int i = 0; i <= k; i++) {
            companyArray[i]->value = i;
        }
        employees = new Hashtable();
        salaries = nullptr;
    }

    ~DataStructure() {};

    Company **companyArray;
    Hashtable *employees;
    rankTree *salaries;
    int k;
};

void *Init(int k) {
    try {
        if (k <= 0) return nullptr;
        DataStructure *DS = new DataStructure(k);
        return DS;
    }
    catch (std::bad_alloc const &) {
        return nullptr;
    }
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade) {
    DataStructure *DSS = (DataStructure *) DS;
    if (DS == nullptr || employeeID <= 0 || companyID <= 0 || companyID > DSS->k || grade < 0) {
        return INVALID_INPUT;
    }
    if ((DSS->employees->findMeInHach(employeeID)) != nullptr) {
        return FAILURE;
    }
    try {
        Company* c = DSS->companyArray[companyID];
        while(c->parent_company!=nullptr) {
            c = c->parent_company;
        }
        companyID = c->id;

        Employee *e = new Employee(employeeID, DSS->companyArray[companyID], 0, grade);
        StatusType status = DSS->employees->insert(e);
        if (status == SUCCESS) {
            DSS->companyArray[companyID]->employee_count++;
        }
        return status;
    }
    catch (std::bad_alloc const &) {
        return ALLOCATION_ERROR;
    }
}

StatusType RemoveEmployee(void *DS, int employeeID) {
    DataStructure *DSS = (DataStructure *) DS;
    try {
        if(DS == nullptr || employeeID <= 0) {
            return INVALID_INPUT;
        }
        tree<Employee>* empNode = DSS->employees->findMeInHach(employeeID);
        if(empNode == nullptr) {
            return FAILURE;
        }
        Company* c = empNode->element->company;
        while(c->parent_company!=nullptr) {
            c = c->parent_company;
        }

        //remove from salary tree
        if(empNode->element->salary > 0) {
            //DSS->salaries
            //// added RTDeleteElement and RTDeleteElementRecursively
            //// need to make sure they were added properly and use them here
            //// for salary tree and company salary tree (employee removal)
        }

        StatusType status = DSS->employees->remove(employeeID);
        if(status == SUCCESS) {
            DSS->companyArray[c->id]->employee_count--;
        }
        return status;
    }
    catch (std::bad_alloc const &) {
        return ALLOCATION_ERROR;
    }
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor);

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease) {

}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade);

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m, void *sumBumpGrade);

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary,
                                                void *averageBumpGrade);

StatusType CompanyValue(void *DS, int companyID, void *standing);

/*
 * 20 point Bonus function:
StatusType BumpGradeToEmployees(void *DS, int lowerSalary, int higherSalary, int bumpGrade);
 */

void Quit(void **DS);
