#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <condition_variable>
//#include <processthreadsapi.h>
#include <Windows.h>
#include <intrin.h>

using namespace std;

int a[10000000];
std::mutex mtx;
std::condition_variable cv;
bool ready = false;


void *doNothing() {
    return NULL;
}


void print_id(int id) {
    std::unique_lock<std::mutex> lck(mtx);
    while (!ready) cv.wait(lck);
    // ...
    //std::cout << "thread " << id << '\n';
}

void go() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}

int main()
{



    // Memory access
    auto start_access = chrono::steady_clock::now();
    for (int i = 0; i < 10000000; i++) {
        a[i] = 5;
    }
    auto end_access = chrono::steady_clock::now();

    std::chrono::duration<double, std::milli> fp_ms_access = end_access - start_access;
    cout << "Elapsed time in seconds(memory access): " << fp_ms_access.count() / 1000 << "\n"; // divide by 1000 => ms -> s

    // Static memory allocation
    auto start_static = chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++) {
        int arr1[100000000];
    }
    auto end_static = chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> fp_ms_static = end_static - start_static;
    cout << "Elapsed time in seconds(static mem allocation): " << fp_ms_static.count() / 1000 << "\n"; // divide by 1000 => ms -> s

    // Dynamic memory allocation
    auto start_dynamic = chrono::steady_clock::now();
    int* arr2;
    for (int i = 0; i < 100000; i++) {
        arr2 = new int[100000000];
        delete [] arr2;
    }
    auto end_dynamic = chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> fp_ms_dynamic = end_dynamic - start_dynamic;
    cout << "Elapsed time in seconds(dynamic mem allocation): " << fp_ms_dynamic.count() / 1000 << "\n"; // divide by 1000 => ms -> s

    // Create 10000 threads
    auto start_threads = chrono::steady_clock::now();
    std::vector<std::thread> thread_pool;
    thread_pool.reserve(10000);
    for (int i = 0; i < 10000; i++) {
        thread_pool.push_back(std::thread(doNothing));
    }
    auto end_threads = chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> fp_ms_threads = end_threads - start_threads;
    cout << "Elapsed time in seconds(create threads):" << fp_ms_threads.count() / 1000 << "\n";

    // Thread context switch
    auto start_context = chrono::steady_clock::now();
    std::thread threads[10];
    for (int i = 0; i < 10; i++) {
        threads[i] = std::thread(print_id, i);
    }
    go();
    for (auto& th : threads) th.join();
    auto end_context = chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> fp_ms_context = end_context - start_context;
    cout << "Elapsed time in seconds(context switch): " << fp_ms_context.count() / 1000 << "\n";


    //Thread migration:
    auto start_migration = chrono::steady_clock::now();
    unsigned num_threads = 4;
    // A mutex ensures orderly access to std::cout from multiple threads.
    std::mutex iomutex;
      
    std::vector<std::thread> threads_mig(num_threads);
    for (unsigned i = 0; i < num_threads; ++i) {
        threads_mig[i] = std::thread([&iomutex, i] {
            while (1) {
                {
                    // Use a lexical scope and lock_guard to safely lock the mutex only
                    // for the duration of std::cout usage
                    std::lock_guard<std::mutex> iolock(iomutex);
                    //std::cout << "Thread #" << i << ": on CPU " <<  GetCurrentProcessorNumber()<< "\n";
                    break;
                }

                // Simulate important work done by the tread by sleeping for a bit..
                std::this_thread::sleep_for(std::chrono::milliseconds(900));
            }
        });
    }

    for (auto& t : threads_mig) {
        t.join();
    }
    auto end_migration = chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> fp_ms_migration = end_migration - start_migration;
    cout << "Elapsed time in seconds(thread migration): " << fp_ms_migration.count() / 1000 << "\n";

    return 0;
}
