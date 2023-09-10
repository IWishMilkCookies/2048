#include "TrainingData.h"


#pragma region TrainingData

TrainingData::TrainingData(std::string fileName):
	m_Topology{}
{
	m_TrainingDataFile.open(fileName.c_str());
}

bool TrainingData::isEof()
{
	return m_TrainingDataFile.eof();
}

void TrainingData::fillTopology()
{
	std::string line;
	std::string label;

	std::getline(m_TrainingDataFile, line);
	std::stringstream ss(line);
	ss >> label;

	if (this->isEof() || label.compare("topology:") != 0)
		abort();

	while (!ss.eof())
	{
		unsigned n;
		ss >> n;
		m_Topology.push_back(n);
	}

	return;
}

unsigned TrainingData::getNextInputs(std::vector<double>& inputVals)
{
	inputVals.clear();

	std::string line;
	std::getline(m_TrainingDataFile, line);
	std::stringstream ss(line);

	std::string label;
	ss >> label;

	if (label.compare("in:") == 0)
	{
		double oneValue;
		while (ss >> oneValue)
		{
			inputVals.push_back(oneValue);
		}
	}
	return static_cast<unsigned>(inputVals.size());
}

unsigned TrainingData::getTargetOutputs(std::vector<double>& targetVals)
{
	targetVals.clear();

	std::string line;
	std::getline(m_TrainingDataFile, line);
	std::stringstream ss(line);

	std::string label;
	ss >> label;

	if (label.compare("out:") == 0)
	{
		double oneValue;
		while (ss >> oneValue)
		{
			targetVals.push_back(oneValue);
		}
	}
	return static_cast<unsigned>(targetVals.size());
}
#pragma endregion

#pragma region TrainingDataW
TrainingDataW::TrainingDataW(std::wstring fileName) :
	m_Topology{}
{
	m_TrainingDataFile.open(fileName.c_str());
}

bool TrainingDataW::isEof()
{
	return m_TrainingDataFile.eof();
}

void TrainingDataW::fillTopology()
{
	std::wstring line;
	std::wstring label;

	std::getline(m_TrainingDataFile, line);
	std::wstringstream ss(line);
	ss >> label;

	if (this->isEof() || label.compare(L"topology:") != 0)
		abort();

	while (!ss.eof())
	{
		unsigned n;
		ss >> n;
		m_Topology.push_back(n);
	}

	return;
}

unsigned TrainingDataW::getNextInputs(std::vector<double>& inputVals)
{
	inputVals.clear();

	std::wstring line;
	std::getline(m_TrainingDataFile, line);
	std::wstringstream ss(line);

	std::wstring label;
	ss >> label;

	if (label.compare(L"in:") == 0)
	{
		double oneValue;
		while (ss >> oneValue)
		{
			inputVals.push_back(oneValue);
		}
	}
	return static_cast<unsigned>(inputVals.size());
}

unsigned TrainingDataW::getTargetOutputs(std::vector<double>& targetVals)
{
	targetVals.clear();

	std::wstring line;
	std::getline(m_TrainingDataFile, line);
	std::wstringstream ss(line);

	std::wstring label;
	ss >> label;

	if (label.compare(L"out:") == 0)
	{
		double oneValue;
		while (ss >> oneValue)
		{
			targetVals.push_back(oneValue);
		}
	}
	return static_cast<unsigned>(targetVals.size());
}
#pragma endregion
