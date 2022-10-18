
#ifndef _ORDER_STATISTIC_CPP
#define _ORDER_STATISTIC_CPP

#include <iostream>
#include <string>
#include <vector>

using key_t = int;

// Order Statistic, int data
namespace OS
{
    enum Color {red, black};
    
    using nodePtr = class Node*;
    
    class Node
    {
        private:
            nodePtr left, right, p;
            static nodePtr nil;
            key_t key;
            Color color;
            unsigned size;

            Node(key_t k): 
                left(nil), right(nil), p(nil),
                key(k), color(red), size(1) {}

            Node():  // sentinel constructor
                left(this), right(this), p(this),
                color(black), size(0) { nil = this; }

            void showAttrs() 
            {
                std::cout << std::endl <<
                " key\tp\tleft\tright\tcolor\tsize";
            }

            void showVals() 
            {
                std::cout << std::endl 
                << " " << this 
                << "\t" << p 
                << "\t" << left 
                << "\t" << right
                << "\t" << (color == red ? "red" : "black")
                << "\t" << size;
            }
            
            void showAll() 
            {
                showAttrs();
                showVals();
            }


        friend class Tree;
        friend std::ostream& operator<<(std::ostream& ostream, nodePtr node);
    };

    nodePtr Node::nil = nullptr;

    std::ostream& operator<<(std::ostream& ostream, nodePtr node)
    {
        if (node == Node::nil) ostream << "nil";
        else ostream << node->key;
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

                // size attribute maintenance
                newRoot->size = oldRoot->size;
                oldRoot->size = oldRoot->left->size 
                              + oldRoot->right->size 
                              + 1;
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

                // size attribute maintenance
                newRoot->size = oldRoot->size;
                oldRoot->size = oldRoot->left->size 
                              + oldRoot->right->size 
                              + 1;
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
                    aux->size += 1;  // size maintenance
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

                // size maintenance
                nodePtr aux = sub2;
                while (aux != root)
                {
                    aux = aux->p;
                    aux->size = aux->left->size + aux->right->size + 1;
                }

                if (color == black) removeFixup(sub2);

                delete node;
            }

            nodePtr search(key_t key, nodePtr node) 
            {
                if (node == nil || key == node->key) return node;
                if (key < node->key) return search(key, node->left);
                return search(key, node->right);
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
                for (int i = 0; i < level*spaces + 1; i++) std::cout << " ";
                std::cout << node;
                // for (int i = 0; i < (height()-level)*spaces; i++) std::cout << "-";

                graph(node->left, level+1);
            }

            nodePtr select(nodePtr node, unsigned i)
            {
                if (node == nil) return nil;

                unsigned r = node->left->size + 1;

                if (i == r) return node;
                if (i < r) return select(node->left, i);
                else return select(node->right, i - r);
            }

            unsigned rank_iterative(nodePtr node)
            {
                unsigned r = node->left->size + 1;
                
                while (node != root) {
                    if (node == node->p->right)
                        r += node->p->left->size + 1;
                    node = node->p;
                }
                return r;
            }

            unsigned rank_recursive(nodePtr node, bool first = true)
            {
                unsigned rank;

                if (node == root)
                    rank = 1;
                else {
                    rank = rank_recursive(node->p, false);
                    if (node == node->p->right)
                        rank += 1 + node->p->left->size;
                }
                if (first) rank += node->left->size;
                return rank;
            }

            unsigned rank_top_down(key_t key, nodePtr node)
            {
                if (key == node->key) return node->left->size + 1;
                if (key < node->key) return rank_top_down(key, node->left);
                return 1 + node->left->size + rank_top_down(key, node->right);
            }

        public:
            Tree()
            {
                nil = new Node();
                root = nil;
            }

            ~Tree()
            {
                remove(false);
                delete nil;
                std::cout << "\n Tree deleted" << std::endl;
            }

            void insert(key_t key, bool log = true) 
            {
                if (search(key, root) == nil) {
                    insert(new Node(key));

                    if (search(key, root) == nil)
                        throw key;
                        
                    if (log) std::cout << std::endl << " "
                        << key << " inserted" << std::endl;
                }
                else if (log) std::cout << std::endl << " "
                    << key << " already exists" << std::endl;
                    
            }

            void remove(bool log = true) 
            {
                while (root != nil) remove(root->key, log);
            }

            void remove(key_t key, bool log = true) 
            {
                nodePtr node = search(key, root);
                if (node != nil) {
                    remove(node);
                    
                    if (search(key, root) != nil)
                        throw key;

                    if (log) std::cout << std::endl << " "
                        << key << " deleted" << std::endl;
                }
                else if (log) std::cout << std::endl << " "
                    << key << " does not exist" << std::endl;
            }

            void search(key_t key) 
            {
                nodePtr node = search(key, root);

                std::cout << std::endl
                << " " << key << " "
                << ((node != nil) ? "" : "not ")
                << "found" << std::endl;
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

                return (h_left > h_right ? h_left : h_right) + 1;
            }

            unsigned population(nodePtr node = nullptr)
            {
                if (node == nullptr) node = root; // default init
                if (node == nil) return 0; // guard clause
                return 1 + population(node->left) + population(node->right);
            }

            key_t select(unsigned i = 1)
            {
                nodePtr node = select(root, i);
                if (node == nil) return -1;
                else return node->key;
            }

            int rank(key_t key)
            {
                if (search(key, root) == nil)
                    return -1;
                return rank_top_down(key, root);
            }
    };
};

#endif
