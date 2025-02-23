#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <barrier>
#include "StopWatch.h"

using namespace std;

binary_semaphore sem(1);
barrier b(4);
mutex mtx;

void mutex_function(int &c, int id, int &needed){
    cout << "thread " << id << " started" << endl;
    while(c < needed){
        lock_guard<mutex> lock(mtx);
        c++;
    }
    cout << "thread " << id << " stoped" << endl;
}

void semaphore_function(int &c, int id, int &needed){
    cout << "thread " << id << " started" << endl;
    while(c < needed){
        sem.acquire();
        c++;
        sem.release();
    }
    cout << "thread " << id << " stoped" << endl;
}

void barrier_function(int &c, int id, int &needed){
    cout << "thread " << id << " started" << endl;
    while(c < needed){
        b.arrive_and_wait();
        c++;
        b.arrive_and_wait();
    }
    cout << "thread " << id << " stoped" << endl;
}




int main(){
    int c = 0;

    int needed = 1000000;
    int threads_count = 4;

    cout << "Starting mutex example:" << endl << endl;

    thread t1(mutex_function, ref(c), 1, ref(needed));
    thread t2(mutex_function, ref(c), 2, ref(needed));
    thread t3(mutex_function, ref(c), 3, ref(needed));
    thread t4(mutex_function, ref(c), 4, ref(needed));

    Stopwatch timer;

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "Ended with execution time: "<< timer.stop() << "ms." << endl << endl;

    c = 0;

    cout << "Starting semafore example:" << endl << endl;

    t1 = thread(semaphore_function, ref(c), 1, ref(needed));
    t2 = thread(semaphore_function, ref(c), 2, ref(needed));
    t3 = thread(semaphore_function, ref(c), 3, ref(needed));
    t4 = thread(semaphore_function, ref(c), 4, ref(needed));


    timer.restart();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "Ended with execution time: "<< timer.stop() << "ms." << endl << endl;

    c = 0;

    cout << "Starting barier example:" << endl << endl;

    t1 = thread(barrier_function, ref(c), 1, ref(needed));
    t2 = thread(barrier_function, ref(c), 2, ref(needed));
    t3 = thread(barrier_function, ref(c), 3, ref(needed));
    t4 = thread(barrier_function, ref(c), 4, ref(needed));


    timer.restart();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "Ended with execution time: "<< timer.stop() << "ms." << endl << endl;
}

