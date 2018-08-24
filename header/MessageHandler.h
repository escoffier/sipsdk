/**
SIP Message 方法处理回调
*/

#ifndef _MESSAGE_HANDLER_H
#define _MESSAGE_HANDLER_H

#include "SipInterface.h"

class ClientMessageHandler : public resip::ClientPagerMessageHandler
{
public:
	ClientMessageHandler( CSipInterface *pSip ) : m_pSip( pSip )
	{
	}

public:
	virtual void onSuccess(resip::ClientPagerMessageHandle h, const resip::SipMessage& status)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnMessageResponse( h.get(), &status );
		}
	}

	virtual void onFailure(resip::ClientPagerMessageHandle h, const resip::SipMessage& status, std::auto_ptr<resip::Contents> contents)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnMessageResponse( h.get(), &status );
		}
	}

private:
	CSipInterface* m_pSip;
};

class ServerMessageHandler : public resip::ServerPagerMessageHandler
{
public:
	ServerMessageHandler( CSipInterface *pSip ) : m_pSip( pSip )
	{
	}

public:
	virtual void onMessageArrived(resip::ServerPagerMessageHandle h, const resip::SipMessage& message)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnMessage( h.get(), &message );
		}
	}

private:
	CSipInterface* m_pSip;
};

#endif    /* ! _MESSAGE_HANDLER_H */

