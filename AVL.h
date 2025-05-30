#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <tuple>

template<class T>
struct Node {
    T value;
    int height;
    Node<T>* nodes[2];
    Node(T v, int h = 1) {
        value = v;
        height = h;
        nodes[0] = nullptr;
        nodes[1] = nullptr;
    }
};

template<class T>
struct CBinTree {
    Node<T>* root = nullptr;
    int cant_nodos = 0;
    std::vector<T> In;
    std::vector<T> Pre;
    bool Find(std::stack<Node<T>**>& stac, T x, Node<T>**& pos);
    void Ins(T x);
    void LevelOrder(int& max_level);
    void InOrder(Node<T>* m_root);
    void PreOrder(Node<T>* m_root);
    Node<T>** InSucc(Node<T>** p);
    Node<T>** InPred(Node<T>** p);
    int Update_height(Node<T>* node);
    int Balance_factor(Node<T>* node);
    void RR(Node<T>**& node);
    void RL(Node<T>**& node);
    void LL(Node<T>**& node);
    void LR(Node<T>**& node);
};


template<class T>
bool CBinTree<T>::Find(std::stack<Node<T>**>& stac, T x, Node<T>**& pos) {
    pos = &root;
    while (*pos) {
        if ((*pos)->value == x) {
            return true;
        }
        stac.push(pos);
        pos = &((*pos)->nodes[(*pos)->value < x]);
    }
    return false;
}

template<class T>
int CBinTree<T>::Update_height(Node<T>* node) {
    int l_height = (node->nodes[0]) ? node->nodes[0]->height : 0;
    int r_height = (node->nodes[1]) ? node->nodes[1]->height : 0;
    return std::max(l_height, r_height) + 1;
}

template<class T>
int CBinTree<T>::Balance_factor(Node<T>* node) {
    int l_height = (node->nodes[0]) ? node->nodes[0]->height : 0;
    int r_height = (node->nodes[1]) ? node->nodes[1]->height : 0;
    return l_height - r_height;
}

template<class T>
void CBinTree<T>::RR(Node<T>**& root) {
    Node<T>* tmp = ((*root)->nodes[1]);
    (*root)->nodes[1] = tmp->nodes[0];
    tmp->nodes[0] = (*root);
    (*root)->height = Update_height(*root);
    tmp->height = Update_height(tmp);
    *root = tmp;
}

template<class T>
void CBinTree<T>::LL(Node<T>**& root) {
    Node<T>* tmp = ((*root)->nodes[0]);
    (*root)->nodes[0] = tmp->nodes[1];
    tmp->nodes[1] = (*root);
    (*root)->height = Update_height(*root);
    tmp->height = Update_height(tmp);
    *root = tmp;

}


template<class T>
void CBinTree<T>::RL(Node<T>**& root) {
    Node<T>* tmp = ((*root)->nodes[1]->nodes[0]);
    (*root)->nodes[1]->nodes[0] = tmp->nodes[1];
    tmp->nodes[1] = (*root)->nodes[1];
    (*root)->nodes[1] = tmp;
    tmp->height = Update_height(tmp);
    tmp->nodes[1]->height = Update_height(tmp->nodes[1]);
    (*root)->height = Update_height(*root);
    RR(root);
}


template<class T>
void CBinTree<T>::LR(Node<T>**& root) {
    Node<T>* tmp = ((*root)->nodes[0]->nodes[1]);
    (*root)->nodes[0]->nodes[1] = tmp->nodes[0];
    tmp->nodes[0] = (*root)->nodes[0];
    (*root)->nodes[0] = tmp;
    tmp->height = Update_height(tmp);
    tmp->nodes[0]->height = Update_height(tmp->nodes[0]);
    (*root)->height = Update_height(*root);
    LL(root);
}

template <class T>
void CBinTree<T>::Ins(T x) {
    Node<T>** pos = nullptr;
    std::stack<Node<T>**> stack;
    Node<T>** tmp;
    if (!Find(stack, x, pos)) {
        *pos = new Node<T>(x);
        while (!stack.empty()) {
            tmp = stack.top();
            (*tmp)->height = Update_height(*tmp);
            stack.pop();
            if (Balance_factor(*tmp) == -2 && Balance_factor((*tmp)->nodes[1]) == -1) {
                RR(tmp);
            }
            else if (Balance_factor(*tmp) == -2 && Balance_factor((*tmp)->nodes[1]) == 1) {
                RL(tmp);
            }
            else if (Balance_factor(*tmp) == 2 && Balance_factor((*tmp)->nodes[0]) == 1) {
                LL(tmp);
            }
            else if (Balance_factor(*tmp) == 2 && Balance_factor((*tmp)->nodes[0]) == -1) {
                LR(tmp);
            }
        }
    }
    cant_nodos++;
}

template<class T>
Node<T>** CBinTree<T>::InSucc(Node<T>** p) {
    while ((*p)->nodes[0]) {
        p = &((*p)->nodes[0]);
    }
    return p;
}

template<class T>
Node<T>** CBinTree<T>::InPred(Node<T>** p) {
    while ((*p)->nodes[1]) {
        p = &((*p)->nodes[1]);
    }
    return p;
}


template<class T>
void CBinTree<T>::LevelOrder(int& max_level) {
    if (!root) {
        return;
    }
    std::cout << " -------------------- " << std::endl;
    std::queue<std::pair<Node<T>*, int>> queue;
    queue.push({ root,0 });
    while (!queue.empty()) {
        Node<T>* tmp = queue.front().first;
        if (queue.front().second > max_level) {
            max_level = queue.front().second;
        }
        if (tmp->nodes[0]) {
            queue.push({ tmp->nodes[0],queue.front().second + 1 });
        }
        if (tmp->nodes[1]) {
            queue.push({ tmp->nodes[1],queue.front().second + 1 });
        }
        queue.pop();
    }
}

template<class T>
void CBinTree<T>::InOrder(Node<T>* m_root) {
    if (!m_root) {
        return;
    }
    InOrder(m_root->nodes[0]);
    In.push_back(m_root->value);
    InOrder(m_root->nodes[1]);
}

template<class T>
void CBinTree<T>::PreOrder(Node<T>* m_root) {
    if (!m_root) {
        return;
    }
    Pre.push_back(m_root->value);
    PreOrder(m_root->nodes[0]);
    PreOrder(m_root->nodes[1]);
}
