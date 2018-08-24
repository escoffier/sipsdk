#ifndef MSG_KEEPALIVE_HPP_
#define MSG_KEEPALIVE_HPP_
#include "MessageMsg.hpp"

class CKeepAliveMsg : public CMessageMsgT< CMessageMsg::MsgCmdNotify >
{
public:
	CKeepAliveMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline bool IsOK() const
	{
		return m_status == CGBMessage::RESULT_OK;
	}

	inline bool HasError() const
	{
		return m_status == CGBMessage::RESULT_ERROR;
	}

	inline CGBMessage::EGBResultType GetStatus() const
	{
		return m_status;
	}

	inline void SetStatus( CGBMessage::EGBResultType status )
	{
		m_status = status;
	}

	inline void SetOK()
	{
		m_status = CGBMessage::RESULT_OK;
	}

	inline void SetError()
	{
		m_status = CGBMessage::RESULT_ERROR;
	}

	inline const std::list< std::string >& GetDevices() const
	{
		return m_devices;
	}

	inline void AddDevice( const std::string &id )
	{
		m_devices.push_back( id );
	}

private:
	EGBResultType			 m_status;
	std::list< std::string > m_devices; // 故障设备列表
};

#endif // GBMESSAGE_HPP_