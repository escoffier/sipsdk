#ifndef MSG_SUBSCRIPTION_MSG_HPP_
#define MSG_SUBSCRIPTION_MSG_HPP_

#include <vector>
#include "tinyxml2.h"
#include "GBMessage.hpp"
#include "resip/dum/ServerSubscription.hxx"
#include "resip/dum/ClientSubscription.hxx"

//////////////////////////////////////////////////////////////////////////
class CSubscribeMsg : public CGBMessage
{
public:
	CSubscribeMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::BaseSubscription* GetHandle()
	{
		return m_handle;
	}

	inline const resip::BaseSubscription* GetHandle() const
	{
		return m_handle;
	}

	inline void SetHandle( resip::BaseSubscription *h )
	{
		m_handle = h;
	}

	inline const std::string& GetSN() const
	{
		return m_sn;
	}

	inline const std::string& GetDeviceID() const
	{
		return m_deviceid;
	}

	inline void SetDeviceID( const std::string &deviceid )
	{
		m_deviceid = deviceid;
	}

	inline void SetSN( const std::string &sn )
	{
		m_sn = sn;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	std::string              m_sn;
	std::string              m_deviceid;
	resip::BaseSubscription* m_handle;
};

//////////////////////////////////////////////////////////////////////////
class CAddSubscribeMsg : public CSubscribeMsg
{
public:
	CAddSubscribeMsg()
	{
	}

	void setMsgProperties(const CAddSubscribeMsg &msg)
	{
		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CCancelSubscribeMsg : public CSubscribeMsg
{
public:
	CCancelSubscribeMsg()
	{
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CSubscribeResponseMsg : public CSubscribeMsg
{
public:
	CSubscribeResponseMsg()
	{
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
#endif // MSG_NOTIFY_MSG_HPP_