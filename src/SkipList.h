//
// Created by Administrator on 2024/6/25.
//

#ifndef KV_STORE_SKIPLIST_H
#define KV_STORE_SKIPLIST_H

#include "Node.h"
#include "util.h"

#include <fstream>
#include <random>
#include <iostream>
#include <mutex>

#define STORE_FILE "..\\data\\data"

template<typename K, typename V>
class SkipList {
public:
    SkipList(int);

    ~SkipList();

    int get_random_level();             //获取节点随机层级
    Node<K, V> *create_node(K, V, int);    //节点创建
    bool search_element(K);             //搜索节点
    int insert_element(K, V);            //插入节点
    void delete_element(K);             //删除节点
    void display_list();                //展示节点
    void dump_file();                   //持久化数据到文件
    void load_file();                   //从文件加载数据
    int size();                         //跳表中的节点个数

private:
    int _max_level;             //跳表的最高层级
    int _skip_list_level;       //跳表的当前层级
    Node<K, V> *_header;         //跳表头节点
    int _element_count;         //跳表的节点数量
    std::ofstream _file_writer; //文件写入器
    std::ifstream _file_reader; //文件读取器
    std::default_random_engine e;
    std::uniform_int_distribution<int> gen;
    std::mutex mtx;
};

template<typename K, typename V>
SkipList<K, V>::SkipList(int max_l):_max_level(max_l), _skip_list_level(0), _element_count(0),
                                    e(std::random_device{}()), gen(0, 1) {
    K k{};  //默认初始化
    V v{};  //默认初始化
    _header = new Node<K, V>(k, v, _max_level);
}

template<typename K, typename V>
SkipList<K, V>::~SkipList() {
    auto curt = _header, next = curt->forward[0];
    while (curt != nullptr) {
        delete curt;
        curt = next;
        if (curt != nullptr)
            next = curt->forward[0];
    }
}

template<typename K, typename V>
int SkipList<K, V>::get_random_level() {
    int k = 0;
    while (gen(e) && k <= _max_level) ++k;

    return k;
}

template<typename K, typename V>
Node<K, V> *SkipList<K, V>::create_node(K k, V v, int level) {
    auto *n = new Node<K, V>(k, v, level);
    return n;
}

template<typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    auto curt = _header;
    //从跳表的当前最高层开始搜索
    for (int i = _skip_list_level; i >= 0; --i) {
        // 遍历当前层级，直到下一个节点的键值大于或等于待查找的键值
        while (curt->forward[i] && curt->forward[i]->get_key() < key)
            curt = curt->forward[i];
    }

    curt = curt->forward[0];
    if (curt && curt->get_key() == key)
        return true;

    return false;
}

template<typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
    auto curt = _header;
    // 用于在各层更新指针的数组,记录每层中待更新指针的节点
    Node<K, V> *update[_max_level + 1];
    std::fill(update, update + _max_level + 1, nullptr);

    mtx.lock();
    // 从最高层向下搜索插入位置
    for (int i = _skip_list_level; i >= 0; --i) {
        while (curt->forward[i] && curt->forward[i]->get_key() < key)
            curt = curt->forward[i];

        // 保存每层中该节点，以便后续插入时更新指针
        update[i] = curt;
    }

    // 移动到最底层的下一节点，准备插入操作
    curt = curt->forward[0];
    // 检查待插入的节点的键是否已存在,如果键已存在，则取消插入
    if (curt && curt->get_key() == key) {
        std::cout << "key:" << key << " exists" << std::endl;
        mtx.unlock();
        return 1;
    } else {
        //取当前节点的最高层级
        int random_level = get_random_level();

        //如果新节点的最高层级超过了当前跳表的最高层级
        if (random_level > _skip_list_level) {
            // 对所有新的更高层级，将头节点设置为它们的前驱节点
            for (int i = _skip_list_level + 1; i <= random_level; ++i)
                update[i] = _header;

            _skip_list_level = random_level;
        }

        auto *new_node = create_node(key, value, random_level);
        for (int i = 0; i <= random_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }

        ++_element_count;
        mtx.unlock();
    }

    return 0;
}

template<typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    auto curt = _header;
    Node<K, V> *update[_max_level + 1];
    std::fill(update, update + _max_level + 1, nullptr);

    mtx.lock();
    for (int i = _skip_list_level; i >= 0; --i) {
        while (curt->forward[i] && curt->forward[i]->get_key() < key)
            curt = curt->forward[i];

        update[i] = curt;
    }

    curt = curt->forward[0];
    if (curt == nullptr || curt->get_key() != key) {
        mtx.unlock();
        return;
    } else {
        for (int i = 0; i <= curt->node_level; ++i) {
            update[i]->forward[i] = curt->forward[i];
            curt->forward[i] = nullptr;
        }

        // 调整跳表层级
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == nullptr)
            --_skip_list_level;

        delete curt;
        --_element_count;
        mtx.unlock();
    }
}

template<typename K, typename V>
void SkipList<K, V>::display_list() {
    for (int i = _skip_list_level; i >= 0; --i) {
        auto node = _header->forward[i];

        std::cout << "Level " << i << ": ";
        while (node != nullptr) {
            std::cout << node->get_key() << ":" << node->get_value() << ';';
            node = node->forward[i];
        }

        std::cout << std::endl;
    }
}

template<typename K, typename V>
void SkipList<K, V>::dump_file() {
    _file_writer.open(STORE_FILE);
    auto node = _header->forward[0];

    while (node != nullptr) {
        _file_writer << node->get_key() << ':' << node->get_value() << "\n";
        node = node->forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
}

template<typename K, typename V>
void SkipList<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    std::string line;
    while (std::getline(_file_reader, line)) {
        std::pair<std::string, std::string> key_value = get_kay_value_from_string(line, ':');
        std::string key = key_value.first, value = key_value.second;
        if (key.empty() || value.empty())
            continue;

        insert_element(std::stoi(key), value);
    }

    std::cout << "Insert complete!" << std::endl;
    display_list();

    _file_reader.close();
}

template<typename K, typename V>
int SkipList<K, V>::size() { return _element_count; }

#endif //KV_STORE_SKIPLIST_H
