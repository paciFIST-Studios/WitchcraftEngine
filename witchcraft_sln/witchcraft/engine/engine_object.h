#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H


class cEngineObject
{
private:
protected:
public:
	unsigned int id;

	cEngineObject()
		: id(0)
	{}

	cEngineObject(unsigned int const & ID)
		: id(ID)
	{}
};

#endif // !ENGINE_OBJECT_H
