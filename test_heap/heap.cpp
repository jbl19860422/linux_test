#include <iostream>
#include <initializer_list>
#include <math.h>
#include <vector>

class Heap {
public:
    Heap(std::initializer_list<int> l) {
        for(auto & s : l) {
            append(s);
        }
    }
public:
    void append(int v) {
        data_.push_back(v);
        int child = data_.size() - 1;
        int parent = floor((child - 1)/2);
        if(parent < 0) {
            return;
        }

        if(data_[child] > data_[parent]) {
            shiftUp(child);
        } else {
            shiftDown(child);
        }
    }
public:
    std::vector<int> data_;
    void print() {
        for(auto s : data_) {
            std::cout << s << " ";
        }
    }
private:
    void shiftUp(int i) {
        int child = i;
        int parent = floor((child - 1)/2);
        while(parent >= 0 && data_[child] > data_[parent]) {
            std::swap(data_[child], data_[parent]);
            child = parent;
            parent = floor((child - 1)/2);
        }
    } 

    void shiftDown(int i) {
        int parent = i;
        int left_child = 2*parent+1;
        while(left_child < data_.size() && data_[left_child] > data_[parent]) {
            std::swap(data_[left_child], data_[parent]);
            parent = left_child;
            left_child = 2*parent+1;
        }
    }
};

int main(char argc, char *argv[]) {
    Heap h({1, 3, 7, 18, 11, 2, 3, 4, 13, 26});
    h.print();
}

