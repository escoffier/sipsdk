#ifndef NPCLIENT_MSG_HPP_
#define NPCLIENT_MSG_HPP_

#include <string>
#include "Message.hpp"

//////////////////////////////////////////////////////////////////////////
class CClientOfflineMsg : public CMessage
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
#endif // NPCLIENT_MSG_HPP_
