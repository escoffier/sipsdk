#include "CatalogNotifyMsg.hpp"
#include "SipInterface.h"

//////////////////////////////////////////////////////////////////////////
static const char* g_event_desc[] =
{
	"ON",
	"OFF",
	"VLOST",
	"DEFECT",
	"ADD",
	"DEL",
	"UPDATE"
};

//////////////////////////////////////////////////////////////////////////
bool CCatalogNotifyMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
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

	// 目录通知消息可能是订阅内消息，也可能是会话外的通知
	__DUM_TRY
	resip::SharedPtr< resip::SipMessage > pMsg;
	if( m_subscription != NULL )
	{
		resip::ServerSubscription *p = dynamic_cast< resip::ServerSubscription* >( m_subscription );
		if( p != NULL )
		{
			pMsg = p->neutralNotify();
		}
	}
	else
	{
		resip::NameAddr naTo;
		naTo.uri().user() = m_to.c_str();
		naTo.uri().host() = m_to.substr( 0, 10 ).c_str();
		pMsg = mDum.makeOutOfDialogRequest( naTo, resip::NOTIFY );
	}
	if (NULL == pMsg)
	{
		ERROR_LOG( "消息体错误，发送失败!" );
		return false;
	}

	// 请求行设置
	// 设置消息的传输模式
	// 传输模式设置顺序是ForceTarget,route,RequestLine
	// 目录通知消息没有设置ForceTarget和route头域，所以可以根据请求行设置传输模式
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

	pMsg->header( resip::h_RequestLine ).uri() = rl;

	// 保存消息的会话ID
	if( pMsg->exists( resip::h_CallID ) )
	{
		m_callid = pMsg->header( resip::h_CallID ).value().c_str();
	}

	resip::Mime mime_type( "application", "MANSCDP+xml" );
	std::auto_ptr<resip::Contents> content(new resip::PlainContents(resip::Data(body.c_str()), mime_type));
	pMsg->setContents(content);
	mDum.send( pMsg );
	__DUM_CATCH
	return true;
}

bool CCatalogNotifyMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Notify>\r\n";
	content<<"<CmdType>Catalog</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	content<<"<Status>OK</Status>\r\n";
	content<<"<SumNum>"<< m_sum <<"</SumNum>\r\n";
	content<<"<DeviceList Num=\"" << m_devices.size() << "\">\r\n";

	for( std::list< SCatalog >::iterator it = m_devices.begin(); it != m_devices.end(); ++it )
	{
		// 校验目录项的必选参数
		const SCatalog &catalog = (*it);
		content<<"<Item>\r\n";
		content<<"<DeviceID>"<<catalog.id<<"</DeviceID>\r\n";
		content<<"<Event>"<< g_event_desc[catalog.event] <<"</Event>\r\n";

		if( catalog.event == EVENT_ADD || catalog.event == EVENT_UPDATE )
		{
			content<<"<Name>"<< catalog.name <<"</Name>\r\n";
			content<<"<Manufacturer>"<< catalog.manufacturer <<"</Manufacturer>\r\n";
			content<<"<Model>"<< catalog.model <<"</Model>\r\n";
			content<<"<Owner>"<< catalog.owner <<"</Owner>\r\n";

			if( !catalog.civil.empty() )
			{
				content<<"<CivilCode>"<<catalog.civil<<"</CivilCode>\r\n";
			}
			else
			{
				content<<"<CivilCode>"<<catalog.parentid<<"</CivilCode>\r\n";
			}

			if( !catalog.block.empty() )
			{
				content<<"<Block>"<< catalog.block <<"</Block>\r\n";
			}

			content<<"<Address>"<< catalog.address <<"</Address>\r\n";
			content<<"<Parental>"<< catalog.parental <<"</Parental>\r\n";
			content<<"<ParentID>"<< catalog.parentid <<"</ParentID>\r\n";

			if( !catalog.safetyway.empty() ) 
			{
				content<<"<SafetyWay>"<< catalog.safetyway <<"</SafetyWay>\r\n";
			}

			content<<"<RegisterWay>"<< catalog.registerway <<"</RegisterWay>\r\n";

			if( !catalog.certnum.empty() )
			{
				content<<"<CertNum>"<< catalog.certnum <<"</CertNum>\r\n";
			}

			if( !catalog.certifiable.empty() ) 
			{
				content<<"<Certifiable>"<< catalog.certifiable <<"</Certifiable>\r\n";
			}

			if( !catalog.errcode.empty() )
			{
				content<<"<ErrCode>"<< catalog.errcode <<"</ErrCode>\r\n";
			}

			if( !catalog.endtime.empty() ) 
			{
				content<<"<EndTime>"<< catalog.endtime <<"</EndTime>\r\n";
			}

			content<<"<Secrecy>"<< catalog.secrecy <<"</Secrecy>\r\n";

			if( !catalog.ip.empty() ) 
			{
				content<<"<IPAddress>"<< catalog.ip <<"</IPAddress>\r\n";
			}

			if( !catalog.port.empty() )
			{
				content<<"<Port>"<< catalog.port <<"</Port>\r\n";
			}

			if( !catalog.password.empty() )
			{
				content<<"<Password>"<< catalog.password <<"</Password>\r\n";
			}

			content<<"<Status>"<<catalog.status<<"</Status>\r\n";

			if( !catalog.longitude.empty() ) 
			{
				content<<"<Longitude>"<<catalog.longitude<<"</Longitude>\r\n";
			}

			if( !catalog.latitude.empty() )
			{
				content<<"<Latitude>"<<catalog.latitude<<"</Latitude>\r\n";
			}

			content<<"<Info>\r\n";
			if( !catalog.ptz.empty() )
			{
				content<<"<PTZType>"<<catalog.ptz<<"</PTZType>\r\n";
			}

			if( !catalog.position.empty() ) 
			{
				content<<"<PositionType>"<<catalog.position<<"</PositionType>\r\n";
			}

			if( !catalog.room.empty() )
			{
				content<<"<RoomType>"<<catalog.room<<"</RoomType>\r\n";
			}

			if( !catalog.use.empty() )
			{
				content<<"<UseType>"<<catalog.use<<"</UseType>\r\n";
			}

			if( !catalog.supplylight.empty() )
			{
				content<<"<SupplyLightType>"<<catalog.supplylight<<"</SupplyLightType>\r\n";
			}

			if( !catalog.direction.empty() )
			{
				content<<"<DirectionType>"<<catalog.direction<<"</DirectionType>\r\n";
			}

			if( !catalog.resolution.empty() )
			{
				content<<"<Resolution>"<<catalog.resolution<<"</Resolution>\r\n";
			}

			if( !catalog.businessgroup.empty() ) 
			{
				content<<"<BusinessGroupID>"<<catalog.businessgroup<<"</BusinessGroupID>\r\n";
			}

			content<<"</Info>\r\n";
		}

		content<<"</Item>\r\n";
	}
	content<<"</DeviceList>\r\n";
	content<<"</Notify>\r\n\r\n";

	message = content.str();
	return true;
}

bool CCatalogNotifyMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// 必须参数校验
	XMLNode *pSumNum = NULL;
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

		if( CGBMessage::CompareNoCase( type, "SumNum" ) )
		{
			pSumNum = pNode;
		}
	}

	// 必选参数必须填
	if( pSumNum == NULL )
	{
        ERROR_LOG( "参数SumNum没有被设置" );
        reason = "参数SumNum没有被设置!";
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

		if( CGBMessage::CompareNoCase( type, "Status" ) )
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
			else
			{
				m_status = value;
			}
		}
		else if( CGBMessage::CompareNoCase( type, "SumNum" ) )
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
			else
			{
				m_sum = atoi( value );
			}
		}
		else if( CGBMessage::CompareNoCase( type, "DeviceList" ) )
		{
			for(XMLNode *pItem = pNode->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
			{
				const char *type = pItem->Value();
				if( type == NULL )
				{
					DEBUG_LOG( "参数名字为空" );
					continue;
				}

				if( CGBMessage::CompareNoCase( type, "Item" ) )
				{
					items.push_back( pItem );
				}
				else
				{
					DEBUG_LOG( "国标未定义的参数:" << type );
					continue;
				}
			}
		}
		else
		{
			DEBUG_LOG( "国标未定义的参数:" << type );
			continue;
		}
	}

	// 目录项
	std::list< XMLNode* >::iterator it = items.begin();
	std::list< XMLNode* >::iterator end = items.end();
	for( /*it*/; it != end; ++it )
	{
		SCatalog catalog;
		for(XMLNode *pItem = (*it)->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
		{
			const char *type = pItem->Value();
			if( type == NULL )
			{
				DEBUG_LOG( "参数名字为空" );
				continue;
			}

			XMLNode *pValue = pItem->FirstChild();
			if( pValue == NULL )
			{
				DEBUG_LOG( "参数值没有设置" );
				continue;
			}

			const char *value = pValue->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "参数值为空" );
				continue;
			}

			if( CGBMessage::CompareNoCase( type, "DeviceID" ) )
			{
				catalog.id = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Event" ) )
			{
				if( CGBMessage::CompareNoCase( value, "ON" ) )
				{
					catalog.event = EVENT_ON;
				}
				else if( CGBMessage::CompareNoCase( value, "OFF" ) )
				{
					catalog.event = EVENT_OFF;
				}
				else if( CGBMessage::CompareNoCase( value, "VLOST" ) )
				{
					catalog.event = EVENT_VLOST;
				}
				else if( CGBMessage::CompareNoCase( value, "DEFECT" ) )
				{
					catalog.event = EVENT_DEFECT;
				}
				else if( CGBMessage::CompareNoCase( value, "ADD" ) )
				{
					catalog.event = EVENT_ADD;
				}
				else if( CGBMessage::CompareNoCase( value, "DEL" ) )
				{
					catalog.event = EVENT_DEL;
				}
				else if( CGBMessage::CompareNoCase( value, "UPDATE" ) )
				{
					catalog.event = EVENT_UPDATE;
				}
				else 
				{
                    ERROR_LOG( "Event参数值\'" << value << "\'无效" );
                    reason = "参数Event值无效!";
					return false;
				}
			}
			else if( CGBMessage::CompareNoCase( type, "Name" ) )
			{
				catalog.name = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Manufacturer" ) )
			{
				catalog.manufacturer = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Model" ) )
			{
				catalog.model = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Owner" ) )
			{
				catalog.owner = value;
			}
			else if( CGBMessage::CompareNoCase( type, "CivilCode" ) )
			{
				catalog.civil = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Block" ) )
			{
				catalog.block = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Address" ) )
			{
				catalog.address = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Parental" ) )
			{
				catalog.parental = value;
			}
			else if( CGBMessage::CompareNoCase( type, "ParentID" ) )
			{
				catalog.parentid = value;
			}
			else if( CGBMessage::CompareNoCase( type, "SafetyWay" ) )
			{
				catalog.safetyway = value;
			}
			else if( CGBMessage::CompareNoCase( type, "RegisterWay" ) )
			{
				catalog.registerway = value;
			}
			else if( CGBMessage::CompareNoCase( type, "CertNum" ) )
			{
				catalog.certnum = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Certifiable" ) )
			{
				catalog.certifiable = value;
			}
			else if( CGBMessage::CompareNoCase( type, "ErrCode" ) )
			{
				catalog.errcode = value;
			}
			else if( CGBMessage::CompareNoCase( type, "EndTime" ) )
			{
				catalog.endtime = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Secrecy" ) )
			{
				catalog.secrecy = value;
			}
			else if( CGBMessage::CompareNoCase( type, "IPAddress" ) )
			{
				catalog.ip = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Port" ) )
			{
				catalog.port = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Password" ) )
			{
				catalog.password = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Status" ) )
			{
				catalog.status = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Longitude" ) )
			{
				catalog.longitude = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Latitude" ) )
			{
				catalog.latitude = value;
			}
			else if( CGBMessage::CompareNoCase( type, "PTZType" ) )
			{
				catalog.ptz = value;
			}
			else if( CGBMessage::CompareNoCase( type, "PositionType" ) )
			{
				catalog.position = value;
			}	
			else if( CGBMessage::CompareNoCase( type, "RoomType" ) )
			{
				catalog.room = value;
			}	
			else if( CGBMessage::CompareNoCase( type, "UseType" ) )
			{
				catalog.use = value;
			}	
			else if( CGBMessage::CompareNoCase( type, "SupplyLightType" ) )
			{
				catalog.supplylight = value;
			}	
			else if( CGBMessage::CompareNoCase( type, "DirectionType" ) )
			{
				catalog.direction = value;
			}	
			else if( CGBMessage::CompareNoCase( type, "Resolution" ) )
			{
				catalog.resolution = value;
			}	
			else if( CGBMessage::CompareNoCase( type, "BusinessGroupID" ) )
			{
				catalog.businessgroup = value;
			}
			else if( CGBMessage::CompareNoCase( type, "Info" ) )
			{
				for(XMLNode *pInfo = pItem->FirstChild(); pInfo != NULL; pInfo = pInfo->NextSibling() )
				{
					pValue = pInfo->FirstChild();
					if( pValue == NULL )
					{
						DEBUG_LOG( "参数值没有设置" );
						continue;
					}

					const char *type = pInfo->Value();
					if( type == NULL )
					{
						DEBUG_LOG( "参数名字为空" );
						continue;
					}

					const char *value = pValue->Value();
					if( value == NULL )
					{
						DEBUG_LOG( "参数值为空" );
						continue;
					}

					if( CGBMessage::CompareNoCase( type, "PTZType" ) )
					{
						catalog.ptz = value;
					}
					else if( CGBMessage::CompareNoCase( type, "PositionType" ) )
					{
						catalog.position = value;
					}	
					else if( CGBMessage::CompareNoCase( type, "RoomType" ) )
					{
						catalog.room = value;
					}	
					else if( CGBMessage::CompareNoCase( type, "UseType" ) )
					{
						catalog.use = value;
					}	
					else if( CGBMessage::CompareNoCase( type, "SupplyLightType" ) )
					{
						catalog.supplylight = value;
					}	
					else if( CGBMessage::CompareNoCase( type, "DirectionType" ) )
					{
						catalog.direction = value;
					}	
					else if( CGBMessage::CompareNoCase( type, "Resolution" ) )
					{
						catalog.resolution = value;
					}	
					else if( CGBMessage::CompareNoCase( type, "BusinessGroupID" ) )
					{
						catalog.businessgroup = value;
					}
					else
					{
						DEBUG_LOG( "国标未定义的参数:" << type );
					}
				}
			}
			else
			{
				DEBUG_LOG( "国标未定义的参数:" << type );
			}
		}

		// add to devices
		m_devices.push_back( catalog );
	}

	return true;
}

void CCatalogNotifyMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnCatalogNotify( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CNotifyResponseMsg::Send( resip::DialogUsageManager &mDum, bool tcp )
{
	if( m_notify != NULL )
	{
		if( m_statusCode >= 200 && m_statusCode < 400 )
		{
			resip::SharedPtr< resip::SipMessage > msg = m_notify->accept( m_statusCode );
			m_notify->send( msg );
		}
		else if( m_statusCode >= 400 )
		{
			resip::SharedPtr< resip::SipMessage > msg = m_notify->reject( m_statusCode );
			m_notify->send( msg );
		}
		else
		{
			resip::SharedPtr< resip::SipMessage > msg = m_notify->reject( 500 );
			m_notify->send( msg );
		}
	}
	else if( m_subscription != NULL )
	{
		if( m_statusCode >= 200 && m_statusCode < 400 )
		{
			resip::SharedPtr< resip::SipMessage > msg = m_subscription->accept( m_statusCode );
			m_subscription->send( msg );
		}
		else if( m_statusCode >= 400 )
		{
			resip::SharedPtr< resip::SipMessage > msg = m_subscription->reject( m_statusCode );
			m_subscription->send( msg );
		}
		else
		{
			resip::SharedPtr< resip::SipMessage > msg = m_subscription->reject( 500 );
			m_subscription->send( msg );
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CNotifyResponseMsg::Encode( std::string &message )
{
	return true;
}

bool CNotifyResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
	}

	return true;
}

void CNotifyResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnNotifyResponse( *this );
	}
}
