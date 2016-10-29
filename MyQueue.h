#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename T>
class MyQueue
{
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condition_;
	bool flag=true;
public:
    void push(T const& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        condition_.notify_one();
    }

    T pop(void)
    {
	if(queue_.empty()&& !flag) return "";
        std::unique_lock<std::mutex> mlock(mutex_);
        while(queue_.empty() )
        {
            condition_.wait(mlock);
        }
        auto  popped_value=queue_.front();
        queue_.pop();
		return popped_value;
    }
	
};
