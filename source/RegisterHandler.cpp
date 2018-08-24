#include "RegisterHandler.h"
#include "resip/dum/ServerRegistration.hxx"
#include "resip/stack/Helper.hxx"
#include "rutil/Logger.hxx"
#include "SipInterface.h"
#include "MessageQueue.hpp"
#include "../Message/MsgFactory.hpp"

#ifndef RESIPROCATE_SUBSYSTEM
#define RESIPROCATE_SUBSYSTEM resip::Subsystem::REPRO
#endif

using namespace resip;
using namespace std;

ServerRegHandler::ServerRegHandler( CSipInterface *pSip ) : m_pSip( pSip )
{
}

ServerRegHandler::~ServerRegHandler()
{
}

void ServerRegHandler::onRemove(resip::ServerRegistrationHandle sr, const resip::SipMessage& reg)
{
	sr->accept();
	if( m_pSip != 0 )
	{
		m_pSip->OnRegisterRemove( NULL, &reg );
	}
}

void ServerRegHandler::onRemoveAll(resip::ServerRegistrationHandle sr, const resip::SipMessage& reg)
{
    sr->accept();
	if( m_pSip != 0 )
	{
		m_pSip->OnRegisterRemoveAll( 0, &reg );
	}
}

void ServerRegHandler::onRefresh(resip::ServerRegistrationHandle sr, const resip::SipMessage& reg)
{
	// 消息处理之后handle不可用，这里不需要再传
	if( m_pSip != 0 )
	{
		m_pSip->OnRefresh( sr.get(), &reg );
	}
}

void ServerRegHandler::onAdd(resip::ServerRegistrationHandle sr, const resip::SipMessage& reg)
{
	// 消息处理之后handle不可用，这里不需要再传
	if( m_pSip != 0 )
	{
		m_pSip->OnRegister( sr.get(), &reg );
	}
}

void ServerRegHandler::onQuery(resip::ServerRegistrationHandle sr, const resip::SipMessage& reg)
{
    sr->accept();
}

bool CNPClientAuthManager::handle(resip::UserProfile& userProfile, resip::SipMessage& origRequest, const resip::SipMessage& response)
{
	if(origRequest.header(resip::h_RequestLine).getMethod() == resip::REGISTER && response.header(resip::h_StatusLine).statusCode() == 401)
	{
		if (response.exists(resip::h_WWWAuthenticates))
		{
			for (resip::Auths::const_iterator i = response.header(resip::h_WWWAuthenticates).begin(); i != response.header(resip::h_WWWAuthenticates).end(); ++i)                    
			{    
				if (i->exists(resip::p_realm))
				{
					if( m_pSip != NULL )
					{
						const resip::Data& realm = (*i).param(resip::p_realm); 
						m_pSip->OnRequetAuth( realm.c_str() );
					}

					break;
				}
			}
		}
	}

	return resip::ClientAuthManager::handle(userProfile, origRequest, response);
}

