#ifndef BPN_H
#define BPN_H

#include <fstream>
#include "bpnlayer.h"

class BPN {
 public:
  BPN(unsigned*, bool*, outFunction*, unsigned, double, double, double);
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
  void PrepareFromFEN(char*);
  void Run();
  bool Train();
  bool DoThreading(unsigned, double (*) (double), void* (*)(void*));

  static double randomNum(double, double);
  static double ApplyFunction(outFunction, double);
  static double ApplyDerivate(outFunction, double);
  static double ApplyLinear(double);
  static double ApplySigmoid(double);
  static double ApplySigmoid2(double);
  static double DerivateLinear(double);
  static double DerivateSigmoid(double);
  static double DerivateSigmoid2(double);

  static void* UnitThreadFuncBias (void*);
  static void* UnitThreadFunc (void*);
  static void* UnitThreadFuncBiasScale (void*);
  static void* UnitThreadFuncScale (void*);
  static void* UnitThreadFuncTrain (void*);
  static void* UnitThreadFuncRenew (void*);

 private:
  double initial_scale;

 public:
  unsigned size;
  bpnLayer **layers;
  double* train_output;
  double scale_factor;
  double alpha;    //  momentum
  double eta;  //  learning rate
};

struct LayerThread {
  unsigned layer, start, end;
  BPN *bp;
  double (*apply) (double);
};

#endif // BPN_H
