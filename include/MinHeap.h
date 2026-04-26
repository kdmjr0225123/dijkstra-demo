#pragma once
#include <vector>
#include <stdexcept>

/**
 * MinHeap.h
 * Hand-written binary min-heap (priority queue) used by Dijkstra's algorithm.
 * Stores (priority, value) pairs; the pair with the smallest priority is always at the top.
 *
 * Time complexity:
 *   push()  - O(log n)
 *   pop()   - O(log n)
 *   top()   - O(1)
 *   empty() - O(1)
 *   size()  - O(1)
 *
 * Space complexity: O(n)
 */
template <typename T>
struct HeapNode {
    int priority;
    T   value;
    bool operator>(const HeapNode& o) const { return priority > o.priority; }
    bool operator<(const HeapNode& o) const { return priority < o.priority; }
};

template <typename T>
class MinHeap {
public:
    MinHeap() = default;

    void push(int priority, T value) {
        data_.push_back({priority, value});
        siftUp(data_.size() - 1);
    }

    HeapNode<T> top() const {
        if (empty()) throw std::runtime_error("MinHeap::top() called on empty heap");
        return data_[0];
    }

    void pop() {
        if (empty()) throw std::runtime_error("MinHeap::pop() called on empty heap");
        data_[0] = data_.back();
        data_.pop_back();
        if (!empty()) siftDown(0);
    }

    bool  empty() const { return data_.empty(); }
    int   size()  const { return static_cast<int>(data_.size()); }

    // Expose underlying data for visualization / testing
    const std::vector<HeapNode<T>>& rawData() const { return data_; }

private:
    std::vector<HeapNode<T>> data_;

    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data_[parent] > data_[i]) {
                std::swap(data_[parent], data_[i]);
                i = parent;
            } else break;
        }
    }

    void siftDown(int i) {
        int n = data_.size();
        while (true) {
            int smallest = i;
            int left  = 2 * i + 1;
            int right = 2 * i + 2;
            if (left  < n && data_[left]  < data_[smallest]) smallest = left;
            if (right < n && data_[right] < data_[smallest]) smallest = right;
            if (smallest == i) break;
            std::swap(data_[i], data_[smallest]);
            i = smallest;
        }
    }
};
