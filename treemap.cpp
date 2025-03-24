#include <iostream>
#include <string>
using namespace std;

// Key-Value Pair Class
class KeyValuePair {
public:
    string key, value;
    KeyValuePair(string k = "", string v = "") : key(k), value(v) {}

    bool operator<(const KeyValuePair& other) const {
        return key < other.key;
    }

    bool operator>(const KeyValuePair& other) const {
        return key > other.key;
    }

    bool operator==(const KeyValuePair& other) const {
        return key == other.key;
    }
    bool operator!=(const KeyValuePair& other) const { // Added this operator
        return key != other.key;
    }
};

// Node Structure
template <typename T>
struct Node {
    T data;
    Node<T>* left;
    Node<T>* right;
    Node(T value) : data(value), left(nullptr), right(nullptr) {}
};

// Splay Tree Class
template <typename T>
class SplayTree {
private:
    Node<T>* root;

    // Right Rotation
    Node<T>* rightRotate(Node<T>* node) {
        Node<T>* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    // Left Rotation
    Node<T>* leftRotate(Node<T>* node) {
        Node<T>* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    // Splay operation
    Node<T>* splay(Node<T>* node, T value) {
        if (!node || node->data == value) return node;

        if (value < node->data) {
            if (!node->left) return node;

            if (value < node->left->data) {
                node->left->left = splay(node->left->left, value);
                node = rightRotate(node);
            } else if (value > node->left->data) {
                node->left->right = splay(node->left->right, value);
                if (node->left->right)
                    node->left = leftRotate(node->left);
            }

            return (node->left) ? rightRotate(node) : node;
        } else {
            if (!node->right) return node;

            if (value > node->right->data) {
                node->right->right = splay(node->right->right, value);
                node = leftRotate(node);
            } else if (value < node->right->data) {
                node->right->left = splay(node->right->left, value);
                if (node->right->left)
                    node->right = rightRotate(node->right);
            }

            return (node->right) ? leftRotate(node) : node;
        }
    }

    // Insert into Splay Tree
    Node<T>* insert(Node<T>* node, T value) {
        if (!node) return new Node<T>(value);

        node = splay(node, value);
        if (node->data == value) return node;

        Node<T>* newNode = new Node<T>(value);
        if (value < node->data) {
            newNode->right = node;
            newNode->left = node->left;
            node->left = nullptr;
        } else {
            newNode->left = node;
            newNode->right = node->right;
            node->right = nullptr;
        }
        return newNode;
    }

    // Delete a node
    Node<T>* remove(Node<T>* node, T value) {
        if (!node) return nullptr;

        node = splay(node, value);
        if (node->data != value) return node;

        if (!node->left) {
            return node->right;
        } else {
            Node<T>* temp = node->right;
            node = splay(node->left, value);
            node->right = temp;
        }
        return node;
    }

    // Search and splay
    Node<T>* search(Node<T>* node, T value) {
        if (!node) return nullptr;
        node = splay(node, value);
        return (node->data == value) ? node : nullptr;
    }

public:
    SplayTree() : root(nullptr) {}

    void insert(T value) {
        root = insert(root, value);
    }

    bool search(T value) {
        root = search(root, value);
        return root && root->data == value;
    }

    void remove(T value) {
        root = remove(root, value);
    }

    string getValue(T value) {
        root = search(root, value);
        return (root && root->data == value) ? root->data.value : "";
    }
};

// TreeMap Class
class TreeMap {
private:
    SplayTree<KeyValuePair> tree;

public:
    void insert(const string& key, const string& value) {
        tree.insert(KeyValuePair(key, value));
    }

    string get(const string& key) {
        return tree.getValue(KeyValuePair(key, ""));
    }

    void remove(const string& key) {
        tree.remove(KeyValuePair(key, ""));
    }
};

// Main Function
int main() {
  TreeMap map;
 
  // Insert a number of key-value pairs into the tree map
  map.insert("keyOne", "valueOne");
  map.insert("keyTwo", "valueTwo");
  map.insert("keyThree", "valueThree");
 
  // Prints out valueOne
  std::cout << map.get("keyOne") << endl;
 
  // Prints out valueThree
  std::cout << map.get("keyThree") << endl;
 
  // Prints out an empty string or some default value of your choice
  std::cout << map.get("keyDoesNotExist") << endl;
 
  // Deletes the key-value pair from the tree map
  map.remove("keyOne");
  cout << map.get("keyOne") << endl;
  return 0;
}