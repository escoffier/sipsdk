#ifndef MSG_QUERY_DEVICESTATUS_HPP_
#define MSG_QUERY_DEVICESTATUS_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CQueryDeviceStatusMsg : public CMessageMsgT< CMessageMsg::MsgCmdQuery >
{
public:
	CQueryDeviceStatusMsg()
	{
	}

	void setMsgProperties(const CQueryDeviceStatusMsg &msg)
	{
		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CQueryDeviceStatusResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
	struct SAlarmStatus
	{
		std::string id;
		std::string status;
	};

public:
	CQueryDeviceStatusResponseMsg()
	{
	}

	void setMsgProperties(const CQueryDeviceStatusResponseMsg &msg)
	{
		m_result = msg.GetResult();
		m_online = msg.GetOnline();
		m_manufacturer = msg.GetManufacturer();
		m_status = msg.GetStatus();
		m_reason = msg.GetReason();
		m_encode = msg.GetEncode();
		m_record = msg.GetRecord();
		m_deivcetime = msg.GetDeviceTime();
		m_items = msg.GetAlarmStatus();

		SetDeviceID(msg.GetDeviceID());
	}

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

	inline void SetOnline( const std::string& online )
	{
		m_online = online;
	}

	inline void SetManufacturer( const std::string &manufacturer )
	{
		m_manufacturer = manufacturer;
	}

	inline void SetStatus( const std::string& status )
	{
		m_status = status;
	}

	inline void SetReason( std::string &reason )
	{
		m_reason = reason;
	}

	inline void SetEncode( const std::string& encode )
	{
		m_encode = encode;
	}

	inline void SetRecord( const std::string& record )
	{
		m_record = record;
	}

	inline void SetDeviceTime( const std::string& time )
	{
		m_deivcetime = time;
	}

	inline void AddAlarmStatus( const std::string &id, const std::string &status )
	{
		SAlarmStatus as;
		as.id = id;
		as.status = status;
		m_items.push_back( as );
	}

	inline const std::string& GetOnline() const
	{
		return m_online;
	}

	inline const std::string& GetManufacturer() const
	{
		return m_manufacturer;
	}

	inline const std::string& GetStatus() const
	{
		return m_status;
	}

	inline const std::string& GetReason() const
	{
		return m_reason;
	}

	inline const std::string& GetEncode() const
	{
		return m_encode;
	}

	inline const std::string& GetRecord() const
	{
		return m_record;
	}

	inline const std::string& GetDeviceTime() const
	{
		return m_deivcetime;
	}

	inline const std::list< SAlarmStatus >& GetAlarmStatus() const
	{
		return m_items;
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

private:
	EGBResultType             m_result;
	std::string               m_online;
	std::string               m_manufacturer;
	std::string				  m_status;
	std::string               m_reason;
	std::string               m_encode;
	std::string               m_record;
	std::string               m_deivcetime;
	std::list< SAlarmStatus > m_items;
};

#endif // MSG_QUERY_DEVICEINFO_HPP_