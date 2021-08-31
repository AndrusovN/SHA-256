#include "HasherSHA256.h"

std::string HashFunctions::HasherSHA256::hash(std::string source)
{
    BitArray workingArray = BitArray(source);
    bool* singleOne = new bool(true);
    //std::cout << "generated\n";
    workingArray += BitArray(singleOne, 1);
    delete singleOne;
    //std::cout << "added\n";
    int count = workingArray.getSize() / 512 + 1;

    int neededSize = count * 512 - sizeof(long long) * BYTE_SIZE;
    workingArray = workingArray.FillToSize(neededSize) + BitArray((long long)(workingArray.getSize() - 1));
    
    int hashValues[8] = {
        #include "constants.h"
    };

    BitArray *hashValuesBA = new BitArray[8];
    for (int i = 0; i < 8; i++)
    {
        hashValuesBA[i] = BitArray(hashValues[i]);
    }

    int roundConstants[64] = {
        #include "roundconstants.h"
    };

    BitArray* roundConstantBA = new BitArray[64];
    for (int i = 0; i < 64; i++)
    {
        roundConstantBA[i] = BitArray(roundConstants[i]);
    }
    //std::cout << "start main\n";
    for (int i = 0; i < count; i++)
    {
        BitArray current = workingArray.Cut(i * 512, (i + 1) * 512);
        current = current.FillToSize(32 * 64);

        BitArray* W = new BitArray[64];
        //std::cout << "loop\n";
        for (int j = 0; j < 64; j++)
        {
            W[j] = current.Cut(j * 32, (j + 1) * 32);
        }

        for (int j = 16; j < 64; j++)
        {
            BitArray S0 = (W[j - 15] >>= 7) ^ (W[j - 15] >>= 18) ^ (W[j - 15] >> 3);
            BitArray S1 = (W[j - 2] >>= 17) ^ (W[j - 2] >>= 19) ^ (W[j - 2] >> 10);
            W[j] = W[j - 16].Sum(S0).Sum(W[j - 7]).Sum(S1);
        }

        BitArray* compressionValues = new BitArray[8];
        for (int j = 0; j < 8; j++)
        {
            compressionValues[j] = hashValuesBA[j].DeepCopy();
        }

        for (int j = 0; j < 64; j++)
        {
            BitArray S1 = (compressionValues[4] >>= 6) ^ (compressionValues[4] >>= 11) ^ (compressionValues[4] >>= 25);
            BitArray ch = (compressionValues[4] && compressionValues[5]) ^ (!compressionValues[4] && compressionValues[6]);
            BitArray temp1 = compressionValues[7].Sum(S1).Sum(ch).Sum(roundConstantBA[j]).Sum(W[j]);
            BitArray S0 = (compressionValues[0] >>= 2) ^ (compressionValues[0] >>= 13) ^ (compressionValues[0] >>= 22);
            BitArray maj = (compressionValues[0] && compressionValues[1]) ^ (compressionValues[0] && compressionValues[2]) ^ (compressionValues[1] && compressionValues[2]);
            BitArray temp2 = S0.Sum(maj);
            compressionValues[7] = compressionValues[6];
            compressionValues[6] = compressionValues[5];
            compressionValues[5] = compressionValues[4];
            compressionValues[4] = compressionValues[3].Sum(temp1);
            compressionValues[3] = compressionValues[2];
            compressionValues[2] = compressionValues[1];
            compressionValues[1] = compressionValues[0];
            compressionValues[0] = temp1.Sum(temp2);
        }

        for (int j = 0; j < 8; j++)
        {
            hashValuesBA[j] = hashValuesBA[j].Sum(compressionValues[j]);
        }

        delete[] W;
    }

    std::string result = "";
    for (int i = 0; i < 8; i++)
    {
        result += hashValuesBA[i].toHex();
    }
    delete[] hashValuesBA;
    delete[] roundConstantBA;


    return result;
}

std::string HashFunctions::HasherSHA256::hash_fast(std::string source)
{
    unsigned int chunksCount = (source.length() + sizeof(long long)) / 64 + 1;
    unsigned char* data = new unsigned char[chunksCount * 64];
    for (int i = 0; i < source.length(); i++)
    {
        data[i] = source[i];
    }
    data[source.length()] = 0x80;
    for (int i = source.length() + 1; i < chunksCount * 64 - sizeof(long long); i++)
    {
        data[i] = 0x00;
    }
    unsigned long long size = source.length();
    for (int i = 0; i < sizeof(long long); i++)
    {
        data[chunksCount * 64 - sizeof(long long) + i] = ((unsigned char*)(&size))[i];
    }

    unsigned int hashValues[8] = {
        #include "constants.h"
    };
    unsigned int roundConstants[64] = {
        #include "roundconstants.h"
    };

    unsigned int* values = (unsigned int *)data;

    for (int i = 0; i < chunksCount; i++)
    {
        unsigned int* currentChunk = new unsigned int[64];
        for (int j = 0; j < 16; j++)
        {
            currentChunk[j] = values[i * 16 + j];
        }
        for (int j = 16; j < 64; j++)
        {
            unsigned int S0 = rightrotate(currentChunk[j - 15], 7) ^ rightrotate(currentChunk[j - 15], 18) ^ (currentChunk[j - 15] >> 3);
            unsigned int S1 = rightrotate(currentChunk[j - 2], 17) ^ rightrotate(currentChunk[j - 2], 19) ^ (currentChunk[j - 2] >> 10);
            currentChunk[j] = currentChunk[j - 16] + S0 + currentChunk[j - 7] + S1;
        }

        unsigned int* hashValuesTmp = new unsigned int[8];

        for (int j = 0; j < 8; j++)
        {
            hashValuesTmp[j] = hashValues[j];
        }

        for (int j = 0; j < 64; j++)
        {
            unsigned int S1 = rightrotate(hashValuesTmp[4], 6) ^ rightrotate(hashValuesTmp[4], 11) ^ rightrotate(hashValuesTmp[4], 25);
            unsigned int ch = (hashValuesTmp[4] & hashValuesTmp[5]) ^ (~hashValuesTmp[4] & hashValuesTmp[6]);
            unsigned int temp1 = hashValuesTmp[7] + S1 + ch + roundConstants[j] + currentChunk[j];
            unsigned int S0 = rightrotate(hashValuesTmp[0], 2) ^ rightrotate(hashValuesTmp[0], 2) ^ rightrotate(hashValuesTmp[0], 22);
            unsigned int maj = (hashValuesTmp[0] & hashValuesTmp[1]) ^ (hashValuesTmp[0] & hashValuesTmp[2]) ^ (hashValuesTmp[1] & hashValuesTmp[2]);
            unsigned int temp2 = S0 + maj;

            hashValuesTmp[7] = hashValuesTmp[6];
            hashValuesTmp[6] = hashValuesTmp[5];
            hashValuesTmp[5] = hashValuesTmp[4];
            hashValuesTmp[4] = hashValuesTmp[3] + temp1;
            hashValuesTmp[3] = hashValuesTmp[2];
            hashValuesTmp[2] = hashValuesTmp[1];
            hashValuesTmp[1] = hashValuesTmp[0];
            hashValuesTmp[0] = temp1 + temp2;

        }

        for (int j = 0; j < 8; j++)
        {
            hashValues[j] += hashValuesTmp[j];
        }

        delete[] currentChunk;
        delete[] hashValuesTmp;
    }

    std::string result = "";

    unsigned char* resultChar = (unsigned char*)hashValues;
    for (int i = 0; i < 32; i++)
    {
        result += toHex(resultChar[i]);

    }
    delete[] data;

    return result;
}

std::string HashFunctions::HasherSHA256::toHex(unsigned char value)
{
    std::string result = "";
    result += HEX_SYMBOLS[value / 16];
    result += HEX_SYMBOLS[value % 16];
    return result;
}

unsigned int HashFunctions::HasherSHA256::rightrotate(unsigned int source, unsigned int offset)
{
    offset %= sizeof(source) * BYTE_SIZE;
    return ((source >> offset) | (source << (sizeof(source) * BYTE_SIZE - offset)));
}

unsigned int HashFunctions::HasherSHA256::leftrotate(unsigned int source, unsigned int offset)
{
    offset %= sizeof(source) * BYTE_SIZE;
    return ((source << offset) | (source >> (sizeof(source) * BYTE_SIZE - offset)));
}
