//
// Created by edenl on 10/06/2022.
//

#ifndef DS_HW2_CLASSES_H
#define DS_HW2_CLASSES_H

class Elementy;

template<class Element>
class tree {
public:
    int id; //iterator
    int height;
    Element *element;
    tree<Element> *left;
    tree<Element> *right;

    explicit tree(int id) : id(id), height(1), element(nullptr), left(nullptr), right(nullptr) {};

    tree(int id, Element *element) : id(id), height(1), element(element), left(nullptr), right(nullptr) {};

    ~tree() {
        element = nullptr;
        left = nullptr;
        right = nullptr;
    };


};

class Company {

public:
    double value;
    int id;
    int employee_count;
    int salary; //garbage value
    int sum_grade_non_salaries;
    tree<Elementy> *employees_pointers_by_salary;
    Company *parent_company;

    Company(int value, int company_id) : value(value), id(company_id), employee_count(0), salary(-1), sum_grade_non_salaries(0),
                                         employees_pointers_by_salary(nullptr), parent_company(nullptr) {}

    ~Company() {

        employees_pointers_by_salary = nullptr;
    };
    Company *getCompany() {

        if(parent_company) {
            while(parent_company->parent_company) {
                value += parent_company->value;
                parent_company = parent_company->parent_company;
            }
        }
        return (parent_company) ? parent_company : this;

        /*Company *cmp = this;
        while(cmp->parent_company){
            if(cmp->parent_company->parent_company!= nullptr){
                cmp->value += cmp->parent_company->value;
                cmp->parent_company=cmp->parent_company->parent_company;
            }
        }
        if(this->parent_company== nullptr)
            return this;
        else{
            this->parent_company=cmp;
            return this->parent_company;
        }*/
    }
};

class Employee {
public:
    int id;
    int salary;
    int grade;

    Employee(int id, Company *company, int salary = 0, int grade = 0) : id(id), salary(salary), grade(grade),
                                                                        company(company) {}

    ~Employee() {
        company = nullptr;
    };

    Company *getCompany() {

        if(company->parent_company) {
            while(company->parent_company->parent_company) {
                company->value += company->parent_company->value;
                company->parent_company = company->parent_company->parent_company;
            }
        }
        return (company->parent_company) ? company->parent_company : company;


        /*Company *cmp = company;
        while(cmp->parent_company){
            if(cmp->parent_company->parent_company){
                cmp->value += cmp->parent_company->value;
                cmp->parent_company = cmp->parent_company->parent_company;
            }
        }

        if(company->parent_company== nullptr)
            return company;
        else{
            company->parent_company=cmp;
            return company->parent_company;
        }*/

    }

    void setCompany(Company* cmp) {
        this->company = cmp;
    }

private:
    Company *company; //only update when head employee
};

class Elementy {
public:
    Employee *employee;
    int id;
    int salary;
    int boys;
    int l_boys;
    int grades_sum_l_boys;

    explicit Elementy(Employee *employee, int l_boys = 0, int boys = 0, int grades_sum_l_boys = 0)
            : employee(employee), id(employee->id), salary(employee->salary), l_boys(l_boys), boys(boys),
              grades_sum_l_boys(grades_sum_l_boys) {}

    ~Elementy() {
        employee = nullptr;
    };
};



#endif //DS_HW2_CLASSES_H
