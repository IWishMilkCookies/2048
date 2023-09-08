#pragma once
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

class TrainingData
{
public:
	TrainingData(std::string fileName);
	bool isEof();
	void getTopology(std::vector<unsigned>& topology);
	unsigned getNextInputs(std::vector<double>& inputVals);
	unsigned getTargetOutputs(std::vector<double>& targetVals);

private:
	std::ifstream m_TrainingDataFile;

};

class TrainingDataW
{
public:
	TrainingDataW(std::wstring fileName);
	bool isEof();
	void getTopology(std::vector<unsigned>& topology);
	unsigned getNextInputs(std::vector<double>& inputVals);
	unsigned getTargetOutputs(std::vector<double>& targetVals);

private:
	std::wifstream m_TrainingDataFile;

};