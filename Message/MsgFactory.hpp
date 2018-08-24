#ifndef GBMSG_FACTORY_HPP_
#define GBMSG_FACTORY_HPP_

#include "GBMessage.hpp"
#include "MsgCreater.hpp"
#include "resip/stack/SipMessage.hxx"

//////////////////////////////////////////////////////////////////////////
class CGBMsgFactory
{
private:
	enum EGBMsgType
	{
		INVITE,
		INVITE_REPONSE,
		ACK,
		BYE,
		CALLINFO,
		CALLINFO_REPONSE,
		CALLMESSAGE,
		CALLMESSAGE_REPONSE,
		MESSAGE,
		MESSAGE_REPONSE,
		NOTIFY,
		NOTIFY_REPONSE,
		INFO,
		INFO_RESPONSE,
		REGISTER_ADD,
		REGISTER_REPONSE,
		REGISTER_REFRESH,
		REGISTER_REMOVE,
		REGISTER_REMOVE_ALL,
		CLIENT_UPDATE_SUBSCRIPTION,
		CLIENT_CANCEL_SUBSCRIPTION,
		CLIENT_ADD_SUBSCRIPTION,
		SERVER_ADD_SUBSCRIPTION,
		SERVER_RESPONSE_SUBSCRIPTION,
	};

public:
	friend class GBMsgTestCase;

public:
	static CGBMessage* CreateInvite( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateInviteResponse( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateAck( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateBye( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateCallInfo( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateCallInfoResponse( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateCallMessage( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateCallMessageResponse( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateMessage( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateMessageResponse( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateNotify( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateNotifyResponse( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateInfo( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateInfoResponse( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateRegisterAdd( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateRegisterResponse( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateRegisterRefresh( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateRegisterRemove( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateRegisterRemoveAll( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateClientUpdateSubscription( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateClientCancelSubscription( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateClientAddSubscription( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateServerAddSubscription( const resip::SipMessage *message, std::string &resaon );
	static CGBMessage* CreateServerResponseSubscription( const resip::SipMessage *message, std::string &resaon );

public:
	/** 禁用消息版本
	 * 
	 *  @param version [in] 被禁用的消息版本，被禁用之后消息工厂创建该版本的消息都返回NULL
	 */
	static void DisableVersion( int version );

	/** 启用消息版本
	 * 
	 *  @param version [in] 启用消息版本，如果消息版本正确，之后可以使用该版本的消息工厂创建消息
	 */
	static void EnableVersion( int version );

private:
	/** 创建一个消息对象，创建的消息对象由使用者自行销毁，消息工厂不负现
	 * 
	 *  @param msgType  [in] 消息类型
	 *  @param message  [in] Sip消息
	 *  @return 消息创建成功返回创建后的消息指针，否则返回NULL
	 *  @note 消息创建失败的原因如下:
	 *        - 消息的版本号不正确
	 *        - 消息格式不正确
	 *        - 内存空间不足
	 */
	static CGBMessage* Create( EGBMsgType msgType, const resip::SipMessage *message, std::string &resaon );

private:
	CGBMsgFactory();
	~CGBMsgFactory();

private:
	CGBMsgFactory( const CGBMsgFactory &rhs );
	CGBMsgFactory& operator=( const CGBMsgFactory &rhs );

private:
	static CGBMsgFactory& GetInstance()
	{
		static CGBMsgFactory obj;
		return obj;
	}

private:
	CGBMsgCreater* m_creater[VER_MAX_MSG_VERSION];
};

#endif // GBMESSAGE_HPP_