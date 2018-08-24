#include "SipClient.hpp"
#include "RegisterHandler.h"
#include "MessageHandler.h"
#include "InviteHandler.h"
#include "SubscriptionHandler.h"
#include "NotifyHandler.h"
#include "RegisterHandler.h"
#include "OutboundProxyContainer.h"
#include "RegisterHandler.h"

CSipClient::CSipClient()
{
}

CSipClient::~CSipClient()
{
}

bool CSipClient::GetTargetAddr( const std::string& code, resip::Uri& targetAddr )
{
    return( mOutboundProxyContainer.isMatchedServerCode( m_sSeverid.c_str(), targetAddr ) );
}

void CSipClient::RunSipStack()
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
  
  	//注册失败后继续注册的时间
    mDum.getMasterProfile()->setDefaultRegistrationRetryTime(m_regtime);

    //默认注册时间？
    mDum.getMasterProfile()->setDefaultRegistrationTime(70);

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

  	mStack.registerTransactionUser(mDum);
  	mStackThread.run();
  	mDumThread.run();
  	__DUM_CATCH
}
