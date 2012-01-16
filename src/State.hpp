/***************************************************************************
 * State.hpp
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
#if !defined(STATE_HPP)
#  define STATE_HPP

#  include <vector>

#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
#  if !defined(POPULATION_HPP)
#    include "Population.hpp"
#  endif
#  if !defined(DENDRITICSYNAPSE_HPP)
#    include "neural/DendriticSynapse.hpp"
#  endif

class State {
 public:
  State(unsigned int ni, int timeStep, const UIVectorDeque &Fired,
        const UIVector &FanInCon, const std::vector<float*>& dendriteQueue,
        const std::vector<float*>& dendriteQueue_inhdiv,
        const std::vector<float*> &dendriteQueue_inhsub,
        const float* const IzhV, const float* const IzhU,
        const DendriticSynapse* const * const inMatrix)
    : m_ni(ni), m_timeStep(timeStep), m_Fired(Fired), m_FanInCon(FanInCon) {
    for (PopulationCIt PCIt = Population::Member.begin();
         PCIt != Population::Member.end(); ++PCIt) {
      const unsigned int filterSize = PCIt->getNeuronType()->getFilterSize();
      for (unsigned int nrn = PCIt->getFirstNeuron();
           nrn <= PCIt->getLastNeuron(); ++nrn) {
        m_dendriteQueue.push_back(new float[filterSize]);
        memcpy(m_dendriteQueue[nrn], dendriteQueue[nrn],
               filterSize*sizeof(dendriteQueue[nrn][0]));
        m_dendriteQueue_inhdiv.push_back(new float[filterSize]);
        memcpy(m_dendriteQueue_inhdiv[nrn], dendriteQueue_inhdiv[nrn],
               filterSize*sizeof(dendriteQueue_inhdiv[nrn][0]));
        m_dendriteQueue_inhsub.push_back(new float[filterSize]);
        memcpy(m_dendriteQueue_inhsub[nrn], dendriteQueue_inhsub[nrn],
               filterSize*sizeof(dendriteQueue_inhsub[nrn][0]));
      }
    }
    m_IzhV = new float[ni];
    memcpy(m_IzhV, IzhV, ni*sizeof(IzhV[0]));
    m_IzhU = new float[ni];
    memcpy(m_IzhU, IzhU, ni*sizeof(IzhU[0]));
    m_inMatrix = new DendriticSynapse *[ni];
    for (unsigned int conRow = 0; conRow < ni; ++conRow) {
      const unsigned int numCon = FanInCon[conRow];
      m_inMatrix[conRow] = new DendriticSynapse[numCon];
      for (unsigned int conCol = 0; conCol < numCon; ++conCol) {
        // Needs to do a deep copy
        m_inMatrix[conRow][conCol] = inMatrix[conRow][conCol];
      }
    }
  }
  ~State() {
    for (unsigned int nrn = 0; nrn < m_dendriteQueue.size(); ++nrn) {
      // delete[] dendriteQueue[nrn];
      // delete[] dendriteQueue_inhdiv[nrn];
      // delete[] dendriteQueue_inhsub[nrn];
    }
    delete[] m_IzhV;
    delete[] m_IzhU;
    for (unsigned int conRow = 0; conRow < m_ni; ++conRow) {
      delete[] m_inMatrix[conRow];
    }
    delete[] m_inMatrix;
  }

  void undoState(int& timeStep, std::vector<float *>& dendriteQueue,
                 std::vector<float *>& dendriteQueue_inhdiv,
                 std::vector<float *>& dendriteQueue_inhsub,
                 UIVectorDeque& Fired, float* IzhV, float* IzhU,
                 DendriticSynapse** inMatrix) const {
    timeStep = m_timeStep;
    for (PopulationCIt PCIt = Population::Member.begin();
         PCIt != Population::Member.end(); ++PCIt) {
      const unsigned int filterSize = PCIt->getNeuronType()->getFilterSize();
      for (unsigned int nrn = PCIt->getFirstNeuron();
           nrn <= PCIt->getLastNeuron(); ++nrn) {
        memcpy(dendriteQueue[nrn], m_dendriteQueue[nrn],
               filterSize*sizeof(dendriteQueue[nrn][0]));
        memcpy(dendriteQueue_inhdiv[nrn], m_dendriteQueue_inhdiv[nrn],
               filterSize*sizeof(dendriteQueue_inhdiv[nrn][0]));
        memcpy(dendriteQueue_inhsub[nrn], m_dendriteQueue_inhsub[nrn],
               filterSize*sizeof(dendriteQueue_inhsub[nrn][0]));
      }
    }
    Fired = m_Fired;
    memcpy(IzhV, m_IzhV, m_ni*sizeof(IzhV[0]));
    memcpy(IzhU, m_IzhU, m_ni*sizeof(IzhU[0]));
    for (unsigned int conRow = 0; conRow < m_ni; ++conRow) {
      const unsigned int numCon = m_FanInCon[conRow];
      for (unsigned int conCol = 0; conCol < numCon; ++conCol) {
        // Needs to do a deep copy
        inMatrix[conRow][conCol] = m_inMatrix[conRow][conCol];
      }
    }
  }

 private:
  unsigned int m_ni;
  int m_timeStep;
  UIVectorDeque m_Fired;
  UIVector m_FanInCon;
  std::vector<float*> m_dendriteQueue;
  std::vector<float*> m_dendriteQueue_inhdiv;
  std::vector<float*> m_dendriteQueue_inhsub;
  float *m_IzhV;
  float *m_IzhU;
  DendriticSynapse **m_inMatrix;
};
#endif
