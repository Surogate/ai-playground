
#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <typename T>
class Singleton {
public:
	static T& getInstance() {
		static T instance;

		return instance;
	}
protected:
	virtual ~Singleton() {};
	Singleton() {};
};

#define SINGLETON_CLASS(name) friend class Singleton< name >

#endif /* SINGLETON_HPP */