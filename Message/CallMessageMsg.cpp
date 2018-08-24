#include "CallMessageMsg.hpp"
#include "SipInterface.h"

//////////////////////////////////////////////////////////////////////////
bool CCallMessageMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	if( m_handle == NULL )
	{
		return false;
	}

	// create sn
	if( m_sn.empty() )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", GetNextSN() );
		m_sn = buf;
	}

	// create body
	std::string body;
	if( !Encode( body ) )
	{
		return false;
	}

	__DUM_TRY
	resip::Mime mime_type( "Application", "MANSCDP+xml" );
	resip::PlainContents content( resip::Data( body.c_str() ), mime_type );
	m_handle->message( content );
	__DUM_CATCH
	return true;
}

bool CCallMessageMsg::Encode( std::string &message )
{
	return false;
}

bool CCallMessageMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	WARN_LOG( "CallMessage消息未实现！" );
	return false;
}

void CCallMessageMsg::Process( CSipInterface *pInterface )
{
}

//////////////////////////////////////////////////////////////////////////
bool CStreamPlayEndMsg::Encode( std::string &message )
{
	std::ostringstream oss;
	oss<<"<?xml version=\"1.0\"?>\r\n";
	oss<<"<Notify>\r\n";
	oss<<"<CmdType>MediaStatus</CmdType>\r\n";
	oss<<"<SN>" << m_sn << "</SN>\r\n";
	oss<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	oss<<"<NotifyType>" << m_type << "</NotifyType>\r\n";
	oss<<"</Notify>\r\n";

	message = oss.str();
	return true;
}

bool CStreamPlayEndMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pType = NULL;
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
		XMLNode *pNode = nodes[i];
		if( pNode == NULL )
		{
			DEBUG_LOG( "参数错误" );
			continue;
		}

		const char* type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "NotifyType" ) )
		{
			pType = pNode;
		}
	}

	// 必选参数必须填
	if( pType == NULL )
	{
        ERROR_LOG( "参数NotifyType没有被设置" );
        reason = "参数NotifyType没有被设置!";
		return false;
	}

	// 参数解析
	std::list< XMLNode* > items;
	for( size_t i = 0; i < size; ++i )
	{
		XMLNode *pNode = nodes[i];
		if( pNode == NULL )
		{
			DEBUG_LOG( "参数错误" );
			continue;
		}

		XMLNode *pChild = pNode->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		const char* type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数值为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "NotifyType" ) )
		{
			const char *value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值为空" );
				continue;
			}
			else
			{
				m_type = value;
			}
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
		}
	}

	return true;
}

void CStreamPlayEndMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnStreamPlayOver( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CCallMessageResponseMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	if( m_handle == NULL )
	{
		return false;
	}

	if( m_statusCode >= 200 && m_statusCode < 400 )
	{
		m_handle->acceptNITCommand( m_statusCode );
	}
	else if( m_statusCode >= 400 )
	{
		m_handle->rejectNITCommand( m_statusCode );
	}
	else
	{
		m_handle->rejectNITCommand( 500 );
	}

	return true;
}

bool CCallMessageResponseMsg::Encode( std::string &message )
{
	return true;
}

bool CCallMessageResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	return true;
}

void CCallMessageResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnCallMessageResponse( *this );
	}
}
