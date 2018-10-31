/*
 * InputParametersReader.h
 *
 *  Created on: 24 окт. 2018 г.
 *      Author: ivan
 */

#ifndef SRC_INPUTPARAMETERSREADER_H_
#define SRC_INPUTPARAMETERSREADER_H_

#include <string>

namespace CRCVerifier
{

/// \brief InvalidParametersCount exception class
class InvalidParametersCount
{
public:
	InvalidParametersCount() = delete;
	InvalidParametersCount(int expected, int provided);
	std::string what() noexcept;

private:
	int _expected;
	int _provided;
	static std::string _message;
};

/// \brief InputParametersReader class provides configuration reading 
/// and parsing facilities
class InputParameterReader
{
	/// \enum Internally used constants
	enum
	{
		WellKnownParametersCount = 5,
		BlockSizePosition = 1,
		BlocksCountPosition = 2,
		ThreadACountPosition = 3,
		ThreadBCountPosition = 4,
	};

public:
	InputParameterReader() = delete;
	InputParameterReader(int argc, char* argv[]);

	~InputParameterReader() = default;

	unsigned getSuppliersCount() const;
	unsigned getWorkersCount() const;
	unsigned long getBlockSize() const;
	unsigned long getBlocksCount() const;

	static std::string getUsage();

private:

	/// \brief InputParameters struct encapsulates config parameters
	struct InputParameters
	{
		unsigned long blockSize;
		unsigned long blocksCount;
		unsigned      threadsACount;
		unsigned      threadsBCount;
	} _parameters;

	static std::string _usageMessage;

	void parseParameters(int argc, char* argv[]);
};

}

#endif /* SRC_INPUTPARAMETERSREADER_H_ */
