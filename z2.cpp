#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <barrier>
#include <atomic>
#include "StopWatch.h"
#include "random.h"

using namespace std;

mutex b;

struct Training{
    int date_d;
    int date_m;
    int date_y;

    int time_s;
    int time_m;
    int time_h;

    string name;
};

void generate_trains(int count, Training* mass){
    for(int i = 0; i < count; i++){
        int month = random_int(1, 12);
        int day;
        if (month == 2) day = random_int(1, 28);
        else day = random_int(1, 29);
        int year = random_int(1900, 2022);
        int hour = random_int(0, 23);
        int minute = random_int(0, 59);
        int second = random_int(0, 59);

        string name = "Oleg Ivanovich";

        mass[i] = Training{day, month, year, second, minute, hour, name};
    }
    
}

int get_day_of_week(const Training& training) {

    chrono::year_month_day ymd{chrono::year(training.date_y), chrono::month(training.date_m), chrono::day(training.date_d)};

    chrono::sys_days days_since_epoch = ymd;

    chrono::weekday day_of_week = chrono::weekday{days_since_epoch};

    return day_of_week.c_encoding();
}

void obr(int trains, int thread_id, int threads_count, Training* mass, Training* result, atomic<int>& next, int needed_day){
    for(int i = thread_id; i < trains; i += threads_count){
        int day_w = get_day_of_week(mass[i]);
        if (day_w == needed_day){
            int current_next = next.fetch_add(1);
            result[current_next] = mass[i];
        }
    }
}





int main(){
    int train_count;
    int threads_count;
    atomic<int> next(0);
    int needed;

    cin >> train_count;
    cin >> threads_count;
    cin >> needed;

    Training* all_trainings = new Training[train_count];
    Training* result = new Training[train_count];
    thread* threads = new thread[threads_count];

    cout << "Generating Trains..." << endl;

    generate_trains(train_count, all_trainings);

    cout << "Starting multithreading ..." << endl; 

    for(int thread_id = 0; thread_id < threads_count; thread_id++){
        threads[thread_id] = thread(obr, train_count, thread_id, threads_count, ref(all_trainings), ref(result), ref(next), needed);
    }

    Stopwatch timer;

    for(int thread_id = 0; thread_id < threads_count; thread_id++){
        threads[thread_id].join();
    }

    cout << "Ended with execution time: " << timer.stop() << endl;
    cout << "Starting one thread test: " << endl;

    timer.restart();

    obr(train_count, 0, 1, all_trainings, result, next, needed);

    cout << "Ended with execution time: " << timer.stop() << endl;


    return 0;
}