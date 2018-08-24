#ifndef MSG_INVITE_MSG_HPP_
#define MSG_INVITE_MSG_HPP_

#include <string>
#include "GBMessage.hpp"
#include "resip/dum/InviteSession.hxx"
#include "resip/dum/InviteSession.hxx"
#include "resip/dum/ClientInviteSession.hxx"

//////////////////////////////////////////////////////////////////////////
class CInviteMsg : public CGBMessage
{
public:
	struct SVideoUri
	{	
		std::string id;
		std::string param;
	};

public:
	CInviteMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::InviteSession* GetHandle() const
	{
		return m_handle;
	}

	inline void SetHandle( resip::InviteSession *h )
	{
		m_handle = h;
	}
	
public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	resip::InviteSession* m_handle;
};

//////////////////////////////////////////////////////////////////////////
class CVodDownloadMsg : public CInviteMsg
{
public:
	CVodDownloadMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetCamera() const
	{
		return m_uri.id;
	}

	inline void SetCamera( const std::string &id )
	{
		m_uri.id = id;
	}

	inline const std::string& GetRecvID() const
	{
		return m_recvid;
	}

	inline void SetRecvID( const std::string& str )
	{
		m_recvid = str;
	}

	inline const std::string& GetSessionID() const
	{
		return m_sessionid;
	}

	inline void SetSessionID( const std::string& str )
	{
		m_sessionid = str;
	}

	inline const std::string& GetVersion() const
	{
		return m_version;
	}

	inline void SetVersion( const std::string& str )
	{
		m_version = str;
	}

	inline const std::string& GetRecvIP() const
	{
		return m_mediaip;
	}

	inline void SetRecvIP( const std::string &ip )
	{
		m_mediaip = ip;
    }

    inline const SVideoUri& GetUri() const
    {
        return m_uri;
    }

    inline const std::string& GetUriID() const
    {
        return m_uri.id;
    }

    inline const std::string& GetUriParam() const
    {
        return m_uri.param;
    }

    inline void SetUri( const SVideoUri &uri )
    {
        m_uri = uri;
    }

    inline void SetUriID( const std::string &id )
    {
        m_uri.id = id;
    }

    inline void SetUriParam( const std::string &param )
    {
        m_uri.param = param;
    }

	inline const std::string& GetConnect() const
	{
		return m_connect;
	}

	inline void SetConnect( const std::string &connect )
	{
		m_connect = connect;
	}

	inline const std::string& GetBeginTime() const
	{
		return m_starttime;
	}

	inline void SetBeginTime( const std::string& str )
	{
		m_starttime = str;
	}

	inline const std::string& GetEndTime() const
	{
		return m_endtime;
	}

	inline void SetEndTime( const std::string& str )
	{
		m_endtime = str;
	}

	inline const std::string& GetMediaType() const
	{
		return m_mediatype;
	}

	inline void SetMediaType( const std::string& type )
	{
		m_mediatype = type;
	}

	inline int GetRecvPort() const
	{
		return atoi( m_mediaport.c_str() );
	}

	inline void SetRecvPort( int port )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", port );
		m_mediaport = buf;
	}

	inline const std::string& GetMediaTransport() const
	{
		return m_transport;
	}

	inline void SetMediaTransport( const std::string& transport )
	{
		m_transport = transport;
	}

	inline const std::string& GetDownloadspeed() const
	{
		return m_downloadspeed;
	}

	inline void SetDownloadspeed( const std::string& downloadspeed )
	{
		m_downloadspeed = downloadspeed;
	}

	inline const std::map< std::string, std::string >& GetRtpMap() const
	{
		return m_rtpmap;
	}

	inline void SetRtpMap( const std::map< std::string, std::string > &rtpmap )
	{
		m_rtpmap = rtpmap;
	}

	inline const std::string& GetSSrc() const
	{
		return m_ssrc;
	}

	inline void SetSSrc( const std::string &ssrc )
	{
		m_ssrc = ssrc;
	}

	inline const std::string& GetAudioProperty() const
	{
		return m_audioproterty;
	}

	inline void SetAudioProperty( const std::string &audio )
	{
		m_audioproterty = audio;
	}

private:
	std::string                          m_recvid;			// o
	std::string                          m_sessionid;		// o
	std::string                          m_version;			// o
	std::string                          m_mediaip;			// o
	SVideoUri                            m_uri;				// u
	std::string                          m_connect;			// c
	std::string                          m_starttime;		// t
	std::string                          m_endtime;			// t
	std::string                          m_mediatype;		// m
	std::string                          m_mediaport;		// m
	std::string							 m_transport;		// m
	std::map< std::string, std::string > m_rtpmap;			// m & a
	std::string                          m_downloadspeed;	// a
	std::string                          m_ssrc;            // y
	std::string                          m_audioproterty;   // f
};

//////////////////////////////////////////////////////////////////////////
class CVodStreamMsg : public CInviteMsg
{
public:
	CVodStreamMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetCamera() const
	{
		return m_uri.id;
	}

	inline void SetCamera( const std::string &id )
	{
		m_uri.id = id;
	}

    inline const std::string& GetRecvID() const
    {
        return m_recvid;
    }

    inline void SetRecvID( const std::string& str )
    {
        m_recvid = str;
    }

    inline const std::string& GetSessionID() const
    {
        return m_sessionid;
    }

    inline void SetSessionID( const std::string& str )
    {
        m_sessionid = str;
	}

	inline const std::string& GetVersion() const
	{
		return m_version;
	}

	inline void SetVersion( const std::string& str )
	{
		m_version = str;
	}

	inline const std::string& GetRecvIP() const
	{
		return m_mediaip;
	}

	inline void SetRecvIP( const std::string &ip )
	{
		m_mediaip = ip;
	}

	inline const SVideoUri& GetUri() const
	{
		return m_uri;
	}

	inline const std::string& GetUriID() const
	{
		return m_uri.id;
    }

    inline const std::string& GetUriParam() const
    {
        return m_uri.param;
    }

	inline void SetUri( const SVideoUri &uri )
	{
		m_uri = uri;
	}

	inline void SetUriID( const std::string &id )
	{
		m_uri.id = id;
    }

    inline void SetUriParam( const std::string &param )
    {
        m_uri.param = param;
    }

	inline const std::string& GetConnect() const
	{
		return m_connect;
	}

	inline void SetConnect( const std::string &connect )
	{
		m_connect = connect;
	}

    inline const std::string& GetBeginTime() const
    {
        return m_starttime;
    }

    inline void SetBeginTime( const std::string& str )
    {
        m_starttime = str;
    }

    inline const std::string& GetEndTime() const
    {
        return m_endtime;
    }

    inline void SetEndTime( const std::string& str )
    {
        m_endtime = str;
	}

	inline const std::string& GetMediaType() const
	{
		return m_mediatype;
	}

	inline void SetMediaType( const std::string& type )
	{
		m_mediatype = type;
	}

    inline int GetRecvPort() const
    {
        return atoi( m_mediaport.c_str() );
    }

    inline void SetRecvPort( int port )
    {
		char buf[16] = { 0 };
		sprintf( buf, "%d", port );
        m_mediaport = buf;
    }

	inline const std::string& GetMediaTransport() const
	{
		return m_transport;
	}

	inline void SetMediaTransport( const std::string& transport )
	{
		m_transport = transport;
	}

	inline const std::map< std::string, std::string >& GetRtpMap() const
	{
		return m_rtpmap;
	}

	inline void SetRtpMap( const std::map< std::string, std::string > &rtpmap )
	{
		m_rtpmap = rtpmap;
	}

	inline const std::string& GetSSrc() const
	{
		return m_ssrc;
	}

	inline void SetSSrc( const std::string &ssrc )
	{
		m_ssrc = ssrc;
	}

	inline const std::string& GetAudioProperty() const
	{
		return m_audioproterty;
	}

	inline void SetAudioProperty( const std::string &audio )
	{
		m_audioproterty = audio;
	}

private:
	std::string                          m_recvid;       // o
	std::string                          m_sessionid;    // o
	std::string                          m_version;      // o
	std::string                          m_mediaip;      // o
	SVideoUri                            m_uri;          // u
	std::string                          m_connect;	     // c
	std::string                          m_starttime;    // t
	std::string                          m_endtime;      // t
	std::string                          m_mediatype;    // m
	std::string                          m_mediaport;    // m
	std::string							 m_transport;    // m
	std::map< std::string, std::string > m_rtpmap;       // m & a
	std::string                          m_ssrc;         // y
	std::string                          m_audioproterty;   // f
};

//////////////////////////////////////////////////////////////////////////
class CRealStreamMsg : public CInviteMsg
{
public:
	CRealStreamMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetCamera() const
	{
		return m_sender.id;
	}

	inline void SetCamera( const std::string &id )
	{
		m_sender.id = id;
	}

	inline const std::string& GetRecvID() const
	{
		return m_recvid;
	}

	inline void SetRecvID( const std::string& str )
	{
		m_recvid = str;
	}

	inline const std::string& GetSessionID() const
	{
		return m_sessionid;
	}

	inline void SetSessionID( const std::string& str )
	{
		m_sessionid = str;
	}

	inline const std::string& GetVersion() const
	{
		return m_version;
	}

	inline void SetVersion( const std::string& str )
	{
		m_version = str;
	}

	inline const std::string& GetRecvIP() const
	{
		return m_mediaip;
	}

	inline void SetRecvIP( const std::string &ip )
	{
		m_mediaip = ip;
	}

	inline const std::string& GetConnect() const
	{
		return m_connect;
	}

	inline void SetConnect( const std::string &connect )
	{
		m_connect = connect;
	}

	inline const std::string& GetBeginTime() const
	{
		return m_starttime;
	}

	inline void SetBeginTime( const std::string& str )
	{
		m_starttime = str;
	}

	inline const std::string& GetEndTime() const
	{
		return m_endtime;
	}

	inline void SetEndTime( const std::string& str )
	{
		m_endtime = str;
	}

	inline const std::string& GetMediaType() const
	{
		return m_mediatype;
	}

	inline void SetMediaType( const std::string& type )
	{
		m_mediatype = type;
	}

	inline int GetRecvPort() const
	{
		return atoi( m_mediaport.c_str() );
	}

	inline void SetRecvPort( int port )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", port );
		m_mediaport = buf;
	}

    inline void SetRecvPort( const std::string &port )
    {
        m_mediaport = port;
    }

	inline const std::string& GetMediaTransport() const
	{
		return m_transport;
	}

	inline void SetMediaTransport( const std::string& transport )
	{
		m_transport = transport;
	}

	inline const std::map< std::string, std::string >& GetRtpMap() const
	{
		return m_rtpmap;
	}

	inline void SetRtpMap( const std::map< std::string, std::string > &rtpmap )
	{
		m_rtpmap = rtpmap;
	}

	inline const std::string& GetSSrc() const
	{
		return m_ssrc;
	}

	inline void SetSSrc( const std::string &ssrc )
	{
		m_ssrc = ssrc;
	}

	inline const std::string& GetAudioProperty() const
	{
		return m_audioproterty;
	}

	inline void SetAudioProperty( const std::string &audio )
	{
		m_audioproterty = audio;
	}

private:
	std::string                          m_recvid;			// o
	std::string                          m_sessionid;		// o
	std::string                          m_version;			// o
	std::string                          m_mediaip;			// o
	std::string                          m_connect;			// c
	std::string                          m_starttime;		// t
	std::string                          m_endtime;			// t
	std::string                          m_mediatype;		// m
	std::string                          m_mediaport;		// m
	std::string							 m_transport;		// m
	std::map< std::string, std::string > m_rtpmap;			// m & a
	std::string                          m_ssrc;			// y
	std::string                          m_audioproterty;   // f
};

//////////////////////////////////////////////////////////////////////////
class CRealStreamToDecorderMsg : public CInviteMsg
{
public:
    CRealStreamToDecorderMsg()
    {
    }

public:
    virtual bool Encode( std::string &message );
    virtual bool Decode( const char *body, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
    inline const std::string& GetCamera() const
    {
        return m_recver.id;
    }

    inline void SetCamera( const std::string &id )
    {
        m_recver.id = id;
    }

    inline const std::string& GetRecvID() const
    {
        return m_recvid;
    }

    inline void SetRecvID( const std::string& str )
    {
        m_recvid = str;
    }

    inline const std::string& GetSessionID() const
    {
        return m_sessionid;
    }

    inline void SetSessionID( const std::string& str )
    {
        m_sessionid = str;
    }

    inline const std::string& GetVersion() const
    {
        return m_version;
    }

    inline void SetVersion( const std::string& str )
    {
        m_version = str;
    }

    inline const std::string& GetRecvIP() const
    {
        return m_mediaip;
    }

    inline void SetRecvIP( const std::string &ip )
    {
        m_mediaip = ip;
    }

    inline const std::string& GetConnect() const
    {
        return m_connect;
    }

    inline void SetConnect( const std::string &connect )
    {
        m_connect = connect;
    }

    inline const std::string& GetBeginTime() const
    {
        return m_starttime;
    }

    inline void SetBeginTime( const std::string& str )
    {
        m_starttime = str;
    }

    inline const std::string& GetEndTime() const
    {
        return m_endtime;
    }

    inline void SetEndTime( const std::string& str )
    {
        m_endtime = str;
    }

    inline const std::string& GetMediaType() const
    {
        return m_mediatype;
    }

    inline void SetMediaType( const std::string& type )
    {
        m_mediatype = type;
    }

    inline int GetRecvPort() const
    {
        return atoi( m_mediaport.c_str() );
    }

    inline void SetRecvPort( int port )
    {
        char buf[16] = { 0 };
        sprintf( buf, "%d", port );
        m_mediaport = buf;
    }

    inline const std::string& GetMediaTransport() const
    {
        return m_transport;
    }

    inline void SetMediaTransport( const std::string& transport )
    {
        m_transport = transport;
    }

    inline const std::map< std::string, std::string >& GetRtpMap() const
    {
        return m_rtpmap;
    }

    inline void SetRtpMap( const std::map< std::string, std::string > &rtpmap )
    {
        m_rtpmap = rtpmap;
    }

    inline const std::string& GetSSrc() const
    {
        return m_ssrc;
    }

    inline void SetSSrc( const std::string &ssrc )
    {
        m_ssrc = ssrc;
    }

    inline const std::string& GetAudioProperty() const
    {
        return m_audioproterty;
    }

    inline void SetAudioProperty( const std::string &audio )
    {
        m_audioproterty = audio;
    }

private:
    std::string                          m_recvid;			// o
    std::string                          m_sessionid;		// o
    std::string                          m_version;			// o
    std::string                          m_mediaip;			// o
    std::string                          m_connect;			// c
    std::string                          m_starttime;		// t
    std::string                          m_endtime;			// t
    std::string                          m_mediatype;		// m
    std::string                          m_mediaport;		// m
    std::string							 m_transport;		// m
    std::map< std::string, std::string > m_rtpmap;			// m & a
    std::string                          m_ssrc;			// y
    std::string                          m_audioproterty;   // f
};

//////////////////////////////////////////////////////////////////////////
class CBroadcastStreamMsg : public CInviteMsg
{
public:
	CBroadcastStreamMsg()
	{
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *context, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetCamera() const
	{
		return m_to;
	}

	inline void SetCamera( const std::string &id )
	{
		m_to = id;
	}

	inline const std::string& GetRecvID() const
	{
		return m_recvid;
	}

	inline void SetRecvID( const std::string& str )
	{
		m_recvid = str;
	}

	inline const std::string& GetSessionID() const
	{
		return m_sessionid;
	}

	inline void SetSessionID( const std::string& str )
	{
		m_sessionid = str;
	}

	inline const std::string& GetVersion() const
	{
		return m_version;
	}

	inline void SetVersion( const std::string& str )
	{
		m_version = str;
	}

	inline const std::string& GetRecvIP() const
	{
		return m_mediaip;
	}

	inline void SetRecvIP( const std::string &ip )
	{
		m_mediaip = ip;
	}

	inline const SVideoUri& GetUri() const
	{
		return m_uri;
	}

	inline void SetUri( const SVideoUri &uri )
	{
		m_uri = uri;
	}

	inline const std::string& GetConnect() const
	{
		return m_connect;
	}

	inline void SetConnect( const std::string &connect )
	{
		m_connect = connect;
	}

	inline const std::string& GetBeginTime() const
	{
		return m_starttime;
	}

	inline void SetBeginTime( const std::string& str )
	{
		m_starttime = str;
	}

	inline const std::string& GetEndTime() const
	{
		return m_endtime;
	}

	inline void SetEndTime( const std::string& str )
	{
		m_endtime = str;
	}

	inline const std::string& GetMediaType() const
	{
		return m_mediatype;
	}

	inline void SetMediaType( const std::string& type )
	{
		m_mediatype = type;
	}

	inline int GetRecvPort() const
	{
		return atoi( m_mediaport.c_str() );
	}

	inline void SetRecvPort( int port )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", port );
		m_mediaport = buf;
	}

	inline const std::string& GetMediaTransport() const
	{
		return m_transport;
	}

	inline void SetMediaTransport( const std::string& transport )
	{
		m_transport = transport;
	}

	inline const std::map< std::string, std::string >& GetRtpMap() const
	{
		return m_rtpmap;
	}

	inline void SetRtpMap( const std::map< std::string, std::string > &rtpmap )
	{
		m_rtpmap = rtpmap;
	}

	inline const std::string& GetSSrc() const
	{
		return m_ssrc;
	}

	inline void SetSSrc( const std::string &ssrc )
	{
		m_ssrc = ssrc;
	}

	inline const std::string& GetAudioProperty() const
	{
		return m_audioproterty;
	}

	inline void SetAudioProperty( const std::string &audio )
	{
		m_audioproterty = audio;
	}

private:
	std::string                          m_recvid;			// o
	std::string                          m_sessionid;		// o
	std::string                          m_version;			// o
	std::string                          m_mediaip;			// o
	SVideoUri                            m_uri;				// u
	std::string                          m_connect;			// c
	std::string                          m_starttime;		// t
	std::string                          m_endtime;			// t
	std::string                          m_mediatype;		// m
	std::string                          m_mediaport;		// m
	std::string							 m_transport;		// m
	std::map< std::string, std::string > m_rtpmap;			// m & a
	std::string                          m_ssrc;			// y
	std::string                          m_audioproterty;   // f
};

//////////////////////////////////////////////////////////////////////////
class CInviteResponseMsg : public CGBMessage
{
public:
	CInviteResponseMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::InviteSession* GetHandle() const
	{
		return m_handle;
	}

	inline void SetHandle( resip::InviteSession *h )
	{
		m_handle = h;
	}

	inline const std::string& GetSendIP() const
	{
		return m_mediaip;
	}

	inline void SetSendIP( const std::string &ip )
	{
		m_mediaip = ip;
	}

	inline void SetSendPort( int port )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", port );
		m_mediaport = buf;
	}

	inline void SetSendPort( const std::string &port )
	{
		m_mediaport = port;
	}

	inline const std::string& GetSendPort() const
	{
		return m_mediaport;
    }

    inline const std::string& GetRecvIP() const
    {
        return m_mediaip;
    }

    inline void SetRecvIP( const std::string &ip )
    {
        m_mediaip = ip;
    }

    inline void SetRecvPort( int port )
    {
        char buf[16] = { 0 };
        sprintf( buf, "%d", port );
        m_mediaport = buf;
    }

    inline void SetRecvPort( const std::string &port )
    {
        m_mediaport = port;
    }

    inline const std::string& GetRecvPort() const
    {
        return m_mediaport;
    }

	inline const std::string& GetConnect() const
	{
		return m_connect;
	}

	inline void SetConnect( const std::string &connect )
	{
		m_connect = connect;
    }

    inline const std::string& GetBeginTime() const
    {
        return m_starttime;
    }

    inline void SetBeginTime( const std::string& str )
    {
        m_starttime = str;
    }

    inline const std::string& GetEndTime() const
    {
        return m_endtime;
    }

    inline void SetEndTime( const std::string& str )
    {
        m_endtime = str;
    }

    inline const std::string& GetMediaType() const
    {
        return m_mediatype;
    }

    inline void SetMediaType( const std::string& type )
    {
        m_mediatype = type;
    }

	inline const std::string& GetRecvID() const
	{
		return m_recvid;
	}

	inline void SetRecvID( const std::string &recvid )
	{
		m_recvid = recvid;
    }

    inline const std::string& GetSessionID() const
    {
        return m_sessionid;
    }

    inline void SetSessionID( const std::string& str )
    {
        m_sessionid = str;
    }

    inline const std::string& GetVersion() const
    {
        return m_version;
    }

    inline void SetVersion( const std::string& str )
    {
        m_version = str;
    }

    inline const std::string& GetMediaTransport() const
    {
        return m_transport;
    }

    inline void SetMediaTransport( const std::string& transport )
    {
        m_transport = transport;
    }

	inline const std::map< std::string, std::string >& GetRtpMap() const
	{
		return m_rtpmap;
	}

	inline void SetRtpMap( const std::map< std::string, std::string > &rtpmap )
	{
		m_rtpmap = rtpmap;
    }

    inline void SetFileSize( const std::string &size )
    {
        m_filesize = size;
    }

    inline const std::string& GetFileSize() const
    {
        return m_filesize;
    }

	inline const std::string& GetSsrc() const
	{
		return m_ssrc;
	}

	inline void SetSsrc( const std::string &ssrc )
	{
		m_ssrc = ssrc;
	}

	inline const std::string& GetAudioProterty() const
	{
		return m_audioproterty;
	}

	inline void SetAudioProterty( const std::string &proterty )
	{
		m_audioproterty = proterty;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message ){return false;}
	virtual bool Decode( const char *body, std::string &reason ){return false;}
	virtual void Process( CSipInterface *pInterface ){}

protected:
    std::string                          m_version;			// v
	std::string                          m_recvid;			// o
    std::string                          m_sessionid;		// o
    std::string                          m_mediaip;			// o
    std::string                          m_sessionname;		// s
	std::string                          m_connect;			// c
	std::string                          m_starttime;		// t
	std::string                          m_endtime;			// t
	std::string                          m_mediatype;		// m
	std::string                          m_mediaport;		// m
	std::string							 m_transport;		// m
    std::map< std::string, std::string > m_rtpmap;			// m & a
    std::string                          m_filesize;        // m & a filesize
	std::string                          m_ssrc;			// y
    std::string                          m_audioproterty;	// f
	resip::InviteSession*                m_handle;			// handle
};

//////////////////////////////////////////////////////////////////////////
class CRealStreamResponseMsg : public CInviteResponseMsg
{
public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CBroadcastStreamResponseMsg : public CInviteResponseMsg
{
public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *context, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CVodStreamResponseMsg : public CInviteResponseMsg
{
public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CVodDownloadResponseMsg : public CInviteResponseMsg
{
public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CUnknowStreamResponseMsg : public CInviteResponseMsg
{
public:
    virtual bool Encode( std::string &message );
    virtual bool Decode( const char *body, std::string &reason );
    virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CAckMsg : public CGBMessage
{
public:
	CAckMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::InviteSession* GetHandle() const
	{
		return m_handle;
	}

	inline void SetHandle( resip::InviteSession *h )
	{
		m_handle = h;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
    inline const std::string& GetCamera() const
    {
        return m_sender.id;
    }

    inline void SetCamera( const std::string &id )
    {
        m_sender.id = id;
    }

    inline const std::string& GetRecvID() const
    {
        return m_recvid;
    }

    inline void SetRecvID( const std::string& str )
    {
        m_recvid = str;
    }

    inline const std::string& GetSessionID() const
    {
        return m_sessionid;
    }

    inline void SetSessionID( const std::string& str )
    {
        m_sessionid = str;
    }

    inline const std::string& GetVersion() const
    {
        return m_version;
    }

    inline void SetVersion( const std::string& str )
    {
        m_version = str;
    }

    inline const std::string& GetSendIP() const
    {
        return m_mediaip;
    }

    inline void SetSendIP( const std::string &ip )
    {
        m_mediaip = ip;
    }

    inline const std::string& GetConnect() const
    {
        return m_connect;
    }

    inline void SetConnect( const std::string &connect )
    {
        m_connect = connect;
    }

    inline const std::string& GetBeginTime() const
    {
        return m_starttime;
    }

    inline void SetBeginTime( const std::string& str )
    {
        m_starttime = str;
    }

    inline const std::string& GetEndTime() const
    {
        return m_endtime;
    }

    inline void SetEndTime( const std::string& str )
    {
        m_endtime = str;
    }

    inline const std::string& GetMediaType() const
    {
        return m_mediatype;
    }

    inline void SetMediaType( const std::string& type )
    {
        m_mediatype = type;
    }

    inline int GetSendPort() const
    {
        return atoi( m_mediaport.c_str() );
    }

    inline void SetSendPort( int port )
    {
        char buf[16] = { 0 };
        sprintf( buf, "%d", port );
        m_mediaport = buf;
    }

	inline void SetSendPort( const std::string &port )
	{
		m_mediaport = port;
	}

    inline const std::string& GetMediaTransport() const
    {
        return m_transport;
    }

    inline void SetMediaTransport( const std::string& transport )
    {
        m_transport = transport;
    }

    inline const std::map< std::string, std::string >& GetRtpMap() const
    {
        return m_rtpmap;
    }

    inline void SetRtpMap( const std::map< std::string, std::string > &rtpmap )
    {
        m_rtpmap = rtpmap;
    }

    inline const std::string& GetSSrc() const
    {
        return m_ssrc;
    }

    inline void SetSSrc( const std::string &ssrc )
    {
        m_ssrc = ssrc;
    }

    inline const std::string& GetAudioProperty() const
    {
        return m_audioproterty;
    }

    inline void SetAudioProperty( const std::string &audio )
    {
        m_audioproterty = audio;
    }

private:
    std::string                          m_recvid;			// o
    std::string                          m_sessionid;		// o
    std::string                          m_version;			// o
    std::string                          m_mediaip;			// o
    std::string                          m_connect;			// c
    std::string                          m_starttime;		// t
    std::string                          m_endtime;			// t
    std::string                          m_mediatype;		// m
    std::string                          m_mediaport;		// m
    std::string							 m_transport;		// m
    std::map< std::string, std::string > m_rtpmap;			// m & a
    std::string                          m_ssrc;			// y
    std::string                          m_audioproterty;   // f

protected:
	resip::InviteSession* m_handle;
};

//////////////////////////////////////////////////////////////////////////
class CByeMsg : public CGBMessage
{
public:
	CByeMsg() : m_handle( NULL )
	{
	}

public:
	inline resip::InviteSession* GetHandle()
	{
		return m_handle;
	}

	inline const resip::InviteSession* GetHandle() const
	{
		return m_handle;
	}

	inline void SetHandle( uint64_t h )
	{
		m_handle = (resip::InviteSession*)( h );
	}

	inline void SetHandle( resip::InviteSession *h )
	{
		m_handle = h;
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

protected:
	resip::InviteSession* m_handle;
};

//////////////////////////////////////////////////////////////////////////
#endif // MSG_INVITE_REQUEST_HPP_
