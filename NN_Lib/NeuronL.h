#pragma once
#include <vector>

struct Connection;

class Neuron;
typedef std::vector<Neuron> Layer;

class Neuron
{
public:
    Neuron(unsigned numOutputs, unsigned myIndex);

    void setOutputVal(double val) { m_OutputValue = val; }
    double getOutputVal(void) const { return m_OutputValue; }
    void FeedForward(const Layer& prevLayer);

    void CalcOutputGradients(double targetVal);
    void CalcHiddenGradients(const Layer& nextLayer);
    void UpdateInputWeights(Layer& prevLayer);

private:
    double m_OutputValue;
    std::vector<Connection*> m_OutputWeights;
    unsigned m_MyIndex;
    double m_Gradient;
    
    static double m_Eta; // [0.0 , 1.0] overall net training rate
    static double m_Alpha; //[0.0, n] multiplier of last weight change (momentum)
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    double sumDOW( const Layer& nextLayer);
};
