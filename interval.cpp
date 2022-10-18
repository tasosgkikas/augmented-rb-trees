
// TODO: get everyhing ready for presentation

#ifndef _INTERVAL_CPP
#define _INTERVAL_CPP

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using key_t = int;

// int data
namespace Interval
{
    enum Color {red, black};
    
    // an interval [low, high],
    // low >= 0, high >= low
    struct interval 
    {
        key_t key, low, high;
        interval(key_t low, key_t high):
            key(low), low(low), high(high) {
                if (low < 0) 
                throw std::invalid_argument(" negative low");
                else if (high < low)
                throw std::invalid_argument(" high < low");
            }
        interval() {}
        bool overlap(interval& i) {
            return (
                  low <= i.high && 
                i.low <=   high
            );
        }
        bool operator==(interval& i) {
            return (
                low  == i.low && 
                high == i.high
            );
        }
    };

    std::ostream& operator<<(std::ostream& ostream, interval& i) {
        return ostream << "[" << i.low << ", " << i.high << "]";
    }

    key_t getMax(key_t a, key_t b)
    { return a > b ? a : b; }

    using nodePtr = class Node*;
    
    class Node
    {
        private:
            nodePtr left, right, p;
            static nodePtr nil;
            Color color;
            interval i;
            key_t max, key;

            Node(interval& i): 
                left(nil), right(nil), p(nil),
                i(i), max(i.high), key(i.low),
                color(red) {}

            Node():  // sentinel constructor
                left(this), right(this), p(this),
                color(black), max(-1) { nil = this; }

            void showAttrs() 
            {
                const char space = ' ';
                std::cout << std::endl << " i" 
                << space << "p" 
                << space << "left" 
                << space << "right" 
                << space << "color" 
                << space << "max";
            }

            void showVals() 
            {
                const char space = ' ';
                std::cout << std::endl << " " << this 
                << space << p 
                << space << left 
                << space << right
                << space << (color == red ? "red" : "black")
                << space << max;
            }
            
            void showAll() 
            {
                showAttrs();
                showVals();
            }

            void updateMax()
            {
                if (left != nil && right != nil)
                    max = getMax(i.high, getMax(left->max, right->max));
                else if (left != nil) 
                    max = getMax(i.high, left->max);
                else if (right != nil) 
                    max = getMax(i.high, right->max);
                else 
                    max = i.high;
            }

        friend class Tree;
        friend std::ostream& operator<<(std::ostream& ostream, nodePtr node);
    };

    nodePtr Node::nil = nullptr;

    std::ostream& operator<<(std::ostream& ostream, nodePtr node)
    {
        if (node == Node::nil) ostream << "nil";
        else ostream << node->i;
        return ostream;
    }
    
    class Tree
    {
        private:
            nodePtr root, nil;

            nodePtr successor(nodePtr node) 
            {
                if (node->right != nil) return treeMinimum(node->right);
                
                nodePtr parent = node->p;
                while (parent != nil && node == parent->right)
                {
                    node = parent;
                    parent = parent->p;
                }
                return parent;
            }

            nodePtr treeMinimum(nodePtr node) 
            {
                if (node->left == nil) return node;
                else return treeMinimum(node->left);
            }

            void transplant(nodePtr oldNode, nodePtr newNode)
            {
                if (oldNode->p == nil) root = newNode;
                else if (oldNode == oldNode->p->left) oldNode->p->left = newNode;
                else oldNode->p->right = newNode;
                newNode->p = oldNode->p;
            }

            void leftRot(nodePtr oldRoot)
            {
                nodePtr newRoot = oldRoot->right;
                oldRoot->right = newRoot->left;
                if (newRoot->left != nil)
                    newRoot->left->p = oldRoot;
                newRoot->p = oldRoot->p;
                if (oldRoot->p == nil)
                    root = newRoot;
                else if (oldRoot == oldRoot->p->left)
                    oldRoot->p->left = newRoot;
                else
                    oldRoot->p->right = newRoot;
                newRoot->left = oldRoot;
                oldRoot->p = newRoot;
                // max attribute maintenance
                newRoot->max = oldRoot->max;
                oldRoot->updateMax();
            }

            void rightRot(nodePtr oldRoot)
            {
                nodePtr newRoot = oldRoot->left;
                oldRoot->left = newRoot->right;
                if (newRoot->right != nil)
                    newRoot->right->p = oldRoot;
                newRoot->p = oldRoot->p;
                if (oldRoot->p == nil)
                    root = newRoot;
                else if (oldRoot == oldRoot->p->left)
                    oldRoot->p->left = newRoot;
                else
                    oldRoot->p->right = newRoot;
                newRoot->right = oldRoot;
                oldRoot->p = newRoot;
                // max attribute maintenance
                newRoot->max = oldRoot->max;
                oldRoot->updateMax();
            }
            
            void insertFixup(nodePtr node)
            {
                nodePtr uncle;
                while (node->p->color == red)
                {
                    if (node->p == node->p->p->left)
                    {
                        uncle = node->p->p->right;
                        if (uncle->color == red)
                        {
                            node->p->color = black;
                            uncle->color = black;
                            node->p->p->color = red;
                            node = node->p->p;
                        }
                        else 
                        {
                            if (node == node->p->right)
                            {
                                node = node->p;
                                leftRot(node);
                            }
                            node->p->color = black;
                            node->p->p->color = red;
                            rightRot(node->p->p);
                        }
                    }
                    else
                    {
                        uncle = node->p->p->left;
                        if (uncle->color == red)
                        {
                            node->p->color = black;
                            uncle->color = black;
                            node->p->p->color = red;
                            node = node->p->p;
                        }
                        else 
                        {
                            if (node == node->p->left)
                            {
                                node = node->p;
                                rightRot(node);
                            }
                            node->p->color = black;
                            node->p->p->color = red;
                            leftRot(node->p->p);
                        }
                    }
                }

                root->color = black;
            }

            void insert(nodePtr node) 
            {
                nodePtr parent = nil, aux = root;
                
                while (aux != nil)
                {
                    // max maintenance
                    if (aux->max < node->max) 
                        aux->max = node->max;

                    parent = aux;
                    if (node->key < aux->key) aux = aux->left;
                    else aux = aux->right;
                }
                node->p = parent;

                if (parent == nil) root = node;
                else if (node->key < parent->key) parent->left = node;
                else parent->right = node;

                insertFixup(node);
            }

            void removeFixup(nodePtr node)
            {
                while (node != root && node->color == black)
                {
                    nodePtr sibling;

                    if (node == node->p->left)
                    {
                        sibling = node->p->right;
                        if (sibling->color == red)
                        {
                            sibling->color = black;
                            node->p->color = red;
                            leftRot(node->p);
                            sibling = node->p->right;
                        }
                        if (sibling->left->color == black &&
                            sibling->right->color == black)
                        {
                            sibling->color = red;
                            node = node->p;
                        }
                        else
                        {
                            if (sibling->right->color == black)
                            {
                                sibling->left->color = black;
                                sibling->color = red;
                                rightRot(sibling);
                                sibling = node->p->right;
                            }
                            sibling->color = node->p->color;
                            node->p->color = black;
                            sibling->right->color = black;
                            leftRot(node->p);
                            node = root;
                        }
                    }
                    else
                    {
                        sibling = node->p->left;
                        if (sibling->color == red)
                        {
                            sibling->color = black;
                            node->p->color = red;
                            rightRot(node->p);
                            sibling = node->p->left;
                        }
                        if (sibling->right->color == black &&
                            sibling->left->color == black)
                        {
                            sibling->color = red;
                            node = node->p;
                        }
                        else
                        {
                            if (sibling->left->color == black)
                            {
                                sibling->right->color = black;
                                sibling->color = red;
                                leftRot(sibling);
                                sibling = node->p->left;
                            }
                            sibling->color = node->p->color;
                            node->p->color = black;
                            sibling->left->color = black;
                            rightRot(node->p);
                            node = root;
                        }
                    }
                }
                node->color = black;
            }

            void remove(nodePtr node) 
            {
                nodePtr sub1, sub2;

                Color color = node->color;

                if (node->left == nil)
                {
                    sub2 = node->right;
                    transplant(node, sub2);
                }
                else if (node->right == nil) 
                {
                    sub2 = node->left;
                    transplant(node, sub2);
                }
                else
                {
                    sub1 = treeMinimum(node->right);
                    color = sub1->color;
                    sub2 = sub1->right;
                    if (sub1->p == node)
                        sub2->p = sub1;
                    else
                    {
                        transplant(sub1, sub2);
                        sub1->right = node->right;
                        sub1->right->p = sub1;
                    }

                    transplant(node, sub1);
                    sub1->left = node->left;
                    sub1->left->p = sub1;
                    sub1->color = node->color;
                }

                // max attribute maintenance
                nodePtr aux = sub2;
                while (aux != root)
                {
                    aux = aux->p;
                    aux->updateMax();
                }

                if (color == black) removeFixup(sub2);

                delete node;
            }

            nodePtr search(interval& i, nodePtr node) 
            {
                if (node == nil || i == node->i) return node;
                if (i.key < node->key) 
                    return search(i, node->left);
                return search(i, node->right);
            }

            nodePtr search(key_t key, nodePtr node) 
            {
                if (node == nil || key == node->key) return node;
                if (key < node->key) 
                    return search(key, node->left);
                return search(key, node->right);
            }

            nodePtr overlap(interval& i, nodePtr node) 
            {
                if (node == nil || i.overlap(node->i)) 
                    return node;
                if (node->left == nil || node->left->max < i.low) 
                    return overlap(i, node->right);
                return overlap(i, node->left);
            }

            void inorderPrint(nodePtr node) 
            {
                if (node == nil) return;
                
                inorderPrint(node->left);
                std::cout << (node == root ? " " : ", ") << node;
                inorderPrint(node->right);
            }

            void graph(nodePtr node, unsigned level = 0) 
            {
                if (node == nil) return;

                unsigned spaces = 3;

                graph(node->right, level+1);

                std::cout << std::endl;
                if (node == root) std::cout << ">";
                for (unsigned i = 0; i < spaces*level + 1; i++) std::cout << " ";
                std::cout << node;

                graph(node->left, level+1);
            }

        public:
            Tree()
            {
                nil = new Node();
                root = nil;
            }

            ~Tree()
            {
                remove();
                delete nil;
                std::cout << "\n Tree deleted" << std::endl;
            }

            bool insert(interval i, bool log = true) 
            {
                if (search(i.key, root) == nil)
                {
                    try {
                        insert(new Node(i));

                        if (search(i, root) == nil)
                            throw i;
                            
                        if (log) std::cout << std::endl << " "
                            << i << " inserted" << std::endl;
                            
                        return true;
                    }
                    catch (interval error_i) {
                        std::cout << " Error at insertion of " 
                        << error_i << std::endl
                        << " press enter to continue";
                        std::cin.get();
                        return false;
                    }
                }
                else 
                {
                    if (log) std::cout << std::endl << " "
                    << i.key << " key already exists" << std::endl;

                    return false;
                }       
            }

            void remove(bool log = true) 
            {
                while (root != nil) remove(root->i, log);
            }

            void remove(interval& i, bool log = true) 
            {
                nodePtr node = search(i, root);
                if (node != nil) {
                    try {
                        remove(node);
                        
                        if (search(i, root) != nil)
                            throw i;

                        if (log) std::cout << std::endl << " "
                            << i << " deleted" << std::endl;
                    }
                    catch (interval error_i) {
                        std::cout << " Error at deletion of " 
                        << error_i << std::endl
                        << " press enter to continue";
                        std::cin.get();
                        timeline();
                        graph();
                        showNodes();
                    }
                }
                else if (log) std::cout << std::endl << " "
                    << i << " does not exist" << std::endl;
            }

            void search(interval& i) 
            {
                nodePtr node = search(i, root);

                std::cout << std::endl 
                << " " << i << " "
                << ((node != nil) ? "" : "not ")
                << "found" << std::endl;
            }

            nodePtr overlap(interval& i, bool log = true) 
            {
                nodePtr node = overlap(i, root);

                if (log) {
                std::cout << std::endl;
                if (node == nil)
                    std::cout << " No interval overlapping " << i;
                else
                    std::cout << " Interval overlapping " << i << ": " << node ;
                std::cout << std::endl;
                }
                return (node != nil ? node : nullptr);
            }

            void inorderPrint()
            {
                std::cout << std::endl;
                if (root == nil) std::cout << " The tree is empty";
                else inorderPrint(root);
                std::cout << std::endl;
            }

            std::vector<nodePtr> inorderAccess(nodePtr node = nullptr) 
            {
                std::vector<nodePtr> returnVec, right;

                if (node == nullptr) node = root; // default init
                if (node == nil) return returnVec; // guard clause
                
                returnVec = inorderAccess(node->left);
                returnVec.push_back(node);
                right = inorderAccess(node->right);
                returnVec.insert(returnVec.end(), right.begin(), right.end());

                return returnVec;
            }

            void showNodes() 
            {
                nil->showAll();
                for (nodePtr node : inorderAccess()) 
                    node->showVals();
            }

            void graph()
            {
                std::cout << std::endl;
                if (root == nil) std::cout << " The tree is empty";
                else graph(root);
                std::cout << std::endl;
            }

            int height(nodePtr node = nullptr) 
            {
                if (node == nullptr) node = root; // default init
                if (node == nil) return -1; // guard clause

                int h_left  = height(node->left);
                int h_right = height(node->right);

                return getMax(h_left, h_right) + 1;
            }

            void timeline()
            {
                for (auto node : inorderAccess())
                {
                    std::cout << std::endl << " ";

                    key_t low = node->i.low;
                    key_t high = node->i.high;

                    for (int i = 0; i < low; i++) 
                        std::cout << " ";
                    std::cout << low;

                    int width = high - low + 1;
                    
                    for (int i = 0; i < width; i++)
                        std::cout << "|";
                    std::cout << high;
                }
            }
            
            unsigned population(nodePtr node = nullptr)
            {
                if (node == nullptr) node = root; // default init
                if (node == nil) return 0; // guard clause
                return 1 + population(node->left) + population(node->right);
            }
    };
};

#endif
