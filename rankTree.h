//
// Created by edenl on 07/06/2022.
//

#ifndef DS_HW2_RANKTREE_H
#define DS_HW2_RANKTREE_H

#include "tree.h"
#include "Elementy.h"

class rankTree : public tree<Elementy> {
public:
//  int l_boys = 0, int boys = 0, int grades_sum_l_boys = 0);


};

/**
 * Lrot
 *
 * @tparam Element
 * @param head
 * @return
 */

tree<Elementy> *RTleft_rot(tree<Elementy> *head) {

    int Y_r_boys = head->right->element->boys - head->right->element->l_boys;

    // updating y:
    head->right->element->l_boys = head->element->l_boys + 1;
    head->right->element->boys += (head->element->boys + 1);
    head->right->element->grades_sum_l_boys = (head->element->grades_sum_l_boys + head->element->employee->grade);

    // updating x:
    head->element->boys -= (Y_r_boys + 1);

    tree<Elementy> *temp1 = head->right; // y
    tree<Elementy> *temp2 = temp1->left;
    temp1->left = head; // x
    head->right = temp2;
    int a = getHeight(head->left);
    int b = getHeight(head->right);
    head->height = (a > b ? a : b) + 1;

    a = getHeight(temp1->left);
    b = getHeight(temp1->right);
    temp1->height = (a > b ? a : b) + 1;

    return temp1;
}

/**
 * Rrot
 * @tparam Element
 * @param head
 * @return
 */

tree<Elementy> *RTright_rot(tree<Elementy> *head) {

    int X_new_boys = head->right->element->boys + 1;

    // updating y:
    head->element->l_boys -= (head->left->element->l_boys + 1);
    head->element->boys -= (head->left->element->l_boys + 1);
    head->element->grades_sum_l_boys -= (head->left->element->grades_sum_l_boys + head->left->element->employee->grade);

    // updating x:
    head->left->element->boys += X_new_boys;

    tree<Elementy> *temp1 = head->left;
    tree<Elementy> *temp2 = temp1->right;
    temp1->right = head;
    head->left = temp2;
    int a = getHeight(head->left);
    int b = getHeight(head->right);
    head->height = (a > b ? a : b) + 1;
    a = getHeight(temp1->left);
    b = getHeight(temp1->right);
    temp1->height = (a > b ? a : b) + 1;

    return temp1;
}

tree<Elementy> *RtreeAddElementRecursively(tree<Elementy> *head, tree<Elementy> *element_tree, int iterator, bool is_salary,
                                     StatusType *status) {
    if (head == nullptr) {
        return element_tree;
    }


    if (head->id > iterator) {
        head->left = RtreeAddElementRecursively(head->left, element_tree, iterator, is_salary, status);
        head->left->element->l_boys++;
        head->left->element->grades_sum_l_boys+=element_tree->element->employee->grade;
        head->left->element->boys++;
    } else if (head->id < iterator) {
        head->right = RtreeAddElementRecursively(head->right, element_tree, iterator, is_salary, status);
        head->right->element->boys++;

    } else { // cant be equal
        if (!is_salary) {
            *status = FAILURE; // already exists in tree
            return head; // possibly element_tree
        } else if (head->element->id > element_tree->element->id) {
            head->right = RtreeAddElementRecursively(head->right, element_tree, iterator, is_salary, status);
            head->right->element->boys++;
        } else if (head->element->id < element_tree->element->id) {
            head->left = RtreeAddElementRecursively(head->left, element_tree, iterator, is_salary, status);
            head->left->element->l_boys++;
            head->left->element->grades_sum_l_boys+=element_tree->element->employee->grade;
            head->left->element->boys++;
        } else {
            *status = FAILURE; // already exists in tree
            return head; // possibly element_tree
        }

    }

    *status = SUCCESS;
    int a = getHeight(head->left);
    int c = getHeight(head->right);
    head->height = (a > c ? a : c) + 1;
    int b = getBalance(head);

    // LL
    if (head->left != nullptr && (b > 1 && iterator < head->left->id)) {
        return RTright_rot(head);
    }
    // LR
    if (head->left != nullptr && (b > 1 && iterator > head->left->id)) {
        head->left = RTleft_rot(head->left);
        return RTright_rot(head);
    }
    // RR
    if (head->right != nullptr && (b < -1 && iterator > head->right->id)) {
        return RTleft_rot(head);
    }

    // RL
    if (head->right != nullptr && (b < -1 && iterator < head->right->id)) {
        head->right = RTright_rot(head->right);
        return RTleft_rot(head);
    }

    if (is_salary) {
        if ((head->left) && (head->left->id == iterator)) {
            // LL
            if (b > 1 && element_tree->element->id > head->left->element->id) {
                return RTright_rot(head);
            }
            // LR
            if (b > 1 && element_tree->element->id < head->left->element->id) {
                head->left =RTleft_rot(head->left);
                return RTright_rot(head);
            }
        }
        if ((head->right) && (head->right->id == iterator)) {
            // RR
            if (b < -1 && element_tree->element->id < head->right->element->id) {
                return RTleft_rot(head);
            }

            // RL
            if (b < -1 && element_tree->element->id > head->right->element->id) {
                head->right = RTright_rot(head->right);
                return RTleft_rot(head);
            }
        }
    }

    //  do nothing
    return head;
}

/**
 * the func add element to a tree by iterator (can be id or salary in our cases)
 * @param e
 * @param iterator
 * @param status
 * @return
 */

tree<Elementy>* RtreeAddElement(tree<Elementy> *head, Elementy *e, StatusType *status) {
    if (head == nullptr || e == nullptr || e->salary <= 0) {
        *status = INVALID_INPUT;
        return head;
    }
    try {
        tree<Elementy> *t = new tree<Elementy>(e->salary , e);
        tree<Elementy> *T = RtreeAddElementRecursively(head, t, e->salary, true, status);
        if (*status != SUCCESS) {
            delete t;
            return nullptr;
        }
        return T;
    }
    catch (std::bad_alloc &) {
        *status = ALLOCATION_ERROR;
        return head;
    }
}


/**
 *
 * DELETE SECTION
 *
 * **/


tree<Elementy> *RtreeDeleteElementRecursively(tree<Elementy> *head, Elementy *e, bool is_salary, bool is_deep_delete, StatusType *status) {

    if (head == nullptr || e == nullptr) {
        *status = FAILURE; // already exists in tree
        return head;
    }
    //choose which id is the condition
    int id = e->id;
    if (is_salary)
        id = e->salary;

    if (id < head->id) {
        head->left = RtreeDeleteElementRecursively(head->left, e, is_salary, is_deep_delete, status);
    } else if (id > head->id) {
        head->right = RtreeDeleteElementRecursively(head->right, e, is_salary, is_deep_delete, status);
    } else if (is_salary && head->element->id != e->id) {
        id = e->id;
        if (id < head->element->id) {
            head->right = RtreeDeleteElementRecursively(head->right, e, is_salary, is_deep_delete, status);
        } else if (id > head->element->id) {
            head->left = RtreeDeleteElementRecursively(head->left, e, is_salary, is_deep_delete, status);
        }
    }
        //found the element to be deleted
    else {

        //case of 1-0 child
        tree<Elementy> *temp = nullptr;
        if (head->left == nullptr || head->right == nullptr) {
            if (head->left == nullptr && head->right == nullptr) {
                //make sure that we don't have memory leak at that point, shared ptr suppose to free
                temp = head;
                head = nullptr;
                if (is_deep_delete) {
                    delete temp->element;
                    temp->element = nullptr;
                }
            } else {
                temp = head->left ? head->left : head->right;
                if (is_deep_delete) {
                    delete head->element;
                }
                head->element = temp->element;
                head->left = temp->left;
                head->right = temp->right;
                head->height = temp->height;

                if (is_salary) {
                    head->id = temp->element->salary;
                } else
                    head->id = temp->id;
            }

            delete temp;


            *status = SUCCESS;
        }
            //2 child case
        else {
            temp = head->right;
            if (temp != nullptr) {
                while (temp->left != nullptr) {
                    temp = temp->left;
                }
                if (is_deep_delete) {
                    delete head->element;
                    is_deep_delete = false;
                }

                head->element = temp->element;
                if (is_salary) {
                    head->id = temp->element->salary;
                } else
                    head->id = temp->id;

                head->right = RtreeDeleteElementRecursively(head->right,
                                                       temp->element, is_salary, is_deep_delete, status);
            }
        }
    }

    *status = SUCCESS;
    if (head == nullptr)
        return nullptr;


    int a = getHeight(head->left);
    int c = getHeight(head->right);
    head->height = (a > c ? a : c) + 1;
    int b = getBalance(head);;

    // LL
    if (b > 1 && getBalance(head->left) >= 0) {
        return RTright_rot(head);
    }

    // LR
    if (b > 1 && getBalance(head->left) < 0) {
        head->left = RTleft_rot(head->left);
        return RTright_rot(head);
    }

    // RR
    if (b < -1 && getBalance(head->right) <= 0) {
        return RTleft_rot(head);
    }

    // RL
    if (b < -1 && getBalance(head->right) > 0) {
        head->right = RTright_rot(head->right);
        return RTleft_rot(head);
    }

    // do nothing:
    return head;

}


tree<Elementy> *RtreeDeleteElement(tree<Elementy> *head, Elementy *e, bool is_salary, bool is_deep_delete, StatusType *status) {
    // need to address deletion of head node at a higher scope (bahootz)
    if (head->id <= 0) {
        *status = INVALID_INPUT;
        return head;
    }
    if (e == nullptr) {
        *status = FAILURE;
        return head;
    }

    return RtreeDeleteElementRecursively(head, e, is_salary, is_deep_delete, status);

}





#endif //DS_HW2_RANKTREE_H
