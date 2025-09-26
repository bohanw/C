

// C++ implementation of the above approach 
#include <condition_variable> 
#include <iostream> 
#include <mutex> 
#include <queue> 

template <typename T>
class ThreadQueue{
private:
    std::queue<T> m_q;
    std::mutex m_mutex;
    std::condition_variable m_cv;

public :
    void push(T item){
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_q.push(item);
        }
        m_cv.notify_one();

    }

    T pop(){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return !m_q.empty(); });
        T item = std::move(m_q.front());
        m_q.pop();
        return item;
    }
};

int main(){
    ThreadQueue<int> q ;
    q.push(1);
    q.push(2);

    return 0;
}