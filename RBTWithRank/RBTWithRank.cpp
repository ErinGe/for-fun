//
//  RBTWithRank.cpp
//  RBTWithRank
//
//  Created by Erin Jane Ge on 12/26/20.
//  Reference: https://www.cnblogs.com/yxsrt/p/12201788.html, CLRS
//  Copyright © 2020 Erin Jane Ge. All rights reserved.
//

#include "RBTWithRank.hpp"

void RBT::inorderWalk(Node * p) const {
    if (p != nil) {
        inorderWalk(p->left);
        cout << p->key << ' ';
        inorderWalk(p->right);
    }
}

RBT::Node * RBT::search(Node * p, const int k) {
    if (p == nil || k == p->key)
        return p;
    if (k < p->key)
        return search(p->left, k);
    else
        return search(p->right, k);
}

RBT::Node * RBT::minimum(Node * p) {
    if (p == nil)
        return p;
    while (p->left != nil)
        p = p->left;
    return p;
}

RBT::Node * RBT::maximum(Node * p) {
    if (p == nil)
        return p;
    while (p->right != nil)
        p = p->right;
    return p;
}

int RBT::predecessor(const int k) {
    Node * p = search(root, k);
    if (p == nil)
        return 0;
    if (p->left != nil)
        return maximum(p->left)->key;
    Node * y = p->parent;
    while (y != nil && y->left == p) {
        p = y;
        y = y->parent;
    }
    return y->key;
}

int RBT::successor(const int k) {
    Node * p = search(root, k);
    if (p == nil)
        return 0;
    if (p->right != nil)
        return minimum(p->right)->key;
    Node * y = p->parent;
    while (y != nil && y->right == p) {
        p = y;
        y = y->parent;
    }
    return y->key;
}

void RBT::leftRotate(Node * x) { //assume:x->right != nil
    Node * y = x->right;
    x->right = y->left;
    if (y->left != nil)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
    y->size = x->size;
    x->size = x->right->size + x->left->size + 1;
}

void RBT::rightRotate(Node * x) { //assume:x->left != nil
    Node * y = x->left;
    x->left = y->right;
    if (y->right != nil)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nil)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
    y->size = x->size;
    x->size = x->right->size + x->left->size + 1;
}

void RBT::insert(Node * p) {
    if (p == nullptr)
        return ;
    Node * x = root;
    Node * y = nil;
    while (x != nil) {
        x->size++;
        y = x;
        if (x->key < p->key)
            x = x->right;
        else
            x = x->left;
    }
    p->parent = y;
    if (y == nil)
        root = p;
    else if (y->key < p->key)
        y->right = p;
    else
        y->left = p;
    fixup_insert(p);
}

void RBT::fixup_insert(Node * p) {
    while (p->parent->color == RED) {
        if (p->parent == p->parent->parent->left) {
            Node * y = p->parent->parent->right;
            if (y->color == RED) { //case 1
                p->parent->color = BLACK;
                y->color = BLACK;
                p->parent->parent->color = RED;
                p = p->parent->parent;
            }
            else {
                if (p == p->parent->right) { //case 2
                    p = p->parent;
                    leftRotate(p);
                }
                p->parent->color = BLACK; //case 3
                p->parent->parent->color = RED;
                rightRotate(p->parent->parent);
            }
        }
        else { //with "right" and "left" exchanged
            Node * y = p->parent->parent->left;
            if (y->color == RED) { //case 1
                p->parent->color = BLACK;
                y->color = BLACK;
                p->parent->parent->color = RED;
                p = p->parent->parent;
            }
            else {
                if (p == p->parent->left) { //case 2
                    p = p->parent;
                    rightRotate(p);
                }
                p->parent->color = BLACK; //case 3
                p->parent->parent->color = RED;
                leftRotate(p->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void RBT::transplant(Node * old_t, Node * new_t) {
    if (old_t->parent == nil)
        root = new_t;
    else if (old_t == old_t->parent->left)
        old_t->parent->left = new_t;
    else
        old_t->parent->right = new_t;
    new_t->parent = old_t->parent;
}

void RBT::fixup_remove(Node * x) {
    Node * t = x->parent;
    while (t != nil) {
        t->size--;
        t = t->parent;
    }
    Node * z = nil;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            z = x->parent->right;
            /*case 1*/  if (z->color == RED) {
                z->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                z = x->parent->right; //new z
            }
            /*case 2*/  if (z->left->color == BLACK && z->right->color == BLACK) {
                z->color = RED;
                x = x->parent;
            }
            else {
                /*case 3*/      if (z->right->color == BLACK) {
                    z->left->color = BLACK;
                    z->color = RED;
                    rightRotate(z);
                    z = x->parent->right;
                }
                /*case 4*/      z->color = x->parent->color;
                x->parent->color = BLACK;
                z->right->color = BLACK;
                leftRotate(x->parent);
                x = root; //exit while
            }
        }
        else {
            z = x->parent->left;
            /*case 1*/  if (z->color == RED) {
                z->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                z = x->parent->left; //new z
            }
            /*case 2*/  if (z->right->color == BLACK && z->left->color == BLACK) {
                z->color = RED;
                x = x->parent;
            }
            else {
                /*case 3*/      if (z->left->color == BLACK) {
                    z->right->color = BLACK;
                    z->color = RED;
                    leftRotate(z);
                    z = x->parent->left;
                }
                /*case 4*/      z->color = x->parent->color;
                x->parent->color = BLACK;
                z->left->color = BLACK;
                rightRotate(x->parent);
                x = root; //exit while
            }
        }
    }
    x->color = BLACK;
}

void RBT::erase(Node * p) {
    if (p == nil)
        return ;
    Node * y = p;
    Node * x = nil;
    bool y_originalColor = y->color;
    if (p->left == nil) {
        x = p->right;
        transplant(p, p->right);
    }
    else if (p->right == nil) {
        x = p->left;
        transplant(p, p->left);
    }
    else {
        y = minimum(p->right);
        y_originalColor = y->color;
        x = y->right;
        if (y->parent == p)
            x->parent = y;   //maybe x == nil
        else {
            transplant(y, y->right);
            y->right = p->right;
            y->right->parent = y;
        }
        transplant(p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }
    delete p;
    if (y_originalColor == BLACK)
        fixup_remove(x);
}

ostream &operator<<(ostream &os, const RBT &t) {
    t.inorderWalk(t.root);
    return os;
}
