#include "QueryCatalogMsg.hpp"
#include "SipInterface.h"

//#define XINJIANG    1

//////////////////////////////////////////////////////////////////////////
bool CQueryCatalogMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Query>\r\n";
	content<<"<CmdType>Catalog</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
	
	if( !m_startTime.empty() )
	{
		content<<"<StartTime>"<< m_startTime <<"</StartTime>\r\n";
	}

	if( !m_endTime.empty() )
	{
		content<<"<EndTime>"<< m_endTime <<"</EndTime>\r\n";
	}

	content<< "</Query>" << "\r\n";
	message = content.str();
	return true;
}

bool CQueryCatalogMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	// ��������
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
		XMLNode *pNode = nodes[i];
		if( pNode == NULL )
		{
			DEBUG_LOG( "��������" );
			continue;
		}

		const char *type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "��������Ϊ��" );
			continue;
		}

		XMLNode *pChild = pNode->FirstChild();
		if( pChild == NULL )
		{
			DEBUG_LOG( "����ֵû������" );
			continue;
		}

		const char *value = pChild->Value();
		if( value == NULL )
		{
			DEBUG_LOG( "����ֵΪ��" );
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "StartTime" ) )
		{
			m_startTime = value;
		}
		else if( CGBMessage::CompareNoCase( type, "EndTime" ) )
		{
			m_endTime = value;
		}
		else
		{
			DEBUG_LOG( "����δ����Ĳ���:" << type );
			continue;
		}
	}

	return true;
}

void CQueryCatalogMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnQueryCatalog( *this );
	}
}

//////////////////////////////////////////////////////////////////////////
bool CQueryCatalogResponseMsg::CheckCatalog( const SCatalog &catalog, std::string& reason ) const
{
	std::ostringstream ss;

	CSipInterface::ECatalogType type = CSipInterface::GetIDType( catalog.id );
	if( type == CSipInterface::CATALOG_CIVIL )
	{
		if( catalog.id.empty() || catalog.name.empty() )
		{			
			ss << "��������Ŀ¼��\'" << catalog.id << "\'����,����id,nameû�б�����";
			reason = ss.str();
			ERROR_LOG( reason);
			return false;
		}
	}
	else if( type == CSipInterface::CATALOG_SYSTEM )
	{
		if( catalog.id.empty() || catalog.name.empty() )
		{			
			ss << "ϵͳĿ¼��\'" << catalog.id << "\'����,����id,name,manufacturer,model,owner,civil,address,registerway,secrecyû�б�����";
			reason = ss.str();
			ERROR_LOG(reason);
			return false;
		}
	}
	else if( type == CSipInterface::CATALOG_BUSINESS )
	{
		if( catalog.id.empty() || catalog.name.empty() )
		{
			ss << "ҵ����֯Ŀ¼��\'" << catalog.id << "\'����,����id,nameû�б�����" ;
			reason = ss.str();
			ERROR_LOG(reason);
			return false;
		}
	}
	else if( type == CSipInterface::CATALOG_ORGANIZATION )
	{
		if( catalog.id.empty() || catalog.name.empty() )
		{			
			ss << "������֯Ŀ¼��\'" << catalog.id << "\'����,����id,nameû�б�����";
			reason = ss.str();
			ERROR_LOG(reason);
			return false;
		}
	}
	else
	{
 		if( catalog.id.empty() || catalog.name.empty() )
 		{			
			ss << "�豸Ŀ¼�����,����id,nameû�б�����";
			reason = ss.str();
			ERROR_LOG(reason);
 			return false;
 		}
	}

	return true;
}

bool CQueryCatalogResponseMsg::Encode( std::string &message )
{
	std::ostringstream content;
	content<<"<?xml version=\"1.0\"?>\r\n";
	content<<"<Response>\r\n";
	content<<"<CmdType>Catalog</CmdType>\r\n";
	content<<"<SN>"<< m_sn <<"</SN>\r\n";

#ifdef XINJIANG
	for( std::list< SCatalog >::iterator it = m_devices.begin(); it != m_devices.end(); ++it )
	{
		const SCatalog &catalog = (*it);
		content<<"<DeviceID>"<< catalog.parentid  <<"</DeviceID>\r\n";
	}
#else
	content<<"<DeviceID>"<< m_deviceid <<"</DeviceID>\r\n";
#endif

	content<<"<SumNum>"<< m_sum <<"</SumNum>\r\n";
	content<<"<DeviceList Num=\"" << m_devices.size() << "\">\r\n";

	std::string reason;
	for( std::list< SCatalog >::iterator it = m_devices.begin(); it != m_devices.end(); ++it )
	{
		// У��Ŀ¼��ı�ѡ����		
		const SCatalog &catalog = (*it);
		if( !CheckCatalog( catalog, reason) )
		{
			continue;
		}

		// Ŀ¼��У��ͨ��������Ŀ¼��
		content<<"<Item>\r\n";
		if( !catalog.id.empty() )
		{
			content<<"<DeviceID>"<<catalog.id<<"</DeviceID>\r\n";
		}

		if( !catalog.name.empty() )
		{
			content<<"<Name>"<< catalog.name <<"</Name>\r\n";
		}

		if( !catalog.manufacturer.empty() )
		{
			content<<"<Manufacturer>"<< catalog.manufacturer <<"</Manufacturer>\r\n";
		}

		if( !catalog.model.empty() )
		{
			content<<"<Model>"<< catalog.model <<"</Model>\r\n";
		}

		if( !catalog.owner.empty() )
		{
			content<<"<Owner>"<< catalog.owner <<"</Owner>\r\n";
		}

		if( !catalog.civil.empty() )
		{
			content<<"<CivilCode>"<<catalog.civil<<"</CivilCode>\r\n";
		}

		if( !catalog.block.empty() )
		{
			content<<"<Block>"<< catalog.block <<"</Block>\r\n";
		}

		if( !catalog.address.empty() ) 
		{
			content<<"<Address>"<< catalog.address <<"</Address>\r\n";
		}

		if( !catalog.parental.empty() ) 
		{
			content<<"<Parental>"<< catalog.parental <<"</Parental>\r\n";
		}

		if( !catalog.parentid.empty() ) 
		{
			content<<"<ParentID>"<< catalog.parentid <<"</ParentID>\r\n";
		}

		if( !catalog.safetyway.empty() ) 
		{
			content<<"<SafetyWay>"<< catalog.safetyway <<"</SafetyWay>\r\n";
		}

		if( !catalog.registerway.empty() )
		{
			content<<"<RegisterWay>"<< catalog.registerway <<"</RegisterWay>\r\n";
		}

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

		if( !catalog.secrecy.empty() )
		{
			content<<"<Secrecy>"<< catalog.secrecy <<"</Secrecy>\r\n";
		}

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

		// ���������ҵ����֯����Ŀ¼
		CSipInterface::ECatalogType type = CSipInterface::GetIDType( catalog.id );
		if( type != CSipInterface::CATALOG_CIVIL && type != CSipInterface::CATALOG_BUSINESS )
		{
			if( !catalog.status.empty() )
			{
				content<<"<Status>"<<catalog.status<<"</Status>\r\n";
			}
		}

		if( !catalog.longitude.empty() ) 
		{
			content<<"<Longitude>"<<catalog.longitude<<"</Longitude>\r\n";
		}

		if( !catalog.latitude.empty() )
		{
			content<<"<Latitude>"<<catalog.latitude<<"</Latitude>\r\n";
		}

		if( !catalog.businessgroup.empty() ) 
		{
			content<<"<BusinessGroupID>"<<catalog.businessgroup<<"</BusinessGroupID>\r\n";
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
		content<<"</Item>\r\n";
	}
	content<<"</DeviceList>\r\n";
	content<<"</Response>\r\n\r\n";

	message = content.str();
	return true;
}

bool CQueryCatalogResponseMsg::Decode( const std::vector< XMLNode* > &nodes, std::string &reason )
{
	std::string sTmpRes(reason);

	// �������У��
	XMLNode *pSumNum = NULL;
	size_t size = nodes.size();
	for( size_t i = 0; i < size; ++i )
	{
		XMLNode *pNode = nodes[i];
		if( pNode == NULL )
		{
			DEBUG_LOG( "��������" );
			sTmpRes = "��������";
			continue;
		}

		const char* type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "��������Ϊ��" );
			sTmpRes = "��������Ϊ��";
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "SumNum" ) )
		{
			pSumNum = pNode;
		}
	}

	// ��ѡ����������
	if( pSumNum == NULL )
	{
        ERROR_LOG( "����SumNum" );
        reason = "����SumNumû�б�����!";
		return false;
	}

	// ��������
	std::list< XMLNode* > items;
	for( size_t i = 0; i < size; ++i )
	{
		XMLNode *pNode = nodes[i];
		if( pNode == NULL )
		{
			DEBUG_LOG( "��������" );
			sTmpRes = "��������";
			continue;
		}

		const char* type = pNode->Value();
		if( type == NULL )
		{
			DEBUG_LOG( "��������Ϊ��" );
			sTmpRes = "��������Ϊ��";
			continue;
		}

		if( CGBMessage::CompareNoCase( type, "SumNum" ) )
		{
			XMLNode *pChild = pNode->FirstChild();
			if( pChild == NULL )
			{
				DEBUG_LOG( "����ֵû������" );
				sTmpRes = "����ֵû������";
				continue;
			}

			const char *value = pChild->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "����ֵΪ��" );
				sTmpRes = "����ֵΪ��";
				continue;
			}
			else
			{
				m_sum = atoi( value );
			}
		}
		else if( CGBMessage::CompareNoCase( type, "DeviceList" ) )
		{
			for( XMLNode *pItem = pNode->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
			{
				const char *type = pItem->Value();
				if( type == NULL )
				{
					DEBUG_LOG( "��������Ϊ��" );
					sTmpRes = "��������Ϊ��";
					continue;
				}

				if( CGBMessage::CompareNoCase( type, "Item" ) )
				{
					items.push_back( pItem );
				}
				else
				{
					DEBUG_LOG( "����δ����Ĳ���:" << type );
					sTmpRes = "����δ����Ĳ���:";
					sTmpRes += std::string(type);
					continue;
				}
			}
		}
		else
		{
			DEBUG_LOG( "����δ����Ĳ���:" << type );
			sTmpRes = "����δ����Ĳ���:";
			sTmpRes += std::string(type);
			continue;
		}
	}

	// Ŀ¼��
	std::list< XMLNode* >::iterator it = items.begin();
	std::list< XMLNode* >::iterator end = items.end();
	for( /*it*/; it != end; ++it )
	{
		SCatalog catalog;
		for( XMLNode *pItem = (*it)->FirstChild(); pItem != NULL; pItem = pItem->NextSibling() )
		{
			const char *type = pItem->Value();
			if( type == NULL )
			{
				DEBUG_LOG( "��������Ϊ��" );
				sTmpRes = "��������Ϊ��";
				continue;
			}

			XMLNode *pValue = pItem->FirstChild();
			if( pValue == NULL )
			{
				DEBUG_LOG( "����ֵû������" );
				sTmpRes = "����ֵû������";
				continue;
			}

			const char *value = pValue->Value();
			if( value == NULL )
			{
				DEBUG_LOG( "����ֵΪ��" );
				sTmpRes = "����ֵΪ��";
				continue;
			}

			if( CGBMessage::CompareNoCase( type, "DeviceID" ) )
			{
				catalog.id = value;
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
				for( XMLNode *pInfo = pItem->FirstChild(); pInfo != NULL; pInfo = pInfo->NextSibling() )
				{
					pValue = pInfo->FirstChild();
					if( pValue == NULL )
					{
						DEBUG_LOG( "����ֵû������" );
						sTmpRes = "����ֵû������";
						continue;
					}

					const char *type = pInfo->Value();
					if( type == NULL )
					{
						DEBUG_LOG( "��������Ϊ��" );
						sTmpRes = "��������Ϊ��";
						continue;
					}

					const char *value = pValue->Value();
					if( value == NULL )
					{
						DEBUG_LOG( "����ֵΪ��" );
						sTmpRes = "����ֵΪ��";
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
						DEBUG_LOG( "����δ����Ĳ���:" << type );
					}
				}
			}
			else
			{
				DEBUG_LOG( "����δ����Ĳ���:" << type );
				sTmpRes = "����δ����Ĳ���:";
				sTmpRes += std::string(type);
			}
		}

		// У��Ŀ¼���ͱ�ѡ��
		if( !CheckCatalog( catalog, sTmpRes) )
		{
			continue;
		}

		// ��ӵ��豸�б���
		m_devices.push_back( catalog );
	}

	//Ŀ¼��������
	//if (size != m_devices.size())
	//{
	//	reason = sTmpRes;
	//}

	return true;
}

void CQueryCatalogResponseMsg::Process( CSipInterface *pInterface )
{
	if( pInterface != NULL )
	{
		pInterface->OnQueryCatalogResponse( *this );
	}
}

