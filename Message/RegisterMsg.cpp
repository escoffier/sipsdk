#include "RegisterMsg.hpp"
#include "SipInterface.h"

//////////////////////////////////////////////////////////////////////////
bool CRegisterRequestMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	__DUM_TRY
	resip::NameAddr userAor;
	userAor.uri().user() = m_from.c_str();
	userAor.uri().host() = m_from.substr( 0, 10 ).c_str();
	resip::SharedPtr< resip::SipMessage > request = mDum.makeRegistration( userAor, m_expires );

	// from
	resip::Uri from;
	from.user() = m_from.c_str();
	from.host() = m_from.substr( 0, 10 ).c_str();
	request->header( resip::h_From ).uri() = from;

	// 消息的发送的transport选择是根据dns查找的结束来发送，而dns查找顺序是ForceTarget,route,RequestLine
	// 而注册消息设置了ForceTarget，因此注册消息需要设置transport参数来选择发送方式
	// 根据国标的要求注册消息是不需要使用TCP传输模式的，因此直接设置为UDP
	// TransportSelector::dnsResolve(DnsResult* result,SipMessage* msg)
	// (msg->hasForceTarget())
	// msg->exists(h_Routes) && !msg->header(h_Routes).empty())
	// mDns.lookup(result, msg->header(h_RequestLine).uri());
	resip::Uri naTo;
	naTo.user() = m_request.c_str();
	naTo.host() = m_ip.c_str();
	naTo.port() = m_port;
	naTo.param( resip::p_transport ) = "UDP";
	request->setForceTarget( naTo );
	request->header( resip::h_RequestLine ).uri() = naTo;

	// 保存消息的会话ID
	if( request->exists( resip::h_CallID ) )
	{
		m_callid = request->header( resip::h_CallID ).value().c_str();
	}

	mDum.send( request );
	__DUM_CATCH
	return true;
}

bool CRegisterRequestMsg::Encode( std::string &message )
{
	return true;
}

bool CRegisterRequestMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CRegisterRequestMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnRegister( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CRegisterMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	__DUM_TRY
	resip::NameAddr userAor;
	userAor.uri().user() = m_from.c_str();
	userAor.uri().host() = m_from.substr( 0, 10 ).c_str();
	resip::SharedPtr< resip::SipMessage > request = mDum.makeRegistration( userAor, m_expires );

	// from
	resip::Uri from;
	from.user() = m_from.c_str();
	from.host() = m_from.substr( 0, 10 ).c_str();
	request->header( resip::h_From ).uri() = from;

	// 消息的发送的transport选择是根据dns查找的结束来发送，而dns查找顺序是ForceTarget,route,RequestLine
	// 而注册消息设置了ForceTarget，因此注册消息需要设置transport参数来选择发送方式
	// 根据国标的要求注册消息是不需要使用TCP传输模式的，因此直接设置为UDP
	// TransportSelector::dnsResolve(DnsResult* result,SipMessage* msg)
	// (msg->hasForceTarget())
	// msg->exists(h_Routes) && !msg->header(h_Routes).empty())
	// mDns.lookup(result, msg->header(h_RequestLine).uri());
	resip::Uri naTo;
	naTo.user() = m_request.c_str();
	naTo.host() = m_ip.c_str();
	naTo.port() = m_port;
	naTo.param( resip::p_transport ) = "UDP";
	request->setForceTarget( naTo );
	request->header( resip::h_RequestLine ).uri() = naTo;

	// 保存消息的会话ID
	if( request->exists( resip::h_CallID ) )
	{
		m_callid = request->header( resip::h_CallID ).value().c_str();
	}

	mDum.send( request );
	__DUM_CATCH
	return true;
}

bool CRegisterMsg::Encode( std::string &message )
{
	return true;
}

bool CRegisterMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CRegisterMsg::Process( CSipInterface *pInterface )
{
}

//////////////////////////////////////////////////////////////////////////
bool CRegisterAddMsg::Encode( std::string &message )
{
	return true;
}

bool CRegisterAddMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CRegisterAddMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnRegister( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CRegisterResponseMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	return false;
}

bool CRegisterResponseMsg::Encode( std::string &message )
{
	return true;
}

bool CRegisterResponseMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CRegisterResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnRegisterResponse( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CRegisterRefreshMsg::Encode( std::string &message )
{
	return true;
}

bool CRegisterRefreshMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CRegisterRefreshMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnRegisterRefresh( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CRegisterRemoveMsg::Encode( std::string &message )
{
	return true;
}

bool CRegisterRemoveMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CRegisterRemoveMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnRegisterRemove( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CRegisterRemoveAllMsg::Encode( std::string &message )
{
	return true;
}

bool CRegisterRemoveAllMsg::Decode( const char *body, std::string &reason )
{
	return true;
}

void CRegisterRemoveAllMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnRegisterRemoveAll( *this );
	}
}
