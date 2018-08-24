#ifndef NPGATEWAY_MSG_HPP_
#define NPGATEWAY_MSG_HPP_

#include <string>
#include "Message.hpp"

//////////////////////////////////////////////////////////////////////////
class COfflineMsg : public CMessage
{
public:
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetID( const std::string &id )
	{
		m_id = id;
	}

	inline const std::string& GetID() const
	{
		return m_id;
	}

private:
	std::string m_id;
};

//////////////////////////////////////////////////////////////////////////
class CPlayOverMsg : public CMessage
{
public:
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetSession( unsigned int session )
	{
		m_session = session;
	}

	inline unsigned int GetSession() const
	{
		return m_session;
	}

private:
	unsigned int m_session;
};

//////////////////////////////////////////////////////////////////////////
class CPlayCloseMsg : public CMessage
{
public:
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetSession( unsigned int session )
	{
		m_session = session;
	}

	inline unsigned int GetSession() const
	{
		return m_session;
	}

private:
	unsigned int m_session;
};

//////////////////////////////////////////////////////////////////////////
class CMediaServerInfo;
class CMediaReconnectMsg : public CMessage
{
public:
	CMediaReconnectMsg() : m_pMedia( 0 )
	{
	}

public:
	inline CMediaServerInfo* GetMediaInfo() 
	{
		return m_pMedia;
	}

	inline const CMediaServerInfo* GetMediaInfo() const
	{
		return m_pMedia;
	}

	inline void SetMediaInfo( CMediaServerInfo *media )
	{
		m_pMedia = media;
	}

public:
	virtual void Process( CSipInterface *pInterface );

private:
	CMediaServerInfo* m_pMedia;
};
//////////////////////////////////////////////////////////////////////////
#endif // MESSAGE_HPP_
