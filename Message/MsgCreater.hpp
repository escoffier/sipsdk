#ifndef GBMSG_CREATER_HPP_
#define GBMSG_CREATER_HPP_

//////////////////////////////////////////////////////////////////////////
/** 兼容的版本定义
 * 
 *   @note 消息工厂使用的版本使用的数组保存对应的消息版本的，因此枚举值的定义是必须保持顺序
 */
enum EGBMsgVerion
{
	VER_GB28181 = 0,
	VER_GB28181Ex,
	VER_MAX_MSG_VERSION,
};

//////////////////////////////////////////////////////////////////////////
/**
 *  如果有新版本的消息，需要特化版本的CGBMsgCreaterT，并且继承CGBMsgCreate
 *  例如：
 *  CGBMsgCreateT< VER_GB28181 > : public CGBMsgCreate;
 *  CGBMsgCreateT< VER_GB28181Ex > : public CGBMsgCreateT< VER_GB28181 >;
 */
template< EGBMsgVerion T > class CGBMsgCreaterT;

//////////////////////////////////////////////////////////////////////////
class CGBMsgCreater
{
public:
	friend class CGBMsgFactory;
	friend class GBMsgTestCase;

protected:
	CGBMsgCreater()
	{
	}

	virtual ~CGBMsgCreater()
	{
	}

private:
	CGBMsgCreater( const CGBMsgCreater &rhs );
	CGBMsgCreater& operator=( const CGBMsgCreater &rhs );

public:
	virtual CGBMessage* CreateInvite( const char *body, const bool bDecoder, std::string &resaon  ) = 0;
	virtual CGBMessage* CreateInviteResponse( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateAck( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateBye( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateCallInfo( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateCallInfoResponse( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateCallMessage( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateCallMessageResponse( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateMessage( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateMessageResponse( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateNotify( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateNotifyResponse( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateInfo( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateInfoResponse( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateRegisterAdd( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateRegisterResponse( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateRegisterRefresh( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateRegisterRemove( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateRegisterRemoveAll( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateClientUpdateSubscription( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateClientCancelSubscription( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateClientAddSubscription( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateServerAddSubscription( const char *body, std::string &resaon ) = 0;
	virtual CGBMessage* CreateServerNotifySubscription( const char *body, std::string &resaon ) = 0;
};

//////////////////////////////////////////////////////////////////////////
#endif // GBMSG_CREATER_HPP_
