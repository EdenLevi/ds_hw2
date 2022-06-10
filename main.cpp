#include <iostream>
#include "classes.h"
#include "Hashtable.h"
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

    auto *DS = new DataStructure(3);
    auto* c1 = DS->companyArray[1];
    auto* c2 = DS->companyArray[2];

    AddEmployee(DS,457,1,15);
    AddEmployee(DS,2*457,2,20);
    AddEmployee(DS,3*457,2,10);
    AddEmployee(DS,4*457,2,10);

    EmployeeSalaryIncrease(DS, 457, 15);
    EmployeeSalaryIncrease(DS, 3*457, 15);
    EmployeeSalaryIncrease(DS, 2*457, 15);
    EmployeeSalaryIncrease(DS, 4*457, 15);

    RemoveEmployee(DS,457);
    RemoveEmployee(DS,3*457);
    RemoveEmployee(DS,2*457);
    RemoveEmployee(DS,4*457);


    return 0;
}
