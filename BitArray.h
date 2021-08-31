#ifndef BIT_ARRAY
#define BIT_ARRAY

#define BYTE_SIZE 8

#include <iostream>

namespace HashFunctions {
	extern char HEX_SYMBOLS[16];

	class BitArray
	{
	public:
		BitArray();
		BitArray(char source);
		BitArray(std::string source);
		BitArray(int source);
		BitArray(long long source);
		BitArray(bool* array, unsigned int size);

		~BitArray();

		BitArray FillToSize(unsigned int neededSize) const;
		BitArray Cut(unsigned int begin, unsigned int end);

		std::string toString();
		std::string toHex();
		std::string toChars();

		void operator =(const BitArray& other);

		BitArray operator >> (const unsigned int offset) const;
		BitArray operator << (const unsigned int offset) const;

		BitArray operator >>= (const unsigned int offset) const;
		BitArray operator <<= (const unsigned int offset) const;

		BitArray operator + (const BitArray& source) const;
		void operator += (const BitArray& source);

		BitArray operator !() const;
		BitArray operator && (const BitArray &second) const;
		BitArray operator || (const BitArray& second) const;
		BitArray operator ^ (const BitArray& second) const;

		BitArray DeepCopy() const;

		BitArray Sum(BitArray source);

		unsigned int getSize();
		bool* getData();

		bool getAt(unsigned int position);
	private:
		bool* m_data;
		unsigned int m_size;
	};
}


#endif

