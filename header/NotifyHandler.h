#ifndef _C94BE2E4_86F1_4025_951E_335156D36D5A_
#define _C94BE2E4_86F1_4025_951E_335156D36D5A_

//#include "OutOfDialogHandler.hxx"
#include "SipInterface.h"

class CNotifyHandler : public resip::OutOfDialogHandler
{
public:
	CNotifyHandler( CSipInterface *pSip ) : m_pSip( pSip )
	{
	}

public:
	virtual void onSuccess(resip::ClientOutOfDialogReqHandle h, const resip::SipMessage& successResponse)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnNotifyResponse( h.get(), &successResponse );
		}
	}

	virtual void onFailure(resip::ClientOutOfDialogReqHandle h, const resip::SipMessage& errorResponse)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnNotifyResponse( h.get(), &errorResponse );
		}
	}

	virtual void onReceivedRequest(resip::ServerOutOfDialogReqHandle h, const resip::SipMessage& request)
	{
		if( m_pSip != NULL )
		{
			m_pSip->OnNotify( h.get(), &request );
		}
	}

private:
	CSipInterface* m_pSip;
};

#endif
