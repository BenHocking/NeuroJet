/***************************************************************************
 * Interneuron.hpp
 *
 *  Copyright 2005, LevyLab, 2011 Informed Simplifications, LLC
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

#if !defined(INTERNEURON_HPP)
#define INTERNEURON_HPP

#include <algorithm>
#include <deque>
#include <string>
#include <vector>

using std::deque;
using std::string;
using std::vector;

#if !defined(ARGFUNCTS_HPP)
#   include "ArgFuncts.hpp"
#endif
#if !defined(DATATYPES_HPP)
#   include "DataTypes.hpp"
#endif
#if !defined(OUTPUT_HPP)
#   include "Output.hpp"
#endif
#if !defined(FILTER_HPP)
#   include "Filter.hpp"
#endif

class Interneuron {
 public:
  Interneuron(const float excitationDecay = 1.0f,
              const unsigned int buffSize = 1);
  Interneuron(const Interneuron& inter);
  ~Interneuron() { }
  Interneuron& operator=(const Interneuron& i);
  float calcExcitation(const float axonalExcitation);
  float calcExcitation(const UIVector& afferentFirings,
                       const unsigned int firstN, const unsigned int lastN);
  float calcExcitation(const xInput& afferentFirings,
                       const unsigned int firstN, const unsigned int lastN);
  void copy(const Interneuron& i);
  inline void enqueueSynapticActivation(const float synAct) {
    m_synapticQueue.pop_back();
    // AKA push_front();
    m_synapticQueue.insert(m_synapticQueue.begin(), synAct);
  }
  inline float getExcitation() const { return m_internalExcitation; }
  inline const DataList getInternrnWeights() const { return m_PyrToInternrnWt; }
  inline float getMult() const { return m_mult; }
  inline void loadSynapseFilterValues(const DataList &filterVals) {
    m_synapticFilter.setFilter(filterVals);
    reset();
  }
  inline void reset() {
    setMaxTimeOffset(m_axonalBuffSize);
    m_synapticQueue.clear();
    m_synapticQueue.resize(m_synapticFilter.size(), 0.0);
    m_internalExcitation = -1.0f;
  }
  inline void setActivityAveragingRate(const double actAvgRate) {
    m_actAvgRate = actAvgRate;
  }
  inline void setDecayRate(const float newDecay) {
    m_excitationDecay = newDecay;
  }
  inline void setDesiredActivity(const double desiredActivity) {
    m_desiredActivity = desiredActivity;
  }
  inline void setMult(const double mult) { m_mult = mult; }
  // change the axonal buffer size
  void setMaxTimeOffset(const unsigned int buffSize);
  inline void setNumWeights(const unsigned int numWeights,
                            const unsigned int firstN) {
    m_PyrToInternrnWt.assign(numWeights, 1.0L);
    m_firstNeuron = firstN;
  };
  inline void setUseWeightedActAvg(const bool useWeightsForActivity) {
    m_useWeightsForActivity = useWeightsForActivity;
  };
  inline void setWeightedActAvgAdj(const float WeightedActAvgAdj) {
    m_WeightedActAvgAdj = WeightedActAvgAdj;
  }
  inline void setSynModRate(const double synModRate) {
    m_SynModRate = synModRate;
  };
  void updateInternrnWeights(const UIVector &JustFired,
                             const UIVector &toModify,
                             const unsigned int firstN,
                             const unsigned int lastN);
  string exportInterneuron() const {
    // FIXME, eventually will have all information here
    string toReturn(reinterpret_cast<const char *>(&m_internalExcitation));
    return toReturn;
  };
  void importInterneuron(string& toImport) {
    m_internalExcitation = m_internalExcitation;  // FIXME, to implement
  };

 private:
  Filter m_synapticFilter;
  DataList m_synapticQueue;
  // The axonal buffer could be considered to be coming into the interneuron,
  // leaving the interneuron, or a combination of both
  deque<double> m_axonalBuffer;
  // Pyramidal-to-Interneuron weights
  DataList m_PyrToInternrnWt;
  // m_excitationDecay is the amount the existing excitation is reduced by for
  // the next excitation calculation. 1.0 = no memory in the interneuron.
  // This value is relative to the size of the time-step. If you want an e-fold
  // decay after d ms, and your time-step size is s ms, then excitationDecay
  // should be set to 1-e^{-s/d}. The correctness of this can be validated by
  // considering the cases where d->0 (decay = 1.0), and d->inf (decay = 0.0).
  float m_excitationDecay;
  float m_internalExcitation;
  // m_axonalBuffSize is the maximum size we will allow m_axonalBuffer to grow
  unsigned int m_axonalBuffSize;
  // Moving average of the activity deviation
  unsigned int m_firstNeuron;  // 0-based
  bool m_useWeightsForActivity;
  float m_WeightedActAvgAdj;
  double m_activityDeviation;
  double m_desiredActivity;
  double m_SynModRate;
  double m_actAvgRate;
  double m_mult;
};

typedef vector<Interneuron> InterneuronVec;
typedef InterneuronVec::iterator InterneuronVecIt;
typedef InterneuronVec::const_iterator InterneuronVecCIt;

#endif  // INTERNEURON_HPP
