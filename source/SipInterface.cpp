#include <sstream>
#include <stdexcept>

#include "SipInterface.h"
//#include "repro/ReproUtils.hxx"
#include "rutil/FdPoll.hxx"
#include "rutil/MD5Stream.hxx"
#include "rutil/ParseException.hxx"
#include "resip/stack/Headers.hxx"
#include "resip/dum/ServerOutOfDialogReq.hxx"
#include "resip/dum/UserAuthInfo.hxx"

#include "RegisterHandler.h"
#include "MessageHandler.h"
#include "InviteHandler.h"
#include "SubscriptionHandler.h"
#include "NotifyHandler.h"
#include "RegisterHandler.h"

#include "WorkThreads.h"
#include "MessageQueue.hpp"
#include "../Message/MsgFactory.hpp"
#include "../Message/InviteMsg.hpp"
#include "../Message/MessageMsg.hpp"
#include "../Message/CatalogNotifyMsg.hpp"
#include "../Message/RegisterMsg.hpp"
#include "../Message/SubscriptionMsg.hpp"
#include "../Message/CallInfoMsg.hpp"
#include "../Message/CallMessageMsg.hpp"

//void TimerCallback( void* para,  NPTimerID id )
//{
//	CSipInterface *pInterface = (CSipInterface*)( para );
//	if( pInterface != NULL )
//	{
//		pInterface->OnTimer();
//	}
//}


using namespace resip;

#if !defined(WIN32)
static resip::FdPollGrp* s_PollGrp = FdPollGrp::create("epoll");
#endif

CSipInterface::CSipInterface()
: m_regtime( 0 )
, m_threads( 0 )
, m_loglevel( 0 )
, m_sdkloglevel( resip::Log::None)
#if !defined(WIN32)
, mEventIntr(new resip::EventThreadInterruptor(*s_PollGrp))
, mStack(0, DnsStub::EmptyNameserverList, mEventIntr, false, 0, 0, s_PollGrp)
#endif
, mDum(mStack)
, mSProfile(new resip::MasterProfile)
#if !defined(WIN32)
, mStackThread(mStack, *mEventIntr, *s_PollGrp)
#else
, mStackThread(mStack)
#endif
, mDumThread(mDum)
//,m_KeepAlive()
{
#if !defined(WIN32)
    mStack.setFallbackPostNotify(mEventIntr);
#endif
	resip::Log::initialize(resip::Log::File, resip::Log::None, "NPSIPAPI", "npsipsdk.log");
	mAClientAuthManager = new CNPClientAuthManager( this );
	mClientRegHandler = new ClientRegHandler( this );
	mServerRegHandler = new ServerRegHandler( this );
	mClientMessageHandler = new ClientMessageHandler( this );
	mServerMessageHandler = new ServerMessageHandler( this );
	mInviteHandler = new InviteHandler( this );
	mNotifyHandler = new CNotifyHandler( this );
	mSubscriptionHandler = new CSubscriptionHandler( this );
}

CSipInterface::~CSipInterface()
{
    __DUM_TRY
    mStackThread.shutdown();
    mStackThread.join();
    mDumThread.shutdown();
    mDumThread.join();
    __DUM_CATCH

	delete mClientRegHandler;
	delete mServerRegHandler;
	delete mClientMessageHandler;
	delete mServerMessageHandler;
	delete mInviteHandler;
	delete mNotifyHandler;
	delete mSubscriptionHandler;
}

// 在Helper.cxx中定义
extern void SetLocalIp(const char* localIp);
void CSipInterface::SetSipLocalInfo( const std::string &ip, int port, const std::string &id )
{
	//SetLocalIp( ip.c_str() );
	m_sLocalIp = ip;
	m_iListenPort = port;
	m_sUserName = id;

	resip::NameAddr naFrom;
	naFrom.uri().user() = id.c_str();
	naFrom.uri().host() = ip.c_str();
	naFrom.uri().port() = port;
	mSProfile->setDefaultFrom( naFrom );
}

void CSipInterface::SetSipLocalIp( const std::string &ip )
{
    //SetLocalIp( ip.c_str() );
    m_sLocalIp = ip;
}

void CSipInterface::SetSipLocalPort( int port )
{
    m_iListenPort = port;
}

void CSipInterface::SetRegisterUserName( const std::string &id )
{
	m_sUserName = id;

	resip::NameAddr naFrom;
	naFrom.uri().user() = id.c_str();
	naFrom.uri().host() = m_sLocalIp.c_str();
	naFrom.uri().port() = m_iListenPort;
	mSProfile->setDefaultFrom( naFrom );
}

void CSipInterface::RequestUserAuthInfo( const char* user, const char* realm, const char* transactionId )
{
	resip::UserAuthInfo* msg = 0;
	std::string a1Str;
	if( GetUserAuthInfo( user, realm, a1Str ) )
	{
		msg = new UserAuthInfo( user, realm, (char*)a1Str.c_str(), transactionId );
	}
	else
	{
		msg = new UserAuthInfo( user, realm, resip::UserAuthInfo::UserUnknown, transactionId );
	}

	__DUM_TRY
		mDum.post(msg);
	__DUM_CATCH
}

CSipInterface::ECatalogType CSipInterface::GetIDType( const std::string &id )
{
	size_t len = id.length();
	if( len > 10 )
	{
		std::string str = id.substr( 10, 3 );
		if( str == "215" )
		{
			return CATALOG_BUSINESS; 
		}
		else if( str == "216" )
		{
			return CATALOG_ORGANIZATION;
		}
		else if( str == "200" )
		{
			return CATALOG_SYSTEM;
		}
		else
		{
			return CATALOG_DEVICE;
		}		
	}
	else
	{
		return CATALOG_CIVIL;
	}
}

std::string CSipInterface::ConvertIntoMD5password(const char* userName, const char* realm, const char* password)
{
	MD5Stream a1;
	a1 << Data(userName) << Symbols::COLON << Data(realm) << Symbols::COLON << Data(password);
	a1.flush();
	return a1.getHex().c_str();
}

void CSipInterface::RunSipStack()
{
}

void CSipInterface::SendMessage( CGBMessage *pMsg, bool tcp )
{
	if( pMsg != NULL )
	{
		std::string to = pMsg->GetUser();
        if( to.empty() )
        {
            to = pMsg->GetTo();
        }
      
		resip::Uri uri;
		if( GetTargetAddr( to, uri ) )
		{			    
			pMsg->SetFrom( m_sUserName );
			pMsg->SetIP( uri.host().c_str() );
			pMsg->SetPort( uri.port() );
			pMsg->SetUser( to );
			pMsg->Send( mDum, tcp );
		}
		else
		{
			DEBUG_LOG( pMsg->GetTo() << "没有上线!" );
		}
	}
}

void CSipInterface::SetLogPath( std::string &path )
{
    whl::log::set_log_path( path );
}

void CSipInterface::SetLogLevel( int level )
{
	whl::log::set_log_level( level );
}

void CSipInterface::SetSdkLogLevel(int level)
{
	switch( level )
	{
	case resip::Log::None:	
		{
			DEBUG_LOG( "协议栈日志级别切换为:None" );
			resip::Log::setLevel( resip::Log::None );
			break;
		}

	//case resip::Log::None:
	//	{
	//		DEBUG_LOG( "协议栈日志级别切换为:No_Use" );
	//		resip::Log::setLevel( resip::Log::None);
	//		break;
	//	}

	case resip::Log::Crit:
		{
			DEBUG_LOG( "协议栈日志级别切换为:Crit" );
			resip::Log::setLevel( resip::Log::Crit ); 
			break;
		}

	case resip::Log::Err:
		{
			DEBUG_LOG( "协议栈日志级别切换为:Err" );
			resip::Log::setLevel( resip::Log::Err ); 
			break;
		}

	case resip::Log::Warning:
		{
			DEBUG_LOG( "协议栈日志级别切换为:Warning" );
			resip::Log::setLevel( resip::Log::Warning );
			break;
		}

	case resip::Log::Info:
		{
			DEBUG_LOG( "协议栈日志级别切换为:Info" );
			resip::Log::setLevel( resip::Log::Info ); 
			break;
		}

	case resip::Log::Debug:
		{
			DEBUG_LOG( "协议栈日志级别切换为:Debug" );
			resip::Log::setLevel( resip::Log::Debug );
			break;
		}

	case resip::Log::Stack:
		{
			DEBUG_LOG( "协议栈日志级别切换为:Stack" );
			resip::Log::setLevel( resip::Log::Stack );
			break;
		}

	case resip::Log::StdErr:
		{
			DEBUG_LOG( "协议栈日志级别切换为:StdErr" );
			resip::Log::setLevel( resip::Log::StdErr );
			break;
		}

	case resip::Log::Bogus:
		{
			DEBUG_LOG( "协议栈日志级别切换为:Bogus" );
			resip::Log::setLevel( resip::Log::Bogus ); 
			break;
		}

	default:
		{
			DEBUG_LOG( "协议栈日志级别错误，设置为默认级别No_Use" );
			resip::Log::setLevel( resip::Log::None);
			break;
		}
	}
}

void CSipInterface::Start()
{
	// 初始化日志
// 	SetLogLevel( m_loglevel );
	SetSdkLogLevel( m_sdkloglevel );

	// 启动协议栈
	RunSipStack();

	// Start Timer
	//m_KeepAlive.Start( TimerCallback, this, 1*1000 );

	// 启动处理线程池
	if( m_threads <= 0 )
	{
		m_threads = 1;
	}

	CWorkThreads::Start( this, m_threads );
}

void CSipInterface::Stop()
{
	//m_KeepAlive.Stop( TRUE );
	CWorkThreads::Stop();
}

void CSipInterface::SetProxy()
{
    //设置出口，未设置目的地时协议栈默认使用此值?
//     if (m_OutboundProxyIp.size() != 0 && m_OutboundProxyPort > 0)
// 	{
//         resip::Uri outbound;
//         outbound.host() = resip::Data(m_OutboundProxyIp);
//         outbound.port() = m_OutboundProxyPort;
//         mSProfile->setOutboundProxy(outbound);
//     }
}

void CSipInterface::SetDefaultFrom()
{
}

void CSipInterface::SetUserAgent()
{
	if (!m_UserAgent.empty())
		mSProfile->setUserAgent(resip::Data(m_UserAgent));
	else
		mSProfile->setUserAgent("NetPosa");
}

void CSipInterface::SetRegTime(int regtime)
{
	//注册失败后的默认注册时间间隔
	m_regtime = regtime;
}

void CSipInterface::OnRequetAuth( const char *realm )
{
	mDum.getMasterProfile()->setDigestCredential( realm, m_sUserName.c_str(), m_sPassword.c_str() );
}

void CSipInterface::OnInvite( resip::ServerInviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateInvite( msg, resaon );
	if( pMsg != NULL )
	{
		CInviteMsg *p = dynamic_cast< CInviteMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			h->rejectCommand( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );
		}
	}
	else
	{
		h->rejectCommand( 400 );
	}
}

void CSipInterface::OnInviteResponse( resip::ClientInviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateInviteResponse( msg, resaon );
	if( pMsg != NULL )
	{
		CInviteResponseMsg *p = dynamic_cast< CInviteResponseMsg* >( pMsg );
		if( p == 0 )
		{
			delete pMsg;
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );
		}
	}
	else
	{
		h->rejectNITCommand( 400 );
	}
}

void CSipInterface::OnAck( resip::InviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateAck( msg, resaon );
	if( pMsg != NULL )
	{
		CAckMsg *p = dynamic_cast< CAckMsg* >( pMsg );
		if( p == 0 )
		{
			delete pMsg;
			h->rejectNITCommand( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( pMsg );
		}
	}
	else
	{
		h->rejectNITCommand( 400 );
	}
}

void CSipInterface::OnBye( resip::InviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateBye( msg, resaon );
	if( pMsg != NULL )
	{
		CByeMsg *p = dynamic_cast< CByeMsg* >( pMsg );
		if( p == 0 )
		{
			delete pMsg;
			h->rejectNITCommand( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( pMsg );
		}
	}
	else
	{
		h->rejectNITCommand( 400 );
	}
}

void CSipInterface::OnCallInfo( resip::InviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateCallInfo( msg, resaon );
	if( pMsg != NULL )
	{
		CCallInfoMsg *p = dynamic_cast< CCallInfoMsg* >( pMsg );
		if( p == 0 )
		{
			delete pMsg;
			h->rejectNITCommand( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( pMsg );

			// response 200
			h->acceptNITCommand( 200 );
		}
	}
	else
	{
		h->rejectNITCommand( 400 );
	}
}


void CSipInterface::OnCallInfoResponse( resip::InviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateCallInfoResponse( msg, resaon );
	if( pMsg != NULL )
	{
		CCallInfoResponseMsg *p = dynamic_cast< CCallInfoResponseMsg* >( pMsg );
		if( p == 0 )
		{
			delete pMsg;
		}
		else
		{
			CMessageQueue::AddMessage( pMsg );
		}
	}
}

void CSipInterface::OnCallMessage( resip::InviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateCallMessage( msg, resaon );
	if( pMsg != NULL )
	{
		CCallMessageMsg *p = dynamic_cast< CCallMessageMsg* >( pMsg );
		if( p == 0 )
		{
			delete pMsg;
			h->rejectNITCommand( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( pMsg );
		}
	}
	else
	{
		h->rejectNITCommand( 400 );
	}
}

void CSipInterface::OnCallMessageResponse( resip::InviteSession *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateCallMessageResponse( msg, resaon );
	if( pMsg != NULL )
	{
		CCallMessageResponseMsg *p = dynamic_cast< CCallMessageResponseMsg* >( pMsg );
		if( p == 0 )
		{
			delete pMsg;
		}
		else
		{
			CMessageQueue::AddMessage( pMsg );
		}
	}
}

void CSipInterface::OnMessage( resip::ServerPagerMessage *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateMessage( msg, resaon );
	if( pMsg != NULL )
	{
		CMessageMsg *p = dynamic_cast< CMessageMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
			h->send( msg );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );

			// 自动回应200消息，如果某些特定消息不能自动回应，需要重载IsAutoResponse返回false
			if( p->IsAutoResponse() )
			{
				resip::SharedPtr< resip::SipMessage > msg = h->accept();
				h->send( msg );
			}
		}
	}
	else
	{
		resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
		h->send( msg );
	}
}

void CSipInterface::OnMessageResponse( resip::ClientPagerMessage *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateMessageResponse( msg, resaon );
	if( pMsg != NULL )
	{
		CMessageResponseMsg *p = dynamic_cast< CMessageResponseMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
		}
		else
		{
			CMessageQueue::AddMessage( p );
		}
	}
}

void CSipInterface::OnNotify( resip::ServerOutOfDialogReq *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateNotify( msg, resaon );
	if( pMsg != NULL )
	{
		CCatalogNotifyMsg *p = dynamic_cast< CCatalogNotifyMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
			h->send( msg );
		}
		else
		{
            p->SetHandle( h );
            resip::SharedPtr< resip::SipMessage > msg = h->accept();
            h->send( msg );
			CMessageQueue::AddMessage( p );
		}
	}
	else
	{
		resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
		h->send( msg );
	}
}

void CSipInterface::OnNotifyResponse( resip::ClientOutOfDialogReq *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateNotifyResponse( msg, resaon );
	if( pMsg != NULL )
	{
		CNotifyResponseMsg *p = dynamic_cast< CNotifyResponseMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
		}
		else
		{
			CMessageQueue::AddMessage( p );
		}
	}
}

void CSipInterface::OnRegisterResponse( resip::ClientRegistration *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateRegisterResponse( msg, resaon );
	if( pMsg != NULL )
	{
		CRegisterResponseMsg *p = dynamic_cast< CRegisterResponseMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
		}
		else
		{
			//resip::SharedPtr< resip::SipMessage > request = h->mLastRequest;
			//if( request != NULL )
			//{
			//	p->SetParentID( request->header( resip::h_RequestLine ).uri().user().c_str() );
			//}

			CMessageQueue::AddMessage( p );
		}
	}
}

void CSipInterface::OnRegister( resip::ServerRegistration *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateRegisterAdd( msg, resaon );
	if( pMsg != NULL )
	{
		CRegisterAddMsg *p = dynamic_cast< CRegisterAddMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			h->reject( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );

			// 回应消息
			SipMessage success;
			Helper::makeResponse(success, *msg, 200);

			if( !msg->empty( h_Expires ) )
			{
				success.header(h_Expires) = msg->header(h_Expires);
			}

			// 需要校时
			if( NeedAdjustTime( p->GetTo() ) )
			{
				time_t rawTime;
				time( &rawTime );
				struct tm *localTime = localtime(&rawTime);				
				success.header(h_Date).year() = localTime->tm_year + 1900;
				success.header(h_Date).month() = static_cast<resip::Month>( localTime->tm_mon + 1);
				success.header(h_Date).dayOfMonth() = localTime->tm_mday;
				success.header(h_Date).hour() = localTime->tm_hour;
				success.header(h_Date).minute() = localTime->tm_min;
				success.header(h_Date).second() = localTime->tm_sec;
			}

			h->accept(success);
		}
	}
	else
	{
		h->reject( 400 );
	}
}

void CSipInterface::OnRefresh( resip::ServerRegistration *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateRegisterRefresh( msg, resaon );
	if( pMsg != NULL )
	{
		CRegisterRefreshMsg *p = dynamic_cast< CRegisterRefreshMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			h->reject( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );

			SipMessage success;
			Helper::makeResponse(success, *msg, 200);
			if( NeedAdjustTime( p->GetTo() ) )
			{
				time_t rawTime;
				time( &rawTime );
				struct tm *localTime = localtime(&rawTime);	
				success.header(h_Date).year() = localTime->tm_year + 1900;
				success.header(h_Date).month() = static_cast<resip::Month>( localTime->tm_mon + 1);
				success.header(h_Date).dayOfMonth() = localTime->tm_mday;
				success.header(h_Date).hour() = localTime->tm_hour;
				success.header(h_Date).minute() = localTime->tm_min;
				success.header(h_Date).second() = localTime->tm_sec;
			}

			h->accept(success);
		}
	}
	else
	{
		h->reject( 400 );
	}
}

void CSipInterface::OnRegisterRemove( resip::ServerRegistration *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateRegisterRemove( msg, resaon );
	if( pMsg != NULL )
	{
		CRegisterRemoveMsg *p = dynamic_cast< CRegisterRemoveMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			h->reject( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );
		}
	}
	else
	{
		h->reject( 400 );
	}
}

void CSipInterface::OnRegisterRemoveAll( resip::ServerRegistration *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateRegisterRemoveAll( msg, resaon );
	if( pMsg != NULL )
	{
		CRegisterRemoveAllMsg *p = dynamic_cast< CRegisterRemoveAllMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			h->reject( 400 );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );
		}
	}
	else
	{
		h->reject( 400 );
	}
}

void CSipInterface::OnUpdateSubscription( resip::ClientSubscription *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateNotify( msg, resaon );
	if( pMsg != NULL )
	{
		CCatalogNotifyMsg *p = dynamic_cast< CCatalogNotifyMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );
		}
	}
}

void CSipInterface::OnCancelSubscription( resip::ClientSubscription *h, const resip::SipMessage *msg )
{
}

void CSipInterface::OnAddSubscription( resip::ClientSubscription *h, const resip::SipMessage *msg )
{
}

void CSipInterface::OnCancelSubscription( resip::ServerSubscription *h )
{
	CCancelSubscribeMsg *p = new CCancelSubscribeMsg;
	p->SetHandle( h );
	CMessageQueue::AddMessage( p );
}

void CSipInterface::OnAddSubscription( resip::ServerSubscription *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateServerAddSubscription( msg, resaon );
	if( pMsg != NULL )
	{
		CAddSubscribeMsg *p = dynamic_cast< CAddSubscribeMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
			h->send( msg );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );
			resip::SharedPtr< resip::SipMessage > msg = h->accept( 200 );
			h->send( msg );
		}
	}
	else
	{
		resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
		h->send( msg );
	}
}

void CSipInterface::OnSubscriptionResponse( resip::ServerSubscription *h, const resip::SipMessage *msg )
{
    std::string resaon = "";
	CGBMessage *pMsg = CGBMsgFactory::CreateNotifyResponse( msg, resaon );
	if( pMsg != NULL )
	{
		CNotifyResponseMsg *p = dynamic_cast< CNotifyResponseMsg* >( pMsg );
		if( p == NULL )
		{
			delete pMsg;
			resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
			h->send( msg );
		}
		else
		{
			p->SetHandle( h );
			CMessageQueue::AddMessage( p );
		}
	}
	else
	{
		resip::SharedPtr< resip::SipMessage > msg = h->reject( 400 );
		h->send( msg );
	}
}

void CSipInterface::SetThreads( int threads )
{
	m_threads = threads;
}

bool CSipInterface::SetLocalTime( int year, int month, int day, int hour, int minute, int second )
{
#ifdef WIN32
	SYSTEMTIME tm;
	tm.wYear = (WORD)year;
	tm.wMonth = (WORD)month;
	tm.wDay = (WORD)day;
	tm.wHour = (WORD)hour;
	tm.wMinute = (WORD)minute;
	tm.wSecond = (WORD)second;
	tm.wDayOfWeek = (WORD)0;
	tm.wMilliseconds = (WORD)0;
	return ( ::SetLocalTime( &tm ) ? true: false );
#else
	char buf[64] = {0};
	sprintf( buf, "date -s \"%04d-%02d-%02d %02d:%02d:%02d\"", year, month, day, hour, minute, second );
	
	system( buf );
#endif
}

//void CSipInterface::RegisterToParent( const std::string parentid )
//{
//
//}

