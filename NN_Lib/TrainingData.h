#pragma once
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

class TrainingData
{
public:
	TrainingData(std::string fileName);
	bool isEof() const;
	void fillTopology();
	std::vector<unsigned>& getTopology()  { return m_Topology; }
	unsigned getNextInputs(std::vector<double>& inputVals);
	unsigned getTargetOutputs(std::vector<double>& targetVals);

private:
	std::ifstream m_TrainingDataFile;
	std::vector<unsigned> m_Topology;
};

class TrainingDataW
{
public:
	TrainingDataW(std::wstring fileName);
	bool isEof() const;
	void fillTopology();
	std::vector<unsigned>& getTopology()  { return m_Topology; }
	unsigned getNextInputs(std::vector<double>& inputVals);
	unsigned getTargetOutputs(std::vector<double>& targetVals);

private:
	std::wifstream m_TrainingDataFile;
	std::vector<unsigned> m_Topology;
};