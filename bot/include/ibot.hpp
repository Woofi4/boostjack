#ifndef IBOT_H
#define IBOT_H

class ibot {
protected:
	unsigned score = 0;

	const char* hit() const
	{ return new const char[1] { 2 }; }

	const char* stand() const
	{ return new const char[1] { 3 }; }

public:
	virtual const char* handle(void* data = nullptr) = 0;

	virtual void reset() = 0;

	void add_score(unsigned s)
	{ score += s; }
};

#endif