#ifndef MSG_QUERY_DEVICEINFO_HPP_
#define MSG_QUERY_DEVICEINFO_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CQueryDeviceInfoMsg : public CMessageMsgT< CMessageMsg::MsgCmdQuery >
{
public:
	CQueryDeviceInfoMsg()
	{
	}

	void setMsgProperties(const CQueryDeviceInfoMsg &msg)
	{
		SetDeviceID(msg.GetDeviceID());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CQueryDeviceInfoResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
	CQueryDeviceInfoResponseMsg()
	{
	}

	void setMsgProperties(const CQueryDeviceInfoResponseMsg &msg)
	{
		m_result = msg.GetResult();
		m_manufacturer = msg.GetManufacturer();
		m_model = msg.GetModel();
		m_firmware = msg.GetFirmware();
		m_channel = msg.GetChannel();
		m_devicename = msg.GetDeviceName();

		SetDeviceID(msg.GetDeviceID());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetManufacturer( const std::string &manufacturer )
	{
		m_manufacturer = manufacturer;
	}

	inline void SetModel( const std::string &model )
	{
		m_model = model;
	}

	inline void SetFirmware( const std::string &firmware )
	{
		m_firmware = firmware;
	}

	inline void SetChannel( const std::string &channel )
	{
		m_channel = channel;
	}

    inline void SetDeviceName( const std::string &name )
	{
		m_devicename = name;
	}

	inline const std::string& GetManufacturer() const
	{
		return m_manufacturer;
	}

	inline const std::string& GetModel() const
	{
		return m_model;
	}

	inline const std::string& GetFirmware() const
	{
		return m_firmware;
	}

	inline const std::string& GetChannel() const
	{
		return m_channel;
	}

	inline const std::string& GetDeviceName() const
	{
		return m_devicename;
	}

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
	std::string   m_manufacturer;
	std::string   m_model;
	std::string   m_firmware;
	std::string   m_channel;
	std::string   m_devicename;
};

#endif // MSG_QUERY_DEVICEINFO_HPP_