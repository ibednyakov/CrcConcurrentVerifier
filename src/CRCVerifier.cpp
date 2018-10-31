/*
 * CRCVerifier.cpp
 *
 *  Created on: 24 окт. 2018 г.
 *      Author: ivan
 */

//Необходимо написать программу на C++ 11 (или C++ 14) которая выполняет избыточные вычисления используя
//несколько потоков:
//
//Потоки А-1..А-M генерируют блоки случайных данных. Количество потоков, блоков и размер блока задается параметрами
//командной строки. Количество блоков может быть очень большим.
//Потоки Б-1..Б-N вычисляют CRC32 (можно использовать готовую реализацию) для блоков сгенерированных потоками A.
//Количество потоков задается параметром командной строки.
//Когда все потоки Б вычислят CRC32 для какого-то блока, надо сравнить полученные значения и если они не совпадают
//записать блок в файл и вывести сообщение в std::cout.
//Потоки A и Б должны работать параллельно.

#include "InputParametersReader.h"
#include "CRC32.h"
#include "Dispatcher.h"

#include <iostream>
#include <stdlib.h>
#include <memory>
#include <thread>
#include <vector>

int main(int argc, char* argv[])
{
	try {
		auto parameters = std::make_shared<CRCVerifier::InputParameterReader>(argc, argv);

		CRCVerifier::Dispatcher& dispatcher = CRCVerifier::Dispatcher::instance(parameters);

		for (unsigned long blockIndex = 0; blockIndex < parameters->getBlocksCount(); ++blockIndex)
		{
			dispatcher.dispatchProcessing();
		}

		dispatcher.finalizeAll();

	} catch(CRCVerifier::InvalidParametersCount& exp) {
		std::cerr << "Invalid parameters provided! " << exp.what() << std::endl;
		std::cout << CRCVerifier::InputParameterReader::getUsage() << std::endl;
		return -1;
	} catch(std::exception& exp) {
		std::cerr << "Error: " << exp.what() << std::endl;
		return -1;
	} catch(...) {
		std::cerr << "Unknown error occurred!" << std::endl;
		return -1;
	}
	std::cout << "Successfully completed" << std::endl;
	return 0;
}

