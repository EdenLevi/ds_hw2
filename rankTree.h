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

    tree<Element> *temp1 = head->left;
    tree<Element> *temp2 = temp1->right;
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

tree<Element> *RtreeAddElementRecursively(tree<Element> *head, tree<Element> *element_tree, int iterator, bool is_salary,
                                     StatusType *status) {
    if (head == nullptr) {
        return element_tree;
    }


    if (head->id > iterator) {
        head->left = addElementRecursively(head->left, element_tree, iterator, is_salary, status);
        head->left->element->l_boys++;
        head->left->element->grades_sum_l_boys+=element_tree->element->employee->grade;
        head->left->element->boys++;
    } else if (head->id < iterator) {
        head->right = addElementRecursively(head->right, element_tree, iterator, is_salary, status);
        head->right->element->boys++;

    } else { // cant be equal
        if (!is_salary) {
            *status = FAILURE; // already exists in tree
            return head; // possibly element_tree
        } else if (head->element->id > element_tree->element->id) {
            head->right = addElementRecursively(head->right, element_tree, iterator, is_salary, status);
            head->right->element->boys++;
        } else if (head->element->id < element_tree->element->id) {
            head->left = addElementRecursively(head->left, element_tree, iterator, is_salary, status);
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

tree<Element>* RtreeAddElement(tree<Element> *head, Element *e, StatusType *status) {
    if (head == nullptr || e == nullptr || e->salary <= 0) {
        *status = INVALID_INPUT;
        return head;
    }
    try {
        tree<Element> *t = new tree<Element>(e->salary , e);
        tree<Element> *T = RtreeAddElementRecursively(head, t, e->salary, true, status);
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








#endif //DS_HW2_RANKTREE_H
