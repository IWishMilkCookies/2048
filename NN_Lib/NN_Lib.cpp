#include <iostream>
#include <memory>
#include <string>
#include <cassert>


#include "NeuralNetwork.h"
#include "TrainingData.h"

void InsertRandomData(std::string filePath);
void ShowVectorVals(std::string label, std::vector<double>& values);

int main()
{
    //using namespace std;
    // 
//Select file button 
    //string filePath{"D:\\documenten\\Test_FOlder\\Test.txt"};
    //InsertRandomData(filePath);
    
    
    //TrainingData* traindata = new TrainingData(filePath);
    
    
    //vector<unsigned int> topology;
    //traindata->getTopology(topology);
    //Net* myNet = new Net(topology);
    
    //vector<double> inputVals, targetVals, resultVals;
   // int trainingPass = 0;
    
     
//Third button to apply training 
// Make sure to check for variables being made: nullptr checks, etc
    //while (!traindata->isEof())
    //{
    //    ++trainingPass;
    //    cout << endl << "Pass " << trainingPass;
    //
    //    //Get new input data and feed it forward
    //    if (traindata->getNextInputs(inputVals) != topology[0])
    //        break;
    //
    //    ShowVectorVals(": Inputs:", inputVals);
    //    myNet->FeedForward(inputVals);
    //
    //    //Collect the net's actual results
    //    myNet->GetResults(resultVals);
    //    ShowVectorVals("Outputs:", resultVals);
    //
    //    //Train the net what the outputs should have been:
    //    traindata->getTargetOutputs(targetVals);
    //    ShowVectorVals("Targets:", targetVals);
    //    assert(targetVals.size() == topology.back());
    //
    //    myNet->BackProp(targetVals);
    //
    //    //Report how well the training is working.
    //    cout << "Net recent average error: " << myNet->GetRecentAverageError() << endl;
    //}
    //
    //cout << endl << "Done" << endl;
}

void InsertRandomData(std::string filePath)
{
    std::ofstream sampleData;

    sampleData.open(filePath);

    sampleData << "topology: 2 4 4 1" << std::endl;
    for (int i = 2000; i >= 0; --i)
    {
        int n1 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
        int n2 = static_cast<int>(2.f * rand() / static_cast<unsigned>(RAND_MAX));
        int t = n1 ^ n2;

        sampleData << "in: " << n1 << ".0 " << n2 << ".0 " << std::endl;
        sampleData << "out: " << t << ".0 " << std::endl;
    }
    sampleData.close();
}

void ShowVectorVals(std::string label, std::vector<double>& values)
{
    using namespace std;
    cout << label << ' ';

    for (unsigned i = 0; i < static_cast<unsigned>(values.size()); ++i)
    {
        cout << values[i] << ' ';
    }
    cout << endl;
}