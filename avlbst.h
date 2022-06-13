#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    virtual void RotateRight(AVLNode<Key,Value>* g);
    virtual void LeftRotate(AVLNode<Key,Value>* g);
    virtual void removeFix(AVLNode<Key,Value>* n, int diff);

};

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this -> root_ == NULL){
        this -> root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode <Key, Value> * nnode;

    AVLNode <Key, Value> * temp = static_cast<AVLNode<Key,Value>*>(this -> root_);

    while(true){

            if((new_item.first) ==  (temp -> getKey())){

                return;

            }else if((new_item.first) >  (temp -> getKey())){
                
                if(temp -> getRight() != NULL){
                    temp = temp -> getRight();
                }else{

                    temp -> setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, temp));
                    nnode = temp -> getRight();

                    break;
                }

            }else{

                if(temp -> getLeft() != NULL){
                    temp = temp -> getLeft();
                }else{

                    temp -> setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, temp));
                    nnode = temp -> getLeft();

                    break;
                }

            }

        }

        if(temp -> getBalance() == -1){

            temp -> setBalance(0);

        }else if(temp -> getBalance() == 1){

            temp -> setBalance(0);

        }else if(temp -> getBalance() == 0){
            
            if(temp -> getRight() == nnode){

                temp -> setBalance(1);

                insertFix(temp, nnode);

            }else if(temp -> getLeft() == nnode){

                temp -> setBalance(-1);

                insertFix(temp, nnode);

            }



        }



}

template<class Key, class Value>
void AVLTree<Key, Value>:: insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n){

    if((p == NULL) || (p -> getParent() == NULL)){

        return;

    }

    AVLNode<Key,Value>* g = p -> getParent();

    if(p -> getLeft() == n){

        if(g -> getLeft() == p){
            g -> setBalance((g -> getBalance()) - 1);
        }else if(g -> getRight() == p){
            g -> setBalance((g -> getBalance()) + 1);
        }

    }else if(p -> getRight() == n){

        if(g -> getLeft() == p){
            g -> setBalance((g -> getBalance()) - 1);
        }else if(g -> getRight() == p){
            g -> setBalance((g -> getBalance()) + 1);
        }

    }

    if(g -> getBalance() == 0){
        return;
    }else if (g -> getBalance() == -1){

        insertFix(g, p);

    }else if(g -> getBalance() == 1){

        insertFix(g, p);

    }else if(g -> getBalance() == -2){

        if(p -> getBalance() == -1){

            RotateRight(g);
            g -> setBalance(0);
            p -> setBalance(0);

        }else if(p -> getBalance() == 1){

            LeftRotate(p);
            RotateRight(g);

            if(n -> getBalance() == -1){
                p -> setBalance(0);
                g -> setBalance(1);
                n -> setBalance(0);
            }else if(n -> getBalance() == 0){
                p -> setBalance(0);
                g -> setBalance(0);
                n -> setBalance(0);
            }else if(n -> getBalance() == 1){
                p -> setBalance(-1);
                g -> setBalance(0);
                n -> setBalance(0);
            }

        }

    }else if( g -> getBalance() == 2){

        if(p -> getBalance() == 1){

            LeftRotate(g);
            g -> setBalance(0);
            p -> setBalance(0);

        }else if(p -> getBalance() == -1){

            RotateRight(p);
            LeftRotate(g);

            if(n -> getBalance() == 1){
                p -> setBalance(0);
                g -> setBalance(-1);
                n -> setBalance(0);
            }else if(n -> getBalance() == 0){
                p -> setBalance(0);
                g -> setBalance(0);
                n -> setBalance(0);
            }else if(n -> getBalance() == -1){
                p -> setBalance(1);
                g -> setBalance(0);
                n -> setBalance(0);
            }

        }

    }

}

template<class Key, class Value>
void AVLTree<Key, Value>:: RotateRight(AVLNode<Key,Value>* g){

    AVLNode<Key,Value>* temp = (g -> getLeft()) -> getRight();
    AVLNode<Key,Value>* parent = g -> getLeft();

    if(g -> getParent() != NULL){

        AVLNode<Key,Value>* ultraparent = g -> getParent();

        if(ultraparent -> getRight() == g){

            ultraparent -> setRight(parent);

        }else if(ultraparent -> getLeft() == g){

            ultraparent -> setLeft(parent);

        }

    }else if(g -> getParent() == NULL){

        this -> root_ = parent;

    }
    parent -> setParent(g -> getParent());

    parent -> setRight(g);
    g -> setLeft(temp);
    if(temp != NULL){
        temp -> setParent(g);
    }
    g -> setParent(parent);

}


template<class Key, class Value>
void AVLTree<Key, Value>:: LeftRotate(AVLNode<Key,Value>* g){

    AVLNode<Key,Value>* temp = (g -> getRight()) -> getLeft();
    AVLNode<Key,Value>* parent = g -> getRight();

    if(g -> getParent() != NULL){

        AVLNode<Key,Value>* ultraparent = g -> getParent();

        if(ultraparent -> getRight() == g){

            ultraparent -> setRight(parent);

        }else if(ultraparent -> getLeft() == g){

            ultraparent -> setLeft(parent);

        }

    }else if(g -> getParent() == NULL){

        this -> root_ = parent;

    }
    parent -> setParent(g -> getParent());

    parent -> setLeft(g);
    g -> setRight(temp);
    if(temp != NULL){
        temp -> setParent(g);
    }
    g -> setParent(parent);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    //this -> print();
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key,Value>*>(this -> internalFind(key));
    AVLNode<Key, Value>* temp2;
    int diff;

    if(temp == NULL){

        return;

    }

    // AVLNode<Key,Value>* p = temp -> getParent();
    // int diff;
    // if(p != NULL){

    //     if(p -> getLeft() == temp){
    //         diff = 1;
    //     }else if(p -> getRight() == temp){
    //         diff = -1;
    //     }
    // }
    
    while(1){
        if((temp -> getRight() == NULL) && (temp -> getLeft() == NULL)){

            if(temp != this -> root_){

                auto parentnode = temp -> getParent();

                if(parentnode -> getLeft() == temp){
                    diff = 1;
                }else if(parentnode -> getRight() == temp){
                    diff = -1;
                }
            

                if(parentnode -> getRight() == temp){

                    parentnode -> setRight(NULL);

                }else if(parentnode -> getLeft() == temp){

                    parentnode -> setLeft(NULL);
                    
                }
            }else{
                this -> root_ = NULL;
            }

            // delete temp;
            // return;
            break;

        }else if((temp -> getRight() == NULL) && (temp -> getLeft() != NULL)){

            if(temp == this -> root_){
                this -> root_ = temp -> getLeft();
                (temp -> getLeft()) -> setParent(NULL);
                // delete temp;
                // return;
                break;
            }

            temp2 = temp -> getParent();

            if(temp2 -> getLeft() == temp){
                diff = 1;
            }else if(temp2 -> getRight() == temp){
                diff = -1;
            }

            if(temp2 -> getLeft() == temp){

                temp2 -> setLeft(temp -> getLeft());
                (temp -> getLeft()) -> setParent(temp2);

            }else if(temp2 -> getRight() == temp){

                temp2 -> setRight(temp -> getLeft());
                (temp -> getLeft()) -> setParent(temp2);
            }
            // delete temp;
            // return;
            break;

        }else if((temp -> getRight() != NULL) && (temp -> getLeft() == NULL)){

            if(temp == this -> root_){
                this -> root_ = temp -> getRight();
                (temp -> getRight()) -> setParent(NULL);
                // delete temp;
                // return;
                break;
            }

            temp2 = temp -> getParent();

            if(temp2 -> getLeft() == temp){
                diff = 1;
            }else if(temp2 -> getRight() == temp){
                diff = -1;
            }

            if(temp2 -> getLeft() == temp){

                temp2 -> setLeft(temp -> getRight());
                (temp -> getRight()) -> setParent(temp2);

            }else if(temp2 -> getRight() == temp){

                temp2 -> setRight(temp -> getRight());
                (temp -> getRight()) -> setParent(temp2);
            }
            
            // delete temp;
            // return;
            break;

        }else{

            nodeSwap(temp, static_cast<AVLNode<Key,Value>*>(this -> predecessor(temp)));

            auto parentnode = temp -> getParent();

            if(parentnode != NULL){

                if(parentnode -> getLeft() == temp){
                    diff = 1;
                }else if(parentnode -> getRight() == temp){
                    diff = -1;
                }
            }

            if((temp -> getRight() == NULL) && (temp -> getLeft() == NULL)){

               if(parentnode -> getRight() == temp){

                    parentnode -> setRight(NULL);

               }else if(parentnode -> getLeft() == temp){

                   parentnode -> setLeft(NULL);

               }

                // delete temp;
                // return;
                break;

            }else if((temp -> getRight() == NULL) && (temp -> getLeft() != NULL)){

                temp2 = temp -> getParent();

                if(temp2 -> getLeft() == temp){

                    temp2 -> setLeft(temp -> getLeft());
                    (temp -> getLeft()) -> setParent(temp2);

                }else if(temp2 -> getRight() == temp){

                    temp2 -> setRight(temp -> getLeft());
                    (temp -> getLeft()) -> setParent(temp2);
                    
                }
                // delete temp;
                // return;
                break;

            }else if((temp -> getRight() != NULL) && (temp -> getLeft() == NULL)){ // I dont think this is possible

                temp2 = temp -> getParent();

                if(temp2 -> getLeft() == temp){

                    temp2 -> setLeft(temp -> getRight());
                    (temp -> getRight()) -> setParent(temp2);

                }else if(temp2 -> getRight() == temp){

                    temp2 -> setRight(temp -> getRight());
                    (temp -> getRight()) -> setParent(temp2);
                    
                }

                // delete temp;
                // return;
                break;
        }

        }
    }
    AVLNode<Key,Value>* p = temp -> getParent();
    // int diff = 0;
    // if(p != NULL){

    //     if(p -> getLeft() == temp){
    //         diff = 1;
    //     }else if(p -> getRight() == temp){
    //         diff = -1;
    //     }
    // }

    delete temp;

    removeFix(p, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key,Value>* n, int diff){

    //AVLNode<Key,Value>* c;

    if(n == NULL){
        return;
    }

    AVLNode<Key,Value>* p = n -> getParent();

    int ndiff;

    if(n -> getParent() != NULL){

        //AVLNode<Key,Value>* p = n -> getParent();

        if(p -> getLeft() == n){
            ndiff = 1;
        }else if (p -> getRight() == n){
            ndiff = -1;
        }
        //removeFix(p, diff);
    }

    if(n -> getBalance() + diff == -2){

        AVLNode<Key,Value>* c = n -> getLeft();

        if(c -> getBalance() == -1){

            RotateRight(n);
            n -> setBalance(0);
            c -> setBalance(0);
            removeFix(p, ndiff);

        }else if(c -> getBalance() == 0){

            RotateRight(n);
            n -> setBalance(-1);
            c -> setBalance(1);

        }else if(c -> getBalance() == 1){

            AVLNode<Key,Value>* g = c -> getRight();

            LeftRotate(c);
            RotateRight(n);
            if(g -> getBalance() == 1){
                n -> setBalance(0);
                c -> setBalance(-1);
                g -> setBalance(0);
            }else if(g -> getBalance() == 0){
                n -> setBalance(0);
                c -> setBalance(0);
                g -> setBalance(0);
            }else if(g -> getBalance() == -1){
                n -> setBalance(1);
                c -> setBalance(0);
                g -> setBalance(0);
            }
            removeFix(p, ndiff);
        }
    }else if(n -> getBalance() + diff == -1){

        n -> setBalance(-1);

    }else if(n -> getBalance() + diff == 0){

        n-> setBalance(0);
        removeFix(p, ndiff);

    }else if(n -> getBalance() + diff == 2){

        AVLNode<Key,Value>* c = n -> getRight();

        if(c -> getBalance() == 1){

            LeftRotate(n);
            n -> setBalance(0);
            c -> setBalance(0);
            removeFix(p, ndiff);

        }else if(c -> getBalance() == 0){

            LeftRotate(n);
            n -> setBalance(1);
            c -> setBalance(-1);

        }else if(c -> getBalance() == -1){

            AVLNode<Key,Value>* g = c -> getLeft();
            RotateRight(c);
            LeftRotate(n);
            if(g -> getBalance() == -1){
                n -> setBalance(0);
                c -> setBalance(1);
                g -> setBalance(0);
            }else if(g -> getBalance() == 0){
                n -> setBalance(0);
                c -> setBalance(0);
                g -> setBalance(0);
            }else if(g -> getBalance() == 1){
                n -> setBalance(-1);
                c -> setBalance(0);
                g -> setBalance(0);
            }
            removeFix(p, ndiff);
        }

    }else if(n -> getBalance() + diff == 1){

        n -> setBalance(1);

    }



}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
