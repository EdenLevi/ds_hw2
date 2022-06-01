//
// Created by omerg on 23/04/2022.
//

#include <memory>
#include "../../Documents/Studying/Technion/Courses/D/MIVNEI/wet1/ds_hw1/library1.h"
#include "Company.h"
#include "Employee.h"
#include "tree.h"

class DataStrcture {
public:
    DataStrcture() = default;

    ~DataStrcture() {};

    int employee_count;
    tree<Company> *company_head;
    tree<Company> *companies_with_employees;
    tree<Employee> *employee_head;
    Employee *highest_earner_employee;
    tree<Employee> *employees_pointers_by_salary;
};


/**
 * O(1)
 * @return
 */
void *Init() {

    try {
        DataStrcture *DS = new DataStrcture();
        DS->highest_earner_employee = nullptr;
        DS->company_head = nullptr;
        DS->companies_with_employees = nullptr;
        DS->employee_head = nullptr;
        return (void *) DS;

    }
    catch (std::bad_alloc &) {
        return nullptr;
    }
}

/**
 * the func add a company to DB
 * O(log(K)) - k num of companies
 * @param DS
 * @param CompanyID
 * @param Value
 * @return
 */
StatusType AddCompany(void *DS, int CompanyID, int Value) {
    Company *c;
    try {
        DataStrcture *DSS = ((DataStrcture *) DS);
        if (DS == nullptr || CompanyID <= 0 || Value <= 0) {
            return INVALID_INPUT;
        }
        tree<Company> *cc = findById(((DataStrcture *) DS)->company_head, CompanyID);
        if (cc != nullptr) return FAILURE; // employee already in tree

        c = new Company(Value, CompanyID);
        if (((DataStrcture *) DS)->company_head == nullptr) {
            ((DataStrcture *) DS)->company_head = new tree<Company>(CompanyID, c);
        } else {
            StatusType status = FAILURE;
            tree<Company> *temp = addElement(DSS->company_head, c, c->id, false, &status); //valgrind segfault cause
            if (status != SUCCESS) {
                delete c;
                return status;
            }
            if (((DataStrcture *) DS)->company_head->id != temp->id) {
                ((DataStrcture *) DS)->company_head = temp; //valgrind segfault cause
                //((DataStrcture *) DS)->company_head.get() = temp;
            }
        }

        return SUCCESS;
    }
    catch (std::bad_alloc &) {
        delete c;
        return ALLOCATION_ERROR;
    }
}

/**
 * O(log(n)) - n num of employees
 * @param c
 * @return
 */
Employee *UpdateHighestEarner(tree<Employee> *c) {
    if (c == nullptr)
        return nullptr;

    while (c->right) {
        c = c->right;
    }
    return c->element;
}

/**
 * the func add an employee to DS (use when have Company pointer)
 * O(log(n)) - n num pf employees
 * @param DS
 * @param EmployeeID
 * @param c
 * @param Salary
 * @param Grade
 * @return
 */
StatusType AddEmployee_C(void *DS, int EmployeeID, Company *c, int Salary, int Grade) {

    if ((DS == nullptr) || (EmployeeID <= 0) || (c->id <= 0) || (Salary <= 0) || (Grade < 0)) {
        return INVALID_INPUT;
    }

    DataStrcture *DSS = (DataStrcture *) DS;

    if (((DataStrcture *) DS)->company_head == nullptr) return FAILURE;
    if (c == nullptr) return FAILURE; // company doesn't exist, nowhere to add employee
    tree<Employee> *ee = findById(((DataStrcture *) DS)->employee_head, EmployeeID);
    if (ee != nullptr) return FAILURE; // employee already in tree

    try {

        Employee *e = new Employee(EmployeeID, c, Salary, Grade);
        if (((DataStrcture *) DS)->employee_head == nullptr) {
            ((DataStrcture *) DS)->employee_head = new tree<Employee>(EmployeeID, e);
        } else {
            StatusType status = FAILURE;
            tree<Employee> *temp = addElement(DSS->employee_head, e, EmployeeID, false, &status);
            if (status != SUCCESS) {
                delete e;
                return status;
            }
            ((DataStrcture *) DS)->employee_head = temp;
        }

        /*** add the employee's pointer to the pointers_employees tree: **/
        /* if employees pointers is empty, initialize one */
        tree<Employee> *t = findById(((DataStrcture *) DS)->employee_head, EmployeeID);

        if (t == nullptr) {
         //   std::cout << "t is null for some stupid reason (EmployeeID: " << EmployeeID << ")" << std::endl;
            delete e;
            return FAILURE;
        }

        if (c->employees_pointers == nullptr) {
            tree<Employee> *ba = new tree<Employee>(EmployeeID, t->element);
            c->employees_pointers = ba;

            /// adding company to the list of companies with employees
            if (((DataStrcture *) DS)->companies_with_employees == nullptr) {
                ((DataStrcture *) DS)->companies_with_employees = new tree<Company>(c->id, c);
            } else {
                StatusType status = FAILURE;
                tree<Company> *temp = addElement(DSS->companies_with_employees, c, c->id, false, &status);
                if (status != SUCCESS) {
                    delete c;
                    return status;
                }
                if (((DataStrcture *) DS)->companies_with_employees->id != temp->id) {
                    ((DataStrcture *) DS)->companies_with_employees = temp;
                }
            }

        } else {
            StatusType status = FAILURE;
            tree<Employee> *temp = addElement(c->employees_pointers, t->element, EmployeeID, false, &status);
            if (status != SUCCESS) return status;
            c->employees_pointers = temp;
        }

        /*** add the employee's pointer by SALARY: **/
        if (c->employees_pointers_by_salary == nullptr) {
            tree<Employee> *ba2 = new tree<Employee>(Salary, t->element);
            c->employees_pointers_by_salary = ba2;
        } else {
            StatusType status = FAILURE;
            tree<Employee> *temp = addElement(c->employees_pointers_by_salary, t->element, Salary, true,
                                              &status);
            if (status != SUCCESS) {
                return status;
            }
            c->employees_pointers_by_salary = temp;
        }

        /*** add the employee's pointer to the general employee by SALARY tree ***/
        if (((DataStrcture *) DS)->employees_pointers_by_salary == nullptr) {
            tree<Employee> *ba3 = new tree<Employee>(Salary, t->element);
            ((DataStrcture *) DS)->employees_pointers_by_salary = ba3;
        } else {
            StatusType status = FAILURE;
            tree<Employee> *temp = addElement(((DataStrcture *) DS)->employees_pointers_by_salary, t->element, Salary,
                                              true, &status);
            if (status != SUCCESS) return status;
            ((DataStrcture *) DS)->employees_pointers_by_salary = temp;
        }
        ((DataStrcture *) DS)->highest_earner_employee = UpdateHighestEarner(
                ((DataStrcture *) DS)->employees_pointers_by_salary);
        c->highest_earner_employee = UpdateHighestEarner(c->employees_pointers_by_salary);

        (((DataStrcture *) DS)->employee_count)++;
        (c->employee_count)++; // raise the company's employee count by 1

        return SUCCESS;
    }
    catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
        /// remember to delete previous steps if failed mid-way
    }
}

/**
 * the func add an employee to DS (use when dint have Company pointer)
 * O(log(n)+log(k)) - n num of employees,k num of companies
 * @param DS
 * @param EmployeeID
 * @param CompanyID
 * @param Salary
 * @param Grade
 * @return
 */
StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade) {
    if ((DS == nullptr) || (EmployeeID <= 0) || (CompanyID <= 0) || (Salary <= 0) || (Grade < 0)) {
        return INVALID_INPUT;
    }

    DataStrcture *DSS = (DataStrcture *) DS;

    if (DSS->company_head == nullptr) return FAILURE;
    tree<Company> *c = findById(DSS->company_head, CompanyID);
    if (c == nullptr) return FAILURE; // company doesn't exist, nowhere to add employee

    return AddEmployee_C(DS,EmployeeID,c->element,Salary,Grade);

}

/**
 *
 * the func add an company to DS
 * O(log(k)) - k num of companies
 * @param DS
 * @param CompanyID
 * @return
 */
StatusType RemoveCompany(void *DS, int CompanyID) {
    try {
        if (CompanyID <= 0 || DS == nullptr) {
            return INVALID_INPUT;
        }
        tree<Company> *c = findById(((DataStrcture *) DS)->company_head, CompanyID);
        if (c == nullptr || c->element->employees_pointers != nullptr)
            return FAILURE; // company doesn't exist, nowhere to add employee

        StatusType status = FAILURE;
        tree<Company> *temp = eraseElement( ((DataStrcture *) DS)->company_head,c->element, false, true, &status);
        if (status != SUCCESS) {
            return status;
        }
        ((DataStrcture *) DS)->company_head = temp;

        return SUCCESS;
    }
    catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
        /// remember to delete previous steps if failed mid-way
    }

}

/**
 *
 * the func remove an employee from DS
 * O(log(n)) - n num pf employees
 * @param DS
 * @param EmployeeID
 * @return
 */
StatusType RemoveEmployee(void *DS, int EmployeeID) {
    try {
        if (EmployeeID <= 0 || DS == nullptr) {
            return INVALID_INPUT;
        }

        tree<Employee> *e = findById(((DataStrcture *) DS)->employee_head, EmployeeID);
        if (e == nullptr || e->element == nullptr) return FAILURE; // company doesn't exist, nowhere to add employee
        Employee *ele = e->element;
        Company *c = e->element->company;
        StatusType status = FAILURE;
        //delete emp pntr by salary DS
        tree<Employee> *temp = eraseElement(((DataStrcture *) DS)->employees_pointers_by_salary,ele
                                            , true, false,&status);
        if (status != SUCCESS) {
            return status;
        }
        ((DataStrcture *) DS)->employees_pointers_by_salary = temp;

        //delete emp pntr Company
        temp = eraseElement(c->employees_pointers, ele, false, false, &status);

        if (status != SUCCESS) {
            return status;
        }
        c->employees_pointers = temp;

        //delete emp pntr by salary Company
        temp = eraseElement(c->employees_pointers_by_salary, ele, true, false, &status);

        if (status != SUCCESS) {
            return status;
        }
        c->employees_pointers_by_salary = temp;

        //delete emp pntr DS
        temp = eraseElement(((DataStrcture *) DS)->employee_head,ele, false, false, &status);
        if (status != SUCCESS) {
            return status;
        }
        ((DataStrcture *) DS)->employee_head = temp;

        //update counters
        c->employee_count--;
        ((DataStrcture *) DS)->employee_count--;

        //update highest earner
        ((DataStrcture *) DS)->highest_earner_employee = UpdateHighestEarner(
                ((DataStrcture *) DS)->employees_pointers_by_salary);
        c->highest_earner_employee = UpdateHighestEarner(c->employees_pointers_by_salary);

        /// if we removed the last employee, the company needs to be removed from the list of companies with employees
        if(c->employee_count == 0) {
            tree<Company> *c2 = findById(((DataStrcture *) DS)->companies_with_employees, c->id);
            if (c2 == nullptr)
                return FAILURE; // company doesn't exist

            StatusType status = FAILURE;
            tree<Company> *temp = eraseElement( ((DataStrcture *) DS)->companies_with_employees,c2->element, false, false, &status);
            if (status != SUCCESS) {
                return status;
            }
            ((DataStrcture *) DS)->companies_with_employees = temp;
        }

        delete ele;
        return SUCCESS;
    }

    catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
        /// remember to delete previous steps if failed mid-way
    }

}

/**
 * the func return company info through Values and NumEmployees
 * o(log(k)) - k num of companies
 * @param DS
 * @param CompanyID
 * @param Value
 * @param NumEmployees
 * @return
 */
StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees) {
    if ((DS == nullptr) || (CompanyID <= 0) || (Value == nullptr) || (NumEmployees == nullptr)) {
        return INVALID_INPUT;
    }
    tree<Company> *c = findById(((DataStrcture *) DS)->company_head, CompanyID);
    if (c == nullptr) return FAILURE; // company doesn't exist

    *Value = c->element->value;
    *NumEmployees = c->element->employee_count;

    return SUCCESS;
}

/**
 * the func return employee info
 * o(log(n)) - n num of employees
 * @param DS
 * @param EmployeeID
 * @param EmployerID
 * @param Salary
 * @param Grade
 * @return
 */
StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade) {
    if ((DS == nullptr) || (EmployeeID <= 0) || (Salary == nullptr) || (Grade == nullptr)) {
        return INVALID_INPUT;
    }
    tree<Employee> *e = findById(((DataStrcture *) DS)->employee_head, EmployeeID);
    if (e == nullptr) return FAILURE; // employee doesn't exist

    *EmployerID = e->element->company->id;
    *Salary = e->element->salary;
    *Grade = e->element->grade;

    return SUCCESS;
}

/**
 * the func increase company value
 * O(log(k)) - k num of companies
 * @param DS
 * @param CompanyID
 * @param ValueIncrease
 * @return
 */
StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease) {
    if ((DS == nullptr) || (CompanyID <= 0) || (ValueIncrease <= 0)) {
        return INVALID_INPUT;
    }
    tree<Company> *c = findById(((DataStrcture *) DS)->company_head, CompanyID);
    if (c == nullptr) return FAILURE; // company doesn't exist

    c->element->value += ValueIncrease;
    return SUCCESS;
}



/**
 * the func promote an employee (salary and grade(pot))
 * O(log(n)) - n num of employees
 * @param DS
 * @param EmployeeID
 * @param SalaryIncrease
 * @param BumpGrade
 * @return
 */
StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade) {
    if ((DS == nullptr) || (EmployeeID <= 0) || (SalaryIncrease <= 0)) {
        return INVALID_INPUT;
    }
    tree<Employee> *e = findById(((DataStrcture *) DS)->employee_head, EmployeeID);
    if (e == nullptr) return FAILURE; // employee doesn't exist

    int temp_salary = e->element->salary + SalaryIncrease;
    int temp_grade = e->element->grade;
    if (BumpGrade > 0) {
        temp_grade++;
    }

    Company *c = e->element->company;
    RemoveEmployee(DS, EmployeeID);
    AddEmployee_C(DS, EmployeeID, c, temp_salary, temp_grade);

    return SUCCESS;
}

/**
 * the func hire an employee (remove him from an old company and add him to a new)
 * o(log(n) + log(k)) - n num of employees, k num of companies
 * @param DS
 * @param EmployeeID
 * @param NewCompanyID
 * @return
 */
StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID) {
    if ((DS == nullptr) || (EmployeeID <= 0) || (NewCompanyID <= 0)) {
        return INVALID_INPUT;
    }

    tree<Employee> *e = findById(((DataStrcture *) DS)->employee_head, EmployeeID);
    tree<Company> *c = findById(((DataStrcture *) DS)->company_head, NewCompanyID);
    if (c == nullptr || e == nullptr || e->element->company->id == NewCompanyID) return FAILURE;

    // delete employee from previous company's pointers tree:
    StatusType status;

    try {
        Employee *new_emp = new Employee(e->element->id, c->element, e->element->salary, e->element->grade);
        status = RemoveEmployee(DS, e->id);
        if (status != SUCCESS) {
            delete new_emp;
            return status;
        }
        status = AddEmployee(DS, new_emp->id, c->id, new_emp->salary, new_emp->grade);
        delete new_emp;

        return status;

    }

    catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }
}

/**
 * the func get an array and convert it to a tree
 * o(n) - n num of employees (base on preorder)
 * @param array
 * @param begin
 * @param end
 * @param isSalary
 * @return
 */
tree<Employee> *arrayToTree(Employee **array, int begin, int end, bool isSalary) {
    if (begin > end) {
        return nullptr;
    }
    int mid = (begin + end) / 2;
    int id=array[mid]->id;
    if(isSalary)
        id=array[mid]->salary;
    tree<Employee> *head = new tree<Employee>(id, array[mid]);
    head->left = arrayToTree(array, begin, mid - 1,isSalary);
    head->right = arrayToTree(array, mid + 1, end,isSalary);

    int a = getHeight(head->left);
    int b = getHeight(head->right);
    head->height = (a > b ? a : b) + 1;

    return head;
}

/**
 * the func get 2 trees and combine them to a new one
 * O(n1+n2) - ni num of employee at i (based on merge sort)
 */
tree<Employee> *
CombineTree(Company *comp, tree<Employee> *head1, tree<Employee> *head2, int size1, int size2, StatusType *status,
            bool isSalary) {


    Employee **tree1 = new Employee *[size1];
    Employee **tree2 = new Employee *[size2];
    int index = 0;
    treeToArray<Employee>(head1, tree1, &index);
    int index2 = 0;
    treeToArray<Employee>(head2, tree2, &index2);

    clear(head1);
    clear(head2);
    index = 0;
    index2 = 0;

    Employee **merged = new Employee *[size1 + size2];
    int i = 0, j = 0, k = 0;
    while ((i < size1) && (j < size2)) {
        if (!isSalary) {
            if (tree1[i]->id > tree2[j]->id) {
                merged[k] = tree2[j];
                merged[k]->company = comp;
                j++;
            } else {
                merged[k] = tree1[i];
                merged[k]->company = comp;
                i++;
            }
        } else {
            //   std::cout << "k: " << k << std::endl;

            // std::cout << "tree1[0]->salary: " << ((Employee*)tree1[0])->salary << std::endl;
            //    std::cout << "tree2[0]->salary: " << ((Employee*)tree2[0])->salary  << std::endl;
            //   std::cout << "tree1[0]->id: " << ((Employee*)tree1[0])->id  << std::endl;
            //     std::cout << "tree2[0]->id: " << ((Employee*)tree2[0])->id << std::endl;
            if (tree1[i]->salary > tree2[j]->salary) { // problem
                merged[k] = tree2[j];
                merged[k]->company = comp;
                j++;
            } else if (tree1[i]->salary < tree2[j]->salary) {
                merged[k] = tree1[i];
                merged[k]->company = comp;
                i++;
            } else { // equal
                if (tree1[i]->id < tree2[j]->id) {
                    merged[k] = tree2[j];
                    merged[k]->company = comp;
                    j++;
                } else {
                    merged[k] = tree1[i];
                    merged[k]->company = comp;
                    i++;
                }
            }
        }
        k++;
    }

    while (j < size2) {
        merged[k] = tree2[j];
        merged[k]->company = comp;
        k++;
        j++;
    }
    while (i < size1) {
        merged[k] = tree1[i];
        merged[k]->company = comp;
        k++;
        i++;
    }

    tree<Employee> *new_head = arrayToTree(merged, 0, size2 + size1 - 1,isSalary);

    delete[] tree1;

    delete[] tree2;

    delete[] merged;

    return new_head;

}


/**
 * Acquire company
 * O(log(K)+nA+nB) - k num of companies, ni num of employee at i
 * @param DS
 * @param AcquirerID
 * @param TargetID
 * @param Factor
 * @return
 */
StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor) {
    try {
        if (DS == nullptr || AcquirerID <= 0 || TargetID <= 0 || AcquirerID == TargetID || Factor < 1.00) {
            return INVALID_INPUT;
        }
         tree<Company> *Acquirer = findById(((DataStrcture *) DS)->company_head, AcquirerID);
        tree<Company> *Target = findById(((DataStrcture *) DS)->company_head, TargetID);
        if (Acquirer == nullptr || Target == nullptr || Acquirer->element->value < (10 * Target->element->value)) {
            return FAILURE;
        }
        Company *Acquirer_c = Acquirer->element;
        Company *Target_c = Target->element;


        // add acquisition here
        int temp_val = (int) ((Acquirer->element->value + Target->element->value) * Factor);
        Employee *temp_highest_earner = Acquirer->element->highest_earner_employee;

        if (Target->element->employees_pointers != nullptr) {
            if (Acquirer->element->highest_earner_employee != nullptr) {
                if (Target->element->highest_earner_employee->salary >=
                    Acquirer->element->highest_earner_employee->salary) {
                    if (Target->element->highest_earner_employee->salary ==
                        Acquirer->element->highest_earner_employee->salary) {
                        if (Target->element->highest_earner_employee->id <
                            Acquirer->element->highest_earner_employee->id) {
                            temp_highest_earner = Target->element->highest_earner_employee;
                        }
                    } else {
                        temp_highest_earner = Target->element->highest_earner_employee;
                    }
                }
            } else
                temp_highest_earner = Target->element->highest_earner_employee;

            int temp_emp_count = Acquirer->element->employee_count + Target->element->employee_count;

            StatusType status = FAILURE; // not really used
            tree<Employee> *merged_emp_ptr = CombineTree(Acquirer->element, Acquirer->element->employees_pointers,
                                                         Target->element->employees_pointers,
                                                         Acquirer->element->employee_count,
                                                         Target->element->employee_count, &status, false);

            Target_c->employees_pointers = nullptr;
            Acquirer_c->employees_pointers = merged_emp_ptr;

            tree<Employee> *merged_emp_salary = CombineTree
                    (Acquirer_c, Acquirer_c->employees_pointers_by_salary, Target_c->employees_pointers_by_salary,
                     Acquirer_c->employee_count, Target_c->employee_count, &status, true);

            Target_c->employees_pointers_by_salary = nullptr;
            Acquirer_c->employees_pointers_by_salary = merged_emp_salary;
            Acquirer_c->highest_earner_employee = temp_highest_earner;
            Acquirer_c->employee_count = temp_emp_count;
            ((DataStrcture *) DS)->company_head = eraseElement(((DataStrcture *) DS)->company_head, Target_c, false, false, &status);

            /// remove both companies from companies with employees
            ((DataStrcture *) DS)->companies_with_employees = eraseElement(((DataStrcture *) DS)->companies_with_employees, Target_c, false, false, &status);
            ((DataStrcture *) DS)->companies_with_employees = eraseElement(((DataStrcture *) DS)->companies_with_employees, Acquirer_c, false, false, &status);
            /// add merged company to companies with employees
            if (((DataStrcture *) DS)->companies_with_employees == nullptr) {
                ((DataStrcture *) DS)->companies_with_employees = new tree<Company>(AcquirerID, Acquirer_c);
            } else {
                status = FAILURE;
                tree<Company> *temp = addElement(((DataStrcture *) DS)->companies_with_employees, Acquirer_c, AcquirerID, false, &status); //valgrind segfault cause
                if (((DataStrcture *) DS)->companies_with_employees->id != temp->id) {
                    ((DataStrcture *) DS)->companies_with_employees = temp;
                }
            }

            delete Target_c;

        } else {
            StatusType status = RemoveCompany(DS, TargetID);
            if (status != SUCCESS) return status;

        }
        Acquirer = findById(((DataStrcture *) DS)->company_head, AcquirerID);
        Acquirer->element->value = temp_val;

        return SUCCESS;
    }
    catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

/**
 * the func return highest earner employee from DS or Company
 * O(log(k))- k, num of companies
 * @param DS
 * @param CompanyID
 * @param EmployeeID
 * @return
 */
StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID) {
    if ((DS == nullptr) || (EmployeeID == nullptr) || (CompanyID == 0)) {
        return INVALID_INPUT;
    }

    tree<Company> *c; // used to avoid calling it when not necessary:
    if (CompanyID > 0) c = findById(((DataStrcture *) DS)->company_head, CompanyID);

    if ((CompanyID < 0 && ((DataStrcture *) DS)->employee_head == nullptr) ||
        (CompanyID > 0 && c == nullptr) ||
        (CompanyID > 0 && c->element->employees_pointers == nullptr)) {
        return FAILURE;
    }

    if (CompanyID < 0) {
        *EmployeeID = ((DataStrcture *) DS)->highest_earner_employee->id;
    } else {
        *EmployeeID = c->element->highest_earner_employee->id;
    }

    return SUCCESS;
}

/**
 * GetAllEmployeesBySalary helper func
 * O(log(n)) n num of employees
 * @param e
 * @param Employees
 * @param NumOfEmployees
 * @return
 */
StatusType GetAllEmployeesBySalary_helper(tree<Employee> *e, int **Employees, int *NumOfEmployees) {
    //if(*counter >= NumOfCompanies) return SUCCESS;
    if (e == nullptr)
        return SUCCESS;

    StatusType result = GetAllEmployeesBySalary_helper(e->right, Employees, NumOfEmployees);
    if (result != SUCCESS) return result;

    (*(Employees))[*NumOfEmployees] = e->element->id;
    (*NumOfEmployees)++;

    result = GetAllEmployeesBySalary_helper(e->left, Employees, NumOfEmployees);
    if (result != SUCCESS) return result;

    return SUCCESS;
}

/**
 * O(log(k)+n) - k, num of companies , n num of employees given
 * @param DS
 * @param CompanyID
 * @param Employees
 * @param NumOfEmployees
 * @return
 */
StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees) {
    try {
        if ((DS == nullptr) || (Employees == nullptr) || (NumOfEmployees == nullptr) || (CompanyID == 0)) {
            return INVALID_INPUT;
        }
        *NumOfEmployees = 0;

        tree<Company> *c; // used to avoid calling it when not necessary:
        if (CompanyID > 0) c = findById(((DataStrcture *) DS)->company_head, CompanyID);
        if (((CompanyID < 0) && (((DataStrcture *) DS)->employee_head == nullptr)) ||
            ((CompanyID > 0) && (c == nullptr)) || ((CompanyID > 0) && (c->element->employees_pointers == nullptr))) {
            return FAILURE;
        }

        if (CompanyID > 0) {
            *NumOfEmployees = c->element->employee_count;
        } else { // we check if 0 earlier
            *NumOfEmployees = ((DataStrcture *) DS)->employee_count;
        }

        *Employees = (int *) malloc(sizeof(int) * (*NumOfEmployees)); // this is supposed to be deleted by 'them'

        int index = 0;
        if (CompanyID < 0) {
            // get an ordered list of employees by salary from all employees
            StatusType result = GetAllEmployeesBySalary_helper(((DataStrcture *) DS)->employees_pointers_by_salary,
                                                               Employees, &index);
            if (result != SUCCESS) return result;
        } else {
            // get an ordered list of employees by salary from all employees in the company
            StatusType result = GetAllEmployeesBySalary_helper(c->element->employees_pointers_by_salary, Employees,
                                                               &index);
            if (result != SUCCESS) return result;
        }
        *NumOfEmployees = index;
        if (index + 1 != *NumOfEmployees) {
            //std::cout << "index: " << index + 1 << " NumOfEmployees: " << *NumOfEmployees << std::endl;
        }
    }
    catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

/**
 *
 * @param DS
 * @param NumOfCompanies
 * @param c
 * @param companies
 * @param counter
 * @return
 */
StatusType GetHighestEarnerInEachCompany_helper(void *DS, int NumOfCompanies, tree<Company> *c, Company **companies,
                                                int *counter) {
    if (c == nullptr) return SUCCESS;

    StatusType result = GetHighestEarnerInEachCompany_helper(DS, NumOfCompanies, c->left, companies, counter);

    if (result != SUCCESS) return result;

    if (*counter < NumOfCompanies) {
        // add c
        if (c->element->employee_count != 0) {
            companies[*counter] = c->element; // make sure each company has at least one employee
            (*counter)++;
        }
    } else
        return SUCCESS;

    result = GetHighestEarnerInEachCompany_helper(DS, NumOfCompanies, c->right, companies, counter);

    if (result != SUCCESS) return result;

    return SUCCESS; //check which status is required here
}

/**
 *
 * @param DS
 * @param NumOfCompanies
 * @param Employees
 * @return
 */
StatusType GetHighestEarnerInEachCompany(void *DS, int NumOfCompanies, int **Employees) {
    try {
        if ((DS == nullptr) || (Employees == nullptr) || NumOfCompanies < 1) return INVALID_INPUT;
        *Employees = (int *) malloc(sizeof(int) * (NumOfCompanies)); // this is supposed to be deleted by 'them'

        Company **companies = new Company *[NumOfCompanies];
        int counter = 0;

        tree<Company> *c = ((DataStrcture *) DS)->companies_with_employees;
        StatusType result = GetHighestEarnerInEachCompany_helper(DS, NumOfCompanies, c, companies, &counter);
        if (result != SUCCESS) {
            delete[] companies;
            free(*Employees);
            return result;
        }
        if (counter != NumOfCompanies) {
            delete[] companies;
            free(*Employees);
            return FAILURE;
        }

        for (int i = 0; i < NumOfCompanies; i++) {
            (*(Employees))[i] = companies[i]->highest_earner_employee->id;
        }

        delete[] companies;
    }
    catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}
/**
 *
 * @param e
 * @param MinEmployeeID
 * @param MaxEmployeeId
 * @param TotalNumOfEmployees
 * @param NumOfEmployees
 * @param MinSalary
 * @param MinGrade
 * @return
 */
StatusType GetNumEmployeesMatching_helper(tree<Employee> *e, int MinEmployeeID, int MaxEmployeeId, int *TotalNumOfEmployees,
                               int *NumOfEmployees, int MinSalary, int MinGrade) {
    if (e == nullptr ) {
        return SUCCESS;
    }
    StatusType result=SUCCESS;

    if (e->id >= MinEmployeeID)
         result = GetNumEmployeesMatching_helper(e->left, MinEmployeeID, MaxEmployeeId, TotalNumOfEmployees,
                                                       NumOfEmployees, MinSalary, MinGrade);

    if (result != SUCCESS) return result;


    // add e
    if (e->id >= MinEmployeeID && e->id <= MaxEmployeeId) {
        (*TotalNumOfEmployees)++;
        if (e->element->salary >= MinSalary && e->element->grade >= MinGrade) {
            (*NumOfEmployees)++;
        }
    }
    if ( e->id <= MaxEmployeeId)
        result = GetNumEmployeesMatching_helper(e->right, MinEmployeeID, MaxEmployeeId, TotalNumOfEmployees,
                                                NumOfEmployees,MinSalary, MinGrade);

    if (result != SUCCESS) return result;

    return SUCCESS; //check which status is required here
}
/**
 *
 * @param DS
 * @param CompanyID
 * @param MinEmployeeID
 * @param MaxEmployeeId
 * @param MinSalary
 * @param MinGrade
 * @param TotalNumOfEmployees
 * @param NumOfEmployees
 * @return
 */
StatusType GetNumEmployeesMatching(void *DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId,
                                   int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees) {
    if (DS == nullptr || TotalNumOfEmployees == nullptr || NumOfEmployees == nullptr || CompanyID == 0 ||
        MinEmployeeID < 0 || MaxEmployeeId < 0 || MinSalary < 0 || MinGrade < 0 || MinEmployeeID > MaxEmployeeId) {
        return INVALID_INPUT;
    }

    *NumOfEmployees = 0;
    *TotalNumOfEmployees = 0;

    tree<Company> *c; // used to avoid calling it when not necessary:
    if (CompanyID > 0) c = findById(((DataStrcture *) DS)->company_head, CompanyID);

    if ((CompanyID < 0 && ((DataStrcture *) DS)->employee_head == nullptr) ||
        (CompanyID > 0 && c == nullptr) ||
        (CompanyID > 0 && c->element->employees_pointers == nullptr)) {
        return FAILURE;
    }

    if (CompanyID < 0) {
        StatusType result = GetNumEmployeesMatching_helper(((DataStrcture *) DS)->employee_head, MinEmployeeID,
                                                           MaxEmployeeId, TotalNumOfEmployees, NumOfEmployees,
                                                           MinSalary, MinGrade);
        if (result != SUCCESS) return result;
    } else {
        StatusType result = GetNumEmployeesMatching_helper(c->element->employees_pointers, MinEmployeeID, MaxEmployeeId,
                                                           TotalNumOfEmployees, NumOfEmployees, MinSalary, MinGrade);
        if (result != SUCCESS) return result;
    }
    return SUCCESS;
}


/**
 * Quit helper
 * O(n+k) - n num of employees , k num of companies
 * @param DS
 * @param curr
 */
void Quit_Helper(void *DS, tree<Company> *&curr) {
    if (curr == nullptr) return;

    Quit_Helper(DS, curr->left);
    curr->left = nullptr;
    Quit_Helper(DS, curr->right);

    clear(curr->element->employees_pointers_by_salary);
    curr->element->employees_pointers_by_salary = nullptr;
    clear(curr->element->employees_pointers);
    curr->element->employees_pointers = nullptr;
    delete curr->element;
    curr->element = nullptr;
    delete curr;
}
/**
 * the func clear the DS
 * O(n+k) - n num of employees , k num of companies
 * @param DS
 */
void Quit(void **DS) {
    DataStrcture *DSS = ((DataStrcture *) *DS);
    tree<Company> *&c = DSS->company_head;
    clear(DSS->companies_with_employees);
    Quit_Helper(DSS, c);
    DSS->company_head = nullptr;
    clear(DSS->employees_pointers_by_salary);
    DSS->employees_pointers_by_salary = nullptr;
    clearAll(DSS->employee_head);
    DSS->employee_head = nullptr;
    DSS->companies_with_employees = nullptr;
    DSS->highest_earner_employee = nullptr;
    delete DSS;
    *DS = nullptr;
}