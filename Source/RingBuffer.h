#include <iostream>
#include <vector>

template<typename T>
class RingBuffer {
public:
    RingBuffer(size_t capacity)
        : buffer(capacity), capacity(capacity), head(0), tail(0), full(false) {
    }

    // データ追加
    void push(const T& item) {
        buffer[tail] = item;
        if (full) {
            head = (head + 1) % capacity; // 上書き → headも進める
        }
        tail = (tail + 1) % capacity;
        full = (tail == head);
    }

    // データ取り出し
    bool pop(T& item) {
        if (empty()) return false;

        item = buffer[head];
        full = false;
        head = (head + 1) % capacity;
        return true;
    }

    bool empty() const {
        return (!full && (head == tail));
    }

    bool isFull() const {
        return full;
    }

private:
    std::vector<T> buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    bool full;
};
