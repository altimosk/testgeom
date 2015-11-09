#ifndef CLIWRAP_H
#define CLIWRAP_H
#include <Windows.h>

class GenericGraphics;

class ClientWrap
{
public:
	ClientWrap() : dll(0), ggf(0), daf(0) {
		}
	bool ReInit(GenericGraphics*);
	void DrawAll() 
	{
		if (daf)
			daf();
	}
	void Unwrap();
	~ClientWrap() {
	void Unwrap(); }
private:
	HMODULE dll;
	typedef void(*SetGGFunc)(GenericGraphics *); SetGGFunc ggf;
	typedef void(*DrawAllFunc)(); DrawAllFunc daf;
};
#endif
