/**
    SIP Subscribe 方法处理回调
    */

#ifndef _Subscribe_HANDLER_H
#define _Subscribe_HANDLER_H
#include "rutil/Logger.hxx"
#include "SipInterface.h"

#ifndef RESIPROCATE_SUBSYSTEM
#define RESIPROCATE_SUBSYSTEM Subsystem::DUM
#endif

class CSubscriptionHandler : public resip::ClientSubscriptionHandler, public resip::ServerSubscriptionHandler
{
public:
	CSubscriptionHandler( CSipInterface *pSip ) : m_pSip( pSip )
	{
	}

public:
    virtual void onUpdatePending(ClientSubscriptionHandle h, const SipMessage& notify, bool outOfOrder)
	{
    }

    virtual void onUpdateActive(ClientSubscriptionHandle h, const SipMessage& notify, bool outOfOrder)
    {
        h->acceptUpdate();
		if( m_pSip != NULL )
		{
			m_pSip->OnUpdateSubscription( h.get(), &notify );
		}
    }

    virtual void onUpdateExtension(ClientSubscriptionHandle h, const SipMessage& notify, bool outOfOrder)
	{
		h->acceptUpdate();
		if( m_pSip != NULL )
		{
			m_pSip->OnUpdateSubscription( h.get(), &notify );
		}
	}

    virtual int onRequestRetry(ClientSubscriptionHandle h, int retrySeconds, const SipMessage& notify)
	{
        return retrySeconds;
    }

    virtual void onTerminated(ClientSubscriptionHandle h, const SipMessage* msg)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnCancelSubscription( h.get(), msg );
		}
    }

	virtual void onNewSubscription(ClientSubscriptionHandle h, const SipMessage& notify)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnAddSubscription( h.get(), &notify );
		}
    }

    virtual void onNewSubscription(ServerSubscriptionHandle h, const SipMessage& sub)
    {
		if( m_pSip != NULL )
		{
			m_pSip->OnAddSubscription( h.get(), &sub );
		}
    }

    virtual void onResponse(ServerSubscriptionHandle h, const SipMessage& msg)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnSubscriptionResponse( h.get(), &msg );
		}
    }

	virtual void onError(ServerSubscriptionHandle h, const SipMessage& msg)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnSubscriptionResponse( h.get(), &msg );
		}
	}

    virtual void onTerminated(ServerSubscriptionHandle h)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnCancelSubscription( h.get() );
		}
    }

private:
	CSipInterface* m_pSip;
};

#endif    /* ! _Subscribe_HANDLER_H */
