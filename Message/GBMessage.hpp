#ifndef GBMESSAGE_HPP_
#define GBMESSAGE_HPP_

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "tinyxml2.h"
#include "Log.hpp"
#include "Message.hpp"
#include "resip/dum/ServerPagerMessage.hxx"

#if defined(CATCH_RESIP_EXCEPTION)
#undef __DUM_TRY
#undef __DUM_CATCH
#define __DUM_TRY try {
#define __DUM_CATCH \
} catch(resip::DumException* be) { \
	resip::Data ex; \
	ex = "RESIP ³öÏÖÒì³££ºname = "; \
	ex += be->name(); \
	ex += "£¬what = "; \
	ex += be->what(); \
	throw new std::exception(ex.c_str()); \
}
#else
#undef  __DUM_TRY
#undef  __DUM_CATCH
#define __DUM_TRY
#define __DUM_CATCH
#endif
using namespace tinyxml2;

//////////////////////////////////////////////////////////////////////////
class CGBMessage : public CMessage
{
public:
	enum EGBResultType
	{
		RESULT_OK = 0,
		RESULT_ERROR,
	};

	struct SDate
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		int millisecond;
	};

	struct SSubject
	{
		std::string id;
		std::string seq;
	};

	struct SUserIdentity
	{
		std::string user;
		std::string deviceid;
		std::string organization;
		std::string policetype;
		std::string duty;
	};

public:
	CGBMessage() : m_adjusttime( false ), m_port( 0 ), m_cseq( 0 ), m_forwards( 0 ), m_expires( 0 ), m_statusCode( 0 )
	{
	}

	virtual ~CGBMessage()
	{
	}

private:
	CGBMessage( const CGBMessage &rhs );
	CGBMessage& operator=( const CGBMessage &rhs );

public:
	inline bool Ok() const
	{
		return( m_statusCode == 200 );
	}

	inline bool AdjustTime() const
	{
		return m_adjusttime;
	}

	inline const std::string& GetSubjectSendID() const
	{
		return m_sender.id;
	}

	inline const std::string& GetSubjectSendSeq() const
	{
		return m_sender.seq;
	}

	inline const std::string& GetSubjectRecvID() const
	{
		return m_recver.id;
	}

	inline const std::string& GetSubjectRecvSeq() const
	{
		return m_recver.seq;
	}

	inline const std::string& GetUser() const
	{
		return m_request;
	}

	inline const std::string& GetSubject() const
	{
		return m_subject;
	}

	inline const std::string& GetFrom() const
	{
		return m_from;
	}

	inline const std::string& GetFromTag() const
	{
		return m_tagfrom;
	}

	inline const std::string& GetTo() const
	{
		return m_to;
	}

	inline const std::string& GetToTag() const
	{
		return m_tagto;
	}

	inline unsigned long GetCSeq() const
	{
		return m_cseq;
	}

	inline int GetStatusCode() const
	{
		return m_statusCode;
	}

	inline int GetMaxFrowards() const
	{
		return m_forwards;
	}

	inline int GetExpires() const
	{
		return m_expires;
	}

	inline const std::string& GetIP() const
	{
		return m_ip;
	}

	inline int GetPort() const
	{
		return m_port;
	}

	inline const std::string& GetCallID() const
	{
		return m_callid;
	}

	inline const SDate& GetDate() const
	{
		return m_date;
	}

	inline const SUserIdentity& GetUserIdentity() const
	{
		return m_useridentity;
	}

	inline const std::vector< std::string >& GetContacts() const
	{
		return m_contacts;
	}
	
	inline const std::vector< std::string >& GetVias() const
	{
		return m_vias;
	}

	inline void SetSubjectSendID( const std::string &id )
	{
		m_sender.id = id;
	}

	inline void SetSubjectSendSeq( const std::string &seq )
	{
		m_sender.seq = seq;
	}

	inline void SetSubjectRecvID( const std::string &id )
	{
		m_recver.id = id;
	}

	inline void SetSubjectRecvSeq( const std::string &seq )
	{
		m_recver.seq = seq;
	}

	inline void SetUser( const char *user )
	{
		if( user != NULL )
		{
			m_request = user;
		}
	}

	inline void SetUser( const std::string &user )
	{
		m_request = user;
	}

	inline void SetSubject( const char *uri )
	{
		if( uri != NULL )
		{
			m_subject = uri;
			DecodeSubject();
		}
	}

	inline void SetSubject( const std::string &uri )
	{
		m_subject = uri;
		DecodeSubject();
	}

	inline void SetFrom( const char *code )
	{
		if( code != NULL )
		{
			m_from = code;
		}
	}

	inline void SetFrom( const std::string &code )
	{
		m_from = code;
	}

	inline void SetFromTag( const char *tag )
	{
		if( tag != NULL )
		{
			m_tagfrom = tag;
		}
	}

	inline void SetFromTag( const std::string &code )
	{
		m_tagfrom = code;
	}

	inline void SetTo( const char *code )
	{
		if( code != NULL )
		{
			m_to = code;
		}
	}

	inline void SetTo( const std::string &code )
	{
		m_to = code;
	}

	inline void SetToTag( const char *tag )
	{
		if( tag != NULL )
		{
			m_tagto = tag;
		}
	}

	inline void SetToTag( const std::string &tag )
	{
		m_tagto = tag;
	}

	inline void SetCSeq( unsigned long cseq )
	{
		m_cseq = cseq;
	}

	inline void SetExpires( int expires )
	{
		m_expires = expires;
	}

	inline void SetStatusCode( int code )
	{
		m_statusCode = code;
	}

	inline void SetMaxFrowards( int forwards )
	{
		m_forwards = forwards;
	}

	inline void SetCallID( const char *callid )
	{
		if( callid != NULL )
		{
			m_callid = callid;
		}
	}

	inline void SetCallID( const std::string &callid )
	{
		m_callid = callid;
	}

	inline void SetIP( const std::string &ip )
	{
		m_ip = ip;
	}

	inline void SetIP( const char *ip )
	{
		if( ip != NULL )
		{
			m_ip = ip;
		}
	}

	inline void SetPort( int port )
	{
		m_port = port;
	}

	inline void SetDate( int year, int month, int day, int hour, int minute, int second, int millisecond )
	{
		m_adjusttime = true;
		m_date.year   = year;
		m_date.month  = month;
		m_date.day    = day;
		m_date.hour   = hour;
		m_date.minute = minute;
		m_date.second = second;
		m_date.millisecond = millisecond;
	}

	inline void AddContacts( const char *contacts )
	{
		if( contacts != NULL )
		{
			m_contacts.push_back( contacts );
		}
	}

	inline void AddContacts( const std::string &contacts )
	{
		m_contacts.push_back( contacts );
	}

	inline void AddVias( const char *host, long port )
	{
		if( host != NULL )
		{
			char buf[16] = { 0 };
			sprintf( buf, "%d", port );
			std::string via = host;
			via += ":";
			via += buf;
			m_vias.push_back( via );
		}
	}

	inline void AddVias( const std::string &host, long port )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", port );
		std::string via = host;
		via += ":";
		via += buf;
		m_vias.push_back( via );
	}

	inline void SetUserIdentity( const std::string &user, const std::string& deviceid, const std::string& organization, const std::string& policetype, const std::string& duty )
	{
		m_useridentity.user = user;
		m_useridentity.deviceid = deviceid;
		m_useridentity.organization = organization;
		m_useridentity.policetype = policetype;
		m_useridentity.duty  = duty;
	}

public:
	virtual bool Encode( std::string &message )
	{
		return false;
	}

	virtual void Process( CSipInterface *pInterface )
	{
	}

	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false )
	{
		return false;
	}

	virtual bool Response( int status )
	{
		return false;
	}

public:
	static bool CompareNoCase( const char *type, const char *label );
	static bool CompareNoCase( const std::string &type, const char *label );
	static unsigned int GetNextSN();
	bool DecodeSubject();

protected:
	SSubject                    m_sender;	 // subject
	SSubject                    m_recver;	// subject
	std::string					m_request;
	std::string                 m_subject;
	std::string					m_callid;
	std::string					m_from;
	std::string					m_tagfrom;
	std::string					m_to;
	std::string					m_tagto;
	SDate                       m_date;
	std::vector< std::string >	m_contacts;
	std::vector< std::string >	m_vias;
	std::string                 m_ip;
	bool                        m_adjusttime;    // message has date header
	int                         m_port;
	unsigned long				m_cseq;
	int                         m_forwards;
	int                         m_expires;
	int                         m_statusCode;
	SUserIdentity               m_useridentity;
};

//////////////////////////////////////////////////////////////////////////
#endif // GBMESSAGE_HPP_
