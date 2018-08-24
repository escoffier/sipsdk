#include "Message.hpp"
#include "MessageQueue.hpp"

void CMessage::Post()
{
	CMessageQueue::AddMessage( this );
}
