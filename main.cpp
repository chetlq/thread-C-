#include "MyQueue.h"
#include <iostream>
#include <stdio.h>
#include <windows.h>

void produce(MyQueue<std::string>& q) {
    HANDLE handle;
    WIN32_FIND_DATA findData;
 
    handle = FindFirstFile("D:\\myfolder\*.txt", &findData);
    if (handle == INVALID_HANDLE_VALUE)
        return 1;
    do
    {
        q.push(findData.cFileName);
    }
    while (FindNextFile(handle, &findData) != 0);
    q.flag=false;
    FindClose(handle);  
}

void consume(MyQueue<std::string>& q, int& count) {
	std::mutex mutex_;
	while((std::string item = q.pop()).length()>0)
	{
		ifstream file(item, ios::binary|ios::ate);
		// print current position (equal file size in bytes)
		std::cout << file.tellg();
		std::unique_lock<std::mutex> mlock(mutex_);
			count++;
		mlock.unlock();
	}
  }
}

#include <thread>
int main()
{
  MyQueue<std::string> q;
  int count = 0;
  
  
  // producer thread
  std::thread prod1(std::bind(produce, std::ref(q)));

  // consumer threads
  std::thread consumer1(consume, std::ref(q),std::ref(count));
  std::thread consumer2(consume, std::ref(q),std::ref(count));
  std::thread consumer3(consume, std::ref(q),std::ref(count));
  std::thread consumer4(consume, std::ref(q),std::ref(count));

  prod1.join();
  consumer1.join();
  consumer2.join();
  consumer3.join();
  consumer4.join();

}
