/***************************************************************************
 * DendriticSynapse.hpp
 *
 *  Copyright 2005, 2009, 2011 Informed Simplifications, LLC
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

#if !defined(DENDRITICSYNAPSE_HPP)
#  define DENDRITICSYNAPSE_HPP

#  include <deque>
#  include <vector>

#  if !defined(ARGFUNCTS_HPP)
#    include "ArgFuncts.hpp"
#  endif
#  if !defined(NOISE_HPP)
#    include "Noise.hpp"
#  endif
#  if !defined(SYNAPSETYPE_HPP)
#    include "SynapseType.hpp"
#  endif
#  if defined(RNG_BUCKET) && !defined(PARALLELRAND_HPP)
#    include "ParallelRand.hpp"
#  endif

// DendriticSynapse = Synapse on dendritic tree
class DendriticSynapse {
 public:
  inline DendriticSynapse():
    m_isExc(true), m_isInhDiv(false), m_srcNeuron(0), m_destNeuron(0),
    m_weight(0.0f), m_riseUntil(0), m_lastActivate(NEVER_ACTIVATED),
    m_prevLastActivate(NEVER_ACTIVATED), m_mvgAvg(0.0f), m_synType(NULL) {
  };
  ~DendriticSynapse() {
     // Don't do anything
     // ESPECIALLY don't destroy m_synType!!
  }
  // activate happens prior to ++timeStep
  virtual void activate(DataList &bus, DataList &bus_inhdiv,
                        DataList &bus_inhsub, const int timeStep);
  inline void connectNeuron(unsigned int destNeuron, SynapseType const* synType,
                            bool isExc, bool isInhDiv) {
    m_destNeuron = destNeuron;
    m_isExc = isExc;
    m_isInhDiv = isInhDiv;
    m_synType = synType;
  }
  inline unsigned int getSrcNeuron() const { return m_srcNeuron; }
  inline void setSrcNeuron(const unsigned int toSet) { m_srcNeuron = toSet; }
  inline float getWeight() const { return m_weight; }
  inline void setWeight(const float toSet) { m_weight = toSet; }
  inline float calcZBar(int timeStep, int lastActivate) {
    const int timeDiff = timeStep - lastActivate;
    const int NMDArise = m_synType->getNMDArise();
    const int fallDiff = timeDiff - NMDArise;
    float zBar = 0;
    if (timeStep <= static_cast<int> (m_riseUntil)) {
      if (timeStep - m_riseActivate > NMDArise) {
        // zBar is saturated at maximum
        zBar = 1;
      } else {
        // zBar is still rising from the first fire
        zBar = m_synType->alphaRiseArray[m_oldZbar][timeStep - m_riseActivate];
      }
    } else if (timeDiff < NMDArise) {
      zBar = m_synType->alphaRiseArray[m_oldZbar][timeDiff];
      if ((m_synType->getLearningRule() == LRT_PostSynB) && !(zBar > 0)) {
        zBar =
          m_synType->alphaRiseArray[m_oldZbar][timeStep - m_prevLastActivate];
      }
    } else {
      zBar = m_synType->alphaFallArray[fallDiff];
    }
    return zBar;
  }
  inline void updateWeight(int timeStep) {
    // Whatever you do, don't do this:
    // const SynapseType mySynType = *m_synType;
    // NEVER_ACTIVATED is set so that if m_lastActivate == NEVER_ACTIVATED,
    // then timeStep - m_lastActivate >= MAX_TIME_STEP (because timeStep >= 0)
    const float synModRate = m_synType->getSynModRate();
    const int timeDiff = timeStep - m_lastActivate;
    const int NMDArise = m_synType->getNMDArise();
    const int fallDiff = timeDiff - NMDArise;
    const unsigned int maxTimeStep = m_synType->getMaxTimeStep();
    // Never_Activated or really old
    if (m_lastActivate == NEVER_ACTIVATED ||
        fallDiff >= static_cast<int>(maxTimeStep)) {
      m_weight -= synModRate * m_weight;
    } else {
      LearningRuleType learningRule = m_synType->getLearningRule();
      if (learningRule == LRT_MultiActPS) {
        for (unsigned int i = 0; i < m_actHistory.size(); i++) {
          const float zBar =
            calcZBar(timeStep, m_actHistory[i]);
          m_weight += static_cast<float>(synModRate * (zBar - m_weight));
        }
      } else {
        const float zBar =
          calcZBar(timeStep, m_lastActivate);
        if (learningRule == LRT_MvgAvg) {
          // For moving averager, it only gets updated when synapse is
          //  activated. Need to do calculations since that's happened (all
          //  zeros)
          m_weight +=
            static_cast<float>(synModRate * (zBar * m_mvgAvg - m_weight));
        } else {
          m_weight += static_cast<float>(synModRate * (zBar - m_weight));
        }
      }
    }
  }
  inline int getLastActivate() const { return m_lastActivate; }
  inline SynapseType const* getSynapseType() const { return m_synType; }
  inline void setLastActivate(const int toSet) {
    m_prevLastActivate = m_lastActivate;
    m_lastActivate = toSet;
  }
  inline void resetLastActivate() {
    m_prevLastActivate = m_lastActivate;
    m_lastActivate = NEVER_ACTIVATED;
  }
  inline void setOldZbar(const float toSet) {
    m_oldZbar = static_cast<int>(toSet*1000);
  }
  static Noise SynNoise;                 // rng for syn failure

  // Static variables
  /**
   * NEVER_ACTIVATED is set so that if m_lastActivate == NEVER_ACTIVATED,
   * then timeStep - m_lastActivate >= MAX_TIME_STEP (because timeStep >= 0)
   */
  static const int NEVER_ACTIVATED;

 private:
  // non construction-copyable
  DendriticSynapse(const DendriticSynapse& other) {}
  DendriticSynapse& operator=(const DendriticSynapse& other) {  // non copyable
    return *this;
  }

  // Member variables
  bool m_isExc;
  bool m_isInhDiv;
  unsigned int m_srcNeuron;
  unsigned int m_destNeuron;
  float m_weight;
  int m_oldZbar;
  unsigned int m_riseUntil;
  int m_riseActivate;
  // actual last activation time - must be signed since NEVER_ACTIVATED is
  //  negative
  int m_lastActivate;
  // adjusted activation time before last
  int m_prevLastActivate;
  float m_mvgAvg;
  std::deque<unsigned int> m_actHistory;
  SynapseType const* m_synType;
};

typedef DendriticSynapse * Dendrite;
typedef DendriticSynapse const * const DendriteConst;

#endif  // DENDRITICSYNAPSE_HPP
