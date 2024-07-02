//
// Created by Administrator on 2024/6/25.
//

#ifndef KV_STORE_NODE_H
#define KV_STORE_NODE_H

#include <algorithm>

template<typename K, typename V>
class Node {
public:
    Node();

    Node(K, V, int);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_value(V);

public:
    int node_level;
    Node<K, V> **forward;

private:
    K key;
    V value;
};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, const int level):key(k), value(v), node_level(level), forward(nullptr) {
    forward = new Node<K, V> *[level + 1];
    std::fill(forward, forward + level + 1, nullptr);
}


template<typename K, typename V>
Node<K, V>::~Node() {
    delete[] forward;
}


template<typename K, typename V>
K Node<K, V>::get_key() const { return key; }


template<typename K, typename V>
V Node<K, V>::get_value() const { return value; }


template<typename K, typename V>
void Node<K, V>::set_value(V v) { value = v; }

#endif //KV_STORE_NODE_H
