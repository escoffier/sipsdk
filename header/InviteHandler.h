#if !defined(_INVITE_HANDLER_H)
#define _INVITE_HANDLER_H

#include "rutil/Logger.hxx"
#include "SipInterface.h"

#ifndef RESIPROCATE_SUBSYSTEM
#define RESIPROCATE_SUBSYSTEM resip::Subsystem::NPSIPSERVER
#endif

using namespace resip;

// Generic InviteSessionHandler
class InviteHandler : public InviteSessionHandler, public OutOfDialogHandler
{
public:
	InviteHandler( CSipInterface *pSip ) : m_pSip( pSip )
	{
	}

public:
    virtual void onNewSession(ClientInviteSessionHandle h, InviteSession::OfferAnswerType oat, const SipMessage& msg)
    {
    }

    virtual void onNewSession(ServerInviteSessionHandle h, InviteSession::OfferAnswerType oat, const SipMessage& msg)
    {
		if( m_pSip != NULL) 
		{
			m_pSip->OnInvite( h.get(), &msg );
		}
    }

    virtual void onFailure(ClientInviteSessionHandle h, const SipMessage& msg)
    {
		if( m_pSip != NULL) 
		{
			m_pSip->OnInviteResponse( h.get(), &msg );
		}
    }

    virtual void onProvisional(ClientInviteSessionHandle, const SipMessage& msg)
    {
    }

    virtual void onConnected(ClientInviteSessionHandle h, const SipMessage& msg)
    {
		if( m_pSip != NULL) 
		{
			m_pSip->OnInviteResponse( h.get(), &msg );
		}
    }

    virtual void onStaleCallTimeout(ClientInviteSessionHandle handle)
    {
    }

    virtual void onConnected(InviteSessionHandle h, const SipMessage& msg)
    {
// 		if( m_pSip != NULL )
// 		{
// 			m_pSip->OnAck( h.get(), &msg );
// 		}
    }

	virtual void onConnectedConfirmed(InviteSessionHandle h, const SipMessage& msg)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnAck( h.get(), &msg );
		}
	}

    virtual void onRedirected(ClientInviteSessionHandle, const SipMessage& msg)
    {
    }

    virtual void onTerminated(InviteSessionHandle h, InviteSessionHandler::TerminatedReason reason, const SipMessage* msg)
    {
		//主动发送的bye消息就没有必要再通过协议栈通知上层
		if (reason == InviteSessionHandler::LocalBye)
		{
			InfoLog(<< "!! ############ onTerminated:terminated by local_byapplication");
		}
		else
		{
			if( m_pSip != NULL )
			{
				m_pSip->OnBye( h.get(), msg );
			}
		}
    }

    virtual void onAnswer(InviteSessionHandle, const SipMessage& msg, const SdpContents& sdp)
    {
    }

    virtual void onOffer(InviteSessionHandle is, const SipMessage& msg, const SdpContents& sdp)      
    {
    }

    virtual void onEarlyMedia(ClientInviteSessionHandle, const SipMessage& msg, const SdpContents& sdp)
    {
    }

    virtual void onOfferRequired(InviteSessionHandle h, const SipMessage& msg)
    {
// 		if( m_pSip != NULL )
// 		{
// 			m_pSip->OnInvite( h.get(), &msg );
// 		}
	}

    virtual void onOfferRejected(InviteSessionHandle, const SipMessage* msg)
    {
    }

    virtual void onRefer(InviteSessionHandle, ServerSubscriptionHandle, const SipMessage& msg)
    {
    }

    virtual void onReferAccepted(InviteSessionHandle, ClientSubscriptionHandle, const SipMessage& msg)
    {
    }

    virtual void onReferRejected(InviteSessionHandle, const SipMessage& msg)
    {
    }

    virtual void onReferNoSub(InviteSessionHandle, const SipMessage& msg)
    {
    }

    virtual void onInfo(InviteSessionHandle h, const SipMessage& msg)
    {
		if( m_pSip != NULL )
		{
			m_pSip->OnCallInfo( h.get(), &msg );
		}
    }

    virtual void onInfoSuccess(InviteSessionHandle h, const SipMessage& msg)
    {
		if( m_pSip != NULL )
		{
			m_pSip->OnCallInfoResponse( h.get(), &msg );
		}
    }

    virtual void onInfoFailure(InviteSessionHandle h, const SipMessage& msg)
    {
		if( m_pSip != NULL )
		{
			m_pSip->OnCallInfoResponse( h.get(), &msg );
		}
    }

    virtual void onMessage(InviteSessionHandle h, const SipMessage& msg)
    {
		h->acceptNIT();
		if( m_pSip != NULL )
		{
			m_pSip->OnCallMessage( h.get(), &msg );
		}
    }

    virtual void onMessageSuccess(InviteSessionHandle h, const SipMessage& msg)
    {
		if( m_pSip != NULL )
		{
			m_pSip->OnCallMessageResponse( h.get(), &msg );
		}
    }

    virtual void onMessageFailure(InviteSessionHandle h, const SipMessage& msg)
    {
		if( m_pSip != NULL )
		{
			m_pSip->OnCallMessageResponse( h.get(), &msg );
		}
    }

    virtual void onForkDestroyed(ClientInviteSessionHandle)
    {
    }

    virtual void onSuccess(ClientOutOfDialogReqHandle, const SipMessage& successResponse)
    {
    }
    virtual void onFailure(ClientOutOfDialogReqHandle, const SipMessage& errorResponse)
    {
    }

    virtual void onReceivedRequest(ServerOutOfDialogReqHandle ood, const SipMessage& request)
    {
    }

private:
	CSipInterface* m_pSip;
};

#endif

