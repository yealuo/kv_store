//
// Created by Administrator on 2024/7/10.
//

#ifndef KV_STORE_LRU_CACHE_H
#define KV_STORE_LRU_CACHE_H

#include <utility>
#include <unordered_map>

template<typename K, typename V>
struct Node {
    Node *pre, *next;
    std::pair<K, V> data;

    Node() : pre(nullptr), next(nullptr) {}

    Node(K k, V v) : pre(nullptr), next(nullptr), data(k, v) {}
};

template<typename K, typename V>
class LRU_Cache {
public:
    LRU_Cache(int max_sz) : head(nullptr), tail(nullptr), size(0), max_size(max_sz) {
        head = new Node<K, V>();
        tail = new Node<K, V>();
        head->next = tail;
        tail->pre = head;
    }

    ~LRU_Cache() {
        Node<K, V> *temp;
        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    V get(K key, V default_value) {
        if (dict.count(key) != 0) {
            auto tar = dict[key];
            move_to_head(tar);
            return tar->data.first;
        } else
            return default_value;
    }

    void put(K key, V value) {
        if (dict.count(key) != 0) {
            auto tar = dict[key];
            tar->data.second = value;
            move_to_head(tar);
        } else {
            if (size != max_size) {
                auto tar = new Node<K, V>(key, value);
                add_to_head(tar);
                dict[key] = tar;
                ++size;
            } else {
                auto tar = tail->pre;
                auto ptr = dict.find(tar->data.first);
                dict.erase(ptr->first);
                dict[key] = tar;
                tar->data.first = key;
                tar->data.second = value;
                move_to_head(tar);
            }
        }
    }

private:
    Node<K, V> *head, *tail;
    int size, max_size;
    std::unordered_map<K, Node<K, V> *> dict;

    void remove_node(Node<K, V> *curt) {
        curt->pre->next = curt->next;
        curt->next->pre = curt->pre;
        curt->next = nullptr;
        curt->pre = nullptr;
    }

    void add_to_head(Node<K, V> *curt) {
        curt->next = head->next;
        curt->pre = head;
        head->next = curt;
        curt->next->pre = curt;
    }

    void move_to_head(Node<K, V> *curt) {
        remove_node(curt);
        add_to_head(curt);
    }
};

#endif //KV_STORE_LRU_CACHE_H
