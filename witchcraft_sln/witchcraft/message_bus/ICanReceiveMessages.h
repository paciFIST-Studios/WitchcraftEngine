#ifndef ICAN_RECEIVE_MESSAGES_H
#define ICAN_RECEIVE_MESSAGES_H

class Message;

class ICanReceiveMessages
{
public:
	virtual void onMessage(Message m) = 0;
}

#endif // !ICAN_RECEIVE_MESSAGES_H
