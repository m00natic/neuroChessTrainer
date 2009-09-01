#ifndef BPN_H
#define BPN_H

#include <fstream>
#include "bpnlayer.h"

class BPN
{
public:
    BPN(unsigned *, bool *, outFunction *, unsigned, double, double, double);
    BPN(char*);
    ~BPN();

    void InitializeWeights();
    bool SaveToFile(char*);
    bool Train(double*, double*);
    void Run(double*);
    bool Train(char*);
    void Run(char*);

private:
    bool SaveLayer(bpnLayer*, std::ofstream&);
    std::string readString(std::ifstream&);
    void LoadLayer(bpnLayer*, std::ifstream&);
    double randomNum(double, double);
    double ApplyFunction(outFunction, double);
    double ApplyDerivate(outFunction, double);
    double ApplyLinear(double);
    double ApplySigmoid(double);
    double ApplySigmoid2(double);
    double DerivateLinear(double);
    double DerivateSigmoid(double);
    double DerivateSigmoid2(double);
    void PrepareFromFEN(char*);
    void Run();
    bool Train();

private:
    double alpha;    //  momentum
    double eta;  //  learning rate
    double initial_scale;

public:
    unsigned size;
    bpnLayer **layers;
    double* train_output;
    double scale_factor;
};

#endif // BPN_H
