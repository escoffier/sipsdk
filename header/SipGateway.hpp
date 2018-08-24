#ifndef _SIP_GATEWARY_HPP__
#define _SIP_GATEWARY_HPP__

#include "SipInterface.h"

//////////////////////////////////////////////////////////////////////////
class CSipGateway : public CSipInterface
{
public:
    CSipGateway();
    virtual ~CSipGateway();

public:
	virtual void RunSipStack();
	virtual bool GetTargetAddr( const std::string& code, resip::Uri& targetAddr );
	virtual bool CheckUserRegExpires( const std::string &user );

public:
	virtual void OnOffline( const COfflineMsg& ) {}
	virtual void OnPlayOver( const CPlayOverMsg& ) {}
	virtual void OnPlayClose( const CPlayCloseMsg& ) {}
	virtual void OnMediaConnect( const CMediaReconnectMsg & ) {}

 public:
     static void SipMsgCallBack(const resip::SipMessage &msg, bool isRecv);
    
protected:
    resip::RegistrationPersistenceManager* m_regData;
};

#endif // _SIP_GATEWARY_HPP__
