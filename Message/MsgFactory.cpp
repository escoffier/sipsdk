#include "MsgFactory.hpp"
#include "GB28181.hpp"
#include "GB28181Ex.hpp"

//////////////////////////////////////////////////////////////////////////
CGBMsgFactory::CGBMsgFactory()
{
	m_creater[VER_GB28181] = new CGBMsgCreaterT< VER_GB28181 >;
	m_creater[VER_GB28181Ex] = new CGBMsgCreaterT< VER_GB28181Ex >;
	m_creater[VER_MAX_MSG_VERSION] = NULL;
}

CGBMsgFactory::~CGBMsgFactory()
{
	for( int i = 0; i < VER_MAX_MSG_VERSION; ++i )
	{
		if( m_creater[i] != NULL )
		{
			delete m_creater[i];
			m_creater[i] = NULL;
		}
	}
}

CGBMessage* CGBMsgFactory::CreateInvite( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( INVITE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateInviteResponse( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( INVITE_REPONSE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateAck( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( ACK, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateBye( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( BYE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateCallInfo( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( CALLINFO, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateCallInfoResponse( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( CALLINFO_REPONSE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateCallMessage( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( CALLMESSAGE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateCallMessageResponse( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( CALLMESSAGE_REPONSE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateMessage( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}
	
	return Create( MESSAGE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateMessageResponse( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( MESSAGE_REPONSE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateNotify( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( NOTIFY, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateNotifyResponse( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( NOTIFY_REPONSE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateInfo( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( INFO, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateInfoResponse( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( INFO_RESPONSE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateRegisterAdd( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( REGISTER_ADD, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateRegisterRefresh( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( REGISTER_REFRESH, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateRegisterRemove( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( REGISTER_REMOVE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateRegisterRemoveAll( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( REGISTER_REMOVE_ALL, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateRegisterResponse( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( REGISTER_REPONSE, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateClientUpdateSubscription( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( CLIENT_UPDATE_SUBSCRIPTION, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateClientCancelSubscription( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( CLIENT_CANCEL_SUBSCRIPTION, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateClientAddSubscription( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( CLIENT_ADD_SUBSCRIPTION, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateServerAddSubscription( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( SERVER_ADD_SUBSCRIPTION, message, resaon );
}

CGBMessage* CGBMsgFactory::CreateServerResponseSubscription( const resip::SipMessage *message, std::string &resaon )
{
	if( message == NULL )
	{
		return NULL;
	}

	return Create( SERVER_RESPONSE_SUBSCRIPTION, message, resaon );
}

CGBMessage* CGBMsgFactory::Create( EGBMsgType msgType, const resip::SipMessage *message, std::string &resaon )
{
	// 参数校验
	if( message == NULL )
	{
		ERROR_LOG( "SIP协议栈消息为NULL，无法解析!" );
		resaon = "SIP协议栈消息为NULL，无法解析!";
		return NULL;
	}

	// 校验消息的版本是正确
	int version = VER_GB28181Ex;
	if( message->exists( resip::h_From ) )
	{
		// 根据用户判断消息的版本
		std::string user = message->header( resip::h_From ).uri().user().c_str();
	}

	// 创建消息
	CGBMsgFactory &factory = CGBMsgFactory::GetInstance();
	CGBMsgCreater *pCreater = factory.m_creater[version];
	if( pCreater != NULL )
	{
		resip::Data data;
		const resip::HeaderFieldValue &hfv = message->getRawBody();
		
	/*	if( hfv != NULL )
		{
			hfv->toBorrowData( data );
        }*/

		//else
		if (hfv.getLength() != 0)
		{
			hfv.toShareData(data);
		}
		else
		{
			resip::Contents *pContents = message->getContents();
			if (NULL != pContents)
			{
				resip::oDataStream temp(data);
				pContents->encode(temp);
			}
		}

		const char *body = 0;
		if( !data.empty() )
		{
			body = data.c_str();
		}

		// 创建消息
		CGBMessage *pMsg = NULL;
		switch( msgType )
		{
		case INVITE:
            {
                CGBMessage gbMsg;
                bool bDecoder = false;
                if( message->exists( resip::h_Subject ) )
                {
                    gbMsg.SetSubject( message->header( resip::h_Subject ).value().c_str() );
                    const std::string recvid = gbMsg.GetSubjectRecvID();
                    if (recvid.size() == 20)
                    {
                        const std::string& type = std::string(recvid.begin()+10, recvid.begin() + 13);
                        if ( (type == "114" || type == "133") )
                        {
                            bDecoder = true;
                        }
                    }
                }
				pMsg = pCreater->CreateInvite( body, bDecoder, resaon );
				break;
			}

		case INVITE_REPONSE:
			{
				pMsg = pCreater->CreateInviteResponse( body, resaon );
				break;
			}

		case CALLMESSAGE:
			{
				pMsg = pCreater->CreateCallMessage( body, resaon );
				break;
			}

		case CALLMESSAGE_REPONSE:
			{
				pMsg = pCreater->CreateCallMessageResponse( body, resaon );
				break;
			}

		case ACK:
			{
				pMsg = pCreater->CreateAck( body, resaon );
				break;
			}

		case BYE:
			{
				pMsg = pCreater->CreateBye( body, resaon );
				break;
			}

		case MESSAGE:
			{
				pMsg = pCreater->CreateMessage( body, resaon );
				break;
			}

		case MESSAGE_REPONSE:
			{
				pMsg = pCreater->CreateMessageResponse( body, resaon );
				break;
			}

		case NOTIFY:
			{
				pMsg = pCreater->CreateNotify( body, resaon );
				break;
			}

		case NOTIFY_REPONSE:
			{
				pMsg = pCreater->CreateNotifyResponse( body, resaon );
				break;
			}

		case CALLINFO:
			{
				pMsg = pCreater->CreateCallInfo( body, resaon );
				break;
			}

		case REGISTER_ADD:
			{
				pMsg = pCreater->CreateRegisterAdd( body, resaon );
				break;
			}

		case REGISTER_REPONSE:
			{
				pMsg = pCreater->CreateRegisterResponse( body, resaon );
				break;
			}

		case REGISTER_REFRESH:
			{
				pMsg = pCreater->CreateRegisterRefresh( body, resaon );
				break;
			}

		case REGISTER_REMOVE:
			{
				pMsg = pCreater->CreateRegisterRemove( body, resaon );
				break;
			}

		case REGISTER_REMOVE_ALL:
			{
				pMsg = pCreater->CreateRegisterRemoveAll( body, resaon );
				break;
			}

		case CLIENT_UPDATE_SUBSCRIPTION:
			{
				pMsg = pCreater->CreateClientUpdateSubscription( body, resaon );
				break;
			}

		case CLIENT_CANCEL_SUBSCRIPTION:
			{
				pMsg = pCreater->CreateClientCancelSubscription( body, resaon );
				break;
			}

		case CLIENT_ADD_SUBSCRIPTION:
			{
				pMsg = pCreater->CreateClientAddSubscription( body, resaon );
				break;
			}

		case SERVER_ADD_SUBSCRIPTION:
			{
				pMsg = pCreater->CreateServerAddSubscription( body, resaon );
				break;
			}

		case SERVER_RESPONSE_SUBSCRIPTION:
			{
				pMsg = pCreater->CreateServerNotifySubscription( body, resaon );
				break;
			}
		}

		// 设置Sip消息头域
		if( pMsg != NULL )
		{
			// 获取SipMessage的消息
			// statuscode 
			if( message->isResponse() )
			{
				pMsg->SetStatusCode( message->header( resip::h_StatusLine ).statusCode() );
			}
			else if( message->isRequest() )
			{
				pMsg->SetUser( message->header( resip::h_RequestLine ).uri().user().c_str() );
			}

			// expires
			if( message->exists( resip::h_Expires ) )
			{
				pMsg->SetExpires( message->header( resip::h_Expires ).value() );
			}

			// MaxForwards
			if( message->exists( resip::h_MaxForwards ) )
			{
				pMsg->SetMaxFrowards( message->header( resip::h_MaxForwards ).value() );
			}

			// subject
			if( message->exists( resip::h_Subject ) )
			{
				pMsg->SetSubject( message->header( resip::h_Subject ).value().c_str() );
			}

			// Date
			if( message->exists( resip::h_Date ) )
			{
				const resip::DateCategory &it = message->header( resip::h_Date );
				pMsg->SetDate( it.year(), it.month(), it.dayOfMonth(), it.hour(), it.minute(), it.second(), /*it.millisecond()*/ 0);
			}

			// to
			if( message->exists( resip::h_To ) )
			{
				pMsg->SetTo( message->header( resip::h_To ).uri().user().c_str() );
				if( message->header( resip::h_To ).exists( resip::p_tag ) )
				{
					pMsg->SetToTag( message->header( resip::h_To ).param( resip::p_tag ).c_str() );
				}
			}

			// from
			if( message->exists( resip::h_From ) )
			{
				pMsg->SetFrom( message->header( resip::h_From ).uri().user().c_str() );
				if( message->header( resip::h_From ).exists( resip::p_tag ) )
				{
					pMsg->SetFromTag( message->header( resip::h_From ).param( resip::p_tag ).c_str() );
				}
			}

			// callid
			if( message->exists( resip::h_CallID ) )
			{
				pMsg->SetCallID( message->header( resip::h_CallID ).value().c_str() );
			}

			// cseq
			if( message->exists( resip::h_CSeq ) )
			{
				pMsg->SetCSeq( message->header( resip::h_CSeq ).sequence() );
			}

			// contacts
			if( message->exists( resip::h_Contacts ) )
			{
				for( resip::NameAddrs::const_iterator it = message->header( resip::h_Contacts ).begin(); it != message->header( resip::h_Contacts ).end(); ++it )
				{
					const resip::Uri &uri = it->uri();
					pMsg->SetIP( uri.host().c_str() );
					pMsg->SetPort( uri.port() );
					pMsg->AddContacts( (*it).uri().user().c_str() );
				}
			}

			// vias
			if( message->exists( resip::h_Vias ) )
			{
				for( resip::Vias::const_iterator it = message->header( resip::h_Vias ).begin(); it != message->header( resip::h_Vias ).end(); ++it )
				{
					pMsg->AddVias( (*it).sentHost().c_str(), (*it).sentPort() );
				}
			}

            //Monitor-User-Identity
			//if( message->exists( resip::h_MonitorUserIdentity ) )
			//{
			//	const resip::MonitorUserIdentityCategory& it = message->header( resip::h_MonitorUserIdentity );
			//	pMsg->SetUserIdentity( it.param( resip::p_lockuser ).data(), it.param(resip::p_deviceid).data(), it.param(resip::p_organization).data(), it.param(resip::p_policetype).data(), it.param(resip::p_duty).data() );
			//}
			return pMsg;
		}
	}

	return NULL;
}

void CGBMsgFactory::DisableVersion( int version )
{
	CGBMsgFactory &factory = CGBMsgFactory::GetInstance();
	if( version >= 0 && version < VER_MAX_MSG_VERSION && factory.m_creater[version] != 0 )
	{
		delete factory.m_creater[version];
		factory.m_creater[version] = NULL;
	}
}

void CGBMsgFactory::EnableVersion( int version )
{
	CGBMsgFactory &factory = CGBMsgFactory::GetInstance();
	switch( version )
	{
	case VER_GB28181:
		{
			if( factory.m_creater[version] == NULL )
			{
				factory.m_creater[version] = new CGBMsgCreaterT< VER_GB28181 >;
			}

			break;
		}

	case VER_GB28181Ex:
		{
			if( factory.m_creater[version] == NULL )
			{
				factory.m_creater[version] = new CGBMsgCreaterT< VER_GB28181Ex >;
			}

			break;
		}
	}
}

