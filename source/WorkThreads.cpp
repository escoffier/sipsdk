#include "WorkThreads.h"
#include "MessageQueue.hpp"
#include "Log.hpp"

static bool g_work_thread_quit = false;

#if defined(WIN32)
DWORD WINAPI WorkThreadProc( void* param )
#elif defined(__linux__)
void* WorkThreadProc( void* param )
#else
void* WorkThreadProc( void* param )
#endif
{
	CSipInterface *pInterface = (CSipInterface*)( param );
	while( !g_work_thread_quit )
	{
		CMessage *msg = CMessageQueue::GetMessage();
		if( msg != NULL )
		{
			msg->Process( pInterface );
			delete msg;
		}
		else
		{
#if defined(WIN32)
			Sleep( 10 );
#else
			usleep( 10*1000 );
#endif
		}
	}
#if defined(WIN32)

	DEBUG_LOG( "线程退出!" );
	return 0;
#endif
}

void CWorkThreads::Start( CSipInterface *pInterface, int threads )
{
    g_work_thread_quit = false;

	// 创建线程池
	for( int i = 0; i < threads; ++i )
	{
#if defined(WIN32)
        HANDLE handle = CreateThread( NULL, 0, WorkThreadProc, (void*)pInterface, 0, 0 );
        if( handle == NULL )
        {
            ERROR_LOG( "线程" << i << "启动失败!" );
        }
        else
        {
            CloseHandle( handle );
        }
#elif defined(__linux__)
        pthread_t handle;
		if( pthread_create( &handle, 0, WorkThreadProc, (void*)pInterface ) == -1 )
        {
            ERROR_LOG( "线程" << i << "启动失败!" );
        }
        else
        {
            pthread_detach( handle );
        }
#else
#endif
	}
}

void CWorkThreads::Stop()
{
	g_work_thread_quit = true;
}

