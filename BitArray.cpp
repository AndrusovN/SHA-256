#include "BitArray.h"
using namespace HashFunctions;

char HashFunctions::HEX_SYMBOLS[16] =
	{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

BitArray::BitArray()
{
	m_size = 0;
	m_data = NULL;
}

HashFunctions::BitArray::BitArray(char source)
{
	m_size = sizeof(source) * BYTE_SIZE;
	m_data = new bool[m_size];
	m_data[0] = (source >= 0);

	long long treshold = 1;
	for (int i = 1; i < m_size; i++) {
		treshold *= 2;
	}

	char copy = source;

	for (int i = 1; i < m_size; i++)
	{
		if (copy > 0) {
			m_data[i] = copy > treshold;
			if (m_data[i]) {
				copy -= treshold;
			}
		}
		else {
			m_data[i] = copy <= -treshold;
			if (m_data[i]) {
				copy += treshold;
			}
		}
		treshold /= 2;
	}
}

BitArray::BitArray(std::string source)
{
	m_size = source.length() * BYTE_SIZE;
	m_data = new bool[m_size];

	for (int i = 0; i < source.length(); i++)
	{
		BitArray part = BitArray(source[i]);

		for (int j = 0; j < part.getSize(); j++) {
			m_data[i * BYTE_SIZE + j] = part.getAt(j);
		}
	}
}

BitArray::BitArray(int source)
{
	m_size = sizeof(source) * BYTE_SIZE;
	m_data = new bool[m_size];
	m_data[0] = (source >= 0);

	long long treshold = 1;
	for (int i = 1; i < m_size; i++) {
		treshold *= 2;
	}

	int copy = source;

	for (int i = 1; i < m_size; i++)
	{
		if (copy > 0) {
			m_data[i] = copy > treshold;
			if (m_data[i]) {
				copy -= treshold;
			}
		}
		else {
			m_data[i] = copy <= -treshold;
			if (m_data[i]) {
				copy += treshold;
			}
		}
		treshold /= 2;
	}
}

BitArray::BitArray(long long source)
{
	m_size = sizeof(source) * BYTE_SIZE;
	m_data = new bool[m_size];
	m_data[0] = (source >= 0);

	long long treshold = 1;
	for (int i = 2; i < m_size; i++) {
		treshold *= 2;
	}

	long long copy = source;

	for (int i = 1; i < m_size; i++)
	{
		if (copy > 0) {
			m_data[i] = copy > treshold;
			if (m_data[i]) {
				copy -= treshold;
			}
		}
		else {
			m_data[i] = copy <= -(long long)treshold;
			if (m_data[i]) {
				copy += treshold;
			}
		}
		treshold /= 2;
	}
}

BitArray::BitArray(bool* array, unsigned int size)
{
	m_data = new bool[size];
	m_size = size;

	for (unsigned int index = 0; index < m_size; index++)
	{
		m_data[ index ] = array[ index ];
	}
}

HashFunctions::BitArray::~BitArray()
{
	//std::cout << toHex() << std::endl;
	//delete[] m_data;
}

BitArray BitArray::FillToSize(unsigned int neededSize) const
{
	if (m_size < neededSize) {
		bool* newData = new bool[neededSize];
		for (int i = 0; i < m_size; i++)
		{
			newData[i] = m_data[i];
		}
		for (int i = m_size; i < neededSize; i++) {
			newData[i] = false;
		}
		BitArray result = BitArray(newData, neededSize);
		//delete[] newData;

		return result;
	}
	else
	{
		return DeepCopy();
	}
}

BitArray HashFunctions::BitArray::Cut(unsigned int begin, unsigned int end)
{
	if (begin >= end) {
		throw std::exception("Begin should be less than end");
	}
	if (end > m_size) {
		throw std::exception("Index out of range exception");
	}

	bool* nData = new bool[end - begin];
	for (int i = begin; i < end; i++)
	{
		nData[i - begin] = m_data[i];
	}

	BitArray result = BitArray(nData, end - begin);
	////delete[] nData;

	return result;
}

std::string HashFunctions::BitArray::toString()
{
	std::string result = "";
	for (unsigned int i = 0; i < m_size; i++)
	{
		result += m_data[i] ? '1' : '0';
	}
	return result;
}

std::string HashFunctions::BitArray::toHex()
{
	std::string result = "";
	int count = m_size / 4;

	for (int i = 0; i < count; i++)
	{
		int p = 0;
		int k = 8;
		for (int j = 0; j < 4; j++)
		{
			if ( m_data[ i * 4 + j ] ) {
				p += k;
			}
			k /= 2;
		}

		result += HEX_SYMBOLS[p];
	}
	return result;
}

std::string HashFunctions::BitArray::toChars()
{
	std::string result = "";
	int count = m_size / BYTE_SIZE;

	for (int i = 0; i < count; i++)
	{
		unsigned char r = 0;
		int k = 128;
		for (int j = 0; j < BYTE_SIZE; j++)
		{
			if (m_data[i * BYTE_SIZE + j]) {
				r += k;
			}
			k /= 2;
		}
		result += r;
	}
	return result;
}

void HashFunctions::BitArray::operator=(const BitArray& other)
{
	m_size = other.m_size;
	m_data = new bool[m_size];
	for (int i = 0; i < m_size; i++)
	{
		m_data[i] = other.m_data[i];
	}
}

BitArray BitArray::operator>>(const unsigned int offset) const {
	bool* nData = new bool[m_size];
	for (int i = m_size - offset - 1; i >= 0; i--)
	{
		nData[i + offset] = nData[i];
	}
	for (int i = 0; i < offset; i++)
	{
		nData[i] = false;
	}
	BitArray result = BitArray(nData, m_size);
	////delete[] nData;

	return result;
}

BitArray BitArray::operator<<(const unsigned int offset) const
{
	bool* nData = new bool[m_size];
	for (int i = offset; i < m_size; i++)
	{
		nData[i - offset] = nData[i];
	}
	for (int i = m_size - offset - 1; i < m_size; i++)
	{
		nData[i] = false;
	}
	BitArray result = BitArray(nData, m_size);
	////delete[] nData;

	return result;
}

BitArray BitArray::operator>>=(const unsigned int offset) const
{
	bool* nData = new bool[m_size];
	for (int i = 0; i < m_size; i++) {
		nData[(i + offset) % m_size] = m_data[i];
	}
	BitArray result = BitArray(nData, m_size);
	//delete[] nData;

	return result;
}

BitArray BitArray::operator<<=(const unsigned int offset) const
{
	bool* nData = new bool[m_size];
	for (int i = 0; i < m_size; i++) {
		nData[(m_size + i - offset) % m_size] = m_data[i];
	}
	BitArray result = BitArray(nData, m_size);
	//delete[] nData;

	return result;
}

BitArray BitArray::operator+(const BitArray& source) const
{
	unsigned int nSize = m_size + source.m_size;
	bool* nData = new bool[nSize];
	for (int i = 0; i < m_size; i++)
	{
		nData[i] = m_data[i];
	}
	for (int i = 0; i < source.m_size; i++)
	{
		nData[m_size + i] = source.m_data[i];
	}

	BitArray result = BitArray(nData, nSize);
	//delete[] nData;

	return result;
}

void BitArray::operator+=(const BitArray& source)
{
	bool* nData = new bool[m_size + source.m_size];
	for (int i = 0; i < m_size; i++)
	{
		nData[i] = m_data[i];
	}
	for (int i = 0; i < source.m_size; i++)
	{
		nData[m_size + i] = source.m_data[i];
	}
	//delete[] m_data;
	m_data = nData;
	m_size += source.m_size;
}

BitArray BitArray::operator!() const
{
	bool* nData = new bool[m_size];
	for (int i = 0; i < m_size; i++)
	{
		nData[i] = !m_data[i];
	}
	BitArray result = BitArray(nData, m_size);
	//delete[] nData;

	return result;
}

BitArray BitArray::operator&&(const BitArray& second) const
{
	int max = m_size > second.m_size ? m_size : second.m_size;
	int min = m_size > second.m_size ? second.m_size : m_size;

	bool* nData = new bool[max];

	for (int i = 0; i < min; i++)
	{
		nData[i] = m_data[i] && second.m_data[i];
	}

	for (int i = min; i < max; i++)
	{
		nData[i] = false;
	}
	BitArray result = BitArray(nData, max);
	//delete[] nData;
	return result;
}

BitArray BitArray::operator||(const BitArray& second) const
{
	int max = m_size > second.m_size ? m_size : second.m_size;
	BitArray f = this->FillToSize(max);
	BitArray s = second.FillToSize(max);

	bool* nData = new bool[max];

	for (int i = 0; i < max; i++)
	{
		nData[i] = f.m_data[i] || s.m_data[i];
	}
	BitArray result = BitArray(nData, max);
	//delete[] nData;

	return result;
}

BitArray BitArray::operator^(const BitArray& second) const
{
	int max = m_size > second.m_size ? m_size : second.m_size;
	BitArray f = this->FillToSize(max);
	BitArray s = second.FillToSize(max);

	bool* nData = new bool[max];

	for (int i = 0; i < max; i++)
	{
		nData[i] = f.m_data[i] ^ s.m_data[i];
	}
	BitArray result = BitArray(nData, max);
	//delete[] nData;

	return result;
}

BitArray HashFunctions::BitArray::DeepCopy() const
{
	return BitArray(m_data, m_size);
}

BitArray HashFunctions::BitArray::Sum(BitArray source)
{
	unsigned int max = m_size > source.m_size ? m_size : source.m_size;
	unsigned int min = m_size + source.m_size - max;

	bool* nData = new bool[max];
	for (int i = 0; i < m_size; i++)
	{
		nData[i] = m_data[i];
	}
	for (int i = m_size; i < max; i++)
	{
		nData[i] = false;
	}

	for (int i = 0; i < source.m_size; i++)
	{
		
		if (source.m_data[source.m_size - i - 1]) {
			int k = i;
			while (nData[max - k - 1] && max - k - 1 > 0)
			{
				nData[max - k - 1] = false;
				k++;
			}
			nData[max - k - 1] = source.m_data[source.m_size - i - 1] ^ nData[max - k - 1];
		}
	}

	BitArray result = BitArray(nData, max);
	//delete[] nData;
	return result;
}

unsigned int BitArray::getSize()
{
	return m_size;
}

bool* HashFunctions::BitArray::getData()
{
	return m_data;
}

bool HashFunctions::BitArray::getAt(unsigned int position)
{
	if (position >= m_size) {
		throw std::exception("Out of range exception!");
		return false;
	}
	return m_data[position];
}
