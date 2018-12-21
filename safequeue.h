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
    ~ThreadSafeQueue();

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
ThreadSafeQueue<T,SIZE>::ThreadSafeQueue()
{
    std::cout << "Init queue" << std::endl;
    initLocks();
}

template <typename T,size_t SIZE>
ThreadSafeQueue<T,SIZE>::~ThreadSafeQueue()
{
    std::cout << "Delete queue" << std::endl;
    deleteLocks();
//    delete m_queue;
}

template <typename T,size_t SIZE>
void ThreadSafeQueue<T,SIZE>::initLocks()
{
    std::cout << "Init locks" << std::endl;
    sem_init(&m_pushLock, 0, SIZE);
    sem_init(&m_popLock, 0, 0);
    if (pthread_mutex_init(&m_syncLock, NULL) != 0)
    {
        std::cout << "mutex init failed" << std::endl;
        deleteLocks();
    }
}

template <typename T,size_t SIZE>
void ThreadSafeQueue<T,SIZE>::deleteLocks()
{
    std::cout << "delete locks" << std::endl;
    sem_destroy(&m_pushLock);
    sem_destroy(&m_popLock);
    pthread_mutex_destroy(&m_syncLock);
}

template <typename T,size_t SIZE>
void ThreadSafeQueue<T,SIZE>::push(const T& dataToInsert)
{
    std::cout << "Enter Push("<< m_queue.size()<<")with " << dataToInsert << std::endl;

    sem_wait(&m_pushLock);
    std::cout<< "passed pushlock" <<std::endl;

    pthread_mutex_lock(&m_syncLock);
    std::cout<< "push mutex lock" <<std::endl;

    m_queue.push(dataToInsert);

    pthread_mutex_unlock(&m_syncLock);

    sem_post(&m_popLock);
}

template <typename T,size_t SIZE>
T ThreadSafeQueue<T,SIZE>::pop()
{
    std::cout << "Enter Pop" << std::endl;

    sem_wait(&m_popLock);

    std::cout<< "passed poplock" <<std::endl;

    pthread_mutex_lock(&m_syncLock);
    std::cout<< "pop mutex lock" <<std::endl;

    T tmp = m_queue.front();
    m_queue.pop();

    pthread_mutex_unlock(&m_syncLock);

    sem_post(&m_pushLock);

    return tmp;
}
#endif //EXCELLENTEAM_ELLA_CONCURRENCY_SAFEQUEUE_WALL_ET_SAFEQUEUE_H
