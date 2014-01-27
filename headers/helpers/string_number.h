



#define HELPERS 1

template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

  template <typename T>
  T StringToNumber ( const std::string &Text )
  {
     istringstream ss(Text);
     T result;
     return ss >> result ? result : 0;
  }

template<typename T> void NumbersToStrings(std::string * dest, T * args, size_t nargs)
	{
	for (unsigned int i = 0; i < nargs; i++)
		{
		dest[i] = NumberToString<T>(args[i]);
		}
	}

// takes a string like "$440" and returns an integer or float
template<typename T>T dollarsToNumber(std::string str)
	{
		return StringToNumber<T>(str.substr(1));
	}




