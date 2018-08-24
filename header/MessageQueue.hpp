#ifndef MESSAGE_QUEUE_HPP_
#define MESSAGE_QUEUE_HPP_

#include <queue>
//#include "ILocker.h"
#include <mutex>
#include "../Message/Message.hpp"

class CMessageQueue
{
public:
	CMessageQueue()
	{
	}

	~CMessageQueue()
	{
		std::lock_guard<std::mutex> locker( m_lock );
		while( !m_queue.empty() )
		{
			CMessage *msg = m_queue.front();
			m_queue.pop();
			delete msg;
		}
	}

private:
	CMessageQueue( const CMessageQueue& );
	CMessageQueue& operator= ( const CMessageQueue& );

public:
	static void AddMessage( CMessage *msg )
	{
		CMessageQueue &obj = GetInstance();
		obj.AddHelper( msg );
	}

	static CMessage* GetMessage()
	{
		CMessageQueue &obj = GetInstance();
		return obj.GetHelper();
	}

private:
	static CMessageQueue& GetInstance()
	{
		static CMessageQueue obj;
		return obj;
	}

	void AddHelper( CMessage *msg )
	{
		std::lock_guard<std::mutex> locker( m_lock );
		if( msg != 0 )
		{
			m_queue.push( msg );
		}		 
	}

	CMessage* GetHelper()
	{
		std::lock_guard<std::mutex> locker( m_lock );
		CMessage *msg = 0;
		if( !m_queue.empty() )
		{
			msg = m_queue.front();
			m_queue.pop();
		}

		return msg;
	}

private:   
	std::queue< CMessage* > m_queue;
	//CLocker                 m_lock;
	std::mutex m_lock;
};   

#endif // MESSAGE_QUEUE_HPP_