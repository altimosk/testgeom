#ifndef TOKENS_H
#define TOKENS_H
#include <iostream>
#include <string>

class Tokens
{
public:
	Tokens(std::istream& i) : in(i) {
		}
	void AddDelims(std::string const & d) {
		delim += d;	}
	Tokens& operator>> (std::string& str);
	bool Good() {
		return in.good();
	}
private:
	std::istream & in;
	std::string delim;
};
#endif
