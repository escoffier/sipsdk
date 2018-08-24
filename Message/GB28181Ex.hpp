#ifndef GBMSG_GB28181EXT_CREATER_HPP_
#define GBMSG_GB28181EXT_CREATER_HPP_

#include "tinyxml2.h"
#include "GB28181.hpp"
#include "MsgCreater.hpp"
#include "AlarmNotifyMsg.hpp"
#include "RegisterMsg.hpp"
#include "InviteMsg.hpp"
#include "BroadcastMsg.hpp"
#include "MessageMsg.hpp"
#include "CatalogNotifyMsg.hpp"
#include "CallInfoMsg.hpp"
#include "CallMessageMsg.hpp"
#include "DeviceControlMsg.hpp"
#include "KeepaliveMsg.hpp"
#include "QueryCatalogMsg.hpp"
#include "QueryDeviceInfoMsg.hpp"
#include "QueryRecordInfoMsg.hpp"
#include "QueryAlarmMsg.hpp"
#include "SubscriptionMsg.hpp"
#include "QueryDeviceStatusMsg.hpp"
#include "ConfigDownloadMsg.hpp"
#include "DeviceConfigMsg.hpp"

//////////////////////////////////////////////////////////////////////////
template<>
class CGBMsgCreaterT< VER_GB28181Ex > : public CGBMsgCreaterT< VER_GB28181 >
{
public:
	friend class CGBMsgFactory;

protected:
	CGBMsgCreaterT()
	{
	}

	virtual ~CGBMsgCreaterT()
	{
	}

private:
	CGBMsgCreaterT( const CGBMsgCreaterT &rhs );
	CGBMsgCreaterT& operator=( const CGBMsgCreaterT &rhs );

public:
    virtual CGBMessage* CreateInvite( const char *body, const bool bDecoder, std::string &resaon )
	{
		CInviteMsg *pMsg = NULL;
		if( body == NULL )
		{
			pMsg = new CRealStreamMsg;
		}
		else
		{
			if( strstr( body, "s=playback" ) != NULL || strstr( body, "s=Playback" ) != NULL )
			{
				pMsg = new CVodStreamMsg;
			}
			else if( strstr( body, "s=download" ) != NULL || strstr( body, "s=Download") != NULL )
			{
				pMsg = new CVodDownloadMsg;
			}
			else if( strstr( body, "s=play" ) != NULL || strstr( body, "s=Play") != NULL )
			{
				if( strstr( body, "m=audio" ) != NULL || strstr( body, "m=Audio") != NULL )
				{
                    if( strstr( body, "m=video" ) != NULL || strstr( body, "m=Video") != NULL )
                    {
                        pMsg = new CRealStreamMsg;
                    }
                    else
                    {
					    pMsg = new CBroadcastStreamMsg;
                    }
				}
				else if( bDecoder )
                {
                    pMsg = new CRealStreamToDecorderMsg;
				}
                else
				{
					pMsg = new CRealStreamMsg;
				}
			}
		}

		if( pMsg != NULL )
		{
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建Invite消息失败!" );
		}
		
		return pMsg;
	}

	virtual CGBMessage* CreateInviteResponse( const char *body, std::string &resaon )
	{
		CInviteResponseMsg *pMsg = NULL;
		if( body == NULL )
		{
			pMsg = new CRealStreamResponseMsg;
		}
		else
		{
			if( strstr( body, "s=NPVodPlay" ) != NULL || strstr( body, "s=playback" ) != NULL || strstr( body, "s=Playback" ) != NULL )
			{
				pMsg = new CVodStreamResponseMsg;
			}
			else if( strstr( body, "s=download" ) != NULL || strstr( body, "s=Download") != NULL )
			{
				pMsg = new CVodDownloadResponseMsg;
			}
			else if( strstr( body, "s=play" ) != NULL || strstr( body, "s=Play") != NULL )
			{
				if(  strstr( body, "m=audio" ) != NULL || strstr( body, "m=Audio") != NULL )
                {
                    if( strstr( body, "m=video" ) != NULL || strstr( body, "m=Video") != NULL )
                    {
                        pMsg = new CRealStreamResponseMsg;
                    }
                    else
                    {
                        pMsg = new CBroadcastStreamResponseMsg;
                    }
				}
				else
				{
					pMsg = new CRealStreamResponseMsg;
				}
			}
            else
            {
                pMsg = new CUnknowStreamResponseMsg;
            }
		}

		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建Invite回应消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateAck( const char *body, std::string &resaon )
	{
		CAckMsg *pMsg = new CAckMsg;
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建Ack消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateBye( const char *body, std::string &resaon )
	{
		CByeMsg *pMsg = new CByeMsg;
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建Bye消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateCallInfo( const char *body, std::string &resaon )
	{
		CVodCtrlMsg *pMsg = new CVodCtrlMsg;		
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建会话内Info消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateCallInfoResponse( const char *body, std::string &resaon )
	{
		CCallInfoResponseMsg *pMsg = new CCallInfoResponseMsg;		
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建会话内Info回应消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateCallMessage( const char *body, std::string &resaon )
	{
		if( body == NULL )
		{
			return NULL;
		}

		//TiXmlDocument doc;
		//doc.Parse( body, 0, TIXML_DEFAULT_ENCODING );
		//TiXmlElement *pRoot = doc.RootElement();
		XMLDocument doc;
		doc.Parse(body);
		XMLElement * pRoot = doc.RootElement();
		if( pRoot == NULL )
		{
			return NULL;
		}

		const char* msgType = pRoot->Value();
		if( msgType == NULL )
		{
			return NULL;
		}

		//XMLNode *pCmd = 0;
		//XMLNode *pSN = 0;
		//XMLNode *pDeviceID = 0;
		//std::vector< XMLNode* > nodes;
		//for( XMLNode *pNode = pRoot->FirstChild(); pNode != 0; pNode = pNode->NextSibling() )
		XMLNode *pCmd = 0;
		XMLNode *pSN = 0;
		XMLNode *pDeviceID = 0;
		std::vector< XMLNode* > nodes;
		for(XMLNode *pNode = pRoot->FirstChild(); pNode != 0; pNode = pNode->NextSibling() )
		{
			const char* type = pNode->Value(); 
			if( type == 0 )
			{
				return NULL;
			}

			if( CGBMessage::CompareNoCase( type, "CmdType" ) )
			{
				pCmd = pNode->FirstChild();
			}
			else if( CGBMessage::CompareNoCase( type, "SN" ) )
			{
				pSN = pNode->FirstChild();
			}
			else if( CGBMessage::CompareNoCase( type, "DeviceID" ) )
			{
				pDeviceID = pNode->FirstChild();
			}
			else
			{
				nodes.push_back( pNode );
			}	
		}

		// 必选参数
		if( pCmd == NULL || pSN == NULL || pDeviceID == NULL )
		{
			return NULL;
		}

		const char *sn = pSN->Value();
		if( sn == NULL )
		{
			return NULL;
		}

		const char *cmdType = pCmd->Value();
		if( cmdType == NULL )
		{
			return NULL;
		}

		const char *deviceid = pDeviceID->Value();
		if( deviceid == NULL )
		{
			return NULL;
		}

		CCallMessageMsg *pMsg = NULL;
		if( CGBMessage::CompareNoCase( msgType, "Notify" ) )
		{
			if( CGBMessage::CompareNoCase( cmdType, "MediaStatus" ) )
			{
				pMsg = new CStreamPlayEndMsg;
			}
			else
			{
				// error
			}
		}
		else
		{
			// error
		}

		if( pMsg != NULL )
        {
			if( !pMsg->Decode( nodes, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
			else
			{
				pMsg->SetSN( sn );
				pMsg->SetDeviceID( deviceid );
			}
		}
		else
		{
			DEBUG_LOG( "创建会话内Message消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateCallMessageResponse( const char *body, std::string &resaon )
	{
		if( body != NULL )
		{
			return NULL;
		}

		return new CCallMessageResponseMsg;
	}

	virtual CGBMessage* CreateMessage( const char *body, std::string &resaon )
	{
		if( body == NULL )
		{
			return NULL;
		}
		XMLDocument doc;
		doc.Parse(body);
		XMLElement* pRoot = doc.RootElement();
		//TiXmlDocument doc;
		//doc.Parse( body, 0, TIXML_ENCODING_LEGACY );
		//TiXmlElement *pRoot = doc.RootElement();
		if( pRoot == NULL )
		{
			return NULL;
		}
		
		const char* msgType = pRoot->Value();
		if( msgType == NULL )
		{
			return NULL;
		}

		//XMLNode *pCmd = 0;
		//XMLNode *pSN = 0;
		//XMLNode *pDeviceID = 0;
		XMLNode* pCmd  = 0/*= pRoot->FirstChildElement("CmdType")*/;
		XMLNode* pSN  = 0/*= pRoot->FirstChildElement("SN")*/;;
		XMLNode* pDeviceID  = 0/*= pRoot->FirstChildElement("DeviceID")*/;;

		//std::vector< XMLNode* > nodes;
		//for( XMLNode *pNode = pRoot->FirstChild(); pNode != 0; pNode = pNode->NextSibling() )
		std::vector< XMLNode* > nodes;
		for (XMLNode *pNode = pRoot->FirstChild(); pNode != 0; pNode = pNode->NextSibling())
		{
			const char* type = pNode->Value(); 
			if( type == 0 )
			{
				return NULL;
			}

			if( CGBMessage::CompareNoCase( type, "CmdType" ) )
			{
				pCmd = pNode->FirstChild();
			}
			else if( CGBMessage::CompareNoCase( type, "SN" ) )
			{
				pSN = pNode->FirstChild();
			}
			else if( CGBMessage::CompareNoCase( type, "DeviceID" ) )
			{
				pDeviceID = pNode->FirstChild();
			}
			else
			{
				nodes.push_back( pNode );
			}			
		}

		// 语言广播消息没有DeviceID，对DeviceID的校验需要根据类型来判断
		if( pCmd == NULL || pSN == NULL/* || pDeviceID == NULL*/ )
		{
			return NULL;
		}

		const char *cmdType = pCmd->Value();
		if( cmdType == NULL )
		{
			return NULL;
		}

		CMessageMsg *pMsg = NULL;
		if( CGBMessage::CompareNoCase( msgType, "Response" ) )
		{
			if( pDeviceID == NULL )
			{
				return NULL;
			}

			if( CGBMessage::CompareNoCase( cmdType, "Catalog" ) )
			{	
				pMsg = new CQueryCatalogResponseMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "RecordInfo" ) )
			{
				pMsg = new CQueryRecordInfoResponseMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "DeviceControl" ) )
			{
				pMsg = new CDeviceControlResponseMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "Alarm" ) )
			{
				pMsg = new CAlarmNotifyResponseMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "DeviceInfo" ) )
			{
				pMsg = new CQueryDeviceInfoResponseMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "DeviceStatus" ) )
			{
				pMsg = new CQueryDeviceStatusResponseMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "Broadcast" ) )
			{
				pMsg = new CBroadcastResponseMsg;
            }
            else if( CGBMessage::CompareNoCase( cmdType, "ConfigDownload" ) )
            {
                size_t size = nodes.size();
                for( size_t i = 0; i < size; ++i )
                {
                    const char *value = nodes[i]->Value();
                    if( value == NULL )
                    {
                        return NULL;
                    }

                    if( CGBMessage::CompareNoCase( value, "BasicParam" ) )
                    {
                        pMsg = new CBasicParamResponseMsg();
                    }
                    else if( CGBMessage::CompareNoCase( value, "VideoParamConfig" ) )
                    {
                        pMsg = new CVideoParamConfigResponseMsg;
                    }
                    else if( CGBMessage::CompareNoCase( value, "AudioParamConfig" ) )
                    {
                        pMsg = new CAudioParamConfigResponseMsg;
                    }
                    else if( CGBMessage::CompareNoCase( value, "SVACEncodeConfig" ) )
                    {
                        pMsg = new CSVACEncodeConfigResponseMsg;
                    }
                    else if( CGBMessage::CompareNoCase( value, "SVACDecodeConfig" ) )
                    {
                        pMsg = new CSVACDecodeConfigResponseMsg;
                    }
                    else
                    {
                        // error
                    }
                }
            }
            else if( CGBMessage::CompareNoCase( cmdType, "DeviceConfig" ) )
            {
                pMsg = new CDeviceConfigResponseMsg;
            }
			else
			{
				// 错误的消息格式，不处理
			}
		}
		else if( CGBMessage::CompareNoCase( msgType, "Query" ) )
		{
			if( pDeviceID == NULL )
			{
				return NULL;
			}

		    if( CGBMessage::CompareNoCase( cmdType, "Catalog" ) )
			{
				pMsg = new CQueryCatalogMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "RecordInfo" ) )
			{
				pMsg = new CQueryRecordInfoMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "DeviceInfo" ) )
			{
				pMsg = new CQueryDeviceInfoMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "DeviceStatus" ) )
			{
				pMsg = new CQueryDeviceStatusMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "Alarm" ) )
			{
				pMsg = new CQueryAlarmMsg;			
			}
            else if ( CGBMessage::CompareNoCase( cmdType, "ConfigDownload" ) )
            {
                pMsg = new CConfigDownloadMsg;	
            }
			else
			{
				ERROR_LOG( "不识别的消息类型： " << cmdType );
				// 错误的消息格式，不处理
			}
		}
		else if( CGBMessage::CompareNoCase( msgType, "Control" ) )
		{
			if( pDeviceID == NULL )
			{
				return NULL;
			}

		    if( CGBMessage::CompareNoCase( cmdType, "DeviceControl" ) )
			{
				size_t size = nodes.size();
				for( size_t i = 0; i < size; ++i )
				{
					const char *value = nodes[i]->Value();
					if( value == NULL )
					{
						return NULL;
					}

					if( CGBMessage::CompareNoCase( value, "PTZCmd" ) )
					{
						pMsg = new CPTZControlMsg();
					}
					else if( CGBMessage::CompareNoCase( value, "RecordCmd" ) )
					{
						pMsg = new CRecordControlMsg;
					}
					else if( CGBMessage::CompareNoCase( value, "TeleBoot" ) )
					{
						pMsg = new CTeleBootMsg;
					}
					else if( CGBMessage::CompareNoCase( value, "GuardCmd" ) )
					{
						pMsg = new CGuardMsg;
					}
					else if( CGBMessage::CompareNoCase( value, "AlarmCmd" ) )
					{
						pMsg = new CResetAlarmMsg;
					}
					else if( CGBMessage::CompareNoCase( value, "DragZoomIn" ) )
					{
						pMsg = new CDragZoomInMsg;
					}
					else if( CGBMessage::CompareNoCase( value, "DragZoomOut" ) )
					{
						pMsg = new CDragZoomOutMsg;
					}
					else if( CGBMessage::CompareNoCase( value, "ControlLock" ) )
					{
						pMsg = new CPTZLockMsg;
                    }
					else
					{
						// error
					}
				}
			}
            else if ( CGBMessage::CompareNoCase( msgType, "DeviceConfig" ) )
            {
                size_t size = nodes.size();
                for( size_t i = 0; i < size; ++i )
                {
                    const char *value = nodes[i]->Value();
                    if( value == NULL )
                    {
                        return NULL;
                    }

                    if( CGBMessage::CompareNoCase( value, "BasicParam" ) )
                    {
                        pMsg = new CBasicParamMsg();
                    }
                    else if( CGBMessage::CompareNoCase( value, "VideoParamConfig" ) )
                    {
                        pMsg = new CVideoParamConfigMsg;
                    }
                    else if( CGBMessage::CompareNoCase( value, "AudioParamConfig" ) )
                    {
                        pMsg = new CAudioParamConfigMsg;
                    }
                    else if( CGBMessage::CompareNoCase( value, "SVACEncodeConfig" ) )
                    {
                        pMsg = new CSVACEncodeConfigMsg;
                    }
                    else if( CGBMessage::CompareNoCase( value, "SVACDecodeConfig" ) )
                    {
                        pMsg = new CSVACDecodeConfigMsg;
                    }
                    else
                    {
                        // error
                    }
                }
            }
			else
			{
				// error
			}
		}
		else if( CGBMessage::CompareNoCase( msgType, "Notify" ) )
		{
		    if( CGBMessage::CompareNoCase( cmdType, "Keepalive" ) )
			{
				if( pDeviceID == NULL )
				{
					return NULL;
				}

				pMsg = new CKeepAliveMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "Alarm" ) )
			{
				if( pDeviceID == NULL )
				{
					return NULL;
				}

				pMsg = new CAlarmNotifyMsg;
			}
			else if( CGBMessage::CompareNoCase( cmdType, "Broadcast" ) )
			{
				pMsg = new CBroadcastMsg;
			}
			else
			{
				// error
			}
		}
		else
		{
			// error
		}

		if( pMsg != NULL )
        {
			if( !pMsg->Decode( nodes, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
			else
			{
				if( pCmd != NULL )
				{
					pMsg->SetCmdType( cmdType );
				}

				if( pSN != NULL )
				{
					pMsg->SetSN( pSN->Value() );
				}

				if( pDeviceID != NULL )
				{
					pMsg->SetDeviceID( pDeviceID->Value() );
				}
			}
		}
		else
		{
			DEBUG_LOG( "创建Message消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateMessageResponse( const char *body, std::string &resaon )
	{
		if( body != NULL )
		{
			return NULL;
		}

		return new CMessageResponseMsg;
	}

	virtual CGBMessage* CreateNotify( const char *body, std::string &resaon )
	{
		if( body == NULL )
		{
			return NULL;
		}

		//TiXmlDocument doc;
		//doc.Parse( body, 0, TIXML_DEFAULT_ENCODING );
		//TiXmlElement *pRoot = doc.RootElement();
		XMLDocument doc;
		doc.Parse(body);
		XMLElement* pRoot = doc.RootElement();
		if( pRoot == NULL )
		{
			return NULL;
		}

		//XMLNode *pCmd = 0;
		//XMLNode *pSN = 0;
		//XMLNode *pDeviceID = 0;
		//std::vector< XMLNode* > nodes;
		//for( XMLNode *pNode = pRoot->FirstChild(); pNode != 0; pNode = pNode->NextSibling() )
		XMLNode* pCmd = 0;
		XMLNode* pSN = 0;
		XMLNode* pDeviceID = 0;
		std::vector< XMLNode* > nodes;
		for (XMLNode *pNode = pRoot->FirstChild(); pNode != 0; pNode = pNode->NextSibling())
		{
			std::string value = pNode->Value();
			if( value == "CmdType" )
			{
				pCmd = pNode->FirstChild();
			}
			else if( value == "SN" )
			{
				pSN = pNode->FirstChild();
			}
			else if( value == "DeviceID" )
			{
				pDeviceID = pNode->FirstChild();
			}
			else
			{
				nodes.push_back( pNode );
			}			
		}

		if( pCmd == NULL || pSN == NULL || pDeviceID == NULL )
		{
			return NULL;
		}

		std::string sn = pSN->Value();
		if( sn.empty() )
		{
			return NULL;
		}

		std::string deviceid = pDeviceID->Value();
		if( deviceid.empty() )
		{
			return NULL;
		}

		// create message by msgType
		CCatalogNotifyMsg *pMsg = NULL;
		std::string msgType = pRoot->Value();
		std::string cmdType = pCmd->Value();
		if( msgType == "Notify" )
		{
			if( cmdType == "Catalog" )
			{
				pMsg = new CCatalogNotifyMsg;
			}
			else
			{
				// error
			}
		}
		else
		{
			// error
		}

		if( pMsg != NULL )
        {
			if( !pMsg->Decode( nodes, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
			else
			{
				pMsg->SetSN( pSN->Value() );
				pMsg->SetDeviceID( pDeviceID->Value() );
			}
		}
		else
		{
			DEBUG_LOG( "创建消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateNotifyResponse( const char *body, std::string &resaon )
	{
		CNotifyResponseMsg *pMsg = new CNotifyResponseMsg;		
		if( pMsg != NULL )
        {
			std::vector< XMLNode* > nodes;
			if( !pMsg->Decode( nodes, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建Notify消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateInfo( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateInfoResponse( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateRegisterAdd( const char *body, std::string &resaon )
	{
		CRegisterAddMsg *pMsg = new CRegisterAddMsg;		
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建注册消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateRegisterResponse( const char *body, std::string &resaon )
	{
		CRegisterResponseMsg *pMsg = new CRegisterResponseMsg;	
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建注册回应消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateRegisterRefresh( const char *body, std::string &resaon )
	{
		CRegisterRefreshMsg *pMsg = new CRegisterRefreshMsg;		
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建刷新注册消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateRegisterRemove( const char *body, std::string &resaon )
	{
		CRegisterRemoveMsg *pMsg = new CRegisterRemoveMsg;		
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建注销消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateRegisterRemoveAll( const char *body, std::string &resaon )
	{
		CRegisterRemoveAllMsg *pMsg = new CRegisterRemoveAllMsg;		
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( body, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
				ERROR_LOG( "消息体解析错误!" );
			}
		}
		else
		{
			DEBUG_LOG( "创建消息失败!" );
		}

		return pMsg;
	}

	virtual CGBMessage* CreateClientUpdateSubscription( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateClientCancelSubscription( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateClientAddSubscription( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateServerAddSubscription( const char *body, std::string &resaon )
	{
		if( body == NULL )
		{
			return NULL;
		}

		//TiXmlDocument doc;
		
		//doc.Parse( body, 0, TIXML_DEFAULT_ENCODING );
		//TiXmlElement *pRoot = doc.RootElement();

		XMLDocument doc;
		doc.Parse(body);
		XMLElement* pRoot = doc.RootElement();
		if( pRoot == NULL )
		{
			return NULL;
		}

		const char* msgType = pRoot->Value();
		if( msgType == NULL )
		{
			return NULL;
		}
		//XMLElement* pCmd = pRoot->FirstChildElement("CmdType");
		//XMLElement* pSN = pRoot->FirstChildElement("SN");;
		//XMLElement* pDeviceID = pRoot->FirstChildElement("DeviceID");;
		XMLNode* pCmd = 0;
		XMLNode* pSN = 0;
		XMLNode* pDeviceID = 0;
		std::vector< XMLNode* > nodes;
		for (XMLNode *pNode = pRoot->FirstChild(); pNode != 0; pNode = pNode->NextSibling())
		{
			const char* type = pNode->Value(); 
			if( type == 0 )
			{
				return NULL;
			}

			if( CGBMessage::CompareNoCase( type, "CmdType" ) )
			{
				pCmd = pNode->FirstChild();
			}
			else if( CGBMessage::CompareNoCase( type, "SN" ) )
			{
				pSN = pNode->FirstChild();
			}
			else if( CGBMessage::CompareNoCase( type, "DeviceID" ) )
			{
				pDeviceID = pNode->FirstChild();
			}
			else
			{
				nodes.push_back( pNode );
			}			
		}

		if( pCmd == NULL || pSN == NULL || pDeviceID == NULL )
		{
			return NULL;
		}

		const char *cmdType = pCmd->Value();
		if( cmdType == NULL )
		{
			return NULL;
		}
		
		CAddSubscribeMsg *pMsg = new CAddSubscribeMsg;
		if( pMsg != NULL )
        {
			if( !pMsg->Decode( nodes, resaon ) )
			{
				delete pMsg;
				pMsg = NULL;
			}
			else
			{
				pMsg->SetSN( pSN->Value() );
				pMsg->SetDeviceID( pDeviceID->Value() );
			}
		}

		return pMsg;
	}

	virtual CGBMessage* CreateServerNotifySubscription( const char *body, std::string &resaon )
    {	
        if( body != NULL )
        {
            return NULL;
        }

        return new CSubscribeResponseMsg;
	}
};

#endif // GBMSG_GB28181EXT_CREATER_HPP_
