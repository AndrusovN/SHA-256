#ifndef HASHER
#define HASHER

#include "BitArray.h"

namespace HashFunctions {
	class HasherSHA256
	{
	public:
		static std::string hash(std::string source);
		static std::string hash_fast(std::string source);
	private:
		static std::string toHex(unsigned char value);

		static unsigned int rightrotate(unsigned int source, unsigned int offset);
		static unsigned int leftrotate(unsigned int source, unsigned int offset);
	};
}


#endif
