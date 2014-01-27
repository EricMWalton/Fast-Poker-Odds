#define MY_STRING 1

#include <sstream>

// extracts a substring of str containing the chars between start and stop (exclusive-exclusive)
std::string extractBetween(std::string str, char start, char stop, size_t offset = 0)
	{
	size_t left = str.find(start, offset) + 1;
	size_t right = str.find(stop, left);

	if (left == std::string::npos || right == std::string::npos || right <= left)
		return "";
	else
		return str.substr(left, right - left);
	}

std::vector<std::string> split(const std::string &s, char delim) 
	{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) 
		{
        elems.push_back(item);
		}
    return elems;
	}