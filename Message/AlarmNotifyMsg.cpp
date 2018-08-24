#include "AlarmNotifyMsg.hpp"
#include "SipInterface.h"

bool CAlarmNotifyMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Notify>\r\n";
	content<<"<CmdType>Alarm</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"<AlarmPriority>"<< m_priority <<"</AlarmPriority>\r\n";
	content<<"<AlarmTime>"<< m_time << "</AlarmTime>\r\n";
	content<<"<AlarmMethod>"<< m_method <<"</AlarmMethod>\r\n";

	if( !m_description.empty() )
	{
		content<<"<AlarmDescription>"<< m_method <<"</AlarmDescription>\r\n";
	}

	if( !m_longitude.empty() )
	{
		content<<"<Longitude>"<< m_longitude <<"</Longitude>\r\n";
	}

	if( !m_latitude.empty() )
	{
		content<<"<Latitude>"<< m_latitude <<"</Latitude>\r\n";
	}

	content<<"</Notify>\r\n";
	message = content.str();
	return true;
}

bool CAlarmNotifyMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pAlarmPriority = NULL;
	XMLNode *pAlarmMethod = NULL;
	XMLNode *pAlarmTime = NULL;
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

		if( CGBMessage::CompareNoCase( type, "AlarmPriority" ) )
		{
			pAlarmPriority = pNode;
		}
		else if( CGBMessage::CompareNoCase( type, "AlarmMethod" ) )
		{
			pAlarmMethod = pNode;
		}
		else if( CGBMessage::CompareNoCase( type, "AlarmTime" ) )
		{
			pAlarmTime = pNode;
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
		}
	}

	// 必选参数必须填
	if( pAlarmPriority == NULL || pAlarmMethod == NULL || pAlarmTime == NULL )
	{
        ERROR_LOG( "参数AlarmPriority,AlarmMethod,AlarmTime没有被设置" );
        reason = "参数AlarmPriority,AlarmMethod,AlarmTime没有被设置!";
		return false;
	}

	// 参数解析
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

		if( CGBMessage::CompareNoCase( type, "AlarmDescription" ) )
		{
			m_description = value;
		}
		else if( CGBMessage::CompareNoCase( type, "Longitude" ) )
		{
			m_longitude = value;
		}
		else if( CGBMessage::CompareNoCase( type, "Latitude" ) )
		{
			m_latitude = value;
		}
		else if( CGBMessage::CompareNoCase( type, "AlarmPriority" ) )
		{
			m_priority = value;
		}
		else if( CGBMessage::CompareNoCase( type, "AlarmMethod" ) )
		{
			m_method = value;
		}
		else if( CGBMessage::CompareNoCase( type, "AlarmTime" ) )
		{
			m_time = value;
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
		}
	}

	return true;
}

void CAlarmNotifyMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnAlarmNotify( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CAlarmNotifyResponseMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Response>\r\n";
	content<<"<CmdType>Alarm</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";

	if( IsOK() )
	{
		content<<"<Result>OK</Result>\r\n";
	}
	else
	{
		content<<"<Result>ERROR</Result>\r\n";
	}

	content<<"</Response>\r\n";
	message = content.str();
	return true;
}

bool CAlarmNotifyResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pResult = NULL;
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

		if( CGBMessage::CompareNoCase( type, "Result" ) )
		{
			pResult = pNode;
		}
	}

	// 必选参数必须填
	if( pResult == NULL )
	{
        ERROR_LOG( "必选参数Result没有设置" );
        reason = "必选参数Result没有设置!";
		return false;
	}

	// 参数解析
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

		if( CGBMessage::CompareNoCase( type, "Result" ) )
		{
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
                reason = "必选参数Result参数值无效!";
				return false;
			}
		}
	}

	return true;
}

void CAlarmNotifyResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnAlarmNotifyResponse( *this );
	}
}
