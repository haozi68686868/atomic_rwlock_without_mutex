#include <iostream>
#include <ctime>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <memory>
#include "dataPack.hpp"
 
std::mutex mtx;
size_t count = 0;

struct A
{
	int a1;
	int a2;
	double a3[128];
	double a4;
};

void threadFun()
{
	for (int i = 0; i < 10000; i++)
	{
		// 防止多个线程同时访问同一资源
		std::lock_guard<std::mutex> lock(mtx);
		count++;
	}
}

void Test1()
{
	std::cout << "-- Test1 --" << std::endl;
	clock_t start_time = clock();
 
	// 启动多个线程
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++)
		threads.push_back(std::thread(threadFun));
	for (auto&thad : threads)
		thad.join();
 
	// 检测count是否正确 10000*10 = 100000
	std::cout << "count number:" << count << std::endl;
 
	clock_t end_time = clock();
	std::cout << "耗时：" << end_time - start_time << " us" << std::endl;
	std::cout << "-----  atomic  ---- " << std::endl;
}

std::atomic<size_t> count1(0);

void threadFun1()
{
	for (int i = 0; i < 10000; i++)
		count1++;
}

void Test2(void)
{
	std::cout << "-- Test2 --" << std::endl;
	clock_t start_time = clock();
 
	// 启动多个线程
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++)
		threads.push_back(std::thread(threadFun1));
	for (auto&thad : threads)
		thad.join();
 
	// 检测count是否正确 10000*10 = 100000
	std::cout << "count number:" << count1 << std::endl;
 
	clock_t end_time = clock();
	std::cout << "耗时：" << end_time - start_time << " us" << std::endl;
}
size_t count2 = 0;
void threadFun2()
{
	for (int i = 0; i < 10000; i++)
		count2++;
}

void Test3(void)
{
	std::cout << "-- Test3 --" << std::endl;
	clock_t start_time = clock();
 
	// 启动多个线程
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++)
		threads.push_back(std::thread(threadFun2));
	for (auto&thad : threads)
		thad.join();
 
	// 检测count是否正确 10000*10 = 100000
	std::cout << "count number:" << count2 << std::endl;
 
	clock_t end_time = clock();
	std::cout << "耗时：" << end_time - start_time << " us" << std::endl;
}
A testA;
std::shared_ptr<A> A_shared_ptr = std::make_shared<A>();
std::atomic<A> A_atomic;
std::atomic<A* > A_ptr;
std::atomic<size_t> counter;
std::atomic<size_t> counter2;
std::mutex mtx_A;
int read_thread_num = 7;
int write_thread_num = 1;

void WriteFunc()
{
	for (int i = 0; i < 10000; i++)
	{
		int tmp = rand()%1000000;
		A a_tmp;
		a_tmp.a1 = tmp;
		a_tmp.a2 = tmp;
		// std::lock_guard<std::mutex> lock(mtx_A);
		// A* tmp = A_ptr.load();
		A_atomic.exchange(a_tmp);
		// delete ptr;
	}
}
void ReadFunc()
{
	for (int i = 0; i < 10000; i++)
	{
		A tmpA;
		tmpA = A_atomic.load();
		if(tmpA.a1 != tmpA.a2)
		{
			counter++;
		}
	}
		
}

void WriteFunc2()
{
	for (int i = 0; i < 10000; i++)
	{
		int tmp = rand()%1000000;
		
		std::shared_ptr<A> ptr =std::make_shared<A>();
		ptr->a1 = tmp;
		ptr->a2 = tmp;
		std::lock_guard<std::mutex> lock(mtx);
		A_shared_ptr = ptr;
	}
}

void ReadFunc2()
{
	for (int i = 0; i < 10000; i++)
	{
		std::lock_guard<std::mutex> lock(mtx);
		if(A_shared_ptr->a1 != A_shared_ptr->a2)
		{
			counter++;
		}
	}	
}

DataPack<A> pack_a;
void WriteFunc3()
{
	for (int i = 0; i < 10000; i++)
	{
		int tmp = rand()%1000000;
		A a;
		a.a1 = tmp;
		a.a2 = tmp;
		pack_a.updatePack(std::make_shared<A>(a));
	}
}

void ReadFunc3()
{
	for (int i = 0; i < 10000; i++)
	{
		auto ptr = pack_a.getDataPtr();
		if(ptr==nullptr)
		{
			continue;
		}
		if(ptr->a1 != ptr->a2)
		{
			counter++;
		}
	}	
}

void Test4(void)
{
	std::cout << "-- Test4 --" << std::endl;
	clock_t start_time = clock();
 
	// 启动多个线程
	std::vector<std::thread> threads;
	for (int i = 0; i < write_thread_num; i++)
		threads.push_back(std::thread(WriteFunc));
	for (int i = 0; i < read_thread_num; i++)
		threads.push_back(std::thread(ReadFunc));
	for (auto&thad : threads)
		thad.join();
 
	// 检测count是否正确 10000*10 = 100000
	std::cout << "count number:" << counter << std::endl;
 
	clock_t end_time = clock();
	std::cout << "耗时：" << end_time - start_time << " us" << std::endl;
}

void Test5(void)
{
	std::cout << std::endl;
	std::cout << "-- Test5 --" << std::endl;
	std::cout << "Not Packed Data" << std::endl;
	
	clock_t start_time = clock();
 
	// 启动多个线程
	std::vector<std::thread> threads;
	for (int i = 0; i < write_thread_num; i++)
		threads.push_back(std::thread(WriteFunc2));
	for (int i = 0; i < read_thread_num; i++)
		threads.push_back(std::thread(ReadFunc2));
	for (auto&thad : threads)
		thad.join();
 
	// 检测count是否正确 10000*10 = 100000
	std::cout << "count number:" << counter << std::endl;
 
	clock_t end_time = clock();
	std::cout << "耗时：" << end_time - start_time << " us" << std::endl;
}

void Test6(void)
{
	std::cout << std::endl;
	std::cout << "-- Test6 --" << std::endl;
	std::cout << "DataPack using std::mutex" << std::endl;
	
	clock_t start_time = clock();
 
	// 启动多个线程
	std::vector<std::thread> threads;
	for (int i = 0; i < write_thread_num; i++)
		threads.push_back(std::thread(WriteFunc3));
	for (int i = 0; i < read_thread_num; i++)
		threads.push_back(std::thread(ReadFunc3));
	for (auto&thad : threads)
		thad.join();
 
	// 检测count是否正确 10000*10 = 100000
	std::cout << "count number:" << counter << std::endl;
 
	clock_t end_time = clock();
	std::cout << "耗时：" << end_time - start_time << " us" << std::endl;
}
DataPack2<A> pack_a2;
void WriteFunc4()
{
	for (int i = 0; i < 10000; i++)
	{
		int tmp = rand()%1000000;
		A a;
		a.a1 = tmp;
		a.a2 = tmp;
		pack_a2.updatePack(std::make_shared<const A>(a));
	}
}

void ReadFunc4()
{
	for (int i = 0; i < 10000; i++)
	{
		auto ptr = pack_a2.getDataPtr();
		if(ptr==nullptr)
		{
			continue;
		}
		if(ptr->a1 != ptr->a2)
		{
			counter++;
		}
	}	
}

void Test7(void)
{
	std::cout << std::endl;
	std::cout << "-- Test 7 --" << std::endl;
	std::cout << "DataPack2 using readWriteLock" << std::endl;
	
	clock_t start_time = clock();
 
	// 启动多个线程
	std::vector<std::thread> threads;
	for (int i = 0; i < write_thread_num; i++)
		threads.push_back(std::thread(WriteFunc4));
	for (int i = 0; i < read_thread_num; i++)
		threads.push_back(std::thread(ReadFunc4));
	for (auto&thad : threads)
		thad.join();
 
	// 检测count是否正确 10000*10 = 100000
	std::cout << "count number:" << counter << std::endl;
 
	clock_t end_time = clock();
	std::cout << "耗时：" << end_time - start_time << " us" << std::endl;
}

int main(void)
{
	std::cout << "请输入写线程的个数: ";
	std::cin >> write_thread_num;
	std::cout << "请输入读线程的个数: ";
	std::cin >> read_thread_num;
	// Test1();
	// Test2();
	// Test3();
	// Test4();
	Test5();
	Test6();
	Test7();
	
 
	return 0;
}


 

 


