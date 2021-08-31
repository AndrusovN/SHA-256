#include "HasherSHA256.h"
#include <chrono>
using namespace HashFunctions;

int main(int argc, char* argv[]) {
	while (true)
	{
		std::string word;
		std::cout << "Enter word to make hash:\n";
		std::cin >> word;
		std::cout << HasherSHA256::hash_fast(word) << std::endl;
	}

}