#ifndef MSG_NOTIFY_MSG_HPP_
#define MSG_NOTIFY_MSG_HPP_

#include "GBMessage.hpp"
#include "resip/dum/ServerOutOfDialogReq.hxx"
#include "resip/dum/ClientOutOfDialogReq.hxx"
#include "resip/dum/BaseSubscription.hxx"

//////////////////////////////////////////////////////////////////////////
class CCatalogNotifyMsg : public CGBMessage
{
public:
	enum EEventType
	{
		EVENT_ON = 0,	// 上线
		EVENT_OFF,		// 离线
		EVENT_VLOST,	// 视频丢失
		EVENT_DEFECT,	// 故障
		EVENT_ADD,		// 增加
		EVENT_DEL,		// 删除
		EVENT_UPDATE,	// 更新

		EVENT_UNKNOW,
	};

	struct SCatalog
	{
		EEventType  event;
		std::string id;
		std::string name;
		std::string manufacturer;
		std::string model;
		std::string owner;
		std::string civil;
		std::string block;
		std::string address;
		std::string safetyway;
		std::string registerway;
		std::string certnum;
		std::string certifiable;
		std::string errcode;
		std::string secrecy;
		std::string parental;
		std::string parentid;
		std::string endtime;
		std::string ip;
		std::string port;
		std::string password;
		std::string status;
		std::string longitude;
		std::string latitude;
		std::string ptz;
		std::string position;
		std::string room;
		std::string use;
		std::string supplylight;
		std::string direction;
		std::string resolution;
		std::string businessgroup;
	};

public:
	CCatalogNotifyMsg() : m_notify( NULL ), m_subscription( NULL )
	{
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

	static std::string GetStrName(EEventType eType)
	{
		switch (eType)
		{
		case EVENT_ON:
			return "ON";
		case EVENT_OFF:
			return "OFF";
		case EVENT_VLOST:
			return "VLOST";
		case EVENT_DEFECT:
			return "DEFECT";
		case EVENT_ADD:
			return "ADD";
		case EVENT_DEL:
			return "DEL";
		case EVENT_UPDATE:
			return "UPDATE";
		default: //EVENT_UNKNOW
			return "UNKNOW";
		}

		return "UNKNOW";
	}

	EEventType GetEnumName(const std::string &sType)
	{
		if (sType == "ON")
			return EVENT_ON;
		if (sType == "OFF")
			return EVENT_OFF;
		if (sType == "VLOSE")
			return EVENT_VLOST;
		if (sType == "DEFECT")
			return EVENT_DEFECT;
		if (sType == "ADD")
			return EVENT_ADD;
		if (sType == "DEL")
			return EVENT_DEL;
		if (sType == "UPDATE")
			return EVENT_UPDATE;

		return EVENT_UNKNOW;

	}


public:
	inline void SetHandle( resip::ServerOutOfDialogReq *h )
	{
		m_notify = h;
	}

	inline void SetHandle( const resip::ServerOutOfDialogReq *h )
	{
		m_notify = const_cast< resip::ServerOutOfDialogReq* >( h );
	}

	inline void SetHandle( resip::BaseSubscription *h )
	{
		m_subscription = h;
	}

	inline void SetHandle( const resip::BaseSubscription *h )
	{
		m_subscription = const_cast< resip::BaseSubscription* >( h );
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

	inline const std::string& GetStatus() const
	{
		return m_status;
	}

	inline void SetStatus( const std::string &status )
	{
		m_status = status;
	}

	inline int GetSum() const
	{
		return m_sum;
	}

	inline void SetSum( int sum )
	{
		m_sum = sum;
	}

	inline const std::list< SCatalog >& GetDevices() const
	{
		return m_devices;
	}

	inline void AddEvent(const std::string &id, EEventType eventType)
	{
		SCatalog catalog;
		catalog.id = id;
		catalog.event = eventType;
		m_devices.push_back( catalog );
	}

	inline void AddOnEvent( const std::string &id )
	{
		SCatalog catalog;
		catalog.id = id;
		catalog.event = EVENT_ON;
		m_devices.push_back( catalog );
	}

	inline void AddOffEvent( const std::string &id )
	{
		SCatalog catalog;
		catalog.id = id;
		catalog.event = EVENT_OFF;
		m_devices.push_back( catalog );
	}

	inline void AddVLostEvent( const std::string &id )
	{
		SCatalog catalog;
		catalog.id = id;
		catalog.event = EVENT_VLOST;
		m_devices.push_back( catalog );
	}

	inline void AddDefectEvent( const std::string &id )
	{
		SCatalog catalog;
		catalog.id = id;
		catalog.event = EVENT_DEFECT;
		m_devices.push_back( catalog );
	}

	inline void AddDelEvent( const std::string &id )
	{
		SCatalog catalog;
		catalog.id = id;
		catalog.event = EVENT_DEL;
		m_devices.push_back( catalog );
	}

	inline void AddAddEvent( const SCatalog &catalog )
	{
		if( catalog.event == EVENT_ADD )
		{
			m_devices.push_back( catalog );
		}
	}

	inline void AddUpdateEvent( const SCatalog &catalog )
	{
		if( catalog.event == EVENT_UPDATE )
		{
			m_devices.push_back( catalog );
		}
	}

private:
	resip::ServerOutOfDialogReq* m_notify;
	resip::BaseSubscription*     m_subscription;
	std::string                  m_sn;
	std::string                  m_deviceid;
	std::string                  m_status;
	int                          m_sum;
	std::list< SCatalog >        m_devices;
};

//////////////////////////////////////////////////////////////////////////
class CNotifyResponseMsg : public CGBMessage
{
public:
	CNotifyResponseMsg()
	{
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetHandle( resip::ServerOutOfDialogReq *h )
	{
		m_notify = h;
	}

	inline void SetHandle( resip::ServerSubscription *h )
	{
		m_subscription = h;
	}

private:
	resip::ServerOutOfDialogReq* m_notify;
	resip::ServerSubscription*   m_subscription;
};

//////////////////////////////////////////////////////////////////////////
#endif // MSG_NOTIFY_MSG_HPP_