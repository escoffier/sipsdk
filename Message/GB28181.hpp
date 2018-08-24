#ifndef GBMSG_GB28181_CREATER_HPP_
#define GBMSG_GB28181_CREATER_HPP_
#include "MsgCreater.hpp"

template<>
class CGBMsgCreaterT< VER_GB28181 > : public CGBMsgCreater
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
public:
	virtual CGBMessage* CreateInvite( const char *body, const bool bDecoder, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateInviteResponse( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateAck( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateBye( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateCallInfo( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateCallInfoResponse( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateCallMessage( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateCallMessageResponse( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateMessage( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateMessageResponse( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateNotify( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateNotifyResponse( const char *body, std::string &resaon )
	{
		return NULL;
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
		return NULL;
	}

	virtual CGBMessage* CreateRegisterResponse( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateRegisterRefresh( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateRegisterRemove( const char *body, std::string &resaon )
	{
		return NULL;
	}

	virtual CGBMessage* CreateRegisterRemoveAll( const char *body, std::string &resaon )
	{
		return NULL;
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
		return NULL;
	}

	virtual CGBMessage* CreateServerNotifySubscription( const char *body, std::string &resaon )
	{
		return NULL;
	}
};

#endif // GBMSG_GB28181_CREATER_HPP_
