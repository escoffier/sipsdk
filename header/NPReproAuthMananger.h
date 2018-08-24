#ifndef _NPREPRO_AUTH_MANANGER_H_
#define _NPREPRO_AUTH_MANANGER_H_

#include "resip/dum/ServerAuthManager.hxx"
#include "OutboundProxyContainer.h"
class CSipInterface;

class NPReproAuthManager : public resip::ServerAuthManager
{
public:
	NPReproAuthManager( CSipInterface *pSip, resip::DialogUsageManager& dum );

public:
	virtual bool isMyRealm(const resip::Data& realm);
	virtual bool proxyAuthenticationMode() const;

public:
	virtual AsyncBool requiresChallenge(const resip::SipMessage& msg);
	virtual void onAuthSuccess(const resip::SipMessage& msg);
	virtual void onAuthFailure(resip::ServerAuthManager::AuthFailureReason reason, const resip::SipMessage& msg);
	virtual void requestCredential(const resip::Data& user, 
		const resip::Data& realm, 
		const resip::SipMessage& msg,
		const resip::Auth& auth,
		const resip::Data& transactionToken );

private:
	CSipInterface* m_pSip;
};

#endif
