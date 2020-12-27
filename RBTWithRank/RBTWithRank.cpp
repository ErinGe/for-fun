//
//  RBTWithRank.cpp
//  RBTWithRank
//
//  Created by Erin Jane Ge on 12/26/20.
//  Reference: https://www.cnblogs.com/yxsrt/p/12201788.html, CLRS
//  Copyright © 2020 Erin Jane Ge. All rights reserved.
//

#include <iostream>
using namespace std;

template <class T>
class RBT {
    private :
    enum {RED = 0, BLACK};
    struct Node {
        T key;
        bool color;
        int size;
        Node * left;
        Node * right;
        Node * parent;
        Node(T k = -9999, bool c = BLACK, int s = 0, Node *l = nullptr, Node *r = nullptr, Node *p = nullptr)
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
    Node * search(Node * p, const T k);
    Node * minimum(Node * p);
    Node * maximum(Node * p);
    void inorderWalk(Node * p) const;
    Node * successor(Node * p);
    Node * predecessor(Node * p);
    int rank(Node * p);
    Node * index(Node * p, const int i);
    public :
    RBT() : nil(new Node), root(nil) {}
    ~RBT() {delete nil;}
    void insert(const T key) {insert(new Node(key, RED, 1, nil, nil, nil));}
    void erase(const T key) {erase(search(root, key));}
    bool search(const T key) {return (search(root, key) == nil ? false : true);}
    T minimum() {return minimum(root)->key;}
    T maximum() {return maximum(root)->key;}
    T predecessor(const T key);
    T successor(const T key);
    int * rank(const T key);
    T index(const int i) {return index(root, i)->key;}
    
    friend ostream &operator<<(ostream &os, const RBT &t);
};

template <class T> void RBT<T>::inorderWalk(Node * p) const {
    if (p != nil) {
        inorderWalk(p->left);
        cout << p->key << ' ';
        inorderWalk(p->right);
    }
}

template <class T> typename RBT<T>::Node * RBT<T>::search(Node * p, const T k) {
    if (p == nil || k == p->key)
        return p;
    if (k < p->key)
        return search(p->left, k);
    else
        return search(p->right, k);
}

template <class T> typename RBT<T>::Node * RBT<T>::minimum(Node * p) {
    if (p == nil)
        return p;
    while (p->left != nil)
        p = p->left;
    return p;
}

template <class T> typename RBT<T>::Node * RBT<T>::maximum(Node * p) {
    if (p == nil)
        return p;
    while (p->right != nil)
        p = p->right;
    return p;
}

template <class T> typename RBT<T>::Node * RBT<T>::predecessor(Node * p) {
    if (p->left != nil)
        return maximum(p->left);
    Node * y = p->parent;
    while (y != nil && y->left == p) {
        p = y;
        y = y->parent;
    }
    return y;
}

template <class T> T RBT<T>::predecessor(const T k) {
    Node * p = search(root, k);
    if (p == nil)
        return -9999;
    return predecessor(p)->key;
}

template <class T> typename RBT<T>::Node * RBT<T>::successor(Node * p) {
    if (p->right != nil)
        return minimum(p->right);
    Node * y = p->parent;
    while (y != nil && y->right == p) {
        p = y;
        y = y->parent;
    }
    return y;
}

template <class T> T RBT<T>::successor(const T k) {
    Node * p = search(root, k);
    if (p == nil)
        return -9999;
    return successor(p)->key;
}

template <class T> void RBT<T>::leftRotate(Node * x) { //assume:x->right != nil
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

template <class T> void RBT<T>::rightRotate(Node * x) { //assume:x->left != nil
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

template <class T> void RBT<T>::insert(Node * p) {
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

template <class T> void RBT<T>::fixup_insert(Node * p) {
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

template <class T> void RBT<T>::transplant(Node * old_t, Node * new_t) {
    if (old_t->parent == nil)
        root = new_t;
    else if (old_t == old_t->parent->left)
        old_t->parent->left = new_t;
    else
        old_t->parent->right = new_t;
    new_t->parent = old_t->parent;
}

template <class T> void RBT<T>::fixup_remove(Node * x) {
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

template <class T> void RBT<T>::erase(Node * p) {
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

template <class T> int RBT<T>::rank(Node * p) {
    int r = p->left->size + 1;
    while (p != root) {
        if (p == p->parent->right) {
            r = r + p->parent->left->size + 1;
        }
        p = p->parent;
    }
    
    return r;
}

template <class T> int * RBT<T>::rank(const T key) {
    Node * p = search(root, key);
    static int ranks[2]; // ranks[0]: lower_bound, ranks[1]: upper_bound
    if (p == nil) {
        ranks[0] = ranks[1] = -1;
        return ranks;
    }
    Node * pr = predecessor(p);
    Node * s = successor(p);
    Node * p_copy = p;
    while (pr->key == key) {
        p = pr;
        pr = predecessor(p);
    }
    ranks[0] = rank(p) - 1;
    while (s->key == key) {
        p_copy = s;
        s = successor(p_copy);
    }
    ranks[1] = rank(p_copy) - 1;
    
    return ranks;
}

template <class T> typename RBT<T>::Node * RBT<T>::index(Node * p, const int i) {
    if (p == nil)
        return p;
    int r = p->left->size;
    if (r == i)
        return p;
    else if (i < r)
        return index(p->left, i);
    else
        return index(p->right, i - r - 1);
}

template <class T> ostream &operator<<(ostream &os, const RBT<T> &t) {
    t.inorderWalk(t.root);
    return os;
}
