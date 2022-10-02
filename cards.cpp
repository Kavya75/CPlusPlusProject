//cards.cpp
//Authors: Kavya Verma
//Implementation of the classes defined in cards.h

#include <iostream>
using namespace std;

#include "cards.h"
#include <vector>

// Default constructor: Initializes root as nullptr
Cards::Cards() : root(0) {}

Cards::~Cards() {
    clear(root);
}

void Cards::clear(Node *n) {
    if (n) {
	    clear(n->left);
	    clear(n->right);
	    delete n;
    }
}

bool Cards::insert(const Card& c) {
    if (!root) {
	    root = new Node(c);
	    return true;
    }
    return insert(c, root);
}

bool Cards::insert(const Card& card, Node *n) {
    if (card == n->info)
	    return false;
    if (card < n->info) {
        if (n->left)
            return insert(card, n->left);
        else {
            n->left = new Node(card);
            n->left->parent = n;
            return true;
        }
    }
    else {
        if (n->right)
            return insert(card, n->right);
        else {
            n->right = new Node(card);
            n->right->parent = n;
            return true;
        }
    }
}

int Cards::getRootInfo() const {
    return root->info.getSuit();
}

Cards::Node* Cards::getNodeFor(Card card, Node* n) const {
    if (!n) return nullptr; // if list is empty
    if (n->info == card) return n;
    if (getNodeFor(card, n->left)) 
        return getNodeFor(card, n->left);
    if (getNodeFor(card, n->right))
        return getNodeFor(card, n->right);
    return nullptr;
}

Cards::Node* Cards::getPredecessorNode(Card card) const{
    return getNodeFor(getPredecessor(card), root);
}

Card Cards::getPredecessor(const Card& card) const {
    Node* n = getNodeFor(card, root);
    // if n has a left subtree
    if (!n) return Card(0, 0);
    Node* temp = n->left;
    if (temp) {
        while(temp->right) {
            temp = temp->right;
        }
        return temp->info;
    }
    else { // no left subtree
        temp = n;
        while (temp->parent && temp->parent->info > n->info) {
            temp = temp->parent;
        }
        if (temp->parent && temp->parent->info < n->info)
            return temp->parent->info;
    }
    return Card(0, 0);
}

// returns the Node containing the successor of the given value
Cards::Node* Cards::getSuccessorNode(const Card& card) const{
    return getNodeFor(getSuccessor(card), root);
}

// returns the successor value of the given value or 0 if there is none
Card Cards::getSuccessor(const Card& card) const{
    Node * n = getNodeFor(card, root);
    // if n has a right subtree
    if (!n) return Card(0, 0);
    Node * temp = n->right;
    if (temp) {
        while(temp->left) {
            temp = temp->left;
        }
        return temp->info;
    }
    else { // no right subtree
        temp = n;
        while (temp->parent) {
            temp = temp->parent;
            if (temp->info > n->info) {
                return temp->info;
            }
        }
    }
    return Card(0, 0);
}

bool Cards::remove(Card card){
   Node* parent = nullptr;
   Node* curr = root;
      
   // Search for the node.
   while (curr) {
      // Check if curr has a matching value.
      if (curr->info == card) {
         if (curr->left == nullptr && curr->right == nullptr) { // Case 1
            if (parent == nullptr) { // Node is root
               root = nullptr;
            }
            else if (parent->left == curr) { 
               parent->left = nullptr;
            }
            else {
               parent->right = nullptr;
            }
            delete curr;
            return true; // Node found and removed
         }
         else if (curr->left && curr->right == nullptr) { // Case 2
            if (parent == nullptr) { // Node is root
               root = curr->left;
            }
            else if (curr->parent->info > curr->info) {
                curr->parent->left = curr->left;
            } else {
                curr->parent->right = curr->left;
            }
            curr->left->parent = curr->parent;
            // else if (parent->left == curr) {
            //    parent->left = curr->left;
            // }
            // else {
            //    parent->right = curr->left;
            // }
            delete curr;
            return true; // Node found and removed
         }
         else if (curr->left == nullptr && curr->right) { // Case 2
            if (parent == nullptr) { // Node is root
               root = curr->right;
            }
            else if (parent->left == curr) {
               parent->left = curr->right;
            }
            else {
               parent->right = curr->right;
            }
            curr->right->parent = curr->parent;
            delete curr;
            return true; // Node found and removed
         }
         else { // Case 3
            // Find successor (leftmost child of right subtree)
            Node* successor = getSuccessorNode(curr->info);
            curr->info = successor->info; // Copy successor's value to current node
            parent = curr;
               
            // Reassigned curr and value so that loop continues with new value
            curr = curr->right;
            card = successor->info;
         }
      }
      else if (curr->info < card) { // Search right
         parent = curr;
         curr = curr->right;
      }
      else { // Search left
         parent = curr;
         curr = curr->left;
      }
   }
   return false; // Node not found
}

string Cards::find(const Card& card) {
    Node* c = findHelper(card, root);
    if (!c) return "";
    string suitString = to_string(c->info.getSuit());
    string valueString = to_string(c->info.getValue());
    return "suit: " + suitString + " value: " + valueString;
}

Cards::Node* Cards::findHelper(const Card& card, Node* curr) {
    if (curr) {
        if (curr->info == card) {
            return curr;
        } else if (card < curr->info) {
            return findHelper(card, curr->left);
        } else if (card > curr->info) {
            return findHelper(card, curr->right);
        }
    }
    return nullptr;
}

Card Cards::getMinCard() {
    Node * temp = root;
    if (temp) {
        while(temp->left) {
            temp = temp->left;
        }
    }
    return temp->info;
}

Card Cards::getMaxCard() {
    Node * temp = root;
    if (temp) {
        while(temp->right) {
            temp = temp->right;
        }
    }
    return temp->info;
}

bool Cards::contains(const Card& card) const {
    Node * n = getNodeFor(card, root);
    if (!n) return false;
    return true;
}

void Cards::aliceTurn(Cards& bobCards, const Card& currCard, bool& sameEncountered) {
    if (bobCards.contains(currCard)) {
        cout << "Alice picked matching card " << numToSuit(currCard.getSuit()) << " ";
        if (currCard.getValue() == 1 || currCard.getValue() >= 11) {
            cout << numToValue(currCard.getValue()) << endl;
        } else {
            cout << currCard.getValue() << endl;
        }
        
        // Remove cards from both bsts
        Card temp1 = currCard;
        Card temp2 = currCard;
        remove(temp1);
        bobCards.remove(temp2);

        sameEncountered = true;
    }
}

void Cards::bobTurn(Cards& aliceCards, const Card& currCard, bool& sameEncountered) {
    if (aliceCards.contains(currCard)) {
        cout << "Bob picked matching card " << numToSuit(currCard.getSuit()) << " ";
        if (currCard.getValue() == 1 || currCard.getValue() >= 11) {
            cout << numToValue(currCard.getValue()) << endl;
        } else {
            cout << currCard.getValue() << endl;
        }

        // Remove cards from both bsts
        Card temp1 = currCard;
        Card temp2 = currCard;
        remove(temp1);
        aliceCards.remove(temp2);

        sameEncountered = true;
    }
}

string Cards::numToSuit(int num) {
  if (num == 1) {
      return "c";
  } else if (num == 2) {
      return "d";
  } else if (num == 3) {
      return "s";
  }
  return "h";
}

string Cards::numToValue(int num) {
    if (num == 1) {
      return "a";
    } else if (num == 11) {
      return "j";
    } else if (num == 12) {
      return "q";
    }
    return "k";
}


// For testing
void Cards::printInOrder() {
    printInOrder(root);
}
void Cards::printInOrder(Node *n) {
    // IMPLEMENT HERE
    if (!n) return;
    printInOrder(n->left);

    // Print info
    string suitString = numToSuit(n->info.getSuit());
    cout << suitString << " ";
    if (n->info.getValue() == 1 || n->info.getValue() >= 11) {
            cout << numToValue(n->info.getValue()) << endl;
        } else {
            cout << n->info.getValue() << endl;
    }

    printInOrder(n->right);
}

void Cards::printInOrderBeforeChange() {
    printInOrderBeforeChange(root);
}
void Cards::printInOrderBeforeChange(Node *n) {
    // IMPLEMENT HERE
    if (!n) return;
    printInOrderBeforeChange(n->left);
    // Print info
    cout << n->info.getSuit() << " " << n->info.getValue() << endl;
    printInOrderBeforeChange(n->right);
}

