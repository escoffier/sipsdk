#ifndef MSG_DEVICE_CONFIG_HPP_
#define MSG_DEVICE_CONFIG_HPP_
#include "MessageMsg.hpp"
#include <list>

//////////////////////////////////////////////////////////////////////////
class CBasicParamMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CBasicParamMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
    inline const std::string& GetName() const
    {
        return m_sName;
    }

    inline void SetName( const std::string& sName )
    {
        m_sName = sName;
    }

    inline const std::string& GetDeviceID() const
    {
        return m_sDeviceID;
    }

    inline void SetDeviceID( const std::string& sID )
    {
        m_sDeviceID = sID;
    }

    inline const std::string& GetSIPServerID() const
    {
        return m_sSIPServerID;
    }

    inline void SetSIPServerID( const std::string& sID )
    {
        m_sSIPServerID = sID;
    }

    inline const std::string& GetServerIP() const
    {
        return m_sSIPServerIP;
    }

    inline void SetServerIP( const std::string& sIP )
    {
        m_sSIPServerIP = sIP;
    }

    inline int GetSIPServerPort() const
    {
        return m_iSIPServerPort;
    }

    inline void SetSIPServerPort( int iPort )
    {
        m_iSIPServerPort = iPort;
    }

    inline const std::string& GetDomainName() const
    {
        return m_sDomainName;
    }

    inline void SetDomainName( const std::string& sDomainName )
    {
        m_sDomainName = sDomainName;
    }

    inline int GetExpiration() const
    {
        return m_iExpiration;
    }

    inline void SetExpiration( int iExpiration )
    {
        m_iExpiration = iExpiration;
    }

    inline const std::string& GetPassword() const
    {
        return m_sPassword;
    }

    inline void SetPassword( const std::string& sPassword )
    {
        m_sPassword = sPassword;
    }

    inline int GetHeartBeatInterval() const
    {
        return m_iHeartBeatInterval;
    }

    inline void SetHeartBeatInterval( int iInterval )
    {
        m_iHeartBeatInterval = iInterval;
    }

    inline int GetHeartBeatCount() const
    {
        return m_iHeartBeatCount;
    }

    inline void SetHeartBeatCount( int iCount )
    {
        m_iHeartBeatCount = iCount;
    }

private:
    std::string m_sName;
    std::string m_sDeviceID;
    std::string m_sSIPServerID;
    std::string m_sSIPServerIP;
    int m_iSIPServerPort;
    std::string m_sDomainName;
    int m_iExpiration;
    std::string m_sPassword;
    int m_iHeartBeatInterval;
    int m_iHeartBeatCount;

};

//////////////////////////////////////////////////////////////////////////
class CVideoParamConfigMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
    struct SVideoParamAttributeType
    {
        std::string sStreamName;
        std::string sVideoFormat;
        std::string sResolution;
        std::string sFrameRate;
        std::string sBitRateType;
        std::string sVideoBitRate;
    };

public:
    CVideoParamConfigMsg()
    {
    }

public:
    virtual bool Encode( std::string &message );
    virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
    inline int GetSum() const
    {
        return m_iNum;
    }

    inline void SetSum( int iNum )
    {
        m_iNum = iNum;
    }

    inline const std::list< SVideoParamAttributeType >& GetAttributeType() const
    {
        return m_sAttributeType;
    }

    inline void SetAttributeType( const std::list< SVideoParamAttributeType >& type )
    {
        m_sAttributeType = type;
    }

    inline void AddAttributeType( const std::string &sStreamName,
        const std::string &sVideoFormat,
        const std::string &sResolution,
        const std::string &sFrameRate,
        const std::string &sBitRateType,
        const std::string &sVideoBitRate )
    {
        SVideoParamAttributeType sAttributeType;
        sAttributeType.sStreamName = sStreamName;
        sAttributeType.sVideoFormat = sVideoFormat;
        sAttributeType.sResolution = sResolution;
        sAttributeType.sFrameRate = sFrameRate;
        sAttributeType.sBitRateType = sBitRateType;
        sAttributeType.sVideoBitRate = sVideoBitRate;
        m_sAttributeType.push_back( sAttributeType );
    }

    inline void AddAttributeType( const SVideoParamAttributeType &type )
    {
        m_sAttributeType.push_back( type );
    }

private:
    int m_iNum;
    std::list< SVideoParamAttributeType > m_sAttributeType;

};

//////////////////////////////////////////////////////////////////////////
class CAudioParamConfigMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
    struct SAudioParamAttributeType
    {
        std::string sStreamName;
        std::string sAudioFormat;
        std::string sAudioBitRate;
        std::string sSamplingRate;
    };

public:
    CAudioParamConfigMsg()
    {
    }

public:
    virtual bool Encode( std::string &message );
    virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
    inline const std::list< SAudioParamAttributeType >& GetAttributeType() const
    {
        return m_sAttributeType;
    }

    inline void SetAttributeType( const std::list< SAudioParamAttributeType >& type )
    {
        m_sAttributeType = type;
    }

    inline void AddAttributeType( const std::string &sStreamName,
        const std::string &sAudioFormat,
        const std::string &sAudioBitRate,
        const std::string &sSamplingRate )
    {
        SAudioParamAttributeType sAttributeType;
        sAttributeType.sStreamName = sStreamName;
        sAttributeType.sAudioFormat = sAudioFormat;
        sAttributeType.sAudioBitRate = sAudioBitRate;
        sAttributeType.sSamplingRate = sSamplingRate;
        m_sAttributeType.push_back( sAttributeType );
    }

    inline void AddAttributeType( const SAudioParamAttributeType &type )
    {
        m_sAttributeType.push_back( type );
    }

private:
    int m_Num;
    std::list< SAudioParamAttributeType > m_sAttributeType;
};

//////////////////////////////////////////////////////////////////////////
class CSVACEncodeConfigMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
    struct SROIItem
    {
        int iROISeq;
        int iTopLeft;
        int iBottomRight;
        int iROIQP;
    };

    struct SROIParam
    {
        bool bEnable;
        bool bReserve[3];
        int iROIFlag;
        int iROINumber;
        std::list< SROIItem > sSROIItem;
        int iBackGroundQP;
        int iBackGroundSkipFlag;
    };

    struct SSVCParam
    {
        bool bEnable;
        bool bReserve[3];
        int iSVCFlag;
        int iSVCSTMMode;
        int iSVCSpaceDomainMode;
        int iSVCTimeDomainMode;
    };

    struct SSurveillanceParam
    {
        bool bEnable;
        bool bReserve[3];
        int iTimeFlag;
        int iEventFlag;
        int iAlertFlag;
    };

    struct SEncryptParam
    {
        bool bEnable;
        bool bReserve[3];
        int iEncryptionFlag;
        int iAuthenticationFlag;
    };

    struct SAudioParam
    {
        bool bEnable;
        bool bReserve[3];
        int iAudioRecognitionFlag;
    };

public:
    CSVACEncodeConfigMsg()
    {
    }

public:
    virtual bool Encode( std::string &message );
    virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
    inline const SROIParam& GetROIParam() const
    {
        return m_sROIParam;
    }

    inline void SetROIParam( const SROIParam& sROIParam )
    {
        memcpy(&m_sROIParam, &sROIParam, sizeof(SROIParam));
    }

    inline const SSVCParam& GetSVCParam() const
    {
        return m_sSVCParam;
    }

    inline void SetSVCParam( const SSVCParam& sSVCParam )
    {
        memcpy(&m_sSVCParam, &sSVCParam, sizeof(SSVCParam));
    }

    inline const SSurveillanceParam& GetSurveillanceParam() const
    {
        return m_sSurveillanceParam;
    }

    inline void SetSurveillanceParam( const SSurveillanceParam& sSurveillanceParam )
    {
        memcpy(&m_sSurveillanceParam, &sSurveillanceParam, sizeof(SSurveillanceParam));
    }

    inline const SEncryptParam& GetEncryptParam() const
    {
        return m_sEncryptParam;
    }

    inline void SetEncryptParam( const SEncryptParam& sSEncryptParam )
    {
        memcpy(&m_sEncryptParam, &sSEncryptParam, sizeof(SEncryptParam));
    }

    inline const SAudioParam& GetAudioParam() const
    {
        return m_sAudioParam;
    }

    inline void SetAudioParam( const SAudioParam& sSAudioParam )
    {
        memcpy(&m_sAudioParam, &sSAudioParam, sizeof(SAudioParam));
    }

private:
    SROIParam m_sROIParam;
    SSVCParam m_sSVCParam;
    SSurveillanceParam m_sSurveillanceParam;
    SEncryptParam m_sEncryptParam;
    SAudioParam m_sAudioParam;

};

//////////////////////////////////////////////////////////////////////////
class CSVACDecodeConfigMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
    CSVACDecodeConfigMsg()
    {
    }

public:
    virtual bool Encode( std::string &message );
    virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
private:
};

//////////////////////////////////////////////////////////////////////////
class CDeviceConfigResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
    struct SSVCParam
    {
        bool bEnable;
        bool bReserve[3];
        int iSVCSTMMode;
    };

    struct SSurveillanceParam
    {
        bool bEnable;
        bool bReserve[3];
        int iTimeShowFlag;
        int iEventShowFlag;
        int iAlerShowtFlag;
    };

public:
	CDeviceConfigResponseMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
    inline const SSVCParam& GetSVCParam() const
    {
        return m_sSVCParam;
    }

    inline void SetSVCParam( const SSVCParam& sSVCParam )
    {
        memcpy(&m_sSVCParam, &sSVCParam, sizeof(SSVCParam));
    }

    inline const SSurveillanceParam& GetSurveillanceParam() const
    {
        return m_sSurveillanceParam;
    }

    inline void SetSurveillanceParam( const SSurveillanceParam& sSurveillanceParam )
    {
        memcpy(&m_sSurveillanceParam, &sSurveillanceParam, sizeof(SSurveillanceParam));
    }

private:
    SSVCParam m_sSVCParam;
    SSurveillanceParam m_sSurveillanceParam;

};
//////////////////////////////////////////////////////////////////////////
#endif // MSG_DEVICE_CONFIG_HPP_
