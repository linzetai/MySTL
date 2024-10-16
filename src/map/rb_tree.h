#pragma once
#include <iostream>
#include <stack>
#include "object_pool.h"
template <typename Key, typename Value>
struct Node {
    Key key{};
    Value value{};
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    bool color = true; // true for red, false for black

    Node() = default;
    // Node(const Key& k, const Value& v) 
    //     : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr), color(true) {}
};

template <typename Key, typename Value>
void printHelper(Node<Key, Value>* root, std::string indent, bool last) {
    if (root != nullptr) {
        std::cout << indent;
        if (last) {
            std::cout << "R----";
            indent += "     ";
        }
        else {
            std::cout << "L----";
            indent += "|    ";
        }

        // �ڵ���ɫ����ɫ�� R ��ʾ����ɫ�� B ��ʾ
        std::string sColor = root->color ? "RED" : "BLACK";
        std::cout << root->key << "(" << sColor << ")" << std::endl;

        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
    }
}

template <typename Key, typename Value>
class RedBlackTree {
public:
    // �����������
    class iterator {
    public:
        iterator(Node<Key, Value>* node = nullptr) : current(node) {}

        // �����ò�����
        Node<Key, Value>& operator*() const {
            return *current;
        }

        // ָ����ʲ�����
        Node<Key, Value>* operator->() const {
            return current;
        }

        // ǰ��++������
        iterator& operator++() {
            current = treeSuccessor(current); // ��ȡ��̽ڵ�
            return *this;
        }

        // ����++������
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        // �Ƚϲ�����
        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }

    private:
        Node<Key, Value>* current;

        // ��ȡ��ǰ�ڵ�ĺ�̽ڵ㣨���������������һ���ڵ㣩
        Node<Key, Value>* treeSuccessor(Node<Key, Value>* node) {
            if (node->right != nullptr) {
                // ����������������ڵ�
                node = node->right;
                while (node->left != nullptr) {
                    node = node->left;
                }
                return node;
            }
            Node<Key, Value>* parent = node->parent;
            while (parent != nullptr && node == parent->right) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }

        friend class RedBlackTree;
    };
public:
    RedBlackTree() : root(nullptr) {}

    // ��ӡ�����
    void printTree() {
        if (root) {
            printHelper(root, "", true);
        }
    }
    void insert(const Key& key, const Value& value) {
        Node<Key, Value>* node = nodePool.allocate();
        node->key = key;
        node->value = value;
        root = insertBST(root, node);
        if (!node->parent && node != root)
        {
            nodePool.deallocate(node);
            return;
        }
        fixInsert(node);
    }

    void travel(Node<Key, Value>* root) {
        if (!root) return;

        std::stack<Node<Key, Value>*> nodeStack;
        nodeStack.push(root);

        while (!nodeStack.empty()) {
            Node<Key, Value>* current = nodeStack.top();
            nodeStack.pop();

            // ��ջ��ȡ���ڵ���Ƚ�����������ջ���������������Ա��ȴ���������
            if (current) {
                nodeStack.push(current->right);
                nodeStack.push(current->left);
                nodePool.deallocate(current);  // �ͷŵ�ǰ�ڵ�
            }
        }
    }

    void clear() {
        if (root) {
            travel(root);
            root = nullptr;
        }
    }


    // �����߼������ص�����
    iterator find(const Key& key) {
        Node<Key, Value>* node = searchTreeIterative(root, key);
        if (node == nullptr) {
            return end(); // ����Ҳ��������� end ������
        }
        return iterator(node);
    }

    // ����ָ����β�ĵ�����
    iterator end() {
        return iterator(nullptr); // end() ����ָ��սڵ�ĵ�����
    }

    // ����ָ����ͷ�ĵ�����
    iterator begin() {
        return iterator(minimum(root)); // �ҵ���С�ڵ㲢�����������
    }

    // ��������...

private:
    Node<Key, Value>* root;
    ObjectPool<Node<Key, Value>, 100> nodePool;

    // ��������������
    Node<Key, Value>* insertBST(Node<Key, Value>* root, Node<Key, Value>* node) {
        if (root == nullptr) return node;

        if (node->key < root->key) {
            root->left = insertBST(root->left, node);
            root->left->parent = root;
        }
        else if (node->key > root->key) {
            root->right = insertBST(root->right, node);
            root->right->parent = root;
        }
        else if (node->key == root->key) {
            root->value = node->value;
        }

        return root;
    }
    // ����������޸�// ����������޸�
    void fixInsert(Node<Key, Value>* node) {
        Node<Key, Value>* parent = nullptr;
        Node<Key, Value>* grandparent = nullptr;

        while ((node != root) && (node->color == true) && (node->parent->color == true)) {
            parent = node->parent;
            grandparent = parent->parent;

            // ���ڵ����游�ڵ�����ӽڵ�
            if (parent == grandparent->left) {
                Node<Key, Value>* uncle = grandparent->right;

                // ��� 1������ڵ��Ǻ�ɫ - ������ɫ�������޸�
                if (uncle != nullptr && uncle->color == true) {
                    grandparent->color = true;
                    parent->color = false;
                    uncle->color = false;
                    node = grandparent;
                }
                else {
                    // ��� 2�������Ǻ�ɫ�����½ڵ��Ǹ��ڵ�����ӽڵ� - ����ת
                    if (node == parent->right) {
                        leftRotate(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    // ��� 3�������Ǻ�ɫ�����½ڵ��Ǹ��ڵ�����ӽڵ� - ����ת
                    rightRotate(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {
                // ���ڵ����游�ڵ�����ӽڵ㣬�����������Գ�
                Node<Key, Value>* uncle = grandparent->left;

                // ��� 1������ڵ��Ǻ�ɫ - ������ɫ�������޸�
                if (uncle != nullptr && uncle->color == true) {
                    grandparent->color = true;
                    parent->color = false;
                    uncle->color = false;
                    node = grandparent;
                }
                else {
                    // ��� 2�������Ǻ�ɫ�����½ڵ��Ǹ��ڵ�����ӽڵ� - ����ת
                    if (node == parent->left) {
                        rightRotate(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    // ��� 3�������Ǻ�ɫ�����½ڵ��Ǹ��ڵ�����ӽڵ� - ����ת
                    leftRotate(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }

        // ���ڵ�����Ǻ�ɫ
        root->color = false;
    }

    // ����ת
    void leftRotate(Node<Key, Value>* x) {
        Node<Key, Value>* y = x->right;
        x->right = y->left;

        if (y->left != nullptr) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    // ����ת
    void rightRotate(Node<Key, Value>* x) {
        Node<Key, Value>* y = x->left;
        x->left = y->right;

        if (y->right != nullptr) {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }
    // �����������ں�����в��Ҽ�
    Node<Key, Value>* searchTreeIterative(Node<Key, Value>* root, const Key& key) {
        Node<Key, Value>* current = root;
        while (current != nullptr) {
            if (current->key == key) {
                return current;  // �ҵ��ڵ�
            }
            if (key < current->key) {
                current = current->left;  // ���������м�������
            }
            else {
                current = current->right;  // ���������м�������
            }
        }
        return nullptr;  // û���ҵ�
    }

    // ������Сֵ�ڵ�
    Node<Key, Value>* minimum(Node<Key, Value>* node) {
        if (node == nullptr) return node;
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
};
