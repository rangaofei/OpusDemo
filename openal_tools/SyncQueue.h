//
// Created by saka on 2018/9/25.
//

#ifndef OPUSDEMO_SYNCQUEUE_H
#define OPUSDEMO_SYNCQUEUE_H

#include <list>
#include <mutex>
#include <condition_variable>
#include <iostream>

struct OpusBody {
    unsigned char memory[1500];
    size_t size;
};

template<typename T>
class SyncQueue {
private:
    bool isFull() const {
        return m_queue.size() == m_maxSize;
    }

    bool isEmpty() const {
        return m_queue.empty();
    }

public:
    SyncQueue(int maxSize) : m_maxSize(maxSize) {

    }

    void put(const T &x) {
        std::lock_guard<std::mutex> locker(m_mutex);
        while (isFull()) {
            std::cout << "the blocking queue is full,waiting..." << std::endl;
            m_notEmpty.wait(m_mutex);
        }
        m_queue.push_back(x);
        m_notEmpty.notify_one();
    }

    void take(T &x) {
        std::lock_guard<std::mutex> locker(m_mutex);

        while (isEmpty()) {
            std::cout << "the blocking queue is empty,waiting..." << std::endl;
            m_notEmpty.wait(m_mutex);
        }
        x = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }

private:
    std::list<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable_any m_notEmpty;
    std::condition_variable_any m_notFull;
    int m_maxSize;
};

#endif //OPUSDEMO_SYNCQUEUE_H
