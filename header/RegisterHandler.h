#ifndef _SERVER_REGISTERHANDLER_H_
#define _SERVER_REGISTERHANDLER_H_

#include <list>
#include <string>
#include "resip/dum/RegistrationHandler.hxx"
#include "resip/dum/DialogUsageManager.hxx"
#include "resip/dum/InMemoryRegistrationDatabase.hxx"
#include "resip/dum/MasterProfile.hxx"
#include "resip/dum/ClientAuthManager.hxx"
#include "resip/stack/Headers.hxx"
#include "rutil/Logger.hxx"
#include "resip/dum/RegistrationHandler.hxx"
#include "resip/dum/DialogUsageManager.hxx"
#include "resip/dum/InMemoryRegistrationDatabase.hxx"
#include "resip/dum/MasterProfile.hxx"
#include "resip/dum/ClientAuthManager.hxx"
#include "resip/stack/Headers.hxx"
#include "SipInterface.h"

class CSipInterface;
class ClientRegHandler : public resip::ClientRegistrationHandler
{
public:
	ClientRegHandler( CSipInterface *pSip ) : m_pSip( pSip )
	{
	}

public:
    virtual void onSuccess(resip::ClientRegistrationHandle h, const resip::SipMessage& response)
    {
		// 向上级注册成功
		if( m_pSip != NULL )
		{
			m_pSip->OnRegisterResponse( h.get(), &response );
		}
    }

    virtual void onRemoved(resip::ClientRegistrationHandle h, const resip::SipMessage& response)
    {
		// 上级主动断开注册
		if( m_pSip != NULL )
		{
		}
    }

    virtual void onFailure(resip::ClientRegistrationHandle h, const resip::SipMessage& response)
    {
		// 向上级注册失败
		if( m_pSip != NULL )
		{
			m_pSip->OnRegisterResponse( h.get(), &response );
		}
    }

    virtual int onRequestRetry(resip::ClientRegistrationHandle, int retrySeconds, const resip::SipMessage& response)
    {
        return 30;
    }

private:
	CSipInterface* m_pSip;
};

class ServerRegHandler: public resip::ServerRegistrationHandler
{
public:
	ServerRegHandler( CSipInterface *pSip );
	virtual ~ServerRegHandler();

public:
	/** 用户刷新注册
	 *
	 */
    virtual void onRefresh(resip::ServerRegistrationHandle, const resip::SipMessage& reg);

	/** 用户注销
	 *
	 */
    virtual void onRemove(resip::ServerRegistrationHandle, const resip::SipMessage& reg);

	/** 用户注销
	 *
	 */
    virtual void onRemoveAll(resip::ServerRegistrationHandle, const resip::SipMessage& reg);

	/** 用户注册
	 *
	 */
    virtual void onAdd(resip::ServerRegistrationHandle, const resip::SipMessage& reg);
    virtual void onQuery(resip::ServerRegistrationHandle, const resip::SipMessage& reg);

private:
	CSipInterface* m_pSip;
};

/** 用户鉴权管理
 *
 */
class CNPClientAuthManager : public resip::ClientAuthManager
{
public:
	CNPClientAuthManager( CSipInterface *pSip ) : m_pSip( pSip )
	{
	}

public:
	virtual bool handle(resip::UserProfile& userProfile, resip::SipMessage& origRequest, const resip::SipMessage& response);

private:
	CSipInterface* m_pSip;
};

#endif
