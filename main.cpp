//
// Created by alian on 12/21/18.
//

#include <iostream>
#include <stdlib.h>
#include "safequeue.h"

//int QUEUE_CAPACITY 6
//int THREADS_COUNT 4
//int ITERATIONS 40
template <typename T>
struct thread_info
{
    int count;
    ThreadSafeQueue<T,6>* queue;
};

int getRandomNumber()
{
    return rand() % 1000000;
}
void *Pusher(void *args)
{

    while(true){
        ((thread_info<int>*)args)->queue->push(getRandomNumber());
    }
    return 0;
}

void *Popper(void *args)
{

    while(true){
        ((thread_info<int>*)args)->queue->pop();
//        std::cout << (*(ThreadSafeQueue<int,6>*)queue).pop() << std::endl;
//        std::cout << ((ThreadSafeQueue<int,6>)queue).pop() << std::endl;
    }
    return 0;
}

void *RandomUser(void *queue)
{
    return 0;
}

void test_threaded_queue()
{
//    ThreadSafeQueue<int,6> * queue = new ThreadSafeQueue<int,6>();
//    ThreadSafeQueue<int,6> queue;

    struct thread_info<int>* args = new thread_info<int>();
    args->queue = new ThreadSafeQueue<int,6>();

//    pthread_t threads[THREADS_COUNT];
    pthread_t eternal_producer,eternal_consumer;

    pthread_create(&eternal_producer, NULL, Pusher, (void *)&args);
    pthread_create(&eternal_consumer, NULL, Popper, (void *)&args);
//
    pthread_join(eternal_consumer, NULL);
    pthread_join(eternal_producer, NULL);

}

void test_basic_queue()
{
    ThreadSafeQueue<int,6> queue;
     int num = getRandomNumber();
     queue.push(num);
     int returnNum = queue.pop();

     if(num == returnNum)
         std::cout << "Basic queue is functioning" << std::endl;
     else
         std::cout << "basic queue error." << std::endl;

//    queue.push(num);
//    queue.push(num);
//    queue.push(num);
//    queue.push(num);
//    queue.push(num);
//    queue.push(num);
//    queue.push(num);
}

int main()
{
//    test_basic_queue();
    test_threaded_queue();
    std::cout << "Hello World" << std::endl;
    return 1;
}
