//
// Created by alian on 12/21/18.
//

#ifndef EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_WALL_ET_SAFEQUEUE_H
#define EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_WALL_ET_SAFEQUEUE_H

#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

template <typename T, size_t SIZE = 6>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue();

    T pop();
    void push(const T&);
private:
    std::queue<T>       m_queue;
    pthread_mutex_t     m_syncLock;
    sem_t               m_pushLock;
    sem_t               m_popLock;

    void initLocks();
    void deleteLocks();
};

template <typename T,size_t SIZE>
void ThreadSafeQueue<T,SIZE>::initLocks()
{
    sem_init(&m_pushLock, 0, SIZE);
    sem_init(&m_popLock, 0, 0);
    if (pthread_mutex_init(&m_syncLock, NULL) != 0)
    {
        std::cout << "mutex init failed" << std::endl;
    }
}

#endif //EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_WALL_ET_SAFEQUEUE_H
