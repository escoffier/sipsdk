#ifndef MSG_MESSAGE_MSG_HPP_
#define MSG_MESSAGE_MSG_HPP_

#include "GBMessage.hpp"

//////////////////////////////////////////////////////////////////////////
class CMessageMsg : public CGBMessage
{
public:
	enum EMsgCmdType
	{
		MsgCmdQuery,
		MsgCmdControl,
		MsgCmdResponse,
		MsgCmdNotify,
		MsgCmdResult,
	};

public:
	CMessageMsg( ) : m_handle( NULL )
	{
		m_cmdtype = "";
		m_sn = "";
		m_deviceid = "";
	}

public:
	virtual bool IsAutoResponse() const { return true; }
	virtual bool IsQuery() const       { return false; }
	virtual bool IsControl() const     { return false; }
	virtual bool IsResponse() const     { return false; }
	virtual bool IsNotify() const      { return false; }
	virtual bool IsResult() const      { return false; }

public:
	inline const std::string& GetCmdType() const
	{
		return m_cmdtype;
	}

	inline void SetCmdType( const std::string &cmdtype )
	{
		m_cmdtype = cmdtype;
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

	inline void SetHandle( resip::ServerPagerMessage *h )
	{
		m_handle = h;
	}

	inline resip::ServerPagerMessage *GetHandle() const
	{
		return m_handle;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );

protected:
	resip::ServerPagerMessage* m_handle;
	std::string                m_cmdtype;
	std::string                m_sn;
	std::string                m_deviceid;
};

//////////////////////////////////////////////////////////////////////////
class CMessageResponseMsg : public CGBMessage
{
public:
	CMessageResponseMsg() : m_handle( NULL )
	{
	}

public:
	inline const resip::ServerPagerMessage* GetHandle() const
	{
		return m_handle;
	}

	//inline resip::ServerPagerMessage* GetHandle()
	//{
	//	return m_handle;
	//}

	inline void SetHandle( resip::ServerPagerMessage *h )
	{
		m_handle = h;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
    virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	resip::ServerPagerMessage* m_handle;
};

//////////////////////////////////////////////////////////////////////////
// Message消息体分类处理
// 消息体采用模板及模板特化来定义，是为了避免Message的消息体与其他的消息体格式重名的问题
template< CMessageMsg::EMsgCmdType T > class CMessageMsgT;

//////////////////////////////////////////////////////////////////////////
template<>
class CMessageMsgT< CMessageMsg::MsgCmdResponse > : public CMessageMsg
{
public:
	CMessageMsgT()
	{
	}

public:
	virtual bool IsResponse() const 
	{ 
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class CMessageMsgT< CMessageMsg::MsgCmdQuery > : public CMessageMsg
{
public:
	CMessageMsgT()
	{
	}

public:
	virtual bool IsQuery() const 
	{ 
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class CMessageMsgT< CMessageMsg::MsgCmdControl > : public CMessageMsg
{
public:
	CMessageMsgT()
	{
	}

public:
	virtual bool IsControl() const
	{ 
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class CMessageMsgT< CMessageMsg::MsgCmdResult > : public CMessageMsg
{
public:
	CMessageMsgT()
	{
	}

public:
	virtual bool IsResult() const 
	{ 
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////
template<>
class CMessageMsgT< CMessageMsg::MsgCmdNotify > : public CMessageMsg
{
public:
	CMessageMsgT()
	{
	}

public:
	virtual bool IsNotify() const 
	{ 
		return true;
	}
};

#endif // MSG_MESSAGE_MSG_HPP_
