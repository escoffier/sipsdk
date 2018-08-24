#include "Log.hpp"

int whl::log::m_count = 7;

whl::log::log()
{
	m_level = log_level_nouse;
	m_year = 0;
	m_month = 0;
	m_day = 0;
#ifdef WIN32
	_mkdir( "log" );
#else
	mkdir( "log", 0 );
#endif
}

whl::log::~log()
{
}

void whl::log::set_log_path( std::string &path )
{
#ifdef WIN32
    _mkdir( path.c_str() );
#else
    mkdir( path.c_str(), 0 );
#endif

    whl::log &obj = instance();
    obj.m_path = path;
}

log_level whl::log::get_log_level()
{
	whl::log &obj = instance();
	return obj.m_level;
}

void whl::log::set_log_level( int level )
{
	whl::log &obj = instance();
	switch( level )
	{
	case log_level_debug: obj.m_level = log_level_debug; break;
	case log_level_info:  obj.m_level = log_level_info;  break;
	case log_level_warn:  obj.m_level = log_level_warn;  break;
	case log_level_error: obj.m_level = log_level_error; break;
	default: obj.m_level = log_level_nouse; break;
	}
}

std::mutex& whl::log::get_lock()
{
	whl::log &obj = instance();
	return( obj.m_lock );
}

whl::log& whl::log::get()
{
	whl::log &obj = instance();
	time( &obj.m_time );
	struct tm *p = localtime( &obj.m_time );
	sprintf( obj.m_timebuf, "%04d-%02d-%02d %02d:%02d:%02d(%03d) ", 
		p->tm_year + 1900,
		p->tm_mon + 1,
		p->tm_mday,
		p->tm_hour,
		p->tm_min,
		p->tm_sec,
		0 );

	if( p->tm_year + 1900 != obj.m_year || p->tm_mon + 1 != obj.m_month || p->tm_mday != obj.m_day )
	{
		obj.m_year  = p->tm_year + 1900;
		obj.m_month = p->tm_mon + 1;
		obj.m_day   = p->tm_mday;

		if( obj.is_open() )
		{
			obj.close();
		}

// 		clear();

        char name[32] = { 0 };
        sprintf( name, "%s/%04d%02d%02d.log", obj.m_path.c_str(), obj.m_year, obj.m_month, obj.m_day );
		obj.open( name, std::ios_base::app );

		if( !obj.is_open() )
		{
			std::cout << "open log file \'" << name << "\' failuee!" << std::endl;
#ifdef WIN32
			exit( -1 );
#else
			_exit( -1 );
#endif
        }
        obj << name;
	}

	obj << obj.m_timebuf;
	return obj;
}
