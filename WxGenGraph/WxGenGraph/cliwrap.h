#ifndef CLIWRAP_H
#define CLIWRAP_H
#include <Windows.h>

class GenericGraphics;

class ClientWrap
{
public:
	ClientWrap() : dll(0), ggf(0) {
		}
	bool ReInit(GenericGraphics*);
	void Unwrap();
	~ClientWrap() {
	void Unwrap(); }
private:
	HMODULE dll;
	typedef void(*SetGGFunc)(GenericGraphics *); SetGGFunc ggf;
};
#endif
