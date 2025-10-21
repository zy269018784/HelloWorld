#include <thread>

#include <boost/thread.hpp>

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_recursive_mutex.hpp>

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_condition_any.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_condition_any.hpp>

#include <boost/interprocess/sync/file_lock.hpp>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <boost/interprocess/file_mapping.hpp>

#include <boost/interprocess/shared_memory_object.hpp>

int TestCXXSynchornization()
{
	std::mutex mtx;
	mtx.lock();
	auto ret = mtx.try_lock();
	mtx.unlock();

	return 0;
}

int TestBoostSynchornization()
{
	bool ret;
	/*
		Mutex
	*/
	boost::mutex mtx;
	mtx.lock();
	ret = mtx.try_lock();
	mtx.unlock();
	mtx.try_lock_for(boost::chrono::milliseconds(500));

	boost::interprocess::named_mutex named_mtx(boost::interprocess::open_or_create, "aaa");
	named_mtx.lock();
	ret = named_mtx.try_lock();
	named_mtx.unlock();
	named_mtx.try_lock_for(boost::chrono::milliseconds(500));
	named_mtx.remove("aaa");


	/*
		Semaphore
	*/
	boost::interprocess::interprocess_semaphore sem(1);
	sem.wait();
	sem.post();
	ret = sem.try_wait();
#if 0
	ret = sem.timed_wait(boost::chrono::milliseconds(500));

	boost::interprocess::named_semaphore named_sem(boost::interprocess::open_or_create, "aaa", 1);
	named_sem.wait();
	named_sem.post();
	ret = named_sem.try_wait();
	ret = named_sem.timed_wait(boost::chrono::milliseconds(500));
	ret = named_sem.remove("aaa");

	/*
		Condition Variable
	*/
	boost::interprocess::interprocess_condition  cond;
	cond.notify_all();
	cond.notify_one();
	cond.wait(boost::chrono::milliseconds(500));
	cond.timed_wait(mtx, boost::chrono::milliseconds(500));
	cond.wait_for(mtx, boost::chrono::milliseconds(500));
	cond.wait_until(mtx, boost::chrono::milliseconds(500));

	boost::interprocess::interprocess_condition_any  cond_any;
	cond_any.notify_all();
	cond_any.notify_one();
	cond_any.wait(boost::chrono::milliseconds(500));
	cond_any.timed_wait(mtx, boost::chrono::milliseconds(500));
	cond_any.wait_for(mtx, boost::chrono::milliseconds(500));
	cond_any.wait_until(mtx, boost::chrono::milliseconds(500));


	boost::interprocess::named_condition named_cond(boost::interprocess::open_or_create, "aaa");
	named_cond.notify_all();
	named_cond.notify_one();
	named_cond.wait(boost::chrono::milliseconds(500));
	named_cond.timed_wait(mtx, boost::chrono::milliseconds(500));
	named_cond.wait_for(mtx, boost::chrono::milliseconds(500));
	named_cond.wait_until(mtx, boost::chrono::milliseconds(500));

	boost::interprocess::named_condition named_cond_any(boost::interprocess::open_or_create, "aaa");
	named_cond_any.notify_all();
	named_cond_any.notify_one();
	named_cond_any.wait(boost::chrono::milliseconds(500));
	named_cond_any.timed_wait(mtx, boost::chrono::milliseconds(500));
	named_cond_any.wait_for(mtx, boost::chrono::milliseconds(500));
	named_cond_any.wait_until(mtx, boost::chrono::milliseconds(500));
	named_cond_any.remove("aaa");

	/*
		file lock
	*/
	boost::interprocess::file_lock flock("my_file");
	flock.lock();	
	flock.unlock();
	flock.lock_sharable();
	flock.unlock_sharable();
	flock.lock_shared();
	flock.unlock_shared();
	flock.try_lock();
	flock.try_lock_for(boost::chrono::milliseconds(500));
	flock.try_lock_until(boost::chrono::milliseconds(500));
	flock.try_lock_shared();
	flock.try_lock_shared_for(boost::chrono::milliseconds(500));
	flock.try_lock_shared_until(boost::chrono::milliseconds(500));
	flock.try_lock_sharable();
	
	flock.timed_lock(boost::chrono::milliseconds(500));
	flock.timed_lock_sharable(boost::chrono::milliseconds(500));
	
	/*
		Message Queue
	*/
	boost::interprocess::message_queue mq(boost::interprocess::open_only, "aaa");
	mq.get_max_msg();
	mq.get_max_msg_size();
	mq.get_num_msg();
	int size = 0;
	unsigned long long recvd_size = 0;
	unsigned int priority = 0;
	mq.receive(nullptr, size, recvd_size, priority);
	mq.try_receive(nullptr, size, recvd_size, priority);
	mq.timed_receive(nullptr, size, recvd_size, priority, boost::chrono::milliseconds(500));
	mq.send(nullptr, size, priority);
	mq.try_send(nullptr, size, priority);
	mq.timed_send(nullptr, size, priority, boost::chrono::milliseconds(500));

	/*
		file mapping
	*/
	boost::interprocess::file_mapping m_file("/usr/home/file", boost::interprocess::read_write);
	m_file.get_mapping_handle();

	/*
		shared memory
	*/
	boost::interprocess::shared_memory_object shm_obj(boost::interprocess::create_only, "shared_memory", boost::interprocess::read_write);
	shm_obj.get_mapping_handle();


	//Map the second half of the memory
	boost::interprocess::mapped_region m_region(shm_obj, boost::interprocess::read_write, 10, 10);
#endif

	return 0;
}