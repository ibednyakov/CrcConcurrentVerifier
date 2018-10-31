/*
 * DataGenerator.h
 *
 *  Created on: 26 окт. 2018 г.
 *      Author: ivan
 */

#ifndef SRC_DATAGENERATOR_H_
#define SRC_DATAGENERATOR_H_

#include <stdlib.h>
#include <stdint.h>

#include <vector>

namespace CRCVerifier
{

/// \brief DataGenerator class provides random data generation functionality
class DataGenerator
{
public:
	explicit DataGenerator(unsigned long blockSize) noexcept;
	DataGenerator() = delete;

	/// \brief Generates random data.
	/// \param result[out]: Vector to store result to
	void generate(std::vector<int8_t>& result) const;

private:
	unsigned long _blockSize;
};

}

#endif /* SRC_DATAGENERATOR_H_ */
