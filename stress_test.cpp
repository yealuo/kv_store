#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include "src/SkipList.h" // 假设您的头文件名为SkipList.h

const int NUM_THREADS = 8;
const int NUM_OPERATIONS = 100000;

std::atomic<int> insert_count(0);
std::atomic<int> delete_count(0);
std::atomic<int> search_count(0);

void insert_operations(SkipList<int, std::string> &skiplist, int num_operations) {
    for (int i = 0; i < num_operations; ++i) {
        int key = rand() % NUM_OPERATIONS;
        skiplist.insert_element(key, "value" + std::to_string(key));
        insert_count++;
    }
}

void delete_operations(SkipList<int, std::string> &skiplist, int num_operations) {
    for (int i = 0; i < num_operations; ++i) {
        int key = rand() % NUM_OPERATIONS;
        skiplist.delete_element(key);
        delete_count++;
    }
}

void search_operations(SkipList<int, std::string> &skiplist, int num_operations) {
    for (int i = 0; i < num_operations; ++i) {
        int key = rand() % NUM_OPERATIONS;
        skiplist.search_element(key);
        search_count++;
    }
}

int main() {
    SkipList<int, std::string> skiplist(10, 60); // max level 10, expiration time 60 seconds
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Create threads for insert operations
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&skiplist]() { insert_operations(skiplist, NUM_OPERATIONS); });
    }

    // Create threads for delete operations
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(delete_operations, std::ref(skiplist), NUM_OPERATIONS);
    }

    // Create threads for search operations
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(search_operations, std::ref(skiplist), NUM_OPERATIONS);
    }

    // Wait for all threads to complete
    for (auto &thread: threads) {
        thread.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Insert operations: " << insert_count.load() << std::endl;
    std::cout << "Delete operations: " << delete_count.load() << std::endl;
    std::cout << "Search operations: " << search_count.load() << std::endl;
    std::cout << "Total time taken: " << duration.count() << " seconds" << std::endl;

    // Final display of the skiplist
    std::cout << "Final display of the skiplist:" << std::endl;
    skiplist.display_list();
    std::cout << "Final size of the skiplist: " << skiplist.size() << std::endl;

    return 0;
}
