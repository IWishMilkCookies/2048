#include "NeuronL.h"
#include "Connection.h"

double Neuron::m_Eta = 0.15;
double Neuron::m_Alpha = 0.5;

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned connections = 0; connections < numOutputs; ++connections)
	{
		m_OutputWeights.push_back(new Connection());
		m_OutputWeights.back()->weight = randomWeight();
	}

	m_MyIndex = myIndex;
}

void Neuron::FeedForward(const Layer& prevLayer)
{
	double sum = 0;

	for (unsigned numbers = 0; numbers < prevLayer.size(); ++numbers)
	{
		sum += prevLayer[numbers].getOutputVal() * prevLayer[numbers].m_OutputWeights[m_MyIndex]->weight;
	}

	m_OutputValue = Neuron::transferFunction(sum);
}

void Neuron::CalcOutputGradients(double targetVal)
{
	double delta = targetVal - m_OutputValue;
	m_Gradient = delta * transferFunctionDerivative(m_OutputValue);
}

void Neuron::CalcHiddenGradients(const Layer& nextLayer)
{
	double dow = sumDOW(nextLayer);
	m_Gradient = dow * transferFunctionDerivative(m_OutputValue);
}

void Neuron::UpdateInputWeights(Layer& prevLayer)
{
	for (unsigned neuron = 0; neuron < prevLayer.size() - 1; ++neuron)
	{
		Neuron& currentNeuron = prevLayer[neuron];
		double oldDeltaWeight = currentNeuron.m_OutputWeights[m_MyIndex]->deltaWeight;

		double newDeltaWeight = m_Eta * currentNeuron.getOutputVal() * m_Gradient + m_Alpha * oldDeltaWeight;

		currentNeuron.m_OutputWeights[m_MyIndex]->deltaWeight = newDeltaWeight;
		currentNeuron.m_OutputWeights[m_MyIndex]->weight += newDeltaWeight;
	}
}

double Neuron::transferFunction(double x)
{
	//Tanh curve: Range [-1.0 - 1.0]
	return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
	//Tanh derivative
	return 1.0 - (x * x);
}

double Neuron::sumDOW(const Layer& nextLayer)
{
	double sum = 0.0;

	for (unsigned neuron = 0; neuron < nextLayer.size() -1; ++neuron)
	{
		sum += m_OutputWeights[neuron]->weight * nextLayer[neuron].m_Gradient;
	}
	return sum;
}
