#include "SubscriptionMsg.hpp"
#include "SipInterface.h"

//////////////////////////////////////////////////////////////////////////
bool CSubscribeMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	return false;
}

bool CSubscribeMsg::Encode( std::string &message )
{
	return false;
}

bool CSubscribeMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	WARN_LOG( "订阅消息未实现！" );
	return false;
}

void CSubscribeMsg::Process( CSipInterface *pInterface )
{
}

//////////////////////////////////////////////////////////////////////////
bool CCancelSubscribeMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	return false;
}

bool CCancelSubscribeMsg::Encode( std::string &message )
{
	return false;
}

bool CCancelSubscribeMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
	}

	return true;
}

void CCancelSubscribeMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnCancelSubscription( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CAddSubscribeMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	if( m_sn.empty() )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", GetNextSN() );
		m_sn = buf;
	}

	std::string body;
	if( !Encode( body ) )
	{
		ERROR_LOG( "消息编码错误，发送失败!" );
		return false;
	}

	resip::NameAddr targetAddr;
	targetAddr.uri().user() = m_to.c_str();
	targetAddr.uri().host() = m_to.substr( 0, 10 ).c_str();
	resip::SharedPtr<resip::SipMessage> reqSubscribe = mDum.makeSubscription( targetAddr, "Catalog", 3500, 600 );

	// 传输模式设置顺序是ForceTarget,route,RequestLine
	// 消息没有设置ForceTarget和route头域，所以可以根据请求行设置传输模式
	// set sip message
	resip::Uri from;
	from.user() = m_from.c_str();
	from.host() = m_from.substr( 0, 10 ).c_str();
	reqSubscribe->header( resip::h_From ).uri() = from;

	resip::Uri rl;
	rl.user() = m_request.c_str();
	rl.host() = m_ip.c_str();
	rl.port() = m_port;
	rl.param( resip::p_transport ) = "UDP";
	reqSubscribe->header( resip::h_RequestLine ).uri() = rl;

	if( reqSubscribe->exists( resip::h_CallID ) )
	{
		m_callid = reqSubscribe->header( resip::h_CallID ).value().c_str();
	}

	resip::Mime mime_type( "Application", "MANSCDP+xml" );
	std::auto_ptr<resip::Contents> content( new resip::PlainContents( resip::Data( body.c_str() ), mime_type ) );
	reqSubscribe->setContents( content );
	mDum.send( reqSubscribe );
	return true;
}

bool CAddSubscribeMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Query>\r\n";
	content<<"<CmdType>Catalog</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"</Query>\r\n\r\n";
	message = content.str();
	return true;
}

bool CAddSubscribeMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
	}

	return true;
}

void CAddSubscribeMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnAddSubscription( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CSubscribeResponseMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	return false;
}

bool CSubscribeResponseMsg::Encode( std::string &message )
{
	return false;
}

bool CSubscribeResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	WARN_LOG( "订阅回复消息未实现！" );
	return false;
}

void CSubscribeResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnSubscriptionResponse( *this );
	}
}
