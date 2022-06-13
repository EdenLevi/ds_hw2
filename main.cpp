#include <iostream>
#include "library2.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    /*Hashtable hash = Hashtable();
    Company c= Company(1,23);
    Company c2= Company(2,23);
    Company c3= Company(3,23);
    auto* e1= new Employee(457,&c,23,2);
    auto*  e2= new  Employee(2*457,&c2,23,33);
    auto*  e3= new Employee(3*457,&c2,23,32);
    auto* e4= new Employee(4*457,&c2,23,1);
    hash.insert(e4);
    hash.insert(e2);
    hash.insert(e3);
    hash.insert(e1);
    hash.remove(457);
    hash.remove(3*457);
    hash.remove(2*457);
    hash.remove(4*457);*/

    //auto *DS = new DataStructure(3);
    //auto* c1 = DS->companyArray[1];
    //auto* c2 = DS->companyArray[2];

    auto* DS = Init(3);

    AddEmployee(DS,457,1,457);
    AddEmployee(DS,2*457,2,2*457);
    AddEmployee(DS,3*457,2,3*457);
    AddEmployee(DS,4*457,2,4*457);
    AddEmployee(DS,5,2,5);
    AddEmployee(DS,167,2,167);
    AddEmployee(DS,44,2,44);
    AddEmployee(DS,16,2,16);
    AddEmployee(DS,36,2,36);
    AddEmployee(DS,12,2,12);
    AddEmployee(DS,13,2,13);

    EmployeeSalaryIncrease(DS, 457, 2);
    EmployeeSalaryIncrease(DS, 3*457, 2);
    EmployeeSalaryIncrease(DS, 2*457, 2);
    EmployeeSalaryIncrease(DS, 4*457, 2);
    EmployeeSalaryIncrease(DS, 5, 2);
    EmployeeSalaryIncrease(DS, 167, 2);
    EmployeeSalaryIncrease(DS, 44, 2);
    EmployeeSalaryIncrease(DS, 16, 0);
    EmployeeSalaryIncrease(DS, 36, 0);
    EmployeeSalaryIncrease(DS, 12, 0);
    EmployeeSalaryIncrease(DS, 13, 0);

    //AcquireCompany(DS, 1, 2, 3);
    double bumpGrade = 0;
    AverageBumpGradeBetweenSalaryByGroup(DS,0,0,1,&bumpGrade);
    AverageBumpGradeBetweenSalaryByGroup(DS,0,0,2, &bumpGrade);
    AverageBumpGradeBetweenSalaryByGroup(DS,0,0,10, &bumpGrade);
    AverageBumpGradeBetweenSalaryByGroup(DS,0,0,15, &bumpGrade);
    AverageBumpGradeBetweenSalaryByGroup(DS,0,0,25, &bumpGrade);
    AverageBumpGradeBetweenSalaryByGroup(DS,0,0,1, &bumpGrade);

    AverageBumpGradeBetweenSalaryByGroup(DS,1,0,40, &bumpGrade);
    AverageBumpGradeBetweenSalaryByGroup(DS,2,0,40, &bumpGrade);

    RemoveEmployee(DS,457);
    RemoveEmployee(DS,3*457);
    RemoveEmployee(DS,2*457);
    RemoveEmployee(DS,4*457);
    RemoveEmployee(DS,5);
    RemoveEmployee(DS,167);
    RemoveEmployee(DS,44);
    RemoveEmployee(DS,16);
    RemoveEmployee(DS,36);
    RemoveEmployee(DS,12);
    RemoveEmployee(DS,13);

    Quit(&DS);
    return 0;
}
