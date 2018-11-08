/*
 * Dispatcher.h
 *
 *  Created on: 27 окт. 2018 г.
 *      Author: ivan
 */

#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include "InputParametersReader.h"
#include "WorkerContainer.h"

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

namespace CRCVerifier
{

/// \brief Dispatcher class provides data processing dispatching across all the available executors.
class Dispatcher : public ICompletionCallback
{
public:
	/// \brief Achieving Dispatcher singleton.
	/// \param parameters[in]: App-configuration shared pointer
	static Dispatcher& instance(std::shared_ptr<CRCVerifier::InputParameterReader>& parameters);

	/// \brief Dispatch async generation/verification
	void dispatchProcessing();
	
	/// \brief Ensures finalizing all the async processes.
	void finalizeAll();

	~Dispatcher() override;

private:
	std::shared_ptr<CRCVerifier::InputParameterReader> _parameters;
	std::atomic<unsigned>                              _workersCount;
	std::vector<std::shared_ptr<WorkerContainer>>      _threadPool;
	std::mutex                                         _mutex;
	std::condition_variable                            _waiter;

	explicit Dispatcher(std::shared_ptr<CRCVerifier::InputParameterReader>& parameters);

	friend class WorkerContainer;
	void completed(bool status) override;
};

}

#endif /* SRC_DISPATCHER_H_ */
