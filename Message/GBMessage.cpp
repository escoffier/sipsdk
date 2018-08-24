#include "GBMessage.hpp"
//#include "ILocker.h"
#include <atomic>
static std::atomic<unsigned int>  g_msgsn(0);
//static CLocker      g_msg_lock;

bool CGBMessage::CompareNoCase( const char *type, const char *label )
{
	// 参数校验
	if( type == NULL || label == NULL )
	{
		return false;
	}

	// 比较是否相等
	char ch1 = type[0];
	char ch2 = label[0];
	size_t i = 0;
	while( ch1 != 0 && ch2 != 0 )
	{
		if( ch1 >= 'a' && ch1 <= 'z' )
		{
			ch1 -= 'a';
			ch1 += 'A';
		}

		if( ch2 >= 'a' && ch2 <= 'z' )
		{
			ch2 -= 'a';
			ch2 += 'A';
		}

		if( ch1 != ch2 )
		{
			return false;
		}
		else
		{
			++i;
			ch1 = type[i];
			ch2 = label[i];
		}
	}

	return( ch1 == 0 && ch2 == 0 );
}


bool CGBMessage::CompareNoCase( const std::string &str, const char *label )
{
	// 参数校验
	const char* type = str.c_str();
	if( type == NULL || label == NULL )
	{
		return false;
	}

	// 比较是否相等
	char ch1 = type[0];
	char ch2 = label[0];
	size_t i = 0;
	while( ch1 != 0 && ch2 != 0 )
	{
		if( ch1 >= 'a' && ch1 <= 'z' )
		{
			ch1 -= 'a';
			ch1 += 'A';
		}

		if( ch2 >= 'a' && ch2 <= 'z' )
		{
			ch2 -= 'a';
			ch2 += 'A';
		}

		if( ch1 != ch2 )
		{
			return false;
		}
		else
		{
			++i;
			ch1 = type[i];
			ch2 = label[i];
		}
	}

	return( ch1 == 0 && ch2 == 0 );
}

unsigned int CGBMessage::GetNextSN()
{
	//CNPAutoLock locker( g_msg_lock );
	g_msgsn++;
	return g_msgsn;
}

bool CGBMessage::DecodeSubject()
{
	std::vector< std::string > vec;
	size_t b = 0;
	size_t e = 0;
	while( b != std::string::npos )
	{
		e = m_subject.find_first_of( ',', b );
		vec.push_back( m_subject.substr( b, e ) );
		if( e != std::string::npos )
		{
			b = e + 1;
		}
		else
		{
			break;
		}
	}

	if( vec.size() != 2 )
	{
		return false;
	}

	// 
	std::string send = vec[0];
	std::string recv = vec[1];

	// 发送者ID和序列号
	vec.clear();
	b = 0;
	while( b != std::string::npos )
	{
		e = send.find_first_of( ':', b );
		vec.push_back( send.substr( b, e ) );
		if( e != std::string::npos )
		{
			b = e + 1;
		}
		else
		{
			break;
		}
	}

	if( vec.size() != 2 )
	{
		return false;
	}
	else
	{
		m_sender.id = vec[0];
		m_sender.seq = vec[1];
	}

	// 接收者ID和序列号
	vec.clear();
	b = 0;
	while( b != std::string::npos )
	{
		e = recv.find_first_of( ':', b );
		vec.push_back( recv.substr( b, e ) );
		if( e != std::string::npos )
		{
			b = e + 1;
		}
		else
		{
			break;
		}
	}

	if( vec.size() != 2 )
	{
		return false;
	}
	else
	{
		m_recver.id = vec[0];
		m_recver.seq = vec[1];
	}

	return true;
}

