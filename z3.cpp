#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

int iterations = 5;

mutex mtx;
condition_variable cv;

vector<bool> writers;
vector<bool> readers;

int readers_count = 0;
int writers_count = 0;
bool writer_waiting = false;
bool reader_waiting = false;

bool check_writers(){
    for(int i = 0; i < 5; i++){
        if (writers[i]) return true;
    }
    return false;
}

bool check_readers(){
    for(int i = 0; i < 5; i++){
        if (readers[i]) return true;
    }
    return false;
}

void reader(int id, bool& readerPrimary) {
    unique_lock<mutex> lock(mtx);
    lock.unlock();

    for(int j = 0; j < iterations; j++){
        if (readerPrimary){
            lock.lock();
            readers[id] = true;
            lock.unlock();
            while(true){
                lock.lock();
                cv.wait(lock);
                if (!check_writers()){
                    lock.unlock();
                    break;
                }else{
                    lock.unlock();
                }
            }

            cout << "Reader " << id << " is reading." << endl;

            this_thread::sleep_for(chrono::milliseconds(500));

            lock.lock();

            readers[id] = false;

            lock.unlock();

            cv.notify_all();

            this_thread::sleep_for(chrono::milliseconds(900));
        }
        else{
            while(true){
                lock.lock();
                cv.wait(lock);
                if (!check_writers()){
                    readers[id] = true;
                    lock.unlock();
                    break;
                }else{
                    lock.unlock();
                }
            }

            cout << "Reader " << id << " is reading." << endl;

            this_thread::sleep_for(chrono::milliseconds(500));

            lock.lock();

            readers[id] = false;

            lock.unlock();

            cv.notify_all();

            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
}

void writer(int id, bool& writersPrimary) {
    unique_lock<mutex> lock(mtx);
    lock.unlock();

    for(int j = 0; j < iterations; j++){
        if (!writersPrimary){
            while(true){
                lock.lock();
                cv.wait(lock);
                if (check_readers() || check_writers()) lock.unlock();
                else {
                    writers[id] = true;
                    lock.unlock();
                    break;
                }
            }
            cout << "Writer " << id << " is writing." << endl;

            this_thread::sleep_for(chrono::milliseconds(1000));

            lock.lock();

            writers[id] = false;

            lock.unlock();

            cv.notify_all();

            this_thread::sleep_for(chrono::milliseconds(200));
        }
        else{
            lock.lock();
            writers[id] = true;
            lock.unlock();
            while(true){
                lock.lock();
                if (check_readers()){
                    cv.wait(lock);
                    lock.unlock();
                }
                else{
                    lock.unlock();
                    break;
                }
            }


            cout << "Writer " << id << " is writing." << endl;

            lock.lock();

            this_thread::sleep_for(chrono::milliseconds(1000));

            writers[id] = false;

            lock.unlock();

            cv.notify_all();

            this_thread::sleep_for(chrono::milliseconds(5000));
        }
    }
    
}

int main() {
    vector<thread> threads;
    bool writersPrimary = true;
    bool readersPrimary = !writersPrimary;

    for (int i = 0; i < 5; ++i) {
        readers.push_back(false);
        writers.push_back(false);
    }

    for (int i = 0; i < 5; ++i) {
        threads.push_back(thread(reader, i, ref(readersPrimary)));
        threads.push_back(thread(writer, i, ref(writersPrimary)));
    }

    cv.notify_all();

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}