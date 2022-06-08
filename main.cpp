#include <iostream>
#include "hashtable.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Hashtable hash = Hashtable();
    Company c= Company(1,23);
    Company c2= Company(2,23);
    Company c3= Company(3,23);
    Employee* e1= new Employee(457,&c,23,2);
    Employee*  e2= new  Employee(2*457,&c2,23,33);
    Employee*  e3= new Employee(3*457,&c2,23,32);
    Employee* e4= new Employee(4*457,&c2,23,1);
    hash.insert(e4);
    hash.insert(e2);
    hash.insert(e3);
    hash.insert(e1);
    hash.remove(457);
    hash.remove(3*457);
    hash.remove(2*457);
    hash.remove(4*457);
    return 0;
}
