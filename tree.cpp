#include "tree.h"
#include <set>

tree::tree() {
    this->node = new node();
    this->node->tags = make_pair(-1,-1);
    this->node->value = -1;
}

node* tree::min(node* fromNode) {
    if (fromNode==NULL) return NULL;
    while (!isLeaf(fromNode)) {
        fromNode = fromNode->lower;
    }
    return fromNode;
}

node* tree::max(node* fromNode) {
    if (fromNode==NULL) return NULL;
    while (!isLeaf(fromNode)) {
        if (fromNode->upper == NULL) fromNode = fromNode->middle;
        else fromNode= fromNode->upper;
    }
    return fromNode;
}

node* tree::search(node* currentNode, int target) {
    if (currentNode==NULL) return NULL;
    if (isLeaf(currentNode->lower)) return currentNode;
    if (target <= currentNode->tags.first) return search(currentNode->lower, target);
    if (currentNode->upper != NULL) {
        if (target <= currentNode->tags.second) return search(currentNode->middle, target);
        return search(currentNode->upper, target);
    }
    return search(currentNode->middle, target);
}

bool tree::isLeaf(node* targetNode) {
    return (targetNode.lower==NULL);
}

bool tree::insert(int newValue) {
    node* place = search(this->root, newValue);
    if (place->upper == NULL) {
        place->upper = new node();
        if (newValue > place->middle->value) {
            place->upper->value = newValue; 
            place->upper->parent = place; 
            checkTags(place);
            return true;
        }
        place->upper->value = place->middle->value;
        if (newValue > place->lower->value) {
            place->middle->value = newValue;
        } else {
            place->middle->value = place->lower->value;
            place->lower->value = newValue;
        }
        checkTags(place);
        return true;
    }
    node* tempNode = new node();
    tempNode->value = newValue;
    tempNode->parent = place;
    if (newValue < place->lower->value) place->overflow = make_pair(0, tempNode);
    else if (newValue < place->middle->value) place->overflow = make_pair(1, tempNode);
    else if (newValue < place->upper->value) place->overflow = make_pair(2, tempNode);
    place->overflow = make_pair(3, tempNode);
    return addSon(place);
}

bool tree::addSon(node* currentNode) {
    node* newNode = new node();
    switch (currentNode->overflow.first) {
        case 0:
            newNode->lower = currentNode->middle;
            newNode->middle = currentNode->upper;
            currentNode->middle = currentNode->lower;
            currentNode->lower = currentNode->currentNode->overflow.second;
            break;
        case 1:
            newNode->lower = currentNode->middle;
            newNode->middle = currentNode->upper;
            currentNode->middle = currentNode->overflow.second;
            break;
        case 2:
            newNode->lower = currentNode->overflow.second;
            newNode->middle = currentNode->upper;
            break;
        case 3:
            newNode->lower = currentNode->upper;
            newNode->middle = currentNode->overflow.second;
            break;
    }
    currentNode->upper = NULL;
    currentNode->overflow = NULL;
    newNode->lower->parent = newNode;
    newNode->middle->parent = newNode;
    if (currentNode->parent == NULL) {
        node* newRoot = new node();
        newRoot->lower = currentNode;
        newRoot->middle = newNode;
        this->root = newRoot;
        checkTags(currentNode);
        return true;
    } else {
        newNode->parent = currentNode->parent;
        if (currentNode->parent->upper == NULL) {
            if (currentNode->parent->lower == currentNode) {
                currentNode->parent->upper = currentNode->parent->middle;
                currentNode->parent->middle = newNode;
                checkTags(currentNode);
                return true;
            } else {
                currentNode->parent->upper = newNode;
                checkTags(currentNode);
                return true;
            }
        } else {
            if (currentNode->parent->lower == currentNode) currentNode->parent->overflow = make_pair(1,newNode);
            else if (currentNode->parent->middle == currentNode) currentNode->parent->overflow = make_pair(2,newNode);
            else currentNode->parent->overflow = make_pair(3, newNode);
            return addSon(currentNode->parent);
        }
    }
}

bool tree::remove(node* target) {
    if (this->root == target) {
        this->root = NULL;
        return true;
    }
    node* place = target->parent;
    if (place->upper!=NULL) {
        if (place->lower == target) {
            place->lower = place->middle;
        }
        place->middle = place->upper;
        place->upper = NULL;
        checkTags(place);
        return true;
    }
    if (place->parent->lower == place) {
        if (place->parent->middle->upper != NULL) {
            if (place->lower == target) {
                place->lower = place->middle;
            }
            place->middle = place->parent->middle->lower;
            place->middle->parent = place;
            place->parent->middle->lower = place->parent->middle->middle;
            place->parent->middle->middle = place->parent->middle->upper;
            place->parent->middle->upper = NULL;
            checkTags(place);
            return true;
        }
        if (place->parent->upper != NULL && place->parent->upper->upper != NULL) {
            if (place->lower == target) {
                place->lower = place->middle;
            }
            place->middle = place->parent->middle->lower;
            place->middle->parent = place;
            place->parent->middle->lower = place->parent->middle->middle;
            place->parent->middle->middle = place->parent->upper->lower;
            place->parent->middle->middle->parent = place->parent->middle;
            place->parent->upper->lower = place->parent->upper->middle;
            place->parent->upper->middle = place->parent->upper->upper;
            place->parent->upper->upper = NULL;
            checkTags(place);
            return true;
        }
    }
    if (place->parent->middle == place) {
        if (place->parent->lower->upper != NULL) {
            if (place->middle == target) {
                place->middle = place->lower;
            }
            place->lower = place->parent->lower->upper;
            place->lower->parent = place;
            place->parent->lower->upper = NULL;
            checkTags(place);
            return true;
        }
        if (place->parent->upper != NULL && place->parent->upper->upper != NULL) {
            if (place->lower == target) {
                place->lower = place->middle;
            }
            place->middle = place->parent->upper->lower;
            place->middle->parent = place;
            place->parent->upper->lower = place->parent->upper->middle;
            place->parent->upper->middle = place->parent->upper->upper;
            place->parent->upper->upper = NULL;
            checkTags(place);
            return true;
        }
    }
    if (place->parent->upper == place) {
        if (place->parent->middle->upper != NULL) {
            if (place->middle == target) {
                place->middle = place->lower;
            }
            place->lower = place->parent->middle->upper;
            place->lower->parent = place;
            place->parent->middle->upper = NULL;
            checkTags(place);
            return true;
        }
        if (place->parent->lower->upper != NULL) {
            if (place->middle == target) {
                place->middle = place->lower
            }
            place->lower = place->parent->middle->middle;
            place->lower->parent = place;
            place->parent->middle->middle = place->parent->middle->lower;
            place->parent->middle->lower = place->parent->lower->upper;
            place->parent->middle->lower->parent = place->parent->middle;
            place->parent->lower->upper = NULL;
            checkTags(place);
            return true;
        }
    }
    if (place->parent->lower == place) {
        if (place->lower == target) {
            place->lower = place->middle;
        }
        place->middle = place->parent->middle->lower;
        place->upper = place->parent->middle->middle;
        place->middle->parent = place;
        place->upper->parent = place;
        if (place->parent->upper != NULL) {
            place->parent->middle = place->parent->upper;
            place->parent->middle->lower->parent = place->parent->middle;
            place->parent->middle->middle->parent = place->parent->middle;
            place->parent->upper = NULL;
            checkTags(place);
            return true;
        }
        return remove(place->parent->middle);
    }
    if (place->parent->middle == place) {
        if (place->parent->upper != NULL) {
            if (place->lower == target) {
                place->lower = place->middle;
            }
            place->middle = place->parent->upper->lower;
            place->upper = place->parent->upper->middle;
            place->middle->parent = place;
            place->upper->parent = place;
            place->parent->upper = NULL;
            checkTags(place);
            return true;
        }
        if (place->lower == target) {
            place->lower = place->middle;
        }
        place->parent->lower->upper = place->lower;
        place->parent->lower->upper->parent = place->parent->lower;
        return remove(place);
    }
    if (place->lower == target) {
        place->lower = place->middle;
    }
    place->parent->middle->upper = place->lower;
    place->parent->middle->upper->parent = place->parent->middle;
    checkTags(place->parent->middle);
    place = NULL;
    return true;
}

/*
 * precondition: otherTree is shorter or has the same height as this
 * postcondition: this includes otherTree in itsel
 */
tree* join(tree* otherTree) {
    node* higherNode, lowerNode;
    int lowerHieght;
    if (getHeight(this->root) > getHeight(otherTree->root)) {
        higherNode = this->root;
        lowerNode = otherTree->root;
    } else if (getHeight(this->root) < getHeight(otherTree->root)) {
        printf("Cannot be joined\n");
        return NULL;
    } else {
        node* newRoot = new node();
        if (this->root->tags.second < otherTree->root->tags.first) {
            newRoot->lower = this->root;
            newRoot->middle = otherTree->root;
        } else {
            newRoot->lower = otherTree->root;
            newRoot->middle = this->root;
        }
        newRoot->lower->parent = newRoot;
        newRoot->middle->parent = newRoot;
        this->root = newRoot;
        checkTags(this->root->lower);
        return this;
    }
    lowerHieght = getHeight(lowerNode);
    while (lowerHeight < getHeight(higherNode)) {
        if (lowerNode->tags.second <= higherNode->tags.first ) {
            higherNode = higherNode->lower;
        } else if (lowerNode->tags.second <= higherNode->tags.second || higherNode->upper == NULL) {
            higherNode = higherNode->middle;
        } else {
            higherNode = higherNode->upper;
        }
    }
    if (isLeaf(higherNode)) {
        insert(lowerNode->value);
        return this;
    }
    node* parentNode = higherNode->parent;
    if (parentNode->upper == NULL) {
        if (lowerNode->tags.second <= parentNode->tags.first ) {
            parentNode->upper = parentNode->middle;
            parentNode->middle = parentNode->lower;
            parentNode->lower = lowerNode;
            parentNode->lower->parent = parentNode;
            checkTags(higherNode);
            return this;
        } else if (parentNode->tags.second <= lowerNode->tags.first) {
            parentNode->upper = lowerNode;
            parentNode->upper->parent = parentNode;
            checkTags(higherNode);
            return this;
        } else {
            parentNode->upper = parentNode->middle;
            parentNode->middle = lowerNode;
            parentNode->middle->parent = parentNode;
            checkTags(higherNode);
            return this;
        }
    }
    if (lowerNode->tags.second <= parentNode->tags.first ) parentNode->overflow = make_pair(0, lowerNode);
    else if (parentNode->tags.second <= lowerNode->tags.first ) parentNode->overflow = make_pair(3, lowerNode);
    else if (lowerNode->tags.second <= parentNode->tags.second) parentNode->overflow = make_pair(1, lowerNode);
    else parentNode->overflow = make_pair(2, lowerNode);
    addSon(currentNode->parent);
    return this;
}

tree* tree::split(int target) {
    set<int> lesserLeaves;
    node* currentNode = this->root;
    while (!isLeaf(currentNode->lower)) {
        if (target <= currentNode->tags.first) currentNode = currentNode->lower;
        else if (currentNode->upper != NULL && target > currentNode->tags.second) {
            //getLeaves of mid and low
            getLeaves(currentNode, &lesserLeaves, 2);
            currentNode = currentNode->upper;
        } else {
            //getLeaves of low
            getLeaves(currentNode, &lesserLeaves, 1);
            currentNode = currentNode->middle;
        }
    }
    tree smallTree = new tree();
    for (set<int>::iterator it=lesserLeaves.begin(); it!=lesserLeaves.end(); ++it) {
        smallTree.insert(*it);
        this->remove(*it);
    }
    return {smallTree, this};
}

int tree::getHeight(node* currentNode) {
    node* place = currentNode;
    int c=0;
    while (place->lower != NULL) {
        place = place->lower;
        c++;
    }
    return c;
}

void tree::checkTags(node* currentNode) {
    if (currentNode->parent == NULL) return;
    node* parentNode = currentNode->parent;
    if (isLeaf(parentNode->lower)) {
        parentNode->tags = make_pair(parentNode->lower->value, parentNode->middle->value);
    } else {
        parentNode->tags = make_pair(max(parentNode->lower)->value, max(parentNode->middle)->value);
    }
    checkTags(parentNode);
}

void tree::getLeaves(node* currentNode, set<int> &leaves, int which) {
    if (isLeaf(currentNode)) {
        (*leaves).insert(currentNode->value);
        return;
    }
    if (which == 0) { //all children
        getLeaves(currentNode->lower, leaves, 0);
        getLeaves(currentNode->middle, leaves, 0);
        if (currentNode->upper!=NULL)  getLeaves(currentNode->upper, leaves, 0);
    } else if (which == 1) { //Only lower
        getLeaves(currentNode->lower, leaves, 0);
    } else { //Lower and mid
        getLeaves(currentNode->lower, leaves, 0);
        getLeaves(currentNode->middle, leaves, 0);
    }
}
