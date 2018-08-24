#ifndef MSG_DEVICE_CONTROL_MSG_HPP_
#define MSG_DEVICE_CONTROL_MSG_HPP_
#include "MessageMsg.hpp"

//////////////////////////////////////////////////////////////////////////
class CPTZControlMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CPTZControlMsg()
	{
	}

	void setMsgProperties(const CPTZControlMsg &msg)
	{
		m_ptzCmd = msg.GetPTZCmd();
		m_priority = msg.GetPriority();

		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetPTZCmd() const
	{
		return m_ptzCmd;
	}

	inline const std::string& GetPriority() const
	{
		return m_priority;
	}

	inline void SetPTZCmd( const std::string &cmd )
	{
		m_ptzCmd = cmd;
	}

	inline void SetPriority( const std::string &priority )
	{
		m_priority = priority;
	}

	inline void SetPriority( int priority )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", priority );
		m_priority = buf;
	}

private:
	std::string m_ptzCmd;
	std::string m_priority;
};

//////////////////////////////////////////////////////////////////////////
class CPTZLockMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CPTZLockMsg()
	{
	}

	void setMsgProperties(const CPTZLockMsg &msg)
	{
		m_lock = msg.m_lock;
		m_period = msg.m_period;

		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline const std::string& GetLock() const
	{
		return m_lock;
	}

	inline const std::string& GetPeriod() const
	{
		return m_period;
	}

	inline bool IsLock() const
	{
		return( m_lock == "1" );
	}

	inline bool IsUnlock() const
	{
		return( m_lock == "2" );
	}

	inline void Lock()
	{
		m_lock = "1";
	}

	inline void Unlock()
	{
		m_lock = "2";
	}

	inline void SetPeriod( const std::string &period )
	{
		m_period = period;
	}

	inline void SetPeriod( int period )
	{
		char buf[16] = { 0 };
		sprintf( buf, "%d", period );
		m_period = buf;
	}

private:
	std::string m_lock;
	std::string m_period;
};

//////////////////////////////////////////////////////////////////////////
class CRecordControlMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CRecordControlMsg()
	{
	}

	void setMsgProperties(const CRecordControlMsg &msg)
	{
		m_record = msg.IsRecord();

		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetRecord()
	{
		m_record = true;
	}

	inline void StopRecord()
	{
		m_record = false;
	}

	inline bool IsRecord() const
	{
		return m_record;
	}

	inline bool IsStopRecord() const
	{
		return !m_record;
	}

private:
	bool m_record;
};

//////////////////////////////////////////////////////////////////////////
class CTeleBootMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CTeleBootMsg()
	{
	}

	void setMsgProperties(const CTeleBootMsg &msg)
	{
		m_cmd = msg.GetCmd();

		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetCmd( const std::string &cmd )
	{
		m_cmd = cmd;
	}

	inline const std::string& GetCmd() const
	{
		return m_cmd;
	}

private:
	std::string m_cmd;
};

//////////////////////////////////////////////////////////////////////////
class CResetAlarmMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CResetAlarmMsg()
	{
	}

	void setMsgProperties(const CResetAlarmMsg &msg)
	{
		m_cmd = msg.GetCmd();
		m_method = msg.GetAlarmMethod();

		SetDeviceID(msg.GetDeviceID());
		SetSN( msg.GetSN() );
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetCmd( const std::string &cmd )
	{
		m_cmd = cmd;
	}

	inline const std::string& GetCmd() const
	{
		return m_cmd;
	}

	inline void SetAlarmMethod( int method )
	{
		m_method = method;
	}

	inline int GetAlarmMethod() const
	{
		return m_method;
	}

private:
	std::string m_cmd;
	int         m_method;
};

//////////////////////////////////////////////////////////////////////////
class CGuardMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CGuardMsg()
	{
	}

	void setMsgProperties(const CGuardMsg &msg)
	{
		m_bSetGuard = msg.GetGuard();

		SetDeviceID(msg.GetDeviceID());
		SetSN( msg.GetSN() );
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline void SetGuard()
	{
		m_bSetGuard = true;
	}

	inline void ResetGuard()
	{
		m_bSetGuard = false;
	}

	inline bool IsSetGuard() const
	{
		return m_bSetGuard;
	}

	inline bool IsResetGuard() const
	{
		return( !m_bSetGuard );
	}

	inline bool GetGuard() const
	{
		return m_bSetGuard;
	}

private:
	bool m_bSetGuard;
};

//////////////////////////////////////////////////////////////////////////
class CDragZoomInMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CDragZoomInMsg()
	{
	}

	void setMsgProperties(const CDragZoomInMsg &msg)
	{
		m_length = msg.GetLength();
		m_width = msg.GetWidth();
		m_midPointX = msg.GetMidPointX();
		m_midPointY = msg.GetMidPointY();
		m_lengthX = msg.GetLengthX();
		m_lengthY = msg.GetLengthY();

		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	inline int GetLength() const
	{
		return m_length;
	}

	inline int GetWidth() const
	{
		return m_width;
	}

	inline int GetMidPointX() const
	{
		return m_midPointX;
	}

	inline int GetMidPointY() const
	{
		return m_midPointY;
	}

	inline int GetLengthX() const
	{
		return m_lengthX;
	}

	inline int GetLengthY() const
	{
		return m_lengthY;
	}

	inline void SetLength( int length )
	{
		m_length = length;
	}

	inline void SetWidth( int width )
	{
		m_width = width;
	}

	inline void SetMidPointX( int midPointX )
	{
		m_midPointX = midPointX;
	}

	inline void SetMidPointY( int midPointY )
	{
		m_midPointY = midPointY;
	}

	inline void SetLengthX( int lengthX )
	{
		m_lengthX = lengthX;
	}

	inline void SetLengthY( int lengthY )
	{
		m_lengthY = lengthY;
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

private:
	int m_length;	 // 播放窗口长度像素值
	int m_width;	 // 播放窗口宽度像素值
	int m_midPointX; // 拉框中心的横轴坐标像素值
	int m_midPointY; // 拉框中心的纵轴坐标像素值
	int m_lengthX;	 // 拉框长度像素值
	int m_lengthY;	 // 拉框宽度像素值
};

//////////////////////////////////////////////////////////////////////////
class CDragZoomOutMsg : public CMessageMsgT< CMessageMsg::MsgCmdControl >
{
public:
	CDragZoomOutMsg()
	{
	}

	void setMsgProperties(const CDragZoomOutMsg &msg)
	{
		m_length = msg.GetLength();
		m_width = msg.GetWidth();
		m_midPointX = msg.GetMidPointX();
		m_midPointY = msg.GetMidPointY();
		m_lengthX = msg.GetLengthX();
		m_lengthY = msg.GetLengthY();

		SetDeviceID(msg.GetDeviceID());
		SetSN(msg.GetSN());
		SetTo(msg.GetTo());
	}

public:
	virtual bool Encode( std::string &message );
	virtual bool Decode( const std::vector< XMLNode* > &nodes, std::string &reason );
	virtual void Process( CSipInterface *pInterface );

public:
	inline int GetLength() const
	{
		return m_length;
	}

	inline int GetWidth() const
	{
		return m_width;
	}

	inline int GetMidPointX() const
	{
		return m_midPointX;
	}

	inline int GetMidPointY() const
	{
		return m_midPointY;
	}

	inline int GetLengthX() const
	{
		return m_lengthX;
	}

	inline int GetLengthY() const
	{
		return m_lengthY;
	}

	inline void SetLength( int length )
	{
		m_length = length;
	}

	inline void SetWidth( int width )
	{
		m_width = width;
	}

	inline void SetMidPointX( int midPointX )
	{
		m_midPointX = midPointX;
	}

	inline void SetMidPointY( int midPointY )
	{
		m_midPointY = midPointY;
	}

	inline void SetLengthX( int lengthX )
	{
		m_lengthX = lengthX;
	}

	inline void SetLengthY( int lengthY )
	{
		m_lengthY = lengthY;
	}

private:
	int m_length;	 // 播放窗口长度像素值
	int m_width;	 // 播放窗口宽度像素值
	int m_midPointX; // 拉框中心的横轴坐标像素值
	int m_midPointY; // 拉框中心的纵轴坐标像素值
	int m_lengthX;	 // 拉框长度像素值
	int m_lengthY;	 // 拉框宽度像素值
};

//////////////////////////////////////////////////////////////////////////
class CDeviceControlResponseMsg : public CMessageMsgT< CMessageMsg::MsgCmdResponse >
{
public:
	CDeviceControlResponseMsg()
	{
	}

	void setMsgProperties(const CDeviceControlResponseMsg &msg)
	{
		m_result = msg.GetResult();
		m_cmdtype = msg.m_cmdtype;

		SetDeviceID(msg.GetDeviceID());
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

//////////////////////////////////////////////////////////////////////////
#endif // MSG_DEVICE_CONTROL_MSG_HPP_