#include "DeviceControlMsg.hpp"
#include "SipInterface.h"

//////////////////////////////////////////////////////////////////////////
bool CPTZControlMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content << "<?xml version=\"1.0\"?>\r\n";
	content << "<Control>\r\n";
	content << "<CmdType>DeviceControl</CmdType>\r\n";
	content << "<SN>" << m_sn << "</SN>\r\n";
	content << "<DeviceID>" << m_deviceid << "</DeviceID>\r\n";
	content << "<PTZCmd>" << m_ptzCmd << "</PTZCmd>\r\n";
	if( !m_priority.empty() )
	{
		content << "<Info>\r\n";
		content << "<ControlPriority>" << m_priority << "</ControlPriority>\r\n";
		content << "</Info>\r\n";
	}

	content << "</Control>\r\n";
	message = content.str();
	return true;
}

bool CPTZControlMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pCmd = NULL;
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

		if( CGBMessage::CompareNoCase( type, "PTZCmd" ) )
		{
			pCmd = pNode;
		}
	}

	// 必选参数必须填
	if( pCmd == NULL )
	{
        ERROR_LOG( "参数PTZCmd没有被设置" );
        reason = "参数PTZCmd没有被设置!";
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
			DEBUG_LOG( "参数值没有设置" );
			continue;
		}

		const char* type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "PTZCmd" ) )
		{
			const char *value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值没有设置" );
				continue;
			}
			else
			{
				m_ptzCmd = value;
			}
		}
		else if( CGBMessage::CompareNoCase( type, "Info" ) )
		{
			for( XMLNode *pItem = pNode->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
			{
				const char *value = pItem->Value();
				if( value != NULL && CGBMessage::CompareNoCase( value, "ControlPriority" ) )
				{
					items.push_back( pItem );
				}
				else
				{
					DEBUG_LOG( "国标未定义的参数:" << value );
				}
			}
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
		}
	}

	// ControlPriority
	std::list< XMLNode* >::iterator it = items.begin();
	std::list< XMLNode* >::iterator end = items.end();
	for( /*it*/; it != end; ++it )
	{
		XMLNode *pChild = (*it)->FirstChild();
		if( pChild != NULL )
		{
			m_priority = pChild->Value();
		}
	}

	return true;
}

void CPTZControlMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnPTZControl( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CPTZLockMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content << "<?xml version=\"1.0\"?>\r\n";
	content << "<Control>\r\n";
	content << "<CmdType>DeviceControl</CmdType>\r\n";
	content << "<SN>" << m_sn << "</SN>\r\n";
	content << "<DeviceID>" << m_deviceid << "</DeviceID>\r\n";
	content << "<ControlLock>" << m_lock << "</ControlLock>\r\n";
	content << "<Period>" << m_period << "</Period>\r\n";
	content << "</Control>\r\n";
	message = content.str();
	return true;
}

bool CPTZLockMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pLock = NULL;
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

		if( CGBMessage::CompareNoCase( type, "ControlLock" ) )
		{
			pLock = pNode;
		}
	}

	// 必选参数必须填
	if( pLock == NULL )
	{
        ERROR_LOG( "参数ControlLock没有被设置" );
        reason = "参数ControlLock没有被设置!";
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
			DEBUG_LOG( "参数值没有设置" );
			continue;
		}

		const char* type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "ControlLock" ) )
		{
			const char *value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值没有设置" );
				continue;
			}
			else
			{
				m_lock = value;
			}
		}
		else if( CGBMessage::CompareNoCase( type, "Period" ) )
		{
			const char *value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值没有设置" );
				continue;
			}
			else
			{
				m_period = value;
			}
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
		}
	}

	return true;
}

void CPTZLockMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnPTZLock( *this );
	}
}


//////////////////////////////////////////////////////////////////////////
bool CRecordControlMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Control>\r\n";
	content<<"<CmdType>DeviceControl</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";

	if( m_record )
	{
		content<<"<RecordCmd>Record</RecordCmd>\r\n";
	}
	else
	{
		content<<"<RecordCmd>StopRecord</RecordCmd>\r\n";
	}

	content<<"</Control>\r\n";
	message = content.str();
	return true;
}

bool CRecordControlMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pRecord = NULL;
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

		if( CGBMessage::CompareNoCase( type, "RecordCmd" ) )
		{
			pRecord = pNode;
		}
	}

	// 必选参数必须填
	if( pRecord == NULL )
	{
        ERROR_LOG( "参数RecordCmd没有被设置" );
        reason = "参数RecordCmd没有被设置!";
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

		if( CGBMessage::CompareNoCase( type, "RecordCmd" ) )
		{
			XMLNode *pChild = pNode->FirstChild();
			if( pChild == NULL )
			{
				DEBUG_LOG( "参数值没有设置" );
				continue;
			}

			const char* value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值为空" );
				continue;
			}

			if( CGBMessage::CompareNoCase( value, "Record" ) )
			{
				m_record = true;
			}
			else if( CGBMessage::CompareNoCase( value, "StopRecord" ) )
			{
				m_record = false;
			}
			else
			{
				ERROR_LOG( "RecordCmd参数值\'" << value << "\'无效" );
				return false;
			}
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
		}
	}

	return true;
}

void CRecordControlMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnRecordControl( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CTeleBootMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Control>\r\n";
	content<<"<CmdType>DeviceControl</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"<TeleBoot>" << m_cmd << "</TeleBoot>\r\n";
	content<<"</Control>\r\n";
	message = content.str();
	return true;
}

bool CTeleBootMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pTeleBoot = NULL;
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
		XMLNode *pNode = nodes[i];
		if( pNode == NULL )
		{
			DEBUG_LOG( "参数错误" );
			continue;
		}

		const char *value = pNode->Value();
		if( value == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( value, "TeleBoot" ) )
		{
			pTeleBoot = pNode;
		}
	}

	// 必选参数必须填
	if( pTeleBoot == NULL )
	{
        ERROR_LOG( "参数TeleBoot没有被设置" );
        reason = "参数TeleBoot没有被设置!";
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

		XMLNode *pChild = pNode->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( pNode->Value(), "TeleBoot" ) )
		{
			const char* value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "TeleBoot参数值没有设置" );
				continue;
			}

			if( CGBMessage::CompareNoCase( value, "Boot" ) )
			{
				m_cmd = value;
			}
			else
			{
				ERROR_LOG( "TeleBoot参数值\'" << value << "\'无效" );
				return false;
			}
		}
		else
		{
			continue;
		}
	}

	return true;
}

void CTeleBootMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnTeleBoot( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CResetAlarmMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Control>\r\n";
	content<<"<CmdType>DeviceControl</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"<AlarmCmd>" << m_cmd << "</AlarmCmd>\r\n";
	content<<"<Info>\r\n";
	content<<"<AlarmMethod>" << m_method << "</AlarmMethod>\r\n";
	content<<"</Info>\r\n";
	content<<"</Control>\r\n";
	message = content.str();
	return true;
}

bool CResetAlarmMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pAlarmCmd = NULL;
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

		if( CGBMessage::CompareNoCase( type, "AlarmCmd" ) )
		{
			pAlarmCmd = pNode;
		}
	}

	// 必选参数必须填
	if( pAlarmCmd == NULL )
	{
        ERROR_LOG( "参数AlarmCmd没有被设置" );
        reason = "参数AlarmCmd没有被设置!";
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

		const char* type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "AlarmCmd" ) )
		{
			XMLNode *pChild = pNode->FirstChild();
			if( pChild == NULL )
			{
				DEBUG_LOG( "参数错误" );
				continue;
			}

			const char* value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数名字为空" );
				continue;
			}

			if( CGBMessage::CompareNoCase( value, "ResetAlarm" ) )
			{
				m_cmd = value;
			}
			else
			{
				DEBUG_LOG( "国标未定义的参数:" << value );
				continue;
			}
		}
		else if( CGBMessage::CompareNoCase( type, "Info" ) )
		{
			for( XMLNode *pItem = pNode->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
			{
				const char* value = pItem->Value();
				if( value == NULL )
				{
					DEBUG_LOG( "参数值没有设置" );
					continue;
				}

				if( CGBMessage::CompareNoCase( value, "AlarmMethod" ) )
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

	// AlarmMethod
	std::list< XMLNode* >::iterator it = items.begin();
	std::list< XMLNode* >::iterator end = items.end();
	for( /*it*/; it != end; ++it )
	{
		XMLNode *pChild = (*it)->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "参数值没有设置" );
			continue;
		}
		else
		{
			const char *value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值为空" );
				continue;
			}
			else
			{
				m_method = atoi( pChild->Value() );
			}
		}
	}

	return true;
}

void CResetAlarmMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnResetAlarm( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CGuardMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Control>\r\n";
	content<<"<CmdType>DeviceControl</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";

	if( m_bSetGuard )
	{
		content<<"<GuardCmd>SetGuard</GuardCmd>\r\n";
	}
	else
	{
		content<<"<GuardCmd>ResetGuard</GuardCmd>\r\n";
	}

	content<<"</Control>\r\n";
	message = content.str();
	return true;
}

bool CGuardMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pGuardCmd = NULL;
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

		if( CGBMessage::CompareNoCase( type, "GuardCmd" ) )
		{
			pGuardCmd = pNode;
		}
	}

	// 必选参数必须填
	if( pGuardCmd == NULL )
	{
        ERROR_LOG( "参数GuardCmd没有被设置" );
        reason = "参数GuardCmd没有被设置!";
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

		if( CGBMessage::CompareNoCase( type, "GuardCmd" ) )
		{
			XMLNode *pChild = pNode->FirstChild();
			if( pChild == NULL )
			{
				DEBUG_LOG( "参数值没有设置" );
				continue;
			}

			const char* value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值为空" );
				continue;
			}

			if( CGBMessage::CompareNoCase( value, "SetGuard" ) )
			{
				m_bSetGuard = true;
			}
			else if( CGBMessage::CompareNoCase( value, "ResetGuard" ) )
			{
				m_bSetGuard = false;
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

void CGuardMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnGuard( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CDragZoomInMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Control>\r\n";
	content<<"<CmdType>DeviceControl</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"<DragZoomIn>\r\n";
	content<<"<Length>" << m_length << "</Length>\r\n";
	content<<"<Width>" << m_width << "</Width>\r\n";
	content<<"<MidPointX>" << m_midPointX << "</MidPointX>\r\n";
	content<<"<MidPointY>" << m_midPointY << "</MidPointY>\r\n";
	content<<"<LengthX>" << m_lengthX << "</LengthX>\r\n";
	content<<"<LengthY>" << m_lengthY << "</LengthY>\r\n";
	content<<"</DragZoomIn>\r\n";
	content<<"</Control>\r\n";
	message = content.str();
	return true;
}

bool CDragZoomInMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pDragZoom = NULL;
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

		if( CGBMessage::CompareNoCase( type, "DragZoomIn" ) )
		{
			pDragZoom = pNode;
		}
	}

	// 必选参数必须填
	if( pDragZoom == NULL )
	{
        ERROR_LOG( "参数DragZoomIn没有被设置" );
        reason = "参数DragZoomIn没有被设置!";
		return false;
	}

	// 参数解析
	for( XMLNode *pItem = pDragZoom->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
	{
		const char* type = pItem->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数错误" );
			continue;
		}

		XMLNode *pChild = pItem->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		const char *value = pChild->Value();
		if( value == NULL )
		{
			DEBUG_LOG( "参数值没有设置" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "Length" ) )
		{
			m_length = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "Width" ) )
		{
			m_width = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "MidPointX" ) )
		{
			m_midPointX = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "MidPointY" ) )
		{
			m_midPointY = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "LengthX" ) )
		{
			m_lengthX = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "LengthY" ) )
		{
			m_lengthY = atoi( value );
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
		}
	}

	return true;
}

void CDragZoomInMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnDragZoomIn( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CDragZoomOutMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Control>\r\n";
	content<<"<CmdType>DeviceControl</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"<DragZoomOut>\r\n";
	content<<"<Length>" << m_length << "</Length>\r\n";
	content<<"<Width>" << m_width << "</Width>\r\n";
	content<<"<MidPointX>" << m_midPointX << "</MidPointX>\r\n";
	content<<"<MidPointY>" << m_midPointY << "</MidPointY>\r\n";
	content<<"<LengthX>" << m_lengthX << "</LengthX>\r\n";
	content<<"<LengthY>" << m_lengthY << "</LengthY>\r\n";
	content<<"</DragZoomOut>\r\n";
	content<<"</Control>\r\n";
	message = content.str();
	return true;
}

bool CDragZoomOutMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pDragZoom = NULL;
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

		if( CGBMessage::CompareNoCase( type, "DragZoomOut" ) )
		{
			pDragZoom = pNode;
		}
	}

	// 必选参数必须填
	if( pDragZoom == NULL )
	{
        ERROR_LOG( "参数DragZoomOut没有被设置" );
        reason = "参数DragZoomOut没有被设置!";
		return false;
	}

	// 参数解析
	for( XMLNode *pItem = pDragZoom->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
	{
		const char* type = pItem->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "参数名字为空" );
			continue;
		}

		XMLNode *pChild = pItem->FirstChild();
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

		if( CGBMessage::CompareNoCase( type, "Length" ) )
		{
			m_length = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "Width" ) )
		{
			m_width = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "MidPointX" ) )
		{
			m_midPointX = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "MidPointY" ) )
		{
			m_midPointY = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "LengthX" ) )
		{
			m_lengthX = atoi( value );
		}
		else if( CGBMessage::CompareNoCase( type, "LengthY" ) )
		{
			m_lengthY = atoi( value );
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
			continue;
		}
	}

	return true;
}

void CDragZoomOutMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnDragZoomOut( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CDeviceControlResponseMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Response>\r\n";
	content<<"<CmdType>" << m_cmdtype << "</CmdType>\r\n";
	content<<"<SN>"<<GetSN()<<"</SN>\r\n";
	content<<"<DeviceID>"<<GetDeviceID()<<"</DeviceID>\r\n";

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

bool CDeviceControlResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
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
        reason = "参数Result没有被设置!";
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
			DEBUG_LOG( "参数值为空" );
			continue;
		}
	
		if( CGBMessage::CompareNoCase( type, "Result" ) )
		{
			XMLNode *pChild = pNode->FirstChild();
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
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
			continue;
		}
	}

	return true;
}

void CDeviceControlResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnDeviceControlResponse( *this );
	}
}

