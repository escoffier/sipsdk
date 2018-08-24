#include "SipGateway.hpp"
#include "NPReproAuthMananger.h"
#include "RegisterHandler.h"
#include "MessageHandler.h"
#include "InviteHandler.h"
#include "SubscriptionHandler.h"
#include "NotifyHandler.h"
#include "RegisterHandler.h"
#include "OutboundProxyContainer.h"
#include "RegisterHandler.h"

CSipGateway::CSipGateway() : m_regData( 0 )
{
	m_regData = new resip::InMemorySyncRegDb( 0 );
}

CSipGateway::~CSipGateway()
{
	if( m_regData != 0 )
	{
		delete m_regData;
		m_regData = 0;
	}
}

void CSipGateway::RunSipStack()
{
	__DUM_TRY
	//禁用统计功能
	mStack.statisticsManagerEnabled() = false;

	//添加网络层
	mDum.addTransport(resip::UDP, m_iListenPort);
	mDum.addTransport(resip::TCP, m_iListenPort);

	//设置主配置
	mDum.setMasterProfile(mSProfile);

	//设置用户代理
	SetUserAgent();

	//设置默认From
	SetDefaultFrom();

	//注册失败后继续注册的时间
	mDum.getMasterProfile()->setDefaultRegistrationRetryTime(m_regtime);

	//默认注册时间？
	mDum.getMasterProfile()->setDefaultRegistrationTime(70);

	//添加认证
	//目前只支持以同一个用户名密码向上注册，之后可以进行扩展，但是要修改协议栈代码
	m_sRealm = m_sUserName.substr(0, 10);
	m_sPassword = mOutboundProxyContainer.getPassword().c_str();

	//设置认证管理器
	std::auto_ptr<resip::ClientAuthManager> uasAuth(mAClientAuthManager);
	mDum.setClientAuthManager(uasAuth);

	//添加支持MESSAGE方法
	mDum.getMasterProfile()->clearSupportedMethods();
	mDum.getMasterProfile()->addSupportedMethod(resip::REGISTER);
	mDum.getMasterProfile()->addSupportedMethod(resip::INVITE);
	mDum.getMasterProfile()->addSupportedMethod(resip::MESSAGE);
	mDum.getMasterProfile()->addSupportedMethod(resip::ACK);
	mDum.getMasterProfile()->addSupportedMethod(resip::BYE);
	mDum.getMasterProfile()->addSupportedMethod(resip::CANCEL);
	mDum.getMasterProfile()->addSupportedMethod(resip::INFO);
	mDum.getMasterProfile()->addSupportedMethod(resip::SUBSCRIBE);
	mDum.getMasterProfile()->addSupportedMethod(resip::NOTIFY);

	// #pragma message (REMIND(SIP方法目前只支持REGISTER、INVITE、MESSAGE、ACK、BYE、INFO、CANCEL。))
	//MESSAGE方法添加消息体类型
	mDum.getMasterProfile()->clearSupportedMimeTypes();
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("text", "plain"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("application", "KSLP"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("application", "MANSCDP+xml"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::SUBSCRIBE, resip::Mime("application", "MANSCDP+xml"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::NOTIFY, resip::Mime("application", "MANSCDP+xml"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("application", "KSPTZ"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("application", "ALARM"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("application", "KSSP"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("application", "KSDU"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::MESSAGE, resip::Mime("application", "cpim-pidf+xml"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::INVITE, resip::Mime("application", "sdp"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::INVITE, resip::Mime("application", "MANSCDP+xml"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::INFO, resip::Mime("application", "KSPTZ"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::INFO, resip::Mime("application", "KSSP"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::INFO, resip::Mime("application", "MANSRTSP"));
	mDum.getMasterProfile()->addSupportedMimeType(resip::INFO, resip::Mime("application", "rtsp"));
	// #pragma message (REMIND(消息体类型目前只支持text/plain、application/sdp、application/KSWLP、application/KSPTZ。))

	//添加注册回调处理
	//REGISTER
	mDum.setClientRegistrationHandler( mClientRegHandler );
	mDum.setServerRegistrationHandler( mServerRegHandler );
	mDum.setRegistrationPersistenceManager( m_regData );    
	resip::SharedPtr<resip::ServerAuthManager> ucsAuth( new NPReproAuthManager( this, mDum ) );
	mDum.setServerAuthManager( ucsAuth );

	//INVITE
	mDum.setInviteSessionHandler( mInviteHandler );

	//MESSAGE
	mDum.setClientPagerMessageHandler( mClientMessageHandler );
	mDum.setServerPagerMessageHandler( mServerMessageHandler );

	//SUBSCRIBE
	//#pragma message (REMIND(eventType 值应该怎么填？))，这里统一处理为presence，
	// 在DialogUsageManager::getServerSubscriptionHandler函数中进行查询
	mDum.addClientSubscriptionHandler(resip::Data("presence"), mSubscriptionHandler);
	mDum.addServerSubscriptionHandler(resip::Data("presence"), mSubscriptionHandler);

	//Notify
	mDum.addOutOfDialogHandler(resip::NOTIFY, mNotifyHandler);

#ifdef AUTO_SIPTESTING
    //all sipmessage
    mDum.setSipMessageCallBack( SipMsgCallBack );
#endif //AUTO_SIPTESTING

	mStack.registerTransactionUser(mDum);
	mStackThread.run();
	mDumThread.run();
	__DUM_CATCH
}

bool CSipGateway::GetTargetAddr( const std::string& code, resip::Uri& targetAddr )
{
	std::string userCode;
	if( GetMatchedUser( code, userCode ) )
	{
		std::string uriStr = "sip:" + userCode +  "@" + userCode.substr(0, 10);
		resip::Uri uri = resip::Uri( uriStr.c_str() );
		resip::ContactList contacts;
		UInt64 now = resip::Timer::getTimeSecs();

		m_regData->lockRecord( uri );

		resip::Uri uri1(userCode.c_str());
		m_regData->getContacts(uri1, contacts );
		resip::ContactList::iterator it = contacts.begin();
		resip::ContactList::iterator end = contacts.end();
		while( it != end )
		{
			if( it->mRegExpires > now )
			{
				targetAddr = it->mContact.uri();
				break;
			}
			else
			{
				m_regData->removeContact( uri, *it );
				++it;
			}
		}

		m_regData->unlockRecord( uri );
		return( it != end );
	}
	else
	{
		return( mOutboundProxyContainer.isMatchedServerCode( code.c_str(), targetAddr ) );
	}
}

bool CSipGateway::CheckUserRegExpires( const std::string &user )
{
	std::string uriStr = "sip:" + user +  "@" + user.substr(0, 10);
	resip::Uri uri = resip::Uri( uriStr.c_str() );
	resip::ContactList contacts;
	UInt64 now = resip::Timer::getTimeSecs();

	m_regData->lockRecord( uri );

	resip::Uri useruri(user.c_str());
	m_regData->getContacts(useruri, contacts );
	resip::ContactList::iterator it = contacts.begin();
	resip::ContactList::iterator end = contacts.end();
	for( /*it*/; it != end; ++it )
	{
		if( it->mRegExpires <= now )
		{
			m_regData->removeContact( uri, *it );
		}
	}

	m_regData->unlockRecord( uri );
	return( contacts.empty() );
}

#ifdef AUTO_SIPTESTING
#include "../Message/MsgFactory.hpp"
#include "../npsipgateway/include/SipTesting/STMgr.h"
// #include "../npsipgateway/include/SipTesting/STDevMgr.h"
void CSipGateway::SipMsgCallBack( const resip::SipMessage &msg, bool isRecv )
{
	const std::string from = msg.header(resip::h_From).uri().user().c_str();
	const std::string to = msg.header(resip::h_To).uri().user().c_str();	
	const std::string callid = msg.header(resip::h_CallId).value().c_str();	
	if (!STMgr::getInstance().isTestingID(from) && !STMgr::getInstance().isTestingID(to))
	{
		return;
	}

	std::string resaon = "";    
	CGBMessage *pMsg = NULL;

	switch (msg.header(resip::h_CSeq).method())
	{
	case REGISTER:
		if (msg.isRequest() && msg.exists(resip::h_Expires))
		{
			unsigned int expires = msg.header(resip::h_Expires).value();
			if (0 == expires)
			{
				pMsg = CGBMsgFactory::CreateRegisterRemove( &msg, resaon );
			} 
			else
			{
				pMsg = CGBMsgFactory::CreateRegisterAdd( &msg, resaon );
			}
		}
		else if (msg.isResponse())
		{
			pMsg = CGBMsgFactory::CreateRegisterResponse( &msg, resaon );
		}
		break;
	case INVITE:
		if (msg.isRequest())
		{
			pMsg = CGBMsgFactory::CreateInvite( &msg, resaon );
		}
		else if (msg.isResponse())
		{
			pMsg = CGBMsgFactory::CreateInviteResponse( &msg, resaon );
		}
		break;
	case BYE:
		if (msg.isRequest())
		{
			pMsg = CGBMsgFactory::CreateBye( &msg, resaon );
		}
		else if (msg.isResponse())
		{
		}
		break;
	case ACK:
		if (msg.isRequest())
		{
			pMsg = CGBMsgFactory::CreateAck( &msg, resaon );
		}
		else if (msg.isResponse())
		{
			pMsg = CGBMsgFactory::CreateAck( &msg, resaon );
		}
		break;
	case CANCEL:
		break;
	case INFO:
		if (msg.isRequest())
		{
			pMsg = CGBMsgFactory::CreateCallInfo( &msg, resaon );
		}
		else if (msg.isResponse())
		{
			pMsg = CGBMsgFactory::CreateCallInfoResponse( &msg, resaon );
		}
		break;
	case MESSAGE:
		if (msg.isRequest())
		{
			pMsg = CGBMsgFactory::CreateMessage( &msg, resaon );           
		}
		else if (msg.isResponse())		
		{
			pMsg = CGBMsgFactory::CreateMessageResponse( &msg, resaon );
		}
		break;
	case UPDATE:
		break;
	case REFER:
		break;
	case SUBSCRIBE:
		break;
	case NOTIFY:
		break;
	default:
		break;
	}

	std::stringstream msgStr;
	msgStr<<msg;

	std::stringstream ss;
	ss << "track msg, send=" << !isRecv << ",callid=" << callid << ",msg=" << msgStr.str() << ",method=" <<msg.header(resip::h_CSeq).method() <<",msg=" << pMsg << ",reason=" << resaon;

	INFO_LOG(("%s", ss.str()));
	STMgr::getInstance().trackMsg( !isRecv, callid, msgStr.str(), msg.header(resip::h_CSeq).method(), pMsg, resaon, 0);
}
#endif //AUTO_SIPTESTING




