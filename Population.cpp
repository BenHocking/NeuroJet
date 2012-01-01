/***************************************************************************
 * Population.cpp
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
#  include "Population.hpp"
#endif

#if !defined(SYSTEMVAR_HPP)
#  include "SystemVar.hpp"
#endif

#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

//////////////
// Functors //
//////////////

class SetInterneuronWeights: public unary_function<Interneuron, void> {
private:
  int m_popSize;
  int m_firstNeuron;
public:
  SetInterneuronWeights(int popSize, int firstNeuron): m_popSize(popSize), m_firstNeuron(firstNeuron) { }
  void operator() (Interneuron& x) {
    x.setNumWeights(m_popSize, m_firstNeuron);
  }
};

class SetMaxTimeOffset: public unary_function<Interneuron, void> {
private:
  int m_axonalBuffLen;
public:
  SetMaxTimeOffset(int axonalBuffLen): m_axonalBuffLen(axonalBuffLen) { }
  void operator() (Interneuron& x) {
    x.setMaxTimeOffset(m_axonalBuffLen);
  }
};

class WeightedAverager: public unary_function<Interneuron, void> {
private:
  float m_denominator;
  float m_numerator;
public:
  WeightedAverager(): m_denominator(0), m_numerator(0) { }
  void operator() (const Interneuron& iv) {
    m_numerator += iv.getMult() * iv.getExcitation();
    m_denominator += iv.getMult();
  }
  float result() const {
    return m_numerator / m_denominator;
  }
};


////////////////////////
// Static definitions //
////////////////////////

vector<Population> Population::Member;

////////////////////////////
// Non-static definitions //
////////////////////////////

//////////////////////////////////////////////
// Begin functions relating to Interneurons //
//////////////////////////////////////////////
void Population::initInterneurons() {
  float act = m_neuronType->getParameter("Activity", SystemVar::GetFloatVar("Activity"));
  const unsigned int popSize = m_lastNeuron - m_firstNeuron + 1;
  for_each(m_feedbackInterneurons.begin(), m_feedbackInterneurons.end(), SetInterneuronWeights(popSize, m_firstNeuron));
  for_each(m_feedforwardInterneurons.begin(), m_feedforwardInterneurons.end(), SetInterneuronWeights(popSize, m_firstNeuron));
  setDesiredActivity(act);
  float lambda = m_neuronType->getParameter("lambdaFB", SystemVar::GetFloatVar("lambdaFB"));
  setFBInternrnSynModRate(lambda);
  lambda = m_neuronType->getParameter("lambdaFF", SystemVar::GetFloatVar("lambdaFF"));
  setFFInternrnSynModRate(lambda);
  float actAvgRate = m_neuronType->getParameter("PyrToInternrnWtAdjDecay", 
                                                SystemVar::GetFloatVar("PyrToInternrnWtAdjDecay"));
  setActivityAveragingRate(actAvgRate);
  unsigned int axDelay = m_neuronType->getParameter("FBInternrnAxonalDelay",
                                                    SystemVar::GetIntVar("FBInternrnAxonalDelay"));
  updateFBInterneuronAxonalDelay(axDelay);
  axDelay = m_neuronType->getParameter("FFInternrnAxonalDelay",
                                       SystemVar::GetIntVar("FFInternrnAxonalDelay"));
  updateFFInterneuronAxonalDelay(axDelay);
  const float decay = m_neuronType->getParameter("InternrnExcDecay", SystemVar::GetFloatVar("InternrnExcDecay"));
  updateInterneuronDecay(static_cast<double>(decay));
}

float Population::getFeedbackInhibition() const {
  return for_each(m_feedbackInterneurons.begin(), m_feedbackInterneurons.end(), WeightedAverager()).result();
}

float Population::getFeedforwardInhibition() const {
  return for_each(m_feedforwardInterneurons.begin(), m_feedforwardInterneurons.end(), WeightedAverager()).result();
}

void Population::updateFBInterneuronAxonalDelay(unsigned int axonalBuffLen) {
  for_each(m_feedbackInterneurons.begin(), m_feedbackInterneurons.end(), SetMaxTimeOffset(axonalBuffLen));
}

void Population::updateFFInterneuronAxonalDelay(unsigned int axonalBuffLen) {
  for_each(m_feedforwardInterneurons.begin(), m_feedforwardInterneurons.end(), SetMaxTimeOffset(axonalBuffLen));
}
