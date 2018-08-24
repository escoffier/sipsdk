#ifndef PROCESS_THREADS_H_
#define PROCESS_THREADS_H_
//#include "npthread.h"
#include "SipInterface.h"

class CWorkThreads
{
public:
	static void Start( CSipInterface *pInterface, int threads );
	static void Stop();

private:
	CWorkThreads()
	{
	}

private:
	static CWorkThreads& GetInstance()
	{
		static CWorkThreads obj;
		return obj;
	}
};

#endif // PROCESS_THREADS_H

