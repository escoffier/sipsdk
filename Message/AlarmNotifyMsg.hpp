#ifndef MSG_ALARM_NOTIFY_HPP_
#define MSG_ALARM_NOTIFY_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CAlarmNotifyMsg : public CMessageMsgT< CMessageMsg::MsgCmdNotify >
{
public:
	CAlarmNotifyMsg()
	{
		m_sum = 0;
	}

	void setMsgProperties(const CAlarmNotifyMsg &msg)
	{
		m_priority = msg.GetPriority();
		m_method = msg.GetMethod();
		m_time = msg.GetTime();
		m_description = msg.GetDescription();
		m_longitude = msg.GetLongitude();
		m_latitude = msg.GetLatitude();
		m_sum = msg.GetSum();

		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetPriority() const
	{
		return m_priority;
	}

	inline const std::string& GetMethod() const
	{
		return m_method;
	}

	inline const std::string& GetTime() const
	{
		return m_time;
	}

	inline const std::string& GetDescription() const
	{
		return m_description;
	}

	inline const std::string& GetLongitude() const
	{
		return m_longitude;
	}

	inline const std::string& GetLatitude() const
	{
		return m_latitude;
	}

	inline void SetPriority( const std::string &priority )
	{
		m_priority = priority;
	}

	inline void SetMethod( const std::string &method )
	{
		m_method = method;
	}

	inline void SetTime( const std::string &time )
	{
		m_time = time;
	}

	inline void SetDescription( const std::string &description )
	{
		m_description = description;
	}

	inline void SetLongitude( const std::string &longitude )
	{
		m_longitude = longitude;
	}

	inline void SetLatitude( const std::string &latitude )
	{
		m_latitude = latitude;
	}


	inline int GetSum() const
	{
		return m_sum;
	}

	inline void SetSum( int sum )
	{
		m_sum = sum;
	}

private:
	std::string m_priority;
	std::string m_method;
	std::string m_time;
	std::string m_description;
	std::string m_longitude;
	std::string m_latitude;
	int m_sum;

};

//////////////////////////////////////////////////////////////////////////
class CAlarmNotifyResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
	CAlarmNotifyResponseMsg()
	{
	}

	void setMsgProperties(const CAlarmNotifyResponseMsg &msg)
	{
		m_result = msg.GetResult();
		SetDeviceID(msg.GetDeviceID());
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

#endif // MSG_QUERY_DEVICESTATUS_HPP_
