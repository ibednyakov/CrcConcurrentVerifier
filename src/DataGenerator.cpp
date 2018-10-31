/*
 * DataGenerator.cpp
 *
 *  Created on: 26 окт. 2018 г.
 *      Author: ivan
 */

#include "DataGenerator.h"

#include <ctime>

namespace CRCVerifier
{

namespace
{
	constexpr uint32_t SizeOfInt = sizeof(int32_t);
	constexpr uint32_t SizeOfByte = 8;
}

DataGenerator::DataGenerator(unsigned long blockSize) noexcept
	: _blockSize(blockSize)
{
	std::srand(std::time(nullptr));
}

void DataGenerator::generate(std::vector<int8_t>& result) const
{
	result.resize(_blockSize);

	for (uint i = 0; i < _blockSize / SizeOfInt; ++i)
	{
		int* dataIterator = reinterpret_cast<int*>(&result[i*SizeOfInt]);
		*dataIterator = std::rand();
	}
	const auto reminder = _blockSize % SizeOfInt;
	if (reminder)
	{
		const int finalPart = std::rand();
		std::vector<int8_t>::reverse_iterator iter = result.rbegin();
		for (uint i = 0; i < reminder; ++i, ++iter)
		{
			*iter = finalPart << i*SizeOfByte;
		}
	}
}


}
