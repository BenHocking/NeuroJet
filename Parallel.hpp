/*******************************************************************************
 * Parallel.hpp
 *
 *  Provides structures and functions to manage parallel networks
 *
 *  Authors: A. P. Shon, Paul Rodriguez, Dave Sullivan, Joe Monaco, Ben Hocking
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
 ******************************************************************************/
#if defined(MULTIPROC)

#  if !defined(PARALLEL_HPP)
#    define PARALLEL_HPP

///////////////////////////////
//
// Important assumption--Portions of the code below ASSUME
//                       that all nodes have the same number of neurons
//                       (ni)--this can be rectified, but it implies
//                       the following: 1) all nodes will have to
//                       know how many neurons are on their afferent
//                       neighbors; 2) the root node has to know
//                       how many neurons are on ALL nodes
//
///////////////////////////////

#    include <string>
#    include <vector>

#    if !defined(MPI_H)
#      include "mpi.h"
#      define MPI_H
#    endif

#    if !defined(PARALLELRAND_HPP)
#      include "ParallelRand.hpp"
#    endif
#    if !defined(SYNAPSE_HPP)
#      include "neural/Synapse.hpp"
#    endif

using std::string;
using std::vector;

// This message prefaces everything written when running in
// multi-processor mode
const char P_StdMsg[] P_StdMsg = "PE: ";

// This contains the node rank ID of the root processing node
const unsigned int P_ROOT_NODE_NUM = 0;

#    if !defined(MSG)
#      define MSG P_StdMsg << ParallelInfo::getRank() << ": "
#    endif

#    define IFROOTNODE if (ParallelInfo::getRank() == P_ROOT_NODE_NUM)
#    define IFCHILDNODE if (ParallelInfo::getRank() <> P_ROOT_NODE_NUM)

// Structure type ParallelInfo tracks all the information a node
// needs to store about 1 of its neighbors
class ParallelInfo {
 public:
  inline ParallelInfo()
    : P_zi(NULL), P_sumwz(NULL) { }
  inline void initialize() {
    P_zi = new int[totalNumNrns];
    P_sumwz = new double[totalNumNrns];
  };
  inline ~ParallelInfo() {
    delArray(P_zi);
    delArray(P_sumwz);
  }

  static string ChildRcv();
  static void staticInitialize(int argc, char *argv[], ParallelRand &pRandComm);
  static inline void resetRandComm(const int s) { P_RandComm->ResetSeed(s); }
  static inline void setRandCommSuccess(const double p) {
    P_RandComm->SetParams(p);
  }
  static void AllocateArrays(const unsigned int ni);
  static void Barrier();
  static inline unsigned int getRank() { return P_MyRank; }
  static inline unsigned int getNumNodes() { return P_NumNodes; }
  static UIMatrix rcvNodeConn(const UIVector &Shuffle,
                              const UIVector &FanInCon);
  static UIVector rcvNrnConn(const UIVector &nrnNodeConn, const int nrn,
                             const UIVector &Shuffle, const UIVector &UnShuffle,
                             const UIVector &FanInCon,
                             const DendriteConst * const InMatrix);
  static UIVector rcvNrnAij(const UIVector &nrnNodeConn, const int nrn,
                            const UIVector &Shuffle, const UIVector &FanInCon,
                            const DendriteConst * const inMatrix,
                            const UIMatrix &FanOutCon,
                            const AxonConst * const outMatrix,
                            const unsigned int minAxonalDelay,
                            const unsigned int maxAxonalDelay);
  static DataList rcvNrnWij(const UIVector &nrnNodeConn, const int nrn,
                            const UIVector &Shuffle, const UIVector &FanInCon,
                            const DendriteConst * const inMatrix);
  static DataList exchangeSumwz(DataList &mySumwz);  // Peer-to-peer
  static DataList rcvSumwz();
  static Pattern rcvZi();
  static void RootSend(const string msg);
  static void sendNodeConn(const UIVector &Shuffle, const UIVector &FanInCon);
  static void sendNrnConn(const UIVector &Shuffle, const UIVector &UnShuffle,
                          const UIVector &FanInCon,
                          const DendriteConst * const InMatrix);
  static void sendNrnAij(const UIVector &Shuffle, const UIVector &FanInCon,
                         const DendriteConst * const inMatrix,
                         const UIMatrix &FanOutCon,
                         const AxonConst * const outMatrix,
                         const unsigned int minAxonalDelay,
                         const unsigned int maxAxonalDelay);
  static void sendNrnWij(const UIVector &Shuffle, const UIVector &FanInCon,
                         const DendriteConst * const inMatrix);
  static void sendSumwz(const DataList &toSend);
  static void sendZi(const Pattern &toSend);

 private:
  static unsigned int totalNumNrns;
  // Total number of nodes (including root)
  static unsigned int P_NumNodes;
  // The rank (id#) of this node
  static unsigned int P_MyRank;
  static ParallelInfo *P_Nodes;
  static ParallelRand *P_RandComm;
  int *P_zi;
  double *P_sumwz;
};

#  endif                          // Termindates #if defined(PARALLEL_HPP)
#else
// We provide a dummy definition for MSG when we're compiling the
// single-processor version
#  define MSG EMPTYSTR
// If single-processor, this node is the root node for all intents and purposes
#  define IFROOTNODE if (true)
#endif                          // Terminates #if defined(MULTIPROC)
