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
#  include "Interneuron.hpp"
#endif

#include <algorithm>
#include <stdexcept>

#if !defined(ARGFUNCTS_HPP)
#  include "ArgFuncts.hpp"
#endif
#if !defined(PARSER_HPP)
#  include "Parser.hpp"
#endif

using std::length_error;

Interneuron::Interneuron(const float excitationDecay,
                         const unsigned int buffSize)
  // For now, Filter defaults to size 1
  : m_synapticFilter(1), m_synapticQueue(1, 0.0), m_axonalBuffer(buffSize, 0.0),
    m_PyrToInternrnWt(0), m_excitationDecay(excitationDecay),
    m_internalExcitation(-1.0f), m_axonalBuffSize(buffSize), m_firstNeuron(0),
    m_useWeightsForActivity(false), m_WeightedActAvgAdj(1.0f),
    m_activityDeviation(0.0f), m_SynModRate(0.0f), m_actAvgRate(0.0f),
    m_mult(1.0f) {
  if (m_axonalBuffSize == 0)
    throw length_error("Attempted to create an Interneuron with no buffer");
}

Interneuron::Interneuron(const Interneuron& i)
  : m_synapticFilter(i.m_synapticFilter), m_synapticQueue(i.m_synapticQueue),
    m_axonalBuffer(i.m_axonalBuffer), m_PyrToInternrnWt(i.m_PyrToInternrnWt),
    m_excitationDecay(i.m_excitationDecay),
    m_internalExcitation(i.m_internalExcitation),
    m_axonalBuffSize(i.m_axonalBuffSize),
    m_firstNeuron(i.m_firstNeuron),
    m_useWeightsForActivity(i.m_useWeightsForActivity),
    m_WeightedActAvgAdj(i.m_WeightedActAvgAdj),
    m_activityDeviation(i.m_activityDeviation),
    m_SynModRate(i.m_SynModRate), m_actAvgRate(i.m_actAvgRate),
    m_mult(i.m_mult) {
  // Does nothing else
}

Interneuron& Interneuron::operator=(const Interneuron& i) {
  if (this != &i) {  // make sure not same object
    this->copy(i);
  }
  return *this;    // Return ref for multiple assignment
}

float Interneuron::calcExcitation(const float axonalExcitation) {
  m_axonalBuffer.push_front(axonalExcitation);
  if (m_axonalBuffer.size() > m_axonalBuffSize) m_axonalBuffer.pop_back();
  float curExcitation = m_axonalBuffer.back();  // arriving at the synapse
  enqueueSynapticActivation(curExcitation);
  float synResponse = m_synapticFilter.apply(m_synapticQueue);
  // m_internalExcitation is initialized to -1 so that we can recognize
  // the first time this is called after a reset (or first time ever)
  m_internalExcitation = (m_internalExcitation < 0.0f) ? synResponse :
    (1.0f - m_excitationDecay) * m_internalExcitation
    + m_excitationDecay * synResponse;
  return (m_mult * m_internalExcitation);
}

float Interneuron::calcExcitation(const UIVector& afferentFirings,
                                  const unsigned int firstN,
                                  const unsigned int lastN) {
  double axonalExcitation = 0.0f;
  for (unsigned int i = 0; i < afferentFirings.size(); ++i) {
    if ((afferentFirings[i] >= firstN)
        && (afferentFirings[i] <= lastN))
      axonalExcitation += m_PyrToInternrnWt[afferentFirings[i] - m_firstNeuron];
  }
  return calcExcitation(axonalExcitation);
}

float Interneuron::calcExcitation(const xInput& afferentFirings,
                                  const unsigned int firstN,
                                  const unsigned int lastN) {
  double axonalExcitation = 0.0f;
  for (unsigned int i = firstN; i <= lastN; ++i) {
    if (afferentFirings[i] == 1)
      axonalExcitation += m_PyrToInternrnWt[i-m_firstNeuron];
  }
  return calcExcitation(axonalExcitation);
}

void Interneuron::copy(const Interneuron& i) {
  m_synapticFilter = i.m_synapticFilter;
  m_axonalBuffer = i.m_axonalBuffer;
  m_PyrToInternrnWt = i.m_PyrToInternrnWt;
  m_excitationDecay = i.m_excitationDecay;
  m_internalExcitation = i.m_internalExcitation;
  m_axonalBuffSize = i.m_axonalBuffSize;
  m_firstNeuron = i.m_firstNeuron;
  m_useWeightsForActivity = i.m_useWeightsForActivity;
  m_WeightedActAvgAdj = i.m_WeightedActAvgAdj;
  m_activityDeviation = i.m_activityDeviation;
  m_SynModRate = i.m_SynModRate;
  m_actAvgRate = i.m_actAvgRate;
  m_mult = i.m_mult;
  m_synapticQueue = i.m_synapticQueue;
}

void Interneuron::setMaxTimeOffset(const unsigned int buffSize) {
  if (buffSize == 0)
    throw length_error("Attempted to alter Interneuron to have no buffer");
  m_axonalBuffSize = buffSize;
  while (m_axonalBuffer.size() < m_axonalBuffSize) {
    m_axonalBuffer.push_front(0.0f);
  }
  while (m_axonalBuffer.size() > m_axonalBuffSize) {
    m_axonalBuffer.pop_back();
  }
}

void Interneuron::updateInternrnWeights(const UIVector &JustFired,
                                        const UIVector &toModify,
                                        const unsigned int firstN,
                                        const unsigned int lastN) {
  // toModify determines which neuron's efferent weights are modified
  // JustFired is used to determine the actual activity
  // So, for feedforward interneurons, toModify corresponds to just external
  // inputs, but JustFired is all neurons
  if (m_SynModRate > verySmallFloat) {
    // "Dave's rule" for inhibition adjustment(Pyramidal-to-Interneuron
    // Synaptic Modification)
    // FLEX: Adjustment could be based off of previous activity instead of
    //  current activity, as well as other modifications
    double actualAct = 0.0f;
    for (unsigned int i = 0; i < JustFired.size(); ++i) {
      const unsigned int iFired = JustFired[i];
      const int idx = iFired - m_firstNeuron;
      if ((iFired >= firstN) && (iFired <= lastN)) {
        actualAct += (m_useWeightsForActivity ? m_PyrToInternrnWt[idx] : 1.0L);
      }
    }
    if (m_useWeightsForActivity) {
      double totalWeights = 0.0f;
      for (unsigned int nrn = firstN; nrn <= lastN; ++nrn) {
        const int idx = nrn - m_firstNeuron;
        totalWeights += m_PyrToInternrnWt[idx];
      }
      actualAct /= totalWeights;
    } else {
      actualAct /= (lastN - firstN + 1);
    }

    m_activityDeviation = (m_actAvgRate * m_activityDeviation)
      + (1 - m_actAvgRate)
      * (actualAct * m_WeightedActAvgAdj - m_desiredActivity);

    for (unsigned int i = 0; i < toModify.size(); i++) {
      const int idx = toModify[i] - m_firstNeuron;
      m_PyrToInternrnWt[idx] += m_SynModRate * m_activityDeviation;
      if (m_PyrToInternrnWt[idx] < 0) {
        m_PyrToInternrnWt[idx] = 0;
      }
    }
  }
}
