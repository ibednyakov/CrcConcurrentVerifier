/*
 * Dispatcher.cpp
 *
 *  Created on: 27 окт. 2018 г.
 *      Author: ivan
 */

#include "Dispatcher.h"

#include <thread>

namespace CRCVerifier
{

Dispatcher& Dispatcher::instance(std::shared_ptr<CRCVerifier::InputParameterReader>& parameters)
{
	static Dispatcher distancherSingleton(parameters);
	return distancherSingleton;
}

void Dispatcher::finalizeAll()
{
	for (auto& threadInstance : _threadPool)
	{
		threadInstance->finalize();
	}
}

Dispatcher::Dispatcher(std::shared_ptr<CRCVerifier::InputParameterReader>& parameters)
	: _parameters(parameters)
	, _workersCount(_parameters->getSuppliersCount())
{
	_threadPool.reserve(std::min(_parameters->getBlocksCount(),
			static_cast<unsigned long>(_parameters->getSuppliersCount())));
}

Dispatcher::~Dispatcher()
{
	finalizeAll(); //ensures everything completed!..
}

void Dispatcher::dispatchProcessing()
{
	if (_parameters->getSuppliersCount() > _threadPool.size())
	{
		_threadPool.push_back(
				std::make_shared<WorkerContainer>(_parameters->getBlockSize(), this, _parameters->getWorkersCount()));
	}
	else do
		{
			std::unique_lock<std::mutex> uniqueLock(_mutex);
			_waiter.wait(uniqueLock);
		} while(!_workersCount);

	for (auto& threadInstance : _threadPool)
	{
		if (threadInstance->startExecution())
		{
			--_workersCount; // atomic operation!
			return;
		}
	}
	// Note: abnormous situation!
	std::cerr << "Free worker not found! (but notification received..." << std::endl;
}

void Dispatcher::completed(bool completed)
{
	++_workersCount; // atomic operation!
	_waiter.notify_one();
}

}
