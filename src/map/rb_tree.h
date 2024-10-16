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

        // 节点颜色，红色用 R 表示，黑色用 B 表示
        std::string sColor = root->color ? "RED" : "BLACK";
        std::cout << root->key << "(" << sColor << ")" << std::endl;

        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
    }
}

template <typename Key, typename Value>
class RedBlackTree {
public:
    // 定义迭代器类
    class iterator {
    public:
        iterator(Node<Key, Value>* node = nullptr) : current(node) {}

        // 解引用操作符
        Node<Key, Value>& operator*() const {
            return *current;
        }

        // 指针访问操作符
        Node<Key, Value>* operator->() const {
            return current;
        }

        // 前置++操作符
        iterator& operator++() {
            current = treeSuccessor(current); // 获取后继节点
            return *this;
        }

        // 后置++操作符
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        // 比较操作符
        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }

    private:
        Node<Key, Value>* current;

        // 获取当前节点的后继节点（即按中序遍历的下一个节点）
        Node<Key, Value>* treeSuccessor(Node<Key, Value>* node) {
            if (node->right != nullptr) {
                // 查找右子树的最左节点
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

    // 打印红黑树
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

            // 从栈中取出节点后，先将其右子树入栈，再入左子树，以便先处理左子树
            if (current) {
                nodeStack.push(current->right);
                nodeStack.push(current->left);
                nodePool.deallocate(current);  // 释放当前节点
            }
        }
    }

    void clear() {
        if (root) {
            travel(root);
            root = nullptr;
        }
    }


    // 查找逻辑并返回迭代器
    iterator find(const Key& key) {
        Node<Key, Value>* node = searchTreeIterative(root, key);
        if (node == nullptr) {
            return end(); // 如果找不到，返回 end 迭代器
        }
        return iterator(node);
    }

    // 返回指向树尾的迭代器
    iterator end() {
        return iterator(nullptr); // end() 返回指向空节点的迭代器
    }

    // 返回指向树头的迭代器
    iterator begin() {
        return iterator(minimum(root)); // 找到最小节点并返回其迭代器
    }

    // 其他方法...

private:
    Node<Key, Value>* root;
    ObjectPool<Node<Key, Value>, 100> nodePool;

    // 二叉搜索树插入
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
    // 红黑树插入修复// 红黑树插入修复
    void fixInsert(Node<Key, Value>* node) {
        Node<Key, Value>* parent = nullptr;
        Node<Key, Value>* grandparent = nullptr;

        while ((node != root) && (node->color == true) && (node->parent->color == true)) {
            parent = node->parent;
            grandparent = parent->parent;

            // 父节点是祖父节点的左子节点
            if (parent == grandparent->left) {
                Node<Key, Value>* uncle = grandparent->right;

                // 情况 1：叔叔节点是红色 - 重新着色并向上修复
                if (uncle != nullptr && uncle->color == true) {
                    grandparent->color = true;
                    parent->color = false;
                    uncle->color = false;
                    node = grandparent;
                }
                else {
                    // 情况 2：叔叔是黑色，且新节点是父节点的右子节点 - 左旋转
                    if (node == parent->right) {
                        leftRotate(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    // 情况 3：叔叔是黑色，且新节点是父节点的左子节点 - 右旋转
                    rightRotate(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {
                // 父节点是祖父节点的右子节点，与上面的情况对称
                Node<Key, Value>* uncle = grandparent->left;

                // 情况 1：叔叔节点是红色 - 重新着色并向上修复
                if (uncle != nullptr && uncle->color == true) {
                    grandparent->color = true;
                    parent->color = false;
                    uncle->color = false;
                    node = grandparent;
                }
                else {
                    // 情况 2：叔叔是黑色，且新节点是父节点的左子节点 - 右旋转
                    if (node == parent->left) {
                        rightRotate(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    // 情况 3：叔叔是黑色，且新节点是父节点的右子节点 - 左旋转
                    leftRotate(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }

        // 根节点必须是黑色
        root->color = false;
    }

    // 左旋转
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

    // 右旋转
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
    // 辅助函数：在红黑树中查找键
    Node<Key, Value>* searchTreeIterative(Node<Key, Value>* root, const Key& key) {
        Node<Key, Value>* current = root;
        while (current != nullptr) {
            if (current->key == key) {
                return current;  // 找到节点
            }
            if (key < current->key) {
                current = current->left;  // 在左子树中继续查找
            }
            else {
                current = current->right;  // 在右子树中继续查找
            }
        }
        return nullptr;  // 没有找到
    }

    // 查找最小值节点
    Node<Key, Value>* minimum(Node<Key, Value>* node) {
        if (node == nullptr) return node;
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
};
