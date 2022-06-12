//
// Created by omerg on 25/04/2022.
//

#ifndef DS_HW1_TREE_H
#define DS_HW1_TREE_H

#include <memory>
#include "library2.h"
#include "classes.h"


template<class Element>
tree<Element> *eraseElement(tree<Element> *&head, Element *e, bool is_salary, bool is_deep_delete, StatusType *status) {
    // need to address deletion of head node at a higher scope (bahootz)
    if (head->id <= 0) {
        *status = INVALID_INPUT;
        return head;
    }
    if (e == nullptr) {
        *status = FAILURE;
        return head;
    }

    return deleteElementRecursively(head, e, is_salary, is_deep_delete, status);

}


template<class Element>
void clear_helper(tree<Element> *&tree) {
    if (tree == nullptr) {
        return;
    }
    clear_helper(tree->left);
    tree->left = nullptr;
    clear_helper(tree->right);
    tree->right = nullptr;

    delete tree;
    tree = nullptr;
}

template<class Element>
tree<Element> *clear(tree<Element> *&tree) {
    if (tree == nullptr) {
        return nullptr;
    }
    clear_helper(tree->left);
    tree->left = nullptr;
    clear_helper(tree->right);
    tree->right = nullptr;

    delete tree;
    tree = nullptr;
    return nullptr;
}

template<class Element>
tree<Element> *clearAll(tree<Element> *tree) {
    if (tree == nullptr) {
        return nullptr;
    }
    clearAll(tree->left);
    tree->left = nullptr;
    clearAll(tree->right);
    tree->right = nullptr;

    delete tree->element;
    tree->element = nullptr;
    delete tree;
    tree = nullptr;
    return nullptr;
}

template<class Element>
tree<Element> *addElement(tree<Element> *head, Element *e, int iterator, bool is_salary, StatusType *status) {
    if (head == nullptr || e == nullptr || iterator <= 0) {
        *status = INVALID_INPUT;
        return head;
    }
    try {
        auto *t = new tree<Element>(iterator, e);
        tree<Element> *T = addElementRecursively(head, t, iterator, is_salary, status);
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

template<class Element>
int getHeight(tree<Element> *head) {
    if (head == nullptr)
        return 0;
    return head->height;
}

template<class Element>
int getBalance(tree<Element> *head) {
    if (head == nullptr)
        return 0;
    return (getHeight(head->left) - getHeight(head->right));
}

template<class Element>
tree<Element> *findById(tree<Element> *head, int id) {
    if (head == nullptr) {
        return nullptr;
    }
    if (head->id == id) {
        return head;
    } else if (head->id < id && head->right != nullptr) {
        return findById(head->right, id);
    } else if (head->id > id && head->left != nullptr) {
        return findById(head->left, id);
    }
    return nullptr;
}

tree<Elementy> *findBySalary(tree<Elementy> *head, Employee* emp) {
    if (head == nullptr||emp== nullptr) {
        return head;
    }
    if (head->id == emp->salary) {
      if(head->element->id==emp->id){
          return head;
      }
      else if (head->element->id > emp->id && head->right != nullptr) {
          return findBySalary(head->right, emp);
      } else if (head->element->id < emp->id && head->left != nullptr) {
          return findBySalary(head->left, emp);
      }

    } else if (head->id < emp->salary && head->right != nullptr) {
        return findBySalary(head->right, emp);
    } else if (head->id > emp->salary && head->left != nullptr) {
        return findBySalary(head->left, emp);
    }
    return nullptr;
}

/*******************************************   rotate functions   ***********************************************************/

/**
 * Lrot
 *
 * @tparam Element
 * @param head
 * @return
 */
template<class Element>
tree<Element> *left_rot(tree<Element> *head) {
    tree<Element> *temp1 = head->right;
    tree<Element> *temp2 = temp1->left;
    temp1->left = head;
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
template<class Element>
tree<Element> *right_rot(tree<Element> *head) {
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

/**
 *
 * @tparam Element
 * @param head
 * @param element_tree
 * @param iterator
 * @param is_salary
 * @param status
 * @return
 */
template<class Element>
tree<Element> *addElementRecursively(tree<Element> *head, tree<Element> *element_tree, int iterator, bool is_salary,
                                     StatusType *status) {
    if (head == nullptr) {
        return element_tree;
    }


    if (head->id > iterator) {
        head->left = addElementRecursively(head->left, element_tree, iterator, is_salary, status);
    } else if (head->id < iterator) {
        head->right = addElementRecursively(head->right, element_tree, iterator, is_salary, status);

    } else { // cant be equal
        if (!is_salary) {
            *status = FAILURE; // already exists in tree
            return head; // possibly element_tree
        } else if (head->element->id > element_tree->element->id) {
            head->right = addElementRecursively(head->right, element_tree, iterator, is_salary, status);
        } else if (head->element->id < element_tree->element->id) {
            head->left = addElementRecursively(head->left, element_tree, iterator, is_salary, status);
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
        return right_rot<Element>(head);
    }
    // LR
    if (head->left != nullptr && (b > 1 && iterator > head->left->id)) {
        head->left = left_rot<Element>(head->left);
        return right_rot<Element>(head);
    }
    // RR
    if (head->right != nullptr && (b < -1 && iterator > head->right->id)) {
        return left_rot<Element>(head);
    }

    // RL
    if (head->right != nullptr && (b < -1 && iterator < head->right->id)) {
        head->right = right_rot<Element>(head->right);
        return left_rot<Element>(head);
    }

    if (is_salary) {
        if ((head->left) && (head->left->id == iterator)) {
            // LL
            if (b > 1 && element_tree->element->id > head->left->element->id) {
                return right_rot<Element>(head);
            }
            // LR
            if (b > 1 && element_tree->element->id < head->left->element->id) {
                head->left = left_rot<Element>(head->left);
                return right_rot<Element>(head);
            }
        }
        if ((head->right) && (head->right->id == iterator)) {
            // RR
            if (b < -1 && element_tree->element->id < head->right->element->id) {
                return left_rot<Element>(head);
            }

            // RL
            if (b < -1 && element_tree->element->id > head->right->element->id) {
                head->right = right_rot<Element>(head->right);
                return left_rot<Element>(head);
            }
        }
    }

    //  do nothing
    return head;
}


/**
 * the func help the delete func to erase element recursively
 * @tparam Element
 * @param head
 * @param id
 * @param status
 * @return
 */
template<class Element>
tree<Element> *deleteElementRecursively(tree<Element> *head, Element *e, bool is_salary, bool is_deep_delete, StatusType *status) {

    if (head == nullptr || e == nullptr) {
        *status = FAILURE; // already exists in tree
        return head;
    }
    //choose which id is the condition
    int id = e->id;
    if (is_salary)
        id = e->salary;

    if (id < head->id) {
        head->left = deleteElementRecursively(head->left, e, is_salary, is_deep_delete, status);
    } else if (id > head->id) {
        head->right = deleteElementRecursively(head->right, e, is_salary, is_deep_delete, status);
    } else if (is_salary && head->element->id != e->id) {
        id = e->id;
        if (id < head->element->id) {
            head->right = deleteElementRecursively(head->right, e, is_salary, is_deep_delete, status);
        } else if (id > head->element->id) {
            head->left = deleteElementRecursively(head->left, e, is_salary, is_deep_delete, status);
        }
    }
        //found the element to be deleted
    else {

        //case of 1-0 child
        tree<Element> *temp = nullptr;
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

                head->right = deleteElementRecursively(head->right,
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
        return right_rot<Element>(head);
    }

    // LR
    if (b > 1 && getBalance(head->left) < 0) {
        head->left = left_rot<Element>(head->left);
        return right_rot<Element>(head);
    }

    // RR
    if (b < -1 && getBalance(head->right) <= 0) {
        return left_rot<Element>(head);
    }

    // RL
    if (b < -1 && getBalance(head->right) > 0) {
        head->right = right_rot<Element>(head->right);
        return left_rot<Element>(head);
    }

    // do nothing:
    return head;

}


/**
 * the func also delete the old values
 * @tparam Element
 * @param head
 * @param dest
 * @param index
 */
template<class Element>
void treeToArray(tree<Element> *head, Element **dest, int *index) {
    if (head == nullptr) {
        return;
    }
    treeToArray(head->left, dest, index);
    dest[*index] = head->element;
    (*index)++;
    treeToArray(head->right, dest, index);

}

#endif //DS_HW1_TREE_H
