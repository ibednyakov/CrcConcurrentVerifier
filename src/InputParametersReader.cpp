/*
 * InputParametersReader.cpp
 *
 *  Created on: 24 окт. 2018 г.
 *      Author: ivan
 */

#include "InputParametersReader.h"

#include <string>
#include <sstream>

namespace CRCVerifier
{

std::string InputParameterReader::_usageMessage = "Usage: CRCVerifier <blockSize> <blocksCount> <M> <N>\n       where M - thread A count, N - thread B count";

std::string InvalidParametersCount::_message = "Invalid count (expected/provided): ";

InvalidParametersCount::InvalidParametersCount(int expected, int provided)
	: _expected(expected)
	, _provided(provided)
{}

std::string InvalidParametersCount::what() noexcept
{
	std::stringstream ss;
	ss << _message;
	ss << _expected;
	ss << "/";
	ss << _provided;
	return ss.str();
}

InputParameterReader::InputParameterReader(int argc, char* argv[])
{
	if (argc != WellKnownParametersCount)
		throw InvalidParametersCount(WellKnownParametersCount, argc);
	parseParameters(argc, argv);
}

unsigned InputParameterReader::getSuppliersCount() const
{
	return _parameters.threadsACount;
}

unsigned InputParameterReader::getWorkersCount() const
{
	return _parameters.threadsBCount;
}

unsigned long InputParameterReader::getBlockSize() const
{
	return _parameters.blockSize;
}

unsigned long InputParameterReader::getBlocksCount() const
{
	return _parameters.blocksCount;
}

std::string InputParameterReader::getUsage()
{
	return _usageMessage;
}

void InputParameterReader::parseParameters(int argc, char* argv[])
{
	_parameters.blockSize = std::stoul(argv[BlockSizePosition]);
	_parameters.blocksCount = std::stoul(argv[BlocksCountPosition]);
	_parameters.threadsACount = std::stoul(argv[ThreadACountPosition]);
	_parameters.threadsBCount = std::stoul(argv[ThreadBCountPosition]);
}


}

