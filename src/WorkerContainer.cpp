/*
 * WorkerContainer.cpp
 *
 *  Created on: 27 окт. 2018 г.
 *      Author: ivan
 */

#include "WorkerContainer.h"

#include "CRC32.h"
#include "iostream"

#include <fstream>
#include <iostream>
#include <thread>

namespace CRCVerifier
{

WorkerContainer::WorkerContainer(unsigned long blockSize,
		ICompletionCallback* completionCallback, unsigned verifiersCount)
	: _producer(blockSize)
	, _callback(completionCallback)
	, _bThreadsCount(verifiersCount)
{}

void WorkerContainer::execute()
{
	auto generatingDataTask = std::packaged_task<void(unsigned)>([this](unsigned verifiersCount)
	{
		doGenerate(verifiersCount);
	});
	std::thread asyncExecutor(std::move(generatingDataTask), _bThreadsCount);
	asyncExecutor.detach();
}

void WorkerContainer::finalize()
{
	bool result = true;
	if (_busyFlag.compare_exchange_strong(result, false))
	{}
}

bool WorkerContainer::startExecution()
{
	bool result = false;
	if (_busyFlag.compare_exchange_strong(result, true))
	{
		execute();
		return true;
	}
	return false;
}

void WorkerContainer::doGenerate(unsigned bThreadsCount)
{
	SharedDataType data = std::make_shared<std::vector<int8_t>>();
	_producer.generate(*data);

	std::future<uint32_t> results[bThreadsCount];
	auto calculateTask = [this](SharedDataType data){
		return doCalculate(data);
	};
	for (unsigned i = 0; i < bThreadsCount; ++i)
	{
		results[i] = std::async(std::launch::async, calculateTask, data);
	}

	std::vector<uint32_t> calculatedCRCs;

	uint32_t previous = 0;
	uint32_t current = 0;
	bool alreadyLogged = false;
	std::string fileName = generateFileName();

	// wait until all the CRC-calculation tasks have been finished:
	for (unsigned i = 0; i < bThreadsCount; ++i)
	{
		previous = current;
		current = results[i].get();
		if (!i)
			continue;
		if (current != previous)
		{
			if (!alreadyLogged)
				logFailedVerification(data, fileName);
			alreadyLogged = true;
		}
	}

	bool result = true;
	if (!_busyFlag.compare_exchange_strong(result, false))
	{
		std::cerr << "Inconsistent state detected!" << std::endl;
	}
	_callback->completed(true);
}

uint32_t WorkerContainer::doCalculate(SharedDataType generatedBlock)
{
	return CRCVerifier::CRC32Calc::instance_pkzip().calc_checksum(
			reinterpret_cast<const void*>(generatedBlock->data()), generatedBlock->size());
}

std::string WorkerContainer::generateFileName() const
{
    static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string result;
    result.resize(WellKnownFilenameLength);

    for(unsigned i = 0; i < WellKnownFilenameLength; ++i)
    {
        result[i] = charset[rand() % charset.length()];
    }

	return result;
}

void WorkerContainer::logFailedVerification(SharedDataType dataBlock, std::string& fileName)
{
    std::ofstream outfile;
    outfile.open(fileName.c_str(), std::ios::out | std::ios::trunc );

    outfile << dataBlock->data();
    outfile.close();

	std::cout << "CRC calculation differs!" << std::endl;
}


}

