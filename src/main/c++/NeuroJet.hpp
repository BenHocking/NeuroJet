/***************************************************************************
 * NeuroJet.hpp
 *
 *  Really Useful Network Interaction Tool (was RUNIT/PUNIT)
 *
 *  Per Sederberg
 *  Matt Harrison
 *
 *  Copyright 2011 Informed Simplifications, LLC
 *  This file is part of NeuroJet.
 *
 *  NeuroJet is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  NeuroJet is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with NeuroJet.  If not, see <http://www.gnu.org/licenses/lgpl.txt>.
 ****************************************************************************/
#if !defined(NEUROJET_HPP)
#define NEUROJET_HPP

#include <climits>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <string>
#include <vector>

// NeuroJet header files
#if !defined(MATLAB_HPP)
#  include "Matlab.hpp"
#endif
#if !defined(PROGRAM_HPP)
#   include "Program.hpp"
#endif
#if !defined(PARALLEL_HPP)
#   include "Parallel.hpp"
#endif
#if !defined(NEURONTYPE_HPP)
#  include "neural/NeuronType.hpp"
#endif
#if !defined(AXONALSYNAPSE_HPP)
#   include "neural/AxonalSynapse.hpp"
#endif
#if !defined(DENDRITICSYNAPSE_HPP)
#   include "neural/DendriticSynapse.hpp"
#endif

using std::string;
using std::vector;
using std::list;

//////////////////////////////////////////////////////////////////////////////
// SYNFAILS_DEBUG_MODE Macros
//////////////////////////////////////////////////////////////////////////////
#if defined(SYNFAILS_DEBUG_MODE)
/* total actual synaptic transmissions and action potentials */
#   define SYNFAILS_DEBUG_MODE_INIT    int  TotalSuccess = 0, TotalAPs = 0;
#   define SYNFAILS_DEBUG_MODE_INC     ++TotalSuccess;
#   define SYNFAILS_DEBUG_MODE_OUTPUT  SynFailsOutput(TotalAPs, TotalSuccess);
inline void SynFailsOutput(int TotalAPs, int TotalSuccess);
#else
#   define SYNFAILS_DEBUG_MODE_INIT
#   define SYNFAILS_DEBUG_MODE_INC
#   define SYNFAILS_DEBUG_MODE_OUTPUT
#endif

//////////////////////////////////////////////////////////////////////////////
// RNG Variables
//////////////////////////////////////////////////////////////////////////////
#if defined(TIMING_RNG)
double rng_start, rng_elapsed;
#endif
#if defined(RNG_BUCK_TIMING)
double rng_start_buck, rng_elapsed_buck;
double rng_start_calc, rng_elapsed_calc;
#endif

#if defined(RNG_BUCK_USG)
int rng_buck_usage = 0;
int rng_buck_empty = 0;
int ttl_rng_buck_usage = 0;
int ttl_rng_buck_empty = 0;
int max_buck_qty = 0;
int max_rng_usg = 0;
#endif
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Timing Macro
//////////////////////////////////////////////////////////////////////////////
#if defined(TIMING_MODE) || defined(TIMING_MODE3)   \
  || defined(TIMING_RNG) || defined(RNG_BUCK_TIMING)
#  define USESTIMING
#endif
#if defined(USESTIMING)
extern "C" unsigned int64 rdtsc(void);
#endif
#define TICKS_PER_SEC 1533354000        // from /proc/cpuinfo
//////////////////////////////////////////////////////////////////////////////

// for timing purposes
#  if defined(TIMING_MODE2)
clock_t t1 = 0, t2 = 0;
#endif

/* Byte type and some definitions */
#define ZERO       0
#define ONE       1
const unsigned int justNow = 0;
const unsigned int lastTime = 1;
const unsigned int defMaxAxonalDelay = 1;

////////////////////////////////////////////////////////////////////////////////
// Internally Regulated Variables
////////////////////////////////////////////////////////////////////////////////
// System (Neural Network) variables
unsigned int maxAxonalDelay;      // size of buffer for Fired
unsigned int minAxonalDelay;      // first place to start looking for synapses
unsigned int ni;                  // number of neurons
unsigned int NumNetworkCon;       // total number of recurrent connections

float K0Soma;
float KFBSoma;
float KFFSoma;
bool useSomaInh;
float K0Dend;
float KFBDend;
float KFFDend;
bool useDendInh;

// Simulation variables
bool TrainingNetwork;            // a flag indicating that the network is being
                                 // trained and not tested
int timeStep;                    // Current timestep
int TotalNumTied;                // total number of neurons tied last timestep
Pattern zi;                      // current firing state of neurons
xInput xAllZeros;                // zero vector input
float Threshold;                 // subtractive form threshold

// Neuron variables (of size ni)
float *Inhibition;
float *VarKConductanceArray;
DataList sumwz;                  // sum of the weights times the firing state
DataList dendExc;                // dendritic excitation
DataList somaExc;                // somatic excitation
vector<DataList> dendriteQueue;  // Queue from dendrite to soma
// inhdiv = Divisive inhibitory interneurons
DataList sumwz_inhdiv;           // sum of the weights times the firing state
vector<DataList> dendriteQueue_inhdiv;  // Queue from dendrite to soma
// inhsub = Subtractive inhibitory interneurons
DataList sumwz_inhsub;           // sum of the weights times the firing state
vector<DataList> dendriteQueue_inhsub;  // Queue from dendrite to soma
DataList IzhV;
DataList IzhU;
UIVector FanInCon;              // the fan in connections of a neuron
UIMatrix FanOutCon;             // the fan out connections of a neuron per
                                // axonal delay

UIVectorDeque Fired;            // what neurons fired last n timesteps(indexed)
                                // n is determined by max axonal delay

DendriticSynapse **inMatrix;    // Fan-in synapses
AxonalSynapse ***outMatrix;     // Fan-out synapses (per axonal delay/segment)

unsigned int StartNeuron;       // Index of first Neuron on a node
unsigned int EndNeuron;         // Index of last Neuron on a node

#if defined(MULTIPROC)
UIVectorDeque FiredHere;  // what neurons fired on this node last timestep
UIVector Shuffle;         // used so that externals are distributed randomly
UIVector UnShuffle;       // used to make firing diagrams look nice
#   if defined(CHECK_BOUNDS)
#      define SHUFFLEIFMULTIPROC(x) Shuffle.at(x)
#      define UNSHUFFLEIFMULTIPROC(x) UnShuffle.at(x)
#   else
#      define SHUFFLEIFMULTIPROC(x) Shuffle[x]
#      define UNSHUFFLEIFMULTIPROC(x) UnShuffle[x]
#   endif
#   define MULTIPROCFILESUFFIX(x) x + to_string(ParallelInfo::getRank())
#   define LOCALFIRED FiredHere
#else
#   define SHUFFLEIFMULTIPROC(x) (x)
#   define UNSHUFFLEIFMULTIPROC(x) (x)
#   define MULTIPROCFILESUFFIX(x) x
#   define LOCALFIRED Fired
#endif

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////
inline double periodicFn(double phi, bool usesin = true) {
  if (usesin) return sin(phi);  // Default periodicFn
  // First put phi on range [0, 2*pi]
  while (phi > 2 * PI) {
    phi -= 2 * PI;  // FIXME: This needs to be optimized
  }
  while (phi < 0) {
    phi += 2 * PI;  // FIXME: This needs to be optimized
  }
  const double offset = PI / 3;
  if (phi < offset) return -1.0L;
  const double maxVal = 0.2095;  // Found from MATLAB
  phi -= offset;
  return (phi * gampdf(phi, 0.4, 1.8) * 2 / maxVal) - 1.0L;
}

// Internal Functions
void AllocateMemory();
vector<float> assignIzhParams(const std::string &IzhNeuronType);
void CalcDendriticExcitation();
void CalcDendriticToSomaInput(const xInput& curPattern, const bool isComp);
double CalcFBInternrnExcitation();
double CalcFFInternrnExcitation(const xInput &curPattern);
void CalcSomaResponse(const xInput &curPattern, DataMatrix &IzhVValues,
                      DataMatrix &IzhUValues);
void CalcSynapticActivation(const UIVectorDeque &FiredArray,
                            const Pattern &inPattern);
void CalcSynapticActivation(const UIVectorDeque &FiredArray,
                            const xInput &curPattern);
bool chkDataExists(const TArg<std::string> &DataName,
                   const DataListType newDataType,
                   const std::string& FunctionName, const CommandLine &ComL);
void CheckIzhikevich();
void createSelectArray(vector<IxSumwz> &excSort, const xInput &curPattern,
                       const int startN, const int endN);
void DeAllocateMemory();
void enqueueDendriticResponse(const DataList& dendriticResponse,
                              const DataList& dendResp_inhdiv,
                              const DataList& dendResp_inhsub);
const NeuronType* findNeuronType(const unsigned int nrn);
void FireNonTiedNeurons(const unsigned int numLeft2Fire,
                        const vector<IxSumwz> &excSort);
void FireSingleNeuron(const int nrn);
void FireTiedNeurons(const unsigned int numLeft2Fire, const double cutOff,
                     vector<IxSumwz> &excSort);
vector<xInput> GenerateInputSequence(UIPtnSequence &Seq, const float inputNoise,
                                     const float exactNoise, int& SumExtFired,
                                     int& PatternCount);
void GetConnectivity(const std::string& filename);

inline void eat_whitespace(std::istream &in) {
  while (isspace(in.peek())) {
    in.get();
  }
}

template <class T> void read_without_comments(std::istream &in, T &var) {
  eat_whitespace(in);
  while (in.peek() == '#' || in.peek() == '\n') {
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    eat_whitespace(in);
  }
  in >> var;
}

void checkNextChar(bool& inIt, unsigned int& depthIn, const bool inString,
                   const char nextChar, const char deeperChar,
                   const char shallowerChar);
void PopulatePopulation(const std::string& globalProps,
                        const std::string& neuronProps,
                        const std::string& synapseProps, UIMatrix& effDelays);
std::string debracket(const std::string& toDebracket, char beginToken,
                      char endToken);
void FillFanOutMatrices();
inline void GetNullTimingData();
void getThetaSettings(const NeuronType& NeurType, int& Period, float& Amplitude,
                      float& MidPoint, float& Phase, bool& UseSin);
inline void InitCurBucketStats();
bool isNJNetworkFileType(const std::string& filename);
bool isNumeric(const std::string& toCheck);
inline bool isLocalNeuron(const unsigned int nrn);
std::map<std::string, std::string> ParseStruct(const std::string& toParse);
void Present(const xInput &curPattern, DataMatrix &IzhVValues,
             DataMatrix &IzhUValues, const bool modifyInhWeights,
             const bool modifyExcWeights);
void CompPresent(const xInput &curPattern, const bool isTesting);
void readDataType(const TArg<std::string> &Type, DataListType &newDataType,
                  std::string& newType, std::string& newSubType,
                  const std::string& FunctionName, const CommandLine &ComL,
                  const bool allowFile = false);
MatlabCommand ReadMATLABcommand(std::ifstream& mfile,
                                const std::string& filename);
void ReadNJNetworkFile(const std::string& filename);
void ReadPopulationFile(const std::string& filename, UIMatrix& effDelays);
inline void RecordSynapticFiring(const int neuron, const std::string &);
void resetDendriticQueues();
void ResetSTM();
double selectCutOff(unsigned int k, unsigned int n, vector<IxSumwz> &arr);
void SetConnectivity(const int &AllowSelf = true, const char &dType = 'p',
                     const float &p1 = 0.0f, const float &p2 = 1.0f,
                     const float &p3 = 0.0f, const float &p4 = 1.0f);
inline void UpdateBucketStats();
void UpdateBuffers(UIPtnSequence &FiringPtns, UIPtnSequence &ExtPtns,
                   DataMatrix &BusLines, DataMatrix &IntBusLines,
                   DataMatrix &KWeights, DataMatrix &Inhibitions,
                   DataMatrix &FBInternrnExcs, DataMatrix &FFInternrnExcs,
                   DataList &ActVect, DataList &ThreshVect,
                   const unsigned int ndx, const xInput &curPattern,
                   const vector<bool> &RecordIdxList);
inline void UpdateMaxBucketStats();
void UpdateParams(const std::string &varName, const std::string &varValue,
                  const std::string &FunctionName);
void UpdateAnalysis(const std::string& name);
inline void UpdateWeights();
void WriteMATLABHeader(std::ofstream &MATfile, int mrows, int ncols, int namlen,
                       bool isSparse, bool isText);

// AtFunctions
void AddInterneuron(ArgListType &arg);
void Analysis(ArgListType &arg);
void AppendData(ArgListType &arg);
void CombineData(ArgListType &arg);
void Context(ArgListType &arg);
void CopyData(ArgListType &arg);
void CreateAnalysis(ArgListType &arg);
void CreateNeuronType(ArgListType &arg);
void CreateNetwork(ArgListType &arg);
void CreateSynapseType(ArgListType &arg);
void CreateVar(ArgListType &arg);
void ExportVars(ArgListType &arg);
void DeleteData(ArgListType &arg);
void FileReset(ArgListType &arg);
void LoadData(ArgListType &arg);
void MakeSequence(ArgListType &arg);
void MakeRandSequence(ArgListType &arg);
void ResetFiring(ArgListType &arg);
void SaveData(ArgListType &arg);
void SaveWeights(ArgListType &arg);
void SeedRNG(ArgListType &arg);
void SetLoopVar(ArgListType &arg);
void SetStream(ArgListType &arg);
void SetVar(ArgListType &arg);
void Sim(ArgListType &arg);
void Test(ArgListType &arg);
void Train(ArgListType &arg);

// CaretFunctions
std::string Num2Int(ArgListType &arg);
std::string PatternLength(ArgListType &arg);
std::string SequenceLength(ArgListType &arg);
std::string PickSeq(ArgListType &arg);
std::string RandomSeed(ArgListType &arg);
std::string SumData(ArgListType &arg);

void InitializeProgram();
void BindUserFunctions();

// T=Pattern => Sequence, T=Vector => Matrix, T=DataList => Analysis
// T=UIVector => UIPtnSequence
template<class S, class T>
S **Convert2Mat(const vector<T> &Seq, int patstart,
                int patend, int bitstart, int bitend,
                const bool transpose = false,
                const S &fillval = 0) {
  int pattot = patend - (--patstart);  // end - start + 1
  int bittot = bitend - (--bitstart);  // end - start + 1
  int numMtxRows;
  int numMtxCols;
  if (transpose) {
    numMtxRows = bittot;
    numMtxCols = pattot;
  } else {
    numMtxRows = pattot;
    numMtxCols = bittot;
  }
  S **RetMat = new S *[numMtxRows];
  for (int fillRow = 0; fillRow < numMtxRows; fillRow++) {
    RetMat[fillRow] = new S[numMtxCols];
    for (int col = 0; col < numMtxCols; col++) {
      RetMat[fillRow][col] = fillval;
    }
  }
  typename vector<T>::const_iterator it = Seq.begin() + patstart;
  for (int row = 0; row < pattot; row++, it++) {
    const T temppat = *it;
    int patlen = temppat.size();
    if (patlen > bitend) {
      patlen = bitend;
    }
    int col = 0;
    for (int j = bitstart; j < patlen; j++, col++) {
      if (transpose) {
        RetMat[col][row] = static_cast<S>(temppat.at(j));
      } else {
        RetMat[row][col] = static_cast<S>(temppat.at(j));
      }
    }
  }
  return RetMat;
}

template<class T>
void writeData(ostream &fout, const int width, const T &data,
               const int startTime, const int endTime, const int startN,
               const int endN) {
  typename T::const_iterator it = data.begin();
  // increment to the start position
  for (int inc = 0; inc < startTime-1; inc++, it++) {}
  for (int i = startTime; i <= endTime; i++, it++) {
    // 0-based vs. 1-based causes - 1 below
    for (int j = startN - 1; j < endN - 1; j++) {
      fout << std::setw(width) << it->at(j);
    }
    fout << "\n";
  }
}

// User include files
#include "User.hpp"

#endif
