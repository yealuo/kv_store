//
// Created by Administrator on 2024/7/16.
//

#include "src/SkipList.h"

int main() {
    SkipList<int, std::string> test_skiplist(10, 5);

    test_skiplist.insert_element(1, "Hello");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    test_skiplist.display_list();
    std::cout << std::endl;

    test_skiplist.insert_element(2, "World");
    test_skiplist.display_list();
    std::cout << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));
    test_skiplist.display_list();
    std::cout << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));
    test_skiplist.display_list();
    std::cout << std::endl;
}