#pragma once
#include <vector>
#include "NeuronL.h"

class Net
{
public:
    Net(const std::vector<unsigned int>& topology);
    void FeedForward(const std::vector<double>& inputVals);
    void BackProp(const std::vector<double>& targetVals);
    void GetResults(std::vector<double>& resultVals) const;
    double GetRecentAverageError() const { return m_RecentAverageError; }

private:
    std::vector<Layer> m_Layers; //m_Layers[layerNum][neuronNum]

    double m_Error;
    double m_RecentAverageError;
    double m_RecentAverageSmoothingFactor;
};
