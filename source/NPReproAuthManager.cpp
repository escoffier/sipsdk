#include <iostream>
#include "NPReproAuthMananger.h"
#include "resip/dum/DialogUsageManager.hxx"
#include "rutil/Logger.hxx"
#include "SipInterface.h"

using namespace std;
using namespace resip;

#ifndef RESIPROCATE_SUBSYSTEM
#define RESIPROCATE_SUBSYSTEM Subsystem::NPSIPSERVER
#endif

NPReproAuthManager::NPReproAuthManager( CSipInterface *pSip, resip::DialogUsageManager& dum )
:ServerAuthManager( dum, dum.dumIncomingTarget() ), m_pSip( pSip )
{
}

void NPReproAuthManager::requestCredential(const resip::Data& user, 
                               const resip::Data& realm, 
                               const resip::SipMessage& msg,
                               const resip::Auth& auth, // the auth line we have chosen to authenticate against
                               const resip::Data& transactionId )
{
	if( m_pSip != NULL )
	{
		m_pSip->RequestUserAuthInfo(user.c_str(), realm.c_str(), transactionId.c_str());
	}
}

bool NPReproAuthManager::isMyRealm(const resip::Data& realm)
{
	return true;
}

bool NPReproAuthManager::proxyAuthenticationMode() const
{
	return false;
}

ServerAuthManager::AsyncBool NPReproAuthManager::requiresChallenge(const resip::SipMessage& msg)
{
	const resip::Uri &uri = msg.header(h_From).uri();
	std::string user = uri.user().c_str();

	// 参数错误
	if( user.empty() || m_pSip == NULL )
	{
		return True;
	}

	// 如果不是注册用户直接返回
    if( msg.method() == resip::REGISTER/* && ( msg.exists(resip::h_Expires) && msg.header(resip::h_Expires).value() != 0 ) */)
	{
		if ( msg.exists(resip::h_Expires) && msg.header(resip::h_Expires).value() != 0 )
		{
			if( m_pSip->NeedRegisted( user ) )
			{
				return True;
			}
			else
			{
				return False;
			}
		}
		else 
		{
			//注销需要鉴权
			return True;//m_pSip->IsRegistedUser( user );
		}
	}
	else
	{
		if( m_pSip->IsRegistedUser( user ) )
		{
			return False;
		}
		else
		{
			return True;
		}
	}
}

void NPReproAuthManager::onAuthSuccess(const SipMessage& msg)
{
    const resip::Uri &uri = msg.header(h_From).uri();
    if( msg.exists( h_Expires ) )
    {
        if( 0 != msg.header( h_Expires ).value() )
        {
            InfoLog( << "接受来自于" << uri.getAorAsUri(msg.getSource().getType()) << "的注册请求");
        }
        else
        {
            InfoLog (<< "接受来自于" << uri.getAorAsUri(msg.getSource().getType()) << "的注销请求");
        }
    }
    else
    {
        InfoLog (<< "接受来自于" << uri.getAorAsUri(msg.getSource().getType()) << "的REGISTER请求");
    }
}

void NPReproAuthManager::onAuthFailure(ServerAuthManager::AuthFailureReason reason, const SipMessage& msg)
{
    const resip::Uri &uri = msg.header(h_From).uri();
	const resip::Uri &src = uri.getAorAsUri( msg.getSource().getType() );
    switch( reason )
    {
    case InvalidRequest:
		{
			cout << "拒绝来自于" << src << "的REGISTER请求，其原因为请求中有错误" << endl;
			break;
		}

    case BadCredentials:
        {
            bool userflg = true;//m_userContainer->isUser(uri.user().c_str());
            Data reason = !userflg ? "，其原因为该用户不存在" : "，其原因为该用户提供了错误的认证信息";
            if (msg.exists(h_Expires))
            {
                if (0 != msg.header(h_Expires).value())
                {
                    cout<< "拒绝来自于" << src << "的注册请求" << reason<< endl;
                }
                else
                {
                    cout<< "拒绝来自于" << src << "的注销请求" << reason<< endl;
                }
            }
            else
            {
                cout<< "拒绝来自于" << src << "的REGISTER请求" << reason<< endl;
            }

			break;
       }

    case Error:
		{
			cout<< "拒绝来自于" << src << "的REGISTER请求，其原因为服务器内部错误"<< endl;
			break;
		}

    default:
		{
			cout<< "拒绝来自于" << src << "的REGISTER请求，其原因未知" << endl;
			break;
		}
    }
}

