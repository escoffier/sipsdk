#ifndef MSG_BROAD_CAST_HPP_
#define MSG_BROAD_CAST_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CBroadcastMsg : public CMessageMsgT< CMessageMsg::MsgCmdNotify >
{
public:
	CBroadcastMsg()
	{
	}

	void setMsgProperties(const CBroadcastMsg &msg)
	{
		m_source = msg.GetSourceID();
		m_target = msg.GetTargetID();

		SetDeviceID(msg.GetDeviceID());
		SetTo(msg.GetTo());
	}


public:
	inline const std::string& GetSourceID() const
	{
		return m_source;
	}

	inline void SetSourceID( const std::string &sid )
	{
		m_source = sid;
	}

	inline const std::string& GetTargetID() const
	{
		return m_target;
	}

	inline void SetTargetID( const std::string &tid )
	{
		m_target = tid;
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

private:
	std::string m_source;
	std::string m_target;
};

//////////////////////////////////////////////////////////////////////////
class CBroadcastResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResult >
{
public:
	CBroadcastResponseMsg()
	{
	}

	void setMsgProperties(const CBroadcastResponseMsg &msg)
	{
		m_result = msg.GetResult();
		SetDeviceID(msg.GetDeviceID());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline bool IsOK() const
	{
		return m_result == CGBMessage::RESULT_OK;
	}

	inline bool HasError() const
	{
		return m_result == CGBMessage::RESULT_ERROR;
	}

	inline CGBMessage::EGBResultType GetResult() const
	{
		return m_result;
	}

	inline void SetResult( CGBMessage::EGBResultType result )
	{
		m_result = result;
	}

	inline void SetOK()
	{
		m_result = CGBMessage::RESULT_OK;
	}

	inline void SetError()
	{
		m_result = CGBMessage::RESULT_ERROR;
	}

private:
	EGBResultType m_result;
};

//////////////////////////////////////////////////////////////////////////
#endif // MSG_BROAD_CAST_HPP_

