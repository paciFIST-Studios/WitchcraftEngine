#ifndef ENGINE_ID_H
#define ENGINE_ID_H

namespace witchcraft
{
	namespace engine
	{
		// Produces a unique id, for every fn call, starting at the configured
		// offset witchcraft::configuration::engine_id_offset
		static unsigned int get_engine_id()
		{
			// EB - 20200922 
			// we want to allow people to send messages to and from their engine id,
			// so in order to manager that, make sure we leave a blank spot for named
			// message channels.  See how many we're using in message_bus.h
			unsigned int const engine_id_offset = 100;
			// EB - 20200922 

			static unsigned int id = engine_id_offset;
			return id++;
		}
	}
}

#endif // !ENGINE_ID_H
