#include <iostream>
#include <future>
#include <functional>
#include <thread>
#include <condition_variable>
#include <vector>
#include <random>
#include <ctime>
#include <latch>
#include <barrier>
#include <queue>
#include <syncstream>

/*
 * Организуй очередь задач: рабочие потоки блокируются на condition_variable,
 * пока очередь пуста; при добавлении задачи главный поток уведомляет один или
 * все ожидающие потоки. Реализуй корректное пробуждение и обработку ложных пробуждений.
*/

std::condition_variable not_empty;
std::mutex mx;
std::mutex mx2;
class Safe_queue{
    std::queue<int> qu;
public:
    void push(int value){
        {
            std::lock_guard<std::mutex> lock(mx);
            qu.push(value);
        }
        not_empty.notify_one();
    }
    int pop(){
        std::unique_lock<std::mutex> lock(mx);
        not_empty.wait(lock, [this](){return !qu.empty();});
        int value = qu.front();
        qu.pop();
        std::cout << "Pop: " << value << "\n";
        return value;
    }

};


int main()
{
    Safe_queue sq;

    std::jthread work1([&](){
        for(int i = 0; i < 1000; ++i){
            sq.push(i);
        }
    });
    std::jthread work([&](){
        for(int i = 0; i < 1000; ++i){

            sq.pop();
        }
    });
    work1.join();
    work.join();
    return 0;
}
