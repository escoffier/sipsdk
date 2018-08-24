#include "MessageMsg.hpp"
#include "SipInterface.h"
#include "resip/dum/InviteSession.hxx"
#include "resip/dum/ClientPagerMessage.hxx"

//////////////////////////////////////////////////////////////////////////
bool CMessageMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	// 设置消息的sn
	if( m_sn.empty() )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", GetNextSN() );
		m_sn = buf;
	}

	// 生成消息体内容
	std::string body;
	if( !Encode( body ) )
	{
		ERROR_LOG( "消息编码错误，发送失败!" );
		return false;
	}

	__DUM_TRY
	resip::NameAddr naTo;
	naTo.uri().user() = m_to.c_str();
	naTo.uri().host() = m_to.substr( 0, 10 ).c_str();
	resip::ClientPagerMessage* cpm = mDum.makePagerMessage( naTo ).get();
	resip::SipMessage &request = cpm->getMessageRequest();

	// 设置消息体
	resip::Uri from;
	from.user() = m_from.c_str();
	from.host() = m_from.substr( 0, 10 ).c_str();
	request.header( resip::h_From ).uri() = from;

	// 请求行设置
	resip::Uri rl;
	rl.user() = m_request.c_str();
	rl.host() = m_ip.c_str();
	rl.port() = m_port;
	if( tcp || body.length() > 1000 )
	{
		rl.param( resip::p_transport ) = "TCP";
	}
	else
	{
		rl.param( resip::p_transport ) = "UDP";
	}

	request.header( resip::h_RequestLine ).uri() = rl;

	// 保存消息的会话ID
	if( request.exists( resip::h_CallID ) )
	{
		m_callid = request.header( resip::h_CallID ).value().c_str();
	}

	resip::Mime mime_type( "Application", "MANSCDP+xml" );
	std::auto_ptr< resip::Contents > content( new resip::PlainContents( resip::Data( body.c_str() ), mime_type ) );
	cpm->pageCommand( content );
	return true;
	__DUM_CATCH
}

bool CMessageMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	WARN_LOG( "未实现！" );
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CMessageResponseMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	if( m_handle == NULL )
	{
		ERROR_LOG( "回应消息的m_handle为空，发送失败!" );
		return false;
	}

	if( m_statusCode >= 200 && m_statusCode < 400 )
	{
		resip::SharedPtr< resip::SipMessage > msg = m_handle->accept( m_statusCode );
		m_handle->send( msg );
	}
	else if( m_statusCode >= 400 )
	{
		resip::SharedPtr< resip::SipMessage > msg = m_handle->reject( m_statusCode );
		m_handle->send( msg );
	}
	else
	{
		resip::SharedPtr< resip::SipMessage > msg = m_handle->reject( 500 );
		m_handle->send( msg );
	}

	return true;
}

bool CMessageResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	WARN_LOG( "反馈消息未实现！" );
	return false;
}

void CMessageResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnMessageResponse( *this );
	}
}
