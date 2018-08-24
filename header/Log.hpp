#ifndef WHL_LOG_HPP_
#define WHL_LOG_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <mutex>
//#include "ILocker.h"

#define DEBUG_LOG( msg ) std::cout << msg << std::endl; /*if( whl::log::get_log_level() >= log_level_debug ) {  std::lock_guard<std::mutex> _gb_lock_( whl::log::m_lock() ); whl::log::get() << "[DEBUG]: " << "<" << __FILE__ << ":" << __LINE__ << "> " << msg << std::endl; }*/
#define INFO_LOG( msg )  std::cout << msg << std::endl; /*if( whl::log::get_log_level() >= log_level_info )  {  std::lock_guard<std::mutex> _gb_lock_( whl::log::m_lock() ); whl::log::get() << "[INFO ]: " << msg << std::endl; }*/
#define WARN_LOG( msg )  std::cout << msg << std::endl; /*if( whl::log::get_log_level() >= log_level_warn )  {  std::lock_guard<std::mutex> _gb_lock_( whl::log::m_lock() ); whl::log::get() << "[WARN ]: " << msg << std::endl; }*/
#define ERROR_LOG( msg ) std::cout << msg << std::endl; /*if( whl::log::get_log_level() >= log_level_error ) { std::lock_guard<std::mutex> _gb_lock_( whl::log::get_lock ); whl::log::get() << "[ERROR]: " << msg << std::endl; }*/

//////////////////////////////////////////////////////////////////////////
enum log_level
{
	log_level_nouse = 0,
	log_level_error,	
	log_level_warn,
	log_level_info,
	log_level_debug,
};

///////////////////////////////////////////////////////////////////////////////////////////////////
namespace whl
{
///////////////////////////////////////////////////////////////////////////////////////////////////
class log : public std::ofstream
{
private:
	log();
	~log();

private:
	log( const log &rhs );
	void operator=( const log &rhs );

public:
    static void set_log_path( std::string &path );
	static log_level get_log_level();
	static void set_log_level( int level );
	static std::mutex& get_lock();
	static log& get();

private:
	static log& instance()
	{
		static log obj;
		return obj;
	}

private:
	
	log_level m_level;
	time_t    m_time;
	int       m_year;
	int       m_month;
	int       m_day;
    char      m_timebuf[24];
    std::string m_path;
public:
	std::mutex	  m_lock;
private:    
    static int m_count;
};
} // end namespace whl
//////////////////////////////////////////////////////////////////////////
#endif // WHL_LOG_HPP_
