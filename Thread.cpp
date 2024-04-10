#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
queue<int> messageQueue;
int runs = 0;

void producerThread()
{
    int number = rand() % 100;

    messageQueue.push(number);

    this_thread::sleep_for(chrono::milliseconds(rand() % 1000));

    cv.notify_one();
    runs++;
    if (runs < 10)
    {
        producerThread();
    }
}

void consumerThread()
{
    for (int i = 1; i <= 10; ++i)
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock);

        int message = messageQueue.front();
        messageQueue.pop();
        lock.unlock();

        int fontColor = message;

        for (int i = 0; i < fontColor; i++)
        {
            int x = rand() % 80;
            int y = rand() % 25;
            cout << "\033[" << y << ";" << x << "H" << (char)(rand() % 256);
        }
    }
}

int main()
{
    thread producer(producerThread);
    thread consumer(consumerThread);

    producer.join();
    consumer.join();

    return 0;
}
