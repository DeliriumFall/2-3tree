#include "tree.h"

tree::tree() {
    this->node = new node();
    this->node->tags = make_pair(-1,-1);
    this->node->value = -1;
}

node* tree::min() {
    node* curr = this->root;
    if (curr==NULL) return -1;
    while (!isLeaf(curr)) {
        curr = curr->lower;
    }
    return curr;
}

node* tree::max() {
    node* curr = this->root;
    if (curr==NULL) return -1;
    while (!isLeaf(curr)) {
        if (curr->upper == NULL) curr = curr->middle;
        else curr = curr->upper;
    }
    return curr;
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

tree* join(tree* otherTree) {
    int thisHeight = getHeight(this), otherHeight = getHieight(otherTree);
    //it this is taller, else inverse process
    if (getHeight(this) == getHeight(otherTree)) {
        node* newRoot = new node();
        if (this->root->tags.second < otherTree->root->tags.first) {
            newRoot->lower = this->root;
            newRoot->middle = otherTree->root;
        } else {
            newRoot->lower = otherTree->root;
            newRoot->middle = this->root;
        }
        this->root = newRoot;
        return this;
    }
    //if diff height, find node with same height and add it there
    //if overflow -> addSon()
}

tree* split(int target) {
}
int getHeight(tree* currentTree);
void checkTags(node* currentNode);
