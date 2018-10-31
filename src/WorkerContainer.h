/*
 * WorkerContainer.h
 *
 *  Created on: 27 окт. 2018 г.
 *      Author: ivan
 */

#ifndef SRC_WORKERCONTAINER_H_
#define SRC_WORKERCONTAINER_H_

#include "DataGenerator.h"

#include <thread>
#include <future>
#include <atomic>

namespace CRCVerifier
{

class ICompletionCallback
{
public:
	virtual ~ICompletionCallback() {}
	virtual void completed(bool) = 0;
};

/// \brief WorkerContainer class encapsulates async operations set
/// for data generation and verification.
class WorkerContainer
{
	typedef std::shared_ptr<std::vector<int8_t>> SharedDataType;

	/// \enum Used constants.
	enum
	{
		WellKnownFilenameLength = 12,
	};

public:
	WorkerContainer() = delete;
	~WorkerContainer()= default;
	WorkerContainer(unsigned long blockSize, ICompletionCallback* completionCallback, unsigned verifiersCount);

	/// \brief Finalizing async worker if needed
	void finalize();

	/// \brief Starts asynchronous execution if not busy
	/// \return true if execution has been started
	bool startExecution();
	
	void doGenerate(unsigned bThreadsCount);

private:
	DataGenerator                        _producer;
	std::atomic<bool>                    _busyFlag;
	ICompletionCallback*                 _callback;
	unsigned                             _bThreadsCount;

	void execute();
	uint32_t doCalculate(SharedDataType generatedBlock);
	std::string generateFileName() const;
	void logFailedVerification(SharedDataType dataBlock, std::string& fileName);
};

}

#endif /* SRC_WORKERCONTAINER_H_ */
