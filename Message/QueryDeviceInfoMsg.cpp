#include "QueryDeviceInfoMsg.hpp"
#include "SipInterface.h"
#include <fstream>

bool CQueryDeviceInfoMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Query>\r\n";
	content<<"<CmdType>DeviceInfo</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"</Query>\r\n";
	message = content.str();
	return true;
}

bool CQueryDeviceInfoMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 参数解析
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
	}

	return true;
}

void CQueryDeviceInfoMsg::Process( CSipInterface *pInterface )
{
    if( NULL != pInterface )
    {
	    pInterface->OnQueryDeviceInfo( *this );
    }
}

//////////////////////////////////////////////////////////////////////////
bool CQueryDeviceInfoResponseMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Response>\r\n";
	content<<"<CmdType>DeviceInfo</CmdType>\r\n";
	content<<"<SN>"<<GetSN()<<"</SN>\r\n";
	content<<"<DeviceID>"<<GetDeviceID()<<"</DeviceID>\r\n";

	if( !m_devicename.empty() )
	{
	    content<<"<DeviceName>"<<m_devicename<<"</DeviceName>\r\n";
	}

	if( IsOK() )
	{
		content<<"<Result>OK</Result>\r\n";
	}
	else
	{
		content<<"<Result>ERROR</Result>\r\n";
	}

	if( !m_manufacturer.empty() )
	{
	    content<<"<Manufacturer>"<< m_manufacturer <<"</Manufacturer>\r\n";
	}

	if( !m_model.empty() )
	{
	    content<<"<Model>" << m_model << "</Model>\r\n";
	}

	if( !m_firmware.empty() )
	{
	    content<<"<Firmware>" << m_firmware << "</Firmware>\r\n";
	}

	if( !m_channel.empty() )
	{
	    content<<"<Channel>" << m_channel << "</Channel>\r\n";
	}
	content<<"</Response>\r\n\r\n";
	message = content.str();
    return true;
}

bool CQueryDeviceInfoResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 参数解析
	size_t size = nodes.size();
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

		XMLNode *pChild = pNode->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "参数值没有设置" );
			continue;
		}

		const char *value = pChild->Value();
		if( value == NULL )
		{
			DEBUG_LOG( "参数值为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "DeviceName" ) )
		{
			m_devicename = value;
		}
		else if( CGBMessage::CompareNoCase( type, "Result" ) )
		{
			if( CGBMessage::CompareNoCase( value, "OK" ) )
			{
				m_result = CGBMessage::RESULT_OK;
			}
			else if( CGBMessage::CompareNoCase( value, "Error" ) )
			{
				m_result = CGBMessage::RESULT_ERROR;
			}
			else
			{
                ERROR_LOG( "参数值\'" << value << "\'无效" );
                reason = "参数Result值无效!";
				return false;
			}
		}
		else if( CGBMessage::CompareNoCase( type, "Manufacturer" ) )
		{
			m_manufacturer = value;
		}
		else if( CGBMessage::CompareNoCase( type, "Model" ) )
		{
			m_model = value;
		}
		else if( CGBMessage::CompareNoCase( type, "Firmware" ) )
		{
			m_firmware = value;
		}
		else if( CGBMessage::CompareNoCase( type, "Channel" ) )
		{
			m_channel = value;
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
			continue;
		}
	}

	return true;
}

void CQueryDeviceInfoResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnQueryDeviceInfoResponse( *this );
	}
}
