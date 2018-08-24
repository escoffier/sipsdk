#ifndef MSG_QUERY_RECORDINFO_HPP_
#define MSG_QUERY_RECORDINFO_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CQueryRecordInfoMsg : public CMessageMsgT< CMessageMsg::MsgCmdQuery >
{
public:
	CQueryRecordInfoMsg()
	{
	}

	void setMsgProperties(const CQueryRecordInfoMsg &msg)
	{
		m_startTime = msg.GetStartTime();
		m_endTime = msg.GetEndTime();
		m_filePath = msg.GetFilePath();
		m_address = msg.GetAddress();
		m_secrecy = msg.GetSecrecy();
		m_type = msg.GetType();
		m_recorder = msg.GetRecorderID();
		m_indistinct = msg.GetIndistinct();

		SetDeviceID(msg.GetDeviceID());
		SetSN( msg.GetSN() );
		SetTo(msg.GetTo());
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

	inline const std::string& GetFilePath() const
	{
		return m_filePath;
	}

	inline const std::string& GetAddress() const
	{
		return m_address;
	}

	inline const std::string& GetSecrecy() const
	{
		return m_secrecy;
	}

	inline const std::string& GetType() const
	{
		return m_type;
	}

	inline const std::string& GetRecorderID() const
	{
		return m_recorder;
	}

	inline const std::string& GetIndistinct() const
	{
		return m_indistinct;
	}

	inline void SetStartTime( const std::string &time )
	{
		m_startTime = time;
	}

	inline void SetEndTime( const std::string &time )
	{
		m_endTime = time;
	}

	inline void SetFilePath( const std::string &path )
	{
		m_filePath = path;
	}

	inline void SetAddress( const std::string &address )
	{
		m_address = address;
	}

	inline void SetSecrecy( const std::string &secrecy )
	{
		m_secrecy = secrecy;
	}

	inline void SetType( const std::string &type )
	{
		m_type = type;
	}

	inline void SetRecorderID( const std::string &id )
	{
		m_recorder = id;
	}

	inline void SetIndistinct( const std::string &indistinct )
	{
		m_indistinct = indistinct;
	}

private:
	std::string m_startTime;
	std::string m_endTime;
	std::string m_filePath;
	std::string m_address;
	std::string m_secrecy;
	std::string m_type;
	std::string m_recorder;
	std::string m_indistinct;
};

//////////////////////////////////////////////////////////////////////////
class CQueryRecordInfoResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
    struct SRecordInfo
    {
		std::string id;
		std::string name;
        std::string avPath;
		std::string address;
        std::string beginTime;
        std::string endTime;
		std::string secrecy;
        std::string type;
		std::string recorder;
    };

public:
    CQueryRecordInfoResponseMsg()
    {
    }

	void setMsgProperties(const CQueryRecordInfoResponseMsg &msg)
	{
		m_sum = msg.m_sum;
		m_name = msg.m_name;
		m_records = msg.m_records;
		SetDeviceID(msg.GetDeviceID());
	}

public:
	virtual bool IsAutoResponse() const { return false; }
    virtual bool Encode( std::string &message );
    virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
    inline int GetSum() const
    {
        return m_sum;
	}

	inline void SetSum( int sum )
	{
		m_sum = sum;
	}

	inline const std::string& GetName() const
	{
		return m_name;
	}

	inline void SetName( const std::string& name )
	{
		m_name = name;
	}

    inline const std::list< SRecordInfo >& GetRecords() const
    {
        return m_records;
	}

	inline void SetRecords( const std::list< SRecordInfo >& rec )
	{
		m_records = rec;
	}

	inline void AddRecordInfo( const std::string &id,
							   const std::string &name,
							   const std::string &avPath,
							   const std::string &address,
							   const std::string &beginTime,
							   const std::string &endTime,
							   const std::string &secrecy,
							   const std::string &type,
							   const std::string &recorder )
	{
		SRecordInfo recordinfo;
		recordinfo.id        = id;
		recordinfo.name      = name;
		recordinfo.avPath    = avPath;
		recordinfo.address   = address;
		recordinfo.beginTime = beginTime;
		recordinfo.endTime   = endTime;
		recordinfo.secrecy   = secrecy;
		recordinfo.type      = type;
		recordinfo.recorder  = recorder;
		m_records.push_back( recordinfo );
	}

    inline void AddRecordInfo( const SRecordInfo &recordinfo )
    {
        m_records.push_back( recordinfo );
    }

private:
    int                      m_sum;
	std::string              m_name;
    std::list< SRecordInfo > m_records;
};

#endif // MSG_QUERY_DEVICEINFO_HPP_