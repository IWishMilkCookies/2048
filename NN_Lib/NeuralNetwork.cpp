#include "NeuralNetwork.h"
#include "NeuronL.h"
#include <iostream>

#include <cassert>

Net::Net(const std::vector<unsigned int>& topology)
{
	unsigned numLayers = static_cast<unsigned>(topology.size());
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	{
		m_Layers.push_back(Layer());
		unsigned numOutputs;
		bool layerNumEqTopSize{ layerNum == topology.size() - 1 };

		if (layerNumEqTopSize)
			numOutputs = 0;
		else
			numOutputs = topology[layerNum + 1];


		//This is one layer, now add the neurons
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)
		{
			m_Layers.back().push_back(Neuron(numOutputs, neuronNum));
			std::cout << "Made a Neuron on layer: " << layerNum << ' ' << "With: " << numOutputs << " outputs." << std::endl;
		}
		
		//Force the bias node's output value to be one, it's the last neuron created above.
		m_Layers.back().back().setOutputVal(1.0);
	}

}

void Net::FeedForward(const std::vector<double>& inputVals)
{
	assert(inputVals.size() == m_Layers[0].size() - 1);

	//Assign the input values into the input neurons
	for (unsigned input = 0; input < inputVals.size(); ++input)
	{
		m_Layers[0][input].setOutputVal(inputVals[input]);
	}

	//Forward propogate
	for (unsigned layerNum = 1; layerNum < m_Layers.size(); ++layerNum)
	{
		Layer& prevLayer = m_Layers[layerNum - 1];
		for (unsigned n = 0; n < m_Layers[layerNum].size() - 1; ++n)
		{
			m_Layers[layerNum][n].FeedForward(prevLayer);
		}
	}
}

void Net::BackProp(const std::vector<double>& targetVals)
{
	//Calculate overall net error (RMS of output neuron errors)
	Layer& outputLayer = m_Layers.back();
	m_Error = 0.0;

	for (unsigned neuron = 0; neuron < outputLayer.size() - 1; ++neuron)
	{
		double delta = targetVals[neuron] - outputLayer[neuron].getOutputVal();
		m_Error += delta * delta;
	}

	m_Error /= outputLayer.size() - 1; //Get average error squared
	m_Error = sqrt(m_Error); // RMS

	//Implement a recent average measurement
	m_RecentAverageError = (m_RecentAverageError * m_RecentAverageSmoothingFactor + m_Error) / (m_RecentAverageSmoothingFactor + 1.0);

	//Calculate output layer gradients
	for (unsigned neuron = 0; neuron < outputLayer.size() -1; ++neuron)
	{
		outputLayer[neuron].CalcOutputGradients(targetVals[neuron]);
	}

	//Calculate gradients on hidden layers
	for (unsigned layerNum = static_cast<unsigned>(m_Layers.size() - 2); layerNum > 0; --layerNum)
	{
		Layer& hiddenLayer = m_Layers[layerNum];
		Layer& nextLayer = m_Layers[layerNum + 1];

		for (unsigned neuron = 0; neuron < hiddenLayer.size(); ++neuron)
		{
			hiddenLayer[neuron].CalcHiddenGradients(nextLayer);
		}
	}

	//For all layers from outputs to firs hidden layer,
	//update connection weights
	for (unsigned layerNum = static_cast<unsigned>(m_Layers.size() - 1); layerNum > 0; --layerNum)
	{
		Layer& layer = m_Layers[layerNum];
		Layer& prevLayer = m_Layers[layerNum - 1];

		for (unsigned neuron = 0; neuron < layer.size() -1; ++neuron)
		{
			layer[neuron].UpdateInputWeights(prevLayer);
		}
	}

}

void Net::GetResults(std::vector<double>& resultVals) const
{
	resultVals.clear();

	for (unsigned neuron = 0; neuron < m_Layers.back().size() - 1; ++neuron)
	{
		resultVals.push_back(m_Layers.back()[neuron].getOutputVal());
	}
}
