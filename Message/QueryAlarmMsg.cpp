#include "QueryAlarmMsg.hpp"
#include "SipInterface.h"

bool CQueryAlarmMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Query>\r\n";
	content<<"<CmdType>Alarm</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";

	if( !m_startAlarmPriority.empty() )
	{
		content<<"<StartAlarmPriority>"<< m_startAlarmPriority <<"</StartAlarmPriority>\r\n";
	}

	if( !m_endAlarmPriority.empty() )
	{
		content<<"<EndAlarmPriority>"<< m_startAlarmPriority <<"</EndAlarmPriority>\r\n";
	}
	
	if( !m_alarmMethod.empty() )
	{
		content<<"<AlarmMethod>"<< m_startAlarmPriority <<"</AlarmMethod>\r\n";
	}
	
	if( !m_startAlarmTime.empty() )
	{
		content<<"<StartAlarmTime>"<< m_startAlarmPriority <<"</StartAlarmTime>\r\n";
	}
	
	if( !m_endAlarmTime.empty() )
	{
		content<<"<EndAlarmTime>"<< m_startAlarmPriority <<"</EndAlarmTime>\r\n";
	}

	content<<"</Query>";
	message = content.str();
	return true;
}

bool CQueryAlarmMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
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

		XMLNode *pChild = pNode->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "参数值没有设置" );
			continue;
		}

		const char *type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		const char *value = pChild->Value();
		if( value == NULL )
		{
			DEBUG_LOG( "参数值为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "StartAlarmPriority" ) )
		{
			m_startAlarmPriority = value;
		}
		else if( CGBMessage::CompareNoCase( type, "EndAlarmPriority" ) )
		{
			m_endAlarmPriority = value;
		}
		else if( CGBMessage::CompareNoCase( type, "AlarmMethod" ) )
		{
			m_alarmMethod = value;
		}
		else if( CGBMessage::CompareNoCase( type, "StartAlarmTime" ) )
		{
			m_startAlarmTime = value;
		}
		else if( CGBMessage::CompareNoCase( type, "EndAlarmTime" ) )
		{
			m_endAlarmTime = value;
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
			continue;
		}
	}

	return true;
}

void CQueryAlarmMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnQueryAlarm( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CQueryAlarmResponseMsg::Encode( std::string &message )
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

bool CQueryAlarmResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
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
		ERROR_LOG( "参数Result没有被设置" );
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
				return false;
			}
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
			continue;
		}
	}

	return true;
}

void CQueryAlarmResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnQueryAlarmResponse( *this );
	}
}
