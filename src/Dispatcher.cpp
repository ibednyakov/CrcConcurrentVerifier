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
{}

void Dispatcher::dispatchProcessing()
{
	if (_parameters->getSuppliersCount() > _threadPool.size())
	{
		_threadPool.push_back(
				std::make_shared<WorkerContainer>(_parameters->getBlockSize(), this, _parameters->getWorkersCount()));
	}
	else 
		while(!_workersCount)
		{
			std::this_thread::yield();
			// Another possible solution - to use cond var here...
			// But it has other disadvantages!
		}

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
}

}
