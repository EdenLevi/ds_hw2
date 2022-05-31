/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 2                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library2.h                                                   */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET2
#define _234218_WET2

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;


void *Init(int k);

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

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET2 */