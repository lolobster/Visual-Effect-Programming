#ifndef _SINGLETON_H_
#define _SINGLETON_H_



namespace core
{

template<class T>
class Singleton
{
private:
	static core::Ref<T> _instance;

public:
    // Returns the instance of this singleton.
	static T* getInstance()
    {
		if (_instance == 0)
        {
			_instance = new T();
        }

		return _instance;
    }

	static T& getRef()
    {
		return *getInstance();
    }

	static void deleteInstance()
	{
		_instance = 0;
	}

};

#define ENGINE_SINGLETON_CLASS_DECLARATION(className) template<> core::Ref<className> core::Singleton<className>::_instance = 0;

}


#endif

