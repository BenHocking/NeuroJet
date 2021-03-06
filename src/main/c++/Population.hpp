/***************************************************************************
 * Population.hpp
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
#if !defined(POPULATION_HPP)
#  define POPULATION_HPP

#  include <vector>

#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
#  if !defined(INTERNEURON_HPP)
#    include "neural/Interneuron.hpp"
#  endif
#  if !defined(NEURONTYPE_HPP)
#    include "neural/NeuronType.hpp"
#  endif

class Population {
 public:
  Population(unsigned int f, unsigned int l, NeuronType& nType):
    m_firstNeuron(f), m_lastNeuron(l), m_neuronType(&nType),
    // If the neuron type changes its force external behavior, this won't update
    m_forceExt(nType.forceExt()) {
    m_feedbackInterneurons.push_back(*(new Interneuron()));
    m_feedforwardInterneurons.push_back(*(new Interneuron()));
  };
  //////////////////////////////////////////////
  // Begin functions relating to Interneurons //
  //////////////////////////////////////////////
  void addInterneuron(bool isFeedback, double excitationDecay = 1.0f,
                      unsigned int buffSize = 1, double synModRate = 0.0f,
                      double actAvgRate = 0.0f, double mult = 1.0f) {
    Interneuron toAdd(excitationDecay, buffSize);
    toAdd.setSynModRate(synModRate);
    toAdd.setActivityAveragingRate(actAvgRate);
    toAdd.setMult(mult);
    if (isFeedback) {
      m_feedbackInterneurons.push_back(toAdd);
    } else {
      m_feedforwardInterneurons.push_back(toAdd);
    }
  }
  void calcNewFeedbackInhibition(const UIVector& afferentFirings) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->calcExcitation(afferentFirings, m_firstNeuron, m_lastNeuron);
    }
  }
  void calcNewFeedforwardInhibition(const UIVector& afferentFirings) {
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->calcExcitation(afferentFirings, m_firstNeuron, m_lastNeuron);
    }
  }
  void calcNewFeedforwardInhibition(const xInput& afferentFirings) {
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->calcExcitation(afferentFirings, m_firstNeuron, m_lastNeuron);
    }
  }
  // Surprisingly, profiling showed this to be significant
  //   bool forceExt() const { return m_neuronType->forceExt(); }
  bool forceExt() const { return m_forceExt; }
  float getFeedbackInhibition() const;
  float getFeedforwardInhibition() const;
  unsigned int getFirstNeuron() const { return m_firstNeuron; }
  DataList getKFBWeights() {
    return m_feedbackInterneurons[0].getInternrnWeights();
  }
  unsigned int getLastNeuron() const {return m_lastNeuron;}
  NeuronType* getNeuronType() const {return m_neuronType;}
  void initInterneurons();
  void loadSynapseFilterValues(const DataList &filterVals) {
    if (SystemVar::GetIntVar("WtFiltIsGeneric")) {
      m_neuronType->loadSynapseFilterValues(filterVals);
    }
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->loadSynapseFilterValues(filterVals);
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->loadSynapseFilterValues(filterVals);
    }
  }
  void resetInterneurons() {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->reset();
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->reset();
    }
  }
  void restoreInhState() {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->copy(m_feedbackInterneurons_dup.back());
      m_feedbackInterneurons_dup.pop_back();
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->copy(m_feedforwardInterneurons_dup.back());
      m_feedforwardInterneurons_dup.pop_back();
    }
  }
  void saveInhState() {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      m_feedbackInterneurons_dup.insert(m_feedbackInterneurons_dup.begin(),
                                        *it);
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      m_feedforwardInterneurons_dup
        .insert(m_feedforwardInterneurons_dup.begin(), *it);
    }
  }
  void setActivityAveragingRate(float actAvgRate) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->setActivityAveragingRate(actAvgRate);
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->setActivityAveragingRate(actAvgRate);
    }
  }
  void setDesiredActivity(const float act) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->setDesiredActivity(act);
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->setDesiredActivity(act);
    }
  }
  void setUseWeightedActAvg(const bool useWeightsForActivity) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->setUseWeightedActAvg(useWeightsForActivity);
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->setUseWeightedActAvg(useWeightsForActivity);
    }
  }
  void setWeightedActAvgAdj(const float WeightedActAvgAdj) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->setWeightedActAvgAdj(WeightedActAvgAdj);
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->setWeightedActAvgAdj(WeightedActAvgAdj);
    }
  }
  void setFBInternrnSynModRate(const float synModRate) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->setSynModRate(synModRate);
    }
  }
  void setFFInternrnSynModRate(const float synModRate) {
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->setSynModRate(synModRate);
    }
  }
  void setNeuronRange(unsigned int f, unsigned int l) {
    m_firstNeuron = f;
    m_lastNeuron = l;
  }
  void updateFBInterneuronAxonalDelay(unsigned int axonalBuffLen);
  void updateFFInterneuronAxonalDelay(unsigned int axonalBuffLen);
  void updateInterneuronDecay(double newDecayRate) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->setDecayRate(newDecayRate);
    }
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->setDecayRate(newDecayRate);
    }
  }
  void updateInternrnWeights(const UIVector &JustFired,
                             const UIVector &oldFired,
                             const xInput &curPattern) {
    for (InterneuronVecIt it = m_feedbackInterneurons.begin();
         it != m_feedbackInterneurons.end(); ++it) {
      it->updateInternrnWeights(JustFired, oldFired, m_firstNeuron,
                                m_lastNeuron);
    }
    UIVector curPtn = xInputToUIPtn(curPattern);
    for (InterneuronVecIt it = m_feedforwardInterneurons.begin();
         it != m_feedforwardInterneurons.end(); ++it) {
      it->updateInternrnWeights(JustFired, curPtn, m_firstNeuron, m_lastNeuron);
    }
  }
  ////////////////////
  // Static members //
  ////////////////////
  static void addMember(Population toAdd) {
    Member.push_back(toAdd);
  }
  static unsigned int getPopulationSize() {
    return Member.size();
  }
  static std::vector<Population> Member;

 private:
  // m_firstNeuron & m_lastNeuron are 0-based
  unsigned int m_firstNeuron;
  unsigned int m_lastNeuron;
  InterneuronVec m_feedbackInterneurons;
  InterneuronVec m_feedforwardInterneurons;
  InterneuronVec m_feedbackInterneurons_dup;
  InterneuronVec m_feedforwardInterneurons_dup;
  NeuronType* m_neuronType;
  bool m_forceExt;
};

typedef std::vector<Population>::const_iterator PopulationCIt;
typedef std::vector<Population>::iterator PopulationIt;

#endif  // POPULATION_HPP
