//
//  RBTWithRank.hpp
//  RBTWithRank
//
//  Created by Erin Jane Ge on 12/26/20.
//  Reference: https://www.cnblogs.com/yxsrt/p/12201788.html
//  Copyright © 2020 Erin Jane Ge. All rights reserved.
//

#ifndef RBTWithRank_hpp
#define RBTWithRank_hpp

#include <iostream>
using namespace std;

class RBT {
    private :
    enum {RED = 0, BLACK};
    struct Node {
        int key;
        bool color;
        int size;
        Node * left;
        Node * right;
        Node * parent;
        Node(int k = -9999, bool c = BLACK, int s = 0, Node *l = nullptr, Node *r = nullptr, Node *p = nullptr)
        : key(k), color(c), size(s), left(l), right(r), parent(p) {}
    };
    private :
    Node * nil;
    Node * root;
    private :
    void leftRotate(Node * x);
    void rightRotate(Node * x);
    void fixup_insert(Node * p);
    void fixup_remove(Node * p);
    void transplant(Node * old_t, Node * new_t);
    void insert(Node * p);
    void erase(Node * p);
    Node * search(Node * p, const int k);
    Node * minimum(Node * p);
    Node * maximum(Node * p);
    void inorderWalk(Node * p) const;
    Node * successor(Node * p);
    Node * predecessor(Node * p);
    int rank(Node * p);
    Node * index(Node * p, const int k);
    public :
    RBT() : nil(new Node), root(nil) {}
    ~RBT() {delete nil;}
    void insert(const int key) {insert(new Node(key, RED, 1, nil, nil, nil));}
    void erase(const int key) {erase(search(root, key));}
    bool search(const int key) {return (search(root, key) == nil ? false : true);}
    int minimum() {return minimum(root)->key;}
    int maximum() {return maximum(root)->key;}
    int predecessor(const int key);
    int successor(const int key);
    int * rank(const int key);
    int index(const int key) {return index(root, key)->key;};
    
    friend ostream &operator<<(ostream &os, const RBT &t);
};

#endif /* RBTWithRank_hpp */
