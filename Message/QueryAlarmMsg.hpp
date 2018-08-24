#ifndef MSG_QUERY_ALARM_HPP_
#define MSG_QUERY_ALARM_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CQueryAlarmMsg : public CMessageMsgT< CMessageMsg::MsgCmdQuery >
{
public:
	CQueryAlarmMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetStartAlarmPriority() const
	{
		return m_startAlarmPriority;
	}

	inline const std::string& GetEndAlarmPriority() const
	{
		return m_endAlarmPriority;
	}

	inline const std::string& GetAlarmMethod() const
	{
		return m_alarmMethod;
	}

	inline const std::string& GetStartAlarmTime() const
	{
		return m_startAlarmTime;
	}

	inline const std::string& GetEndAlarmTime() const
	{
		return m_endAlarmTime;
	}

	inline void SetStartAlarmPriority( const std::string &priority )
	{
		m_startAlarmPriority = priority;
	}

	inline void SetEndAlarmPriority( const std::string &priority )
	{
		m_endAlarmPriority = priority;
	}

	inline void SetAlarmMethod( const std::string &method )
	{
		m_alarmMethod = method;
	}

	inline void SetStartAlarmTime( const std::string &time )
	{
		m_startAlarmTime = time;
	}

	inline void SetEndAlarmTime( const std::string &time )
	{
		m_endAlarmTime = time;
	}

private:
	std::string m_startAlarmPriority;
	std::string m_endAlarmPriority;
	std::string m_alarmMethod;
	std::string m_startAlarmTime;
	std::string m_endAlarmTime;
};

//////////////////////////////////////////////////////////////////////////
class CQueryAlarmResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
	CQueryAlarmResponseMsg()
	{
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

#endif // MSG_QUERY_Alarm_HPP_