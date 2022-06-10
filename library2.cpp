//
// Created by eden levi on 31/05/2022.
//
#include "library2.h"
#include "classes.h"
#include "Hashtable.h"
#include "rankTree.h"
#include "tree.h"


void *Init(int k) {
    try {
        if (k <= 0) return nullptr;
        auto *DS = new DataStructure(k);
        return DS;
    }
    catch (std::bad_alloc const &) {
        return nullptr;
    }
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade) {
    auto *DSS = (DataStructure *) DS;
    if (DS == nullptr || employeeID <= 0 || companyID <= 0 || companyID > DSS->k || grade < 0) {
        return INVALID_INPUT;
    }
    if ((DSS->employees->findMeInHach(employeeID)) != nullptr) {
        return FAILURE;
    }
    try {
        Company *c = DSS->companyArray[companyID];
        while (c->parent_company != nullptr) {
            c = c->parent_company;
        }
        companyID = c->id;

        auto *e = new Employee(employeeID, DSS->companyArray[companyID], 0, grade);
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
    auto *DSS = (DataStructure *) DS;
    try {
        if (DS == nullptr || employeeID <= 0) {
            return INVALID_INPUT;
        }
        tree<Employee> *empNode = DSS->employees->findMeInHach(employeeID);
        if (empNode == nullptr) {
            return FAILURE;
        }
        Company *c = empNode->element->getCompany();
        while (c->parent_company != nullptr) {
            c = c->parent_company;
        }

        //remove from salary tree
        if (empNode->element->salary > 0) {
            //DSS->salaries
            //// added RTDeleteElement and RTDeleteElementRecursively
            //// need to make sure they were added properly and use them here
            //// for salary tree and company salary tree (employee removal)
        }

        StatusType status = DSS->employees->remove(employeeID);
        if (status == SUCCESS) {
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
    auto *DSS = (DataStructure *) DS;
    try {
        /// input check & validating employee exists
        if (DS == nullptr || employeeID <= 0 || salaryIncrease <= 0) {
            return INVALID_INPUT;
        }
        tree<Employee> *empNode = DSS->employees->findMeInHach(employeeID);
        if (empNode == nullptr) {
            return FAILURE;
        }

        /// updating employee salary
        empNode->element->salary += salaryIncrease;

        /// adding employee to both salary trees
        if (((empNode->element->salary) - salaryIncrease) == 0) {
            Company* c = empNode->element->getCompany();
            tree<Elementy> *etz = c->employees_pointers_by_salary;

            StatusType status = SUCCESS;
            c->employees_pointers_by_salary = RtreeAddElement(etz, empNode->element, &status);
            if (status != SUCCESS) {
                empNode->element->salary += salaryIncrease;
                return status;
            }

            DSS->salaries = RtreeAddElement(DSS->salaries, empNode->element, &status);
            if (status != SUCCESS) {
                c->employees_pointers_by_salary = RtreeDeleteElement(etz, empNode->element, &status);
                empNode->element->salary -= salaryIncrease;
                return status;
            }
        }

        /// updating employee salary
        empNode->element->salary += salaryIncrease;
        return SUCCESS;
    }
    catch (std::bad_alloc const &) {
        return ALLOCATION_ERROR;
    }
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade) {
    if (DS == nullptr || employeeID <= 0)
        return INVALID_INPUT;

    auto *DSS = (DataStructure *) DS;
    tree<Employee> *emp = DSS->employees->findMeInHach(employeeID); // employee* in hashtable
    if (emp == nullptr)
        return FAILURE;

    Company *cmp = emp->element->getCompany();
    StatusType status = FAILURE;
    tree<Elementy> *temp = RtreeDeleteElement(cmp->employees_pointers_by_salary, emp->element, &status);
    if (status != SUCCESS)
        return status;
    cmp->employees_pointers_by_salary = temp;
    temp = RtreeDeleteElement(DSS->salaries, emp->element, &status);
    if (status != SUCCESS)
        return status;
    cmp->employees_pointers_by_salary = temp;
    //update grade
    emp->element->grade += bumpGrade;

    //insert employee to DSS and company





    return SUCCESS;

}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m, void *sumBumpGrade);

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary,
                                                void *averageBumpGrade);

StatusType CompanyValue(void *DS, int companyID, void *standing);

/*
 * 20 point Bonus function:
StatusType BumpGradeToEmployees(void *DS, int lowerSalary, int higherSalary, int bumpGrade);
 */

void Quit(void **DS);
