#ifndef tree_h
#define tree_h

#include <cstdlib>
#include <utility>

struct node {
    node *lower, *middle, *upper, *parent;
    pair <int, int> tags;
    pair <int, node*> overflow;
    int value;
};

class tree {
    public:
        node* root;

        tree();
        node* min();
        node* max();
        node* search(int target);
        bool isLeaf(node* targetNode);
        bool insert(int newValue);
        bool addSon(node* currentNode);
        bool remove(node* target);
        tree* join(tree* otherTree);
        tree* split(int target);
        int getHeight(tree* currentTree);
        void checkTags(node* currentNode);
};

#endif
