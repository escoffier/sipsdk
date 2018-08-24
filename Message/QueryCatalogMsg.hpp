#ifndef MSG_QUERY_CATALOG_HPP_
#define MSG_QUERY_CATALOG_HPP_
#include "MessageMsg.hpp"
#include <list>

//////////////////////////////////////////////////////////////////////////
/** �����յ���Ŀ¼��ѯ��Ϣ������Ϣ�������ϼ�,sipclient,siphost������Ϣ�������ڱ����¹����¼��͹����豸����
 *  �����Ƕ�����Ϣ����Ȼ�����ò�ѯ���豸ID����Ϣ��to�ֶ�
 *
 */
class CQueryCatalogMsg : public CMessageMsgT< CMessageMsg::MsgCmdQuery >
{
public:
	CQueryCatalogMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetStartTime() const
	{
		return m_startTime;
	}

	inline const std::string& GetEndTime() const
	{
		return m_endTime;
	}

	inline void SetStartTime( const std::string &time )
	{
		m_startTime = time;
	}

	inline void SetEndTime( const std::string &time )
	{
		m_endTime = time;
	}

private:
	std::string m_startTime;
	std::string m_endTime;
};

//////////////////////////////////////////////////////////////////////////
/** Ŀ¼������Ϣ���յ�Ŀ¼��ѯ֮�󣬽���ѯ��Ŀ¼���͸���ѯ��
 *
 */
class CQueryCatalogResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
	struct SCatalog
	{
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
	CQueryCatalogResponseMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline int GetSum() const
	{
		return m_sum;
	}

	inline const std::list< SCatalog >& GetDevices() const
	{
		return m_devices;
	}

	inline void SetSum( int sum )
	{
		m_sum = sum;
	}

	inline void AddCatalog( const SCatalog &catalog )
	{
		m_devices.push_back( catalog );
	}

private:
	bool CheckCatalog( const SCatalog &catalog, std::string& reason) const;

private:
	int                   m_sum;
	std::list< SCatalog > m_devices;
};
//////////////////////////////////////////////////////////////////////////
#endif // MSG_QUERY_CATALOG_HPP_
