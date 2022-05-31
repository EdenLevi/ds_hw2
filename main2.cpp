/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Spring 2022                                          */
/* Homework : Wet 2                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main2.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library2.h"
#include <iostream>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

/* The command's strings */
typedef enum {
    NONE_CMD = -2,
    COMMENT_CMD = -1,
    INIT_CMD = 0,
    ADD_EMPLOYEE_CMD = 1,
    REMOVE_EMPLOYEE_CMD = 2,
    ACQUIRE_COMPANY_CMD = 3,
    EMPLOYEE_SALARY_INCREASE_CMD = 4,
    PROMOTE_EMPLOYEE_CMD = 5,
    SUM_OF_BUMP_GRADE_BETWEEN_TOP_WORKERS_BY_GROUP_CMD = 6,
    AVERAGE_BUMP_GRADE_BETWEEN_SALARY_BY_GROUP_CMD = 7,
    COMPANY_VALUE_CMD = 8,
    BUMP_GRADE_TO_EMPLOYEES_CMD = 9,
    QUIT_CMD = 10
} commandType;

static const int numActions = 11;
static const char *commandStr[] = {
        "Init",
        "AddEmployee",
        "RemoveEmployee",
        "AcquireCompany",
        "EmployeeSalaryIncrease",
        "PromoteEmployee",
        "SumOfBumpGradeBetweenTopWorkersByGroup ",
        "AverageBumpGradeBetweenSalaryByGroup ",
        "CompanyValue",
        "BumpGradeToEmployees",
        "Quit" };

static const char* ReturnValToStr(int val) {
    switch (val) {
        case SUCCESS:
            return "SUCCESS";
        case ALLOCATION_ERROR:
            return "ALLOCATION_ERROR";
        case FAILURE:
            return "FAILURE";
        case INVALID_INPUT:
            return "INVALID_INPUT";
        default:
            return "";
    }
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
    error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {
    char buffer[MAX_STRING_INPUT_SIZE];

    // Reading commands
    while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
        fflush(stdout);
        if (parser(buffer) == error)
            break;
    };
    return 0;
}

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
                                const char** const command_arg) {
    if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
        return (NONE_CMD);
    if (StrCmp("#", command)) {
        if (strlen(command) > 1)
            printf("%s", command);
        return (COMMENT_CMD);
    };
    for (int index = 0; index < numActions; index++) {
        if (StrCmp(commandStr[index], command)) {
            *command_arg = command + strlen(commandStr[index]) + 1;
            return ((commandType) index);
        };
    };
    return (NONE_CMD);
}

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddEmployee(void* DS, const char* const command);
static errorType OnRemoveEmployee(void* DS, const char* const command);
static errorType OnAcquireCompany(void* DS, const char* const command);
static errorType OnEmployeeSalaryIncrease(void* DS, const char* const command);
static errorType OnPromoteEmployee(void* DS, const char* const command);
static errorType OnSumOfBumpGradeBetweenTopWorkersByGroup(void* DS, const char* const command);
static errorType OnAverageBumpGradeBetweenSalaryByGroup(void* DS, const char* const command);
static errorType OnCompanyValue(void* DS, const char* const command);
static errorType OnBumpGradeToEmployees(void* DS, const char* const command);
static errorType OnQuit(void** DS, const char* const command);

/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/

static errorType parser(const char* const command) {
    static void *DS = NULL; /* The general data structure */
    const char* command_args = NULL;
    errorType rtn_val = error;

    commandType command_val = CheckCommand(command, &command_args);

    switch (command_val) {

        case (INIT_CMD):
            rtn_val = OnInit(&DS, command_args);
            break;
        case (ADD_EMPLOYEE_CMD):
            rtn_val = OnAddEmployee(DS, command_args);
            break;
        case (REMOVE_EMPLOYEE_CMD):
            rtn_val = OnRemoveEmployee(DS, command_args);
            break;
        case (ACQUIRE_COMPANY_CMD):
            rtn_val = OnAcquireCompany(DS, command_args);
            break;
        case (EMPLOYEE_SALARY_INCREASE_CMD):
            rtn_val = OnEmployeeSalaryIncrease(DS, command_args);
            break;
        case (PROMOTE_EMPLOYEE_CMD):
            rtn_val = OnPromoteEmployee(DS, command_args);
            break;
        case (SUM_OF_BUMP_GRADE_BETWEEN_TOP_WORKERS_BY_GROUP_CMD):
            rtn_val = OnSumOfBumpGradeBetweenTopWorkersByGroup(DS, command_args);
            break;
        case (AVERAGE_BUMP_GRADE_BETWEEN_SALARY_BY_GROUP_CMD):
            rtn_val = OnAverageBumpGradeBetweenSalaryByGroup(DS, command_args);
            break;
        case (COMPANY_VALUE_CMD):
            rtn_val = OnCompanyValue(DS, command_args);
            break;
        case (BUMP_GRADE_TO_EMPLOYEES_CMD):
            rtn_val = OnBumpGradeToEmployees(DS, command_args);
            break;
        case (QUIT_CMD):
            rtn_val = OnQuit(&DS, command_args);
            break;
        case (COMMENT_CMD):
            rtn_val = error_free;
            break;
        case (NONE_CMD):
            rtn_val = error;
            break;
        default:
            assert(false);
            break;
    };
    return (rtn_val);
}

/***************************************************************************/
/* OnInit                                                                  */
/***************************************************************************/
static errorType OnInit(void** DS, const char* const command) {
    if (isInit) {
        printf("Init was already called.\n");
        return (error_free);
    };
    isInit = true;
    int k;
    ValidateRead(sscanf(command, "%d", &k), 1, "Init failed.\n");
    *DS = Init(k);
    if (*DS == NULL) {
        printf("Init failed.\n");
        return error;
    };
    printf("Init done.\n");

    return error_free;
}

/***************************************************************************/
/* OnAddEmployee                                                             */
/***************************************************************************/
static errorType OnAddEmployee(void* DS, const char* const command) {
    int employeeID;
    int companyID;
    int grade;
    ValidateRead(sscanf(command, "%d %d %d", &employeeID, &companyID, &grade), 3,
                 "AddEmployee failed.\n");
    StatusType res = AddEmployee(DS, employeeID, companyID, grade);

    if (res != SUCCESS) {
        printf("AddEmployee: %s\n", ReturnValToStr(res));
        return error_free;
    } else {
        printf("AddEmployee: %s\n", ReturnValToStr(res));
    }

    return error_free;
}

/***************************************************************************/
/* OnRemoveEmployee                                                          */
/***************************************************************************/
static errorType OnRemoveEmployee(void* DS, const char* const command) {
    int employeeID;
    ValidateRead(
            sscanf(command, "%d", &employeeID),1,
            "RemoveEmployee failed.\n");
    StatusType res = RemoveEmployee(DS, employeeID);

    if (res != SUCCESS) {
        printf("RemoveEmployee: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("RemoveEmployee: %s\n", ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnAcquireCompany                                                            */
/***************************************************************************/
static errorType OnAcquireCompany(void* DS, const char* const command) {
    int companyID1;
    int companyID2;
    double factor;
    ValidateRead(sscanf(command, "%d %d %lf", &companyID1, &companyID2, &factor), 3,
                 "AcquireCompany failed.\n");
    StatusType res = AcquireCompany(DS, companyID1, companyID2, factor);
    if (res != SUCCESS) {
        printf("AcquireCompany: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("AcquireCompany: %s\n", ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnEmployeeSalaryIncrease                                                         */
/***************************************************************************/
static errorType OnEmployeeSalaryIncrease(void* DS, const char* const command) {
    int employeeID;
    int salaryIncrease;
    ValidateRead(sscanf(command, "%d %d", &employeeID, &salaryIncrease), 2,
                 "EmployeeSalaryIncrease failed.\n");
    StatusType res = EmployeeSalaryIncrease(DS, employeeID, salaryIncrease);

    if (res != SUCCESS) {
        printf("EmployeeSalaryIncrease: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("EmployeeSalaryIncrease: %s\n", ReturnValToStr(res));
    return error_free;
}


/***************************************************************************/
/* OnPromoteEmployee                                                         */
/***************************************************************************/
static errorType OnPromoteEmployee(void* DS, const char* const command) {
    int emplyeeID;
    int bumpGrade;
    ValidateRead(sscanf(command, "%d %d", &emplyeeID, &bumpGrade), 2, "PromoteEmployee failed.\n");
    StatusType res = PromoteEmployee(DS, emplyeeID, bumpGrade);

    if (res != SUCCESS) {
        printf("PromoteEmployee: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("PromoteEmployee: %s\n", ReturnValToStr(res));
    return error_free;
}

/***************************************************************************/
/* OnSumOfBumpGradeBetweenTopWorkersByGroup                                                        */
/***************************************************************************/

static errorType OnSumOfBumpGradeBetweenTopWorkersByGroup (void* DS, const char* const command) {
    int companyID;
    int m;
    ValidateRead(sscanf(command, "%d %d", &companyID, &m), 2,
                 "SumOfBumpGradeBetweenTopWorkersByGroup failed.\n");
    void* sumBumpGrade;
    StatusType res = SumOfBumpGradeBetweenTopWorkersByGroup(DS, companyID, m, &sumBumpGrade);

    if (res != SUCCESS) {
        printf("SumOfBumpGradeBetweenTopWorkersByGroup: %s\n", ReturnValToStr(res));
        return error_free;
    }

    /*
     * Implement here the printing of the variable
     */
    return error_free;
}

/***************************************************************************/
/* OnAverageBumpGradeBetweenSalaryByGroup                                                        */
/***************************************************************************/

static errorType OnAverageBumpGradeBetweenSalaryByGroup (void* DS, const char* const command) {
    int companyID;
    int lowerSalary;
    int higherSalary;
    ValidateRead(sscanf(command, "%d %d %d", &companyID, &lowerSalary, &higherSalary), 3,
                 "AverageBumpGradeBetweenSalaryByGroup failed.\n");
    void* averageBumpGrade;
    StatusType res = AverageBumpGradeBetweenSalaryByGroup(DS, companyID, lowerSalary, higherSalary, &averageBumpGrade);

    if (res != SUCCESS) {
        printf("AverageBumpGradeBetweenSalaryByGroup: %s\n", ReturnValToStr(res));
        return error_free;
    }

    /*
     * Implement here the printing of the variable
     */
    return error_free;
}

/***************************************************************************/
/* OnCompanyValue                                                        */
/***************************************************************************/

static errorType OnCompanyValue(void* DS, const char* const command) {
    int companyID;
    ValidateRead(sscanf(command, "%d", &companyID), 1,
                 "CompanyValue failed.\n");
    void* standing;
    StatusType res = CompanyValue(DS, companyID, &standing);

    if (res != SUCCESS) {
        printf("CompanyValue: %s\n", ReturnValToStr(res));
        return error_free;
    }

    /*
     * Implement here the printing of the variable
     */

    return error_free;
}

/***************************************************************************/
/* OnBumpGradeToEmployees                                                        */
/***************************************************************************/
/*
 * 20 points Bonus function:
static errorType OnBumpGradeToEmployees(void* DS, const char* const command) {
    int lowerSalary;
    int higherSalary;
    int bumpGrade;
    ValidateRead(sscanf(command, "%d %d %d", &lowerSalary, &higherSalary, &bumpGrade), 3,
                 "BumpGradeToEmployees failed.\n");
    StatusType res = BumpGradeToEmployees(DS, lowerSalary, higherSalary, bumpGrade);

    if (res != SUCCESS) {
        printf("BumpGradeToEmployees: %s\n", ReturnValToStr(res));
        return error_free;
    }

    printf("BumpGradeToEmployees: %s\n", ReturnValToStr(res));

    return error_free;
}
*/

/***************************************************************************/
/* OnQuit                                                                  */
/***************************************************************************/
static errorType OnQuit(void** DS, const char* const command) {
    Quit(DS);
    if (*DS != NULL) {
        printf("Quit failed.\n");
        return error;
    };

    isInit = false;
    printf("Quit done.\n");

    return error_free;
}

#ifdef __cplusplus
}
#endif