/**
http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

This class is probably unneeded as there exists a concurrent queue implementation in boost.
http://www.boost.org/doc/libs/1_53_0_beta1/doc/html/lockfree/examples.html
*/

#pragma once
#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <queue>
#include <boost/thread/mutex.hpp>

template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> m_queue;
    mutable boost::mutex m_mutex;
    boost::condition_variable m_condition_variable;
public:
	/** 
	push is used to add items to the queue
	*/
    void push(Data const& data) {
        boost::mutex::scoped_lock lock(m_mutex);
        m_queue.push(data);
        lock.unlock();
        m_condition_variable.notify_one();
    }

	/**
	empty is used to check if the queue is empty.
	*/
    bool empty() const {
        boost::mutex::scoped_lock lock(m_mutex);
        return m_queue.empty();
    }

	/**
	removes an item from the queue.
	*/
    bool try_pop(Data& popped_value)
    {
        boost::mutex::scoped_lock lock(m_mutex);
        if(m_queue.empty())
        {
            return false;
        }
        
        popped_value=m_queue.front();
        m_queue.pop();
        return true;
    }

	/**
	pop an item from the queue, unless the queue is empty.
	In that case the call blocks until an item is pushed 
	onto the queue
	*/
    void wait_and_pop(Data& popped_value)
    {
        boost::mutex::scoped_lock lock(m_mutex);
        while(m_queue.empty())
        {
            m_condition_variable.wait(lock);
        }
        
        popped_value=m_queue.front();
        m_queue.pop();
    }

};

#endif // CONCURRENT_QUEUE_H