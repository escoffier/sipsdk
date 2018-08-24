#ifndef MSG_REGISTER_MSG_HPP_
#define MSG_REGISTER_MSG_HPP_

#include "GBMessage.hpp"
#include "resip/dum/ServerRegistration.hxx"
#include "resip/dum/ClientRegistration.hxx"

//////////////////////////////////////////////////////////////////////////
class CRegisterRequestMsg : public CGBMessage
{
public:
	CRegisterRequestMsg()
	{
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CRegisterMsg : public CGBMessage
{
public:
	CRegisterMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::ServerRegistration* GetHandle()
	{
		return m_handle;
	}

	inline void SetHandle( resip::ServerRegistration *h )
	{
		m_handle = h;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	resip::ServerRegistration* m_handle;
};

//////////////////////////////////////////////////////////////////////////
class CRegisterAddMsg : public CRegisterMsg
{
public:
	CRegisterAddMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CRegisterRefreshMsg : public CRegisterMsg
{
public:
	CRegisterRefreshMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CRegisterRemoveMsg : public CRegisterMsg
{
public:
	CRegisterRemoveMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	std::string m_ip;
	int         m_port;
};

//////////////////////////////////////////////////////////////////////////
class CRegisterRemoveAllMsg : public CRegisterMsg
{
public:
	CRegisterRemoveAllMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	std::string m_ip;
	int         m_port;
};

//////////////////////////////////////////////////////////////////////////
class CRegisterResponseMsg : public CGBMessage
{
public:
	CRegisterResponseMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::ClientRegistration* GetHandle()
	{
		return m_handle;
	}

	inline void SetHandle( resip::ClientRegistration *h )
	{
		m_handle = h;
	}

	inline const std::string& GetParentID() const
	{
		return m_parentid;
	}

	inline void SetParentID( const char *id )
	{
		if( id != NULL )
		{
			m_parentid = id;
		}
	}

	inline void SetParentID( const std::string &id )
	{
		m_parentid = id;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	resip::ClientRegistration* m_handle;
	std::string                m_parentid;
};

//////////////////////////////////////////////////////////////////////////
#endif // MSG_NOTIFY_MSG_HPP_