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

/// if i'm in right half, the left half is smaller than me and not under me
/// right half -> remove left half from my grades_sum
/// left half -> do nothing
/// need to figure out how to do this for all those that are in the right half without iterating them

/**
 * the func get an array and convert it to a tree
 * o(n) - n num of employees (base on preorder)
 * @param array
 * @param begin
 * @param end
 * @param isSalary
 * @return
 */
tree<Elementy> *arrayToTree(Elementy **array, int begin, int end, bool isSalary,int* sums) {
    if (begin > end) {
        return nullptr;
    }
    int mid = (begin + end) / 2;
    int id = array[mid]->id;
    if (isSalary)
        id = array[mid]->salary;
    tree<Elementy> *head = new tree<Elementy>(id, array[mid]);
    head->left = arrayToTree(array, begin, mid - 1, isSalary,sums);
    head->right = arrayToTree(array, mid + 1, end, isSalary,sums);
    if(head->left!= nullptr) {
        head->element->l_boys = head->left->element->boys + 1;
    }
    else
        head->element->l_boys=0;
    head->element->boys=end-begin;
    head->element->grades_sum_l_boys=sums[end]-sums[begin];

    int a = getHeight(head->left);
    int b = getHeight(head->right);
    head->height = (a > b ? a : b) + 1;

    return head;
}


/// O(log(n))
int getTotalSum( tree<Elementy> *head){
    int tot_sum=0;
    if(head!= nullptr) {
        tot_sum = head->element->grades_sum_l_boys + head->element->employee->grade;
        while (head->right) {
            tot_sum += head->right->element->grades_sum_l_boys + head->right->element->employee->grade;
            head = head->right;
        }
    }
    return tot_sum;
}


/// O(log(n))
int getRank(tree<Elementy> *head,int id){
    int rank=0;
    while(head){
        if(head->id==id){
            return rank+head->element->l_boys;
        } else if(head->id<id){
            rank+=head->element->l_boys+1;
            head=head->right;
        }
        else
            head=head->left;
    }
    return -1;
}


/**
 * the func get 2 trees and combine them to a new one
 * O(n1+n2) - ni num of employee at i (based on merge sort)
 */
/** need to support RANK TREES ** DO NOT FORGET** **/
tree<Elementy> *
CombineTree(Company *comp, tree<Elementy> *head1, tree<Elementy> *head2, int size1, int size2, StatusType *status) {


    Elementy **tree1 = new Elementy *[size1];
    Elementy **tree2 = new Elementy *[size2];
    int tot_sum= getTotalSum(head1)+ getTotalSum(head2);
    int index = 0;
    treeToArray<Elementy>(head1, tree1, &index);
    int index2 = 0;
    treeToArray<Elementy>(head2, tree2, &index2);

    clear(head1);
    clear(head2);
    index = 0;
    index2 = 0;

    Elementy **merged = new Elementy *[size1 + size2];
    int i = 0, j = 0, k = 0;
    while ((i < size1) && (j < size2)) {
            if (tree1[i]->salary > tree2[j]->salary) {
                merged[k] = tree2[j];
                j++;
            } else if (tree1[i]->salary < tree2[j]->salary) {
                merged[k] = tree1[i];
                i++;
            } else { // equal
                if (tree1[i]->id < tree2[j]->id) {
                    merged[k] = tree2[j];
                    j++;
                } else {
                    merged[k] = tree1[i];
                    i++;
                }
            }
            merged[k]->employee->setCompany(comp);
            k++;
    }

    while (j < size2) {
        merged[k] = tree2[j];
        merged[k]->employee->setCompany(comp);
        k++;
        j++;
    }
    while (i < size1) {
        merged[k] = tree1[i];
        merged[k]->employee->setCompany(comp);
        k++;
        i++;
    }

    int* sums=new int[size2 + size1];
    int sum=0;
    for(int i=0;i<(size2+size1);i++){
        sums[i]=sum;
        sum+=merged[i]->employee->grade;
    }

    tree<Elementy> *new_head = arrayToTree(merged, 0, size2 + size1 - 1, true,sums);

    delete[] tree1;

    delete[] tree2;

    delete[] merged;

    delete[] sums;

    return new_head;

}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor) {
    StatusType status = SUCCESS;
    DataStructure *DSS = (DataStructure *) DS;
    if (DS == nullptr || companyID1 <= 0 || companyID2 <= 0 || companyID2 > DSS->k || companyID1 > DSS->k ||
        companyID1 == companyID2 || factor <= 0)
        return INVALID_INPUT;
    Company *target = DSS->companyArray[companyID1]->getCompany();
    Company *acquirer = DSS->companyArray[companyID2]->getCompany();
    //update company value
    acquirer->value += ((target->value) * factor);
    target->value -= acquirer->value;
    //merge salary tree
    tree<Elementy>* temp = CombineTree(acquirer, target->employees_pointers_by_salary,
                                                         acquirer->employees_pointers_by_salary, target->employee_count,
                                                         acquirer->employee_count, &status);
    if (status != SUCCESS)
        return status;
    acquirer->employees_pointers_by_salary=temp;
    target->employees_pointers_by_salary= nullptr;
    //update parent company of target
    acquirer->employee_count += target->employee_count;
    target->parent_company = acquirer;
    return SUCCESS;

}

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
            Company *c = empNode->element->getCompany();
            tree<Elementy> *etz = c->employees_pointers_by_salary;

            StatusType status = SUCCESS;
            c->employees_pointers_by_salary = RtreeAddElement(etz, empNode->element, &status);
            if (status != SUCCESS) {
                empNode->element->salary -= salaryIncrease;
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
    DSS->salaries = temp;

    //update grade
    emp->element->grade += bumpGrade;

    //insert employee to DSS and company
    temp = RtreeAddElement(cmp->employees_pointers_by_salary, emp->element, &status);
    if (status != SUCCESS)
        return status;

    cmp->employees_pointers_by_salary = temp;
    temp = RtreeAddElement(DSS->salaries, emp->element, &status);
    if (status != SUCCESS)
        return status;
    DSS->salaries = temp;

    return SUCCESS;
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m, void *sumBumpGrade) {

}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary,
                                                void *averageBumpGrade);


/// NOTE: company value needs to be updated from int to long double to support large numbers & fractions **
StatusType CompanyValue(void *DS, int companyID, void *standing) {
    try {
        auto *DSS = (DataStructure *) DS;
        if (DS == nullptr || standing == nullptr || companyID <= 0 || companyID > DSS->k) {
            return INVALID_INPUT;
        }

        long double val = 0;
        auto* cmp = DSS->companyArray[companyID];
        while (cmp->parent_company) {
            val += cmp->value;
            if(cmp->parent_company->parent_company) { /// updating highest parent_company (union find)
                cmp->parent_company = cmp->parent_company->parent_company;
                cmp->value += cmp->parent_company->value; /// updating company value along union find (except for top)
            }
            cmp = cmp->parent_company;
        }
        DSS->companyArray[companyID] = cmp;
        val += cmp->value;
        *(long double *)standing = val;
        return SUCCESS;
    }
    catch (std::bad_alloc const &) {
            return ALLOCATION_ERROR;
    }
}

/*
 * 20 point Bonus function:
StatusType BumpGradeToEmployees(void *DS, int lowerSalary, int higherSalary, int bumpGrade);
 */

void Quit(void **DS);
