#ifndef MSG_CALLINFO_MSG_HPP_
#define MSG_CALLINFO_MSG_HPP_

#include "GBMessage.hpp"
#include "resip/dum/InviteSession.hxx"

//////////////////////////////////////////////////////////////////////////
class CCallInfoMsg : public CGBMessage
{
public:
	CCallInfoMsg() : m_handle( NULL )
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

	inline void SetHandle( const resip::InviteSession *h )
	{
		m_handle = const_cast< resip::InviteSession* >( h );
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
class CCallInfoResponseMsg : public CCallInfoMsg
{
public:
	CCallInfoResponseMsg()
	{
	}

public:
	virtual bool Send( resip::DialogUsageManager &mDum, bool tcp = false );
	virtual bool Encode( std::string &message );
	virtual bool Decode( const char *body, std::string &reason );
	virtual void Process( CSipInterface *pInterface );
};

//////////////////////////////////////////////////////////////////////////
class CVodCtrlMsg : public CCallInfoMsg
{
public:
	enum EPlayType
	{
		PLAY_TYPE_PLAY      = 0,
		PLAY_TYPE_PAUSE     = 1,
		PLAY_TYPE_TEARDOWN  = 2,  // 国标文档中有定义,一般情况停止都是使用的BYE
	};

public:
	CVodCtrlMsg() : m_type( PLAY_TYPE_PLAY )
    {
    }

	void setMsgProperties(const CVodCtrlMsg &msg)
	{
		m_speed = msg.GetSpeed();
		m_pos = msg.GetPosition();
		m_seq = msg.GetSeq();
		m_type = msg.GetType();

		SetTo(msg.GetTo());
	}


public:
    virtual bool Encode( std::string &message );
    virtual bool Decode( const char *body, std::string &reason );
    virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetSeq() const
	{
		return m_seq;
	}

	inline void SetSeq( const std::string &seq )
	{
		m_seq = seq;
	}

    inline const std::string& GetSpeed() const
    {
		return m_speed;
    }

    inline void SetSpeed( const std::string& speed )
    {
		m_speed = speed;
    }

	inline const std::string& GetPosition() const
	{
		return m_pos;
	}

	inline void SetPosition( const std::string& pos )
	{
		m_pos = pos;
	}

	inline void SetPlay()
	{
		m_type = PLAY_TYPE_PLAY;
	}

	inline void SetPause()
	{
		m_type = PLAY_TYPE_PAUSE;
	}

	inline EPlayType GetType() const
	{
		return m_type;
	}

	inline bool IsPlay() const
	{
		return( m_type == PLAY_TYPE_PLAY );
	}

	inline bool IsPause() const
	{
		return( m_type == PLAY_TYPE_PAUSE );
	}

private:
	std::string  m_speed;
	std::string  m_pos;
	std::string  m_seq;
	EPlayType    m_type;
};

#endif // MSG_CALLINFO_MSG_HPP_
