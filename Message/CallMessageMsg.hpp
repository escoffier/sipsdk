#ifndef MSG_CALLMESSAGE_MSG_HPP_
#define MSG_CALLMESSAGE_MSG_HPP_

#include "GBMessage.hpp"
#include "resip/dum/InviteSession.hxx"

//////////////////////////////////////////////////////////////////////////
class CCallMessageMsg : public CGBMessage
{
public:
	CCallMessageMsg() : m_handle( NULL )
	{
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
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

	inline void SetNotifyType( const std::string &type )
	{
		m_type = type;
	}

	inline const std::string& GetNotifyType() const
	{
		return m_type;
	}

	inline resip::InviteSession* GetHandle()
	{
		return m_handle;
	}

	inline const resip::InviteSession* GetHandle() const
	{
		return m_handle;
	}

	inline void SetHandle( uint64_t handle )
	{
		m_handle = (resip::InviteSession*)( handle );
	}

	inline void SetHandle( resip::InviteSession *h )
	{
		m_handle = h;
	}

protected:
	resip::InviteSession* m_handle;
	std::string           m_sn;
	std::string           m_deviceid;
	std::string           m_type;
};

//////////////////////////////////////////////////////////////////////////
class CCallMessageResponseMsg : public CGBMessage
{
public:
	CCallMessageResponseMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::InviteSession* GetHandle()
	{
		return m_handle;
	}

	inline void SetHandle( resip::InviteSession *h )
	{
		m_handle = h;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	resip::InviteSession* m_handle;
};

//////////////////////////////////////////////////////////////////////////
class CStreamPlayEndMsg : public CCallMessageMsg
{
public:
	CStreamPlayEndMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
#endif // MSG_CALLMESSAGE_MSG_HPP_
