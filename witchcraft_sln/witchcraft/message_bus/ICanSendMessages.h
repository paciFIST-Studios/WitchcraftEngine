#ifndef ICAN_SEND_MESSAGES_H
#define ICAN_SEND_MESSAGES_H

class Message;

class ICanSendMessages
{
public:
	virtual void sendMessage(Message m) = 0;
};

#endif // !ICAN_SEND_MESSAGES_H
