#ifndef _SIP_CLIENT_HPP__
#define _SIP_CLIENT_HPP__

#include "SipInterface.h"

typedef void (*Callback_SipOffline)();
typedef void (*CallBack_RealStream)( std::string id, int videoType, char* data, int len, int isKey, int64_t pts );

//////////////////////////////////////////////////////////////////////////
class CSipClient : public CSipInterface
{
public:
    CSipClient();
    virtual ~CSipClient();

public:
	virtual void RunSipStack();
	virtual bool GetTargetAddr( const std::string& code, resip::Uri& targetAddr );

    virtual void OnOffline( const CClientOfflineMsg& ) {}

    std::string m_sSeverid;
};

#endif // _SIP_CLIENT_HPP__
