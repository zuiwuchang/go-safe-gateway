#pragma once

template<typename T>
class singleton:boost::noncopyable
{
private:
	static T& get()
	{
		static T t;
		return t;
	}
public:
	static const T & get_const_instance()
	{
		return get();
	}
	static T & get_mutable_instance()
	{
		return get();
	}
};