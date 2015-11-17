#include "tokens.h"
#include <cctype>

bool SkipSpaces(std::istream& in)
{
	while(in.good())
	{
		int c = in.peek();
		if (!std::isspace(c))
			return in.good();
		in.get();
	}
	return in.good();
}

Tokens& Tokens::operator>>(std::string& str)
{
	str.erase();
	if (SkipSpaces(in))
	{
		while (in.good())
		{
			int c = in.peek();
			if (!in.good() || std::isspace(c) )
				break;
			char ch = char(c);
			bool is_delim = (delim.find(ch) != std::string::npos);
			if (!is_delim || str.empty())
			{
				str.push_back(ch);
				in.get();
			}
			if( is_delim)
				break;
		}
	}
	return *this;
}
