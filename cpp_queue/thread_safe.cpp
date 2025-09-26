#include <mutex>
#include <condition_variable>
#include <queue>
#include <utility>
template<class T>

class SafeQueue{

    std::queue<T> q;

    std::mutex mtx;
    std::condition_variable cv;
    std::condition_variable sync_wait;

    

}