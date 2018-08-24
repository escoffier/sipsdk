#include "KeepaliveMsg.hpp"
#include "SipInterface.h"

bool CKeepAliveMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Notify>\r\n";
	content<<"<CmdType>Keepalive</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";

	// 设备状态
	if( IsOK() )
	{
		content<<"<Status>OK</Status>\r\n";
	}
	else
	{
		content<<"<Status>ERROR</Status>\r\n";
	}

	// 故障设备列表
	if( !m_devices.empty() )
	{
		content<<"<Info>\r\n";
		std::list< std::string >::iterator it = m_devices.begin();
		std::list< std::string >::iterator end = m_devices.end();
		for( /*it*/; it != end; ++it )
		{
			content<<"<DeviceID>" << *it << "</DeviceID>\r\n";
		}
		content<<"</Info>\r\n";
	}

	content<<"</Notify>\r\n";
	message = content.str();
	return true;
}

bool CKeepAliveMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pStatus = NULL;
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

		if( CGBMessage::CompareNoCase( type, "Status" ) )
		{
			pStatus = pNode;
		}
	}

	// 必选参数必须填
	if( pStatus == NULL )
	{
        ERROR_LOG( "参数Status没有被设置" );
        reason = "参数Status没有被设置!";
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

		const char *type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "Status" ) )
		{
			XMLNode *pChild = pNode->FirstChild();
			if( pChild == NULL )
			{
				continue;
			}

			const char* value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值为空" );
				continue;
			}

			if( CGBMessage::CompareNoCase( value, "OK" ) )
			{
				m_status = CGBMessage::RESULT_OK;
			}
			else if( CGBMessage::CompareNoCase( value, "Error" ) )
			{
				m_status = CGBMessage::RESULT_ERROR;
			}
			else
			{
                ERROR_LOG( "参数值\'" << value << "\'无效" );
                reason = "参数Status值无效!";
				return false;
			}
		}
		else if( CGBMessage::CompareNoCase( type, "Info" ) )
		{
			for( XMLNode *pItem = pNode->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
			{
				const char *type = pItem->Value();
				if( type == NULL )
				{
					DEBUG_LOG( "参数名字为空" );
					continue;
				}

				if( CGBMessage::CompareNoCase( type, "DeviceID" ) )
				{
					items.push_back( pItem );
				}				
			}
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
			continue;
		}
	}

	// 设备故障列表
	std::list< XMLNode* >::iterator it = items.begin();
	std::list< XMLNode* >::iterator end = items.end();
	for( /*it*/; it != end; ++it )
	{
		XMLNode *pChild = (*it)->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "参数错误" );
			continue;
		}

		const char *value = pChild->Value();
		if( value == NULL )
		{
			DEBUG_LOG( "参数值为空" );
			continue;
		}
		else
		{
			m_devices.push_back( value );
		}
	}

	return true;
}

void CKeepAliveMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnKeepAlive( *this );
	}
}
