#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Header.h"
#include <unordered_map>
#include <chrono>
////double haversine(double lat1, double lat2, double lon1, double lon2) {
//    double latdiff = (lat1 - lat2) * (3.14159265358979323846 / 180);
//    double longdiff = (lon1 - lon2) *(3.14159265358979323846 / 180);
//    lat1 = (lat1)* 3.14159265358979323846 / 180.0;
//    lat2 = (lat2)* 3.14159265358979323846 / 180.0;
//    double a = pow(sin(latdiff / 2), 2) + pow(sin(longdiff/ 2), 2) *cos(lat1) * cos(lat2);
//    double rad = 6371;
//    double c = 2 * asin(sqrt(a));
//    return (rad * c) / 1.609;
//}

struct RBNode {
	bool isBlack;
    Node* aHospital;
    int key;
	RBNode* parent;
	RBNode* left;
    RBNode* right;
    RBNode(Node* aHospital) {
        this->aHospital = aHospital;
        left = nullptr;
        right = nullptr;
        key = this->aHospital->overallRating;
    };
    RBNode() {
        right = nullptr;
        left = nullptr;
        aHospital = nullptr;
        isBlack = true;
    };
    RBNode& operator=(const RBNode& rhs) {
        this->right = rhs.right;
        this->left = rhs.left;
        this->isBlack = rhs.isBlack;
        this->aHospital = rhs.aHospital;
        this->key = rhs.key;
        this->parent = rhs.parent;
    }
    RBNode(const RBNode& rhs) {
        this->right = rhs.right;
        this->left = rhs.left;
        this->isBlack = rhs.isBlack;
        this->aHospital = rhs.aHospital;
        this->key = rhs.key;
        this->parent = rhs.parent;
    }
};
struct redBlackTree {
    RBNode* root;
    RBNode* rootParent;
    RBNode* rotateRight(RBNode* root);//O1
    RBNode* rotateLeft(RBNode* root);//O1
    void RBinsert(RBNode* node);//O(logn)
    void BSTinsert(RBNode* node);//O(logn)
    void traverseInorder(RBNode* root);//O(n)
    redBlackTree() { root = nullptr;

    rootParent = new RBNode();
    };
    void reverseInorder(RBNode* root,int &count,int treatment);

};
RBNode* redBlackTree::rotateRight(RBNode* root) {
    RBNode* grandchild = root->left->right;
    if (grandchild != nullptr) {
        grandchild->parent = root;
    }
    RBNode* newRoot = root->left;
    newRoot->right = root;
    newRoot->parent = root->parent;
    newRoot->parent->left = newRoot;
    root->left = grandchild;
    root->parent = newRoot;
    return newRoot;
}
RBNode* redBlackTree::rotateLeft(RBNode* root) {
    RBNode* grandchild = root->right->left;
    if (grandchild != nullptr) {
        grandchild->parent = root;
    }
    RBNode* newRoot = root->right;
    newRoot->left = root;
    newRoot->parent = root->parent;
    newRoot->parent->right = newRoot;
    root->right = grandchild;
    root->parent = newRoot;
    return newRoot;
}
void redBlackTree::BSTinsert(RBNode* node) {
    bool inserted = false;
    RBNode* temp = root;
    if (root == nullptr) {
        inserted = true;
        root = node;
        root->isBlack = true;
        root->parent = rootParent;
    }
    while (!inserted) {
        if (node->key < temp->key) {
            //check left chill, if null insert, else go left
            if (temp->left == nullptr) {
                temp->left = node;
                node->parent = temp;
                inserted = true;
            }
            else {
                temp = temp->left;
            }
        }
        else {
            if (temp->right == nullptr) {
                temp->right = node;
                node->parent = temp;
                inserted = true;
            }
            else {
                temp = temp->right;
            }
        }
    }
}
void redBlackTree::RBinsert(RBNode* node) {
    BSTinsert(node); //insert node normally
 
    node->isBlack = false;//color the inserted node red

    //check for violations
    //if parent is black, exit as tree is already valid
    //if not enter while
    while (!node->parent->isBlack) {
        //if parent is right child of grandparent, uncle is left
        RBNode* grandparent = node->parent->parent;
        if (node->parent == grandparent->right) {
            RBNode* uncle = grandparent->left;
            //check uncle color
            if (uncle == nullptr || uncle->isBlack) {
                //black, rotate
                
                if (node == node->parent->left) {//node is a left child
                    //make node its parent and rotate right
                    node = node->parent;
                    rotateRight(node);
                }
                //we now have a right right case, recolor then rotate left at grandparent
                node->parent->isBlack = true;
                node->parent->parent->isBlack = false;
                rotateLeft(node->parent->parent);
            }
            else {
                //red, flip
                uncle->isBlack = true;
                node->parent->isBlack = true;
                grandparent->isBlack = false;
                //set node to grandparent to check up tree
                node = grandparent;
            }

        }
        else {
            //parent is left child, uncle is on the right,symettric with other case
            RBNode* uncle = grandparent->right;
            //check uncle color
            if (uncle == nullptr || uncle->isBlack) {
                //black, rotate

                if (node == node->parent->right) {//node is a right child,left right case
                    //make node its parent and rotate left
                    node = node->parent;
                    rotateLeft(node);
                }
                //we now have a left left, recolor then rotate left at grandparent
                node->parent->isBlack = true;
                node->parent->parent->isBlack = false;
                rotateLeft(node->parent->parent);
            }
            else {
                //red, flip
                uncle->isBlack = true;
                node->parent->isBlack = true;
                grandparent->isBlack = false;
                //set node to grandparent to check up tree
                node = grandparent;
            }
        }
    }
    //color root black
    while (this->root->parent != rootParent) {
        this->root = this->root->parent;
    }
    root->isBlack = true;
}
void redBlackTree::traverseInorder(RBNode* root) {
    if (root == nullptr) {
        return;
    }

    if (root->left != nullptr) {
        traverseInorder(root->left);
    }

    if (root->right != nullptr) {
        traverseInorder(root->right);
    }
    return;
}
void redBlackTree::reverseInorder(RBNode* root,int& count,int treatment) {
    if (root == nullptr) {
        return;
    }
    if (root->right != nullptr) {
        reverseInorder(root->right, count,treatment);
    }
    count++;
    root->aHospital->print(root->aHospital,treatment);
    if (count >= 5) {
        return;
    }
    if (root->left != nullptr) {
        reverseInorder(root->left,count,treatment);
    }
    return;
}

bool isValidHospital(Node aNode, std::string userState, int treatment, double budget) {
    if (budget != 0) {


        if (treatment == 1) {//heart failure
            if (aNode.heartFailCost > budget) {
                return false;
            }

        }
        else if (treatment == 2) {//heart atk
            if (aNode.heartAttkCost > budget) {
                return false;
            }
        }
        else if (treatment == 3) {//hip/knee
            if (aNode.hipCost > budget) {
                return false;
            }
        }
        else if (treatment == 4) {//pnuemonia
            if (aNode.pneumCost > budget) {
                return false;
            }
        }
      
       
    }
    if (aNode.state != userState) {
        return false;
    }
 return true;
}

int main() {
    std::string userState;
    int treatment;
    double budget;
    //double maxDist;
    std::unordered_map<int, std::pair<double, double>> zipcodes;

    //get user input for zip code
    std::cout << "Please enter your State: " << std::endl;
    std::cin >> userState;
    //get user input for treatment type
    std::cout << "What kind of treatment are you looking for? Please choose an option from the list below." << std::endl;
    std::cout << "1. Heart Failure" << std::endl << "2. Heart Attack"
        << std::endl << "3. Hip/Knee Replacement" << std::endl << "4. Pnuemonia"
        << std::endl << "5. Other" << std::endl;

    std::cin >> treatment;
    //get user input for budget
    std::cout << "Do you have a treatment budget? If not, enter 0." << std::endl;
    std::cin >> budget;


//}

    redBlackTree* rbTree=new redBlackTree();
    //BPlusTree* bplus = new BPlusTree();
   
    std::vector<Node> Nodes= fileExtract(zipcodes);
   // insert valid Nodes based on criteria
    for (int i = 0; i < Nodes.size(); i++) {
        if (isValidHospital(Nodes[i],userState,treatment,budget)) {
            //bplus->insert(Nodes[i].hospital,Nodes[i].overallRating);
            RBNode* temp = new RBNode(&Nodes.at(i));
            rbTree->RBinsert(temp);
        }
    }
//

    std::vector<Node> Nodes = fileExtract(zipcodes);
   

    ////testing purposes
    using namespace std::chrono;
    
  /*  redBlackTree tree = redBlackTree();
    for (int i=0; i <= 100000; i++) {
        Node* temp = new Node();
        temp->overallRating = i;
        RBNode* node = new RBNode(temp);
        tree.RBinsert(node);
    }*/
    auto start = high_resolution_clock::now();
     
    int printCount = 0;
    rbTree->reverseInorder(rbTree->root, printCount,treatment);
    if (printCount > 0) {
        std::cout << "Here is the best hospital in your State based on your input." << std::endl;
    }
    else {
        std::cout << "There were no valid hospitals in your area."<< std::endl;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout <<"Red-Black operation time: " << duration.count() << endl;
    return 0;
}

