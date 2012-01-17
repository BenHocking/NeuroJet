/***************************************************************************
 * DendriticSynapse.cpp
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
#   include "DendriticSynapse.hpp"
#endif
#if !defined(SYNAPSETYPE_HPP)
#   include "SynapseType.hpp"
#endif

Noise DendriticSynapse::SynNoise;
const int DendriticSynapse::NEVER_ACTIVATED =
  -1 - static_cast<int>(SynapseType::MAX_TIME_STEP);

// activate happens prior to ++timeStep
void DendriticSynapse::activate(DataList &bus, DataList &bus_inhdiv,
                                DataList &bus_inhsub, const int timeStep) {
  // throw the coin
  // Whatever you do, don't do this:
  // const SynapseType mySynType = *m_synType;
#if defined(RNG_BUCKET)
  bool result = ParallelRand::RandComm.RandBernoulli();
#else        // not RNG_BUCKET
  // relies on programmer to invoke chkNoiseInit in code before use
  bool result = SynNoise.Bernoulli(m_synType->getSynSuccRate());
#endif       // RNG_BUCKET
  if (result) {
    const bool useMvgAvg = (m_synType->getLearningRule() == LRT_MvgAvg);
    const unsigned int NMDArise = m_synType->getNMDArise();
    const unsigned int timeDiff = timeStep - m_lastActivate;
    const unsigned int maxTimeStep = (m_synType->getMaxTimeStep());
    if (useMvgAvg) {
      const bool has_fired = (timeStep - m_lastActivate) <
        static_cast<int>(SynapseType::MAX_TIME_STEP);
      const float inv_alpha = 1 - m_synType->getAlpha();
      if (timeDiff < NMDArise) {
        const float rise = m_synType->alphaRiseArray[m_oldZbar][timeDiff+1];
        m_mvgAvg = has_fired ? rise * m_mvgAvg + inv_alpha : inv_alpha;
      } else {
        // Zeroth element is 0, first element is 1, 2nd element is alpha,
        // nth element is alpha^(n-1)
        const float fall = m_synType->alphaFallArray[timeDiff+1-NMDArise];
        m_mvgAvg = has_fired ? fall * m_mvgAvg + inv_alpha : inv_alpha;
      }
    }
    // Calculates net excitation
    // FLEX: Could add AMPA-like biology/dendritic capacitance(with lookup
    //  table)
    if (m_isExc) {
      bus[m_destNeuron] += m_synType->getKsyn() * m_weight;
    } else if (m_isInhDiv) {
      bus_inhdiv[m_destNeuron] += m_synType->getKsyn() * m_weight;
    } else {
      bus_inhsub[m_destNeuron] += m_synType->getKsyn() * m_weight;
    }
    // FLEX: Could add more biology to this functionality(see NMDArise.ps
    // for an explanation)
    // NEVER_ACTIVATED is set so that if m_lastActivate == NEVER_ACTIVATED,
    // then timeStep - m_lastActivate >= MAX_TIME_STEP
    // (because timeStep >= 0)
    if (m_lastActivate == NEVER_ACTIVATED) {
      m_oldZbar = 0;
      m_riseUntil = 0;
    } else if (timeDiff < NMDArise) {  // Activation during a Rise
      if (timeStep < static_cast<int>(m_riseUntil)) {
        // previously had fired on a rise, i.e. at least the second fire
        // on this rise
        m_riseUntil = timeStep + NMDArise - 1;
      } else {
        // first fire on this rise
        m_riseUntil = timeStep + NMDArise - 1;
        m_riseActivate = m_lastActivate;
      }
    } else {            // Activation during a Fall
      const float fall = m_synType->alphaFallArray[timeDiff-NMDArise];
      m_oldZbar = static_cast<int>(1000.0f * fall);
      m_riseUntil = 0;
    }
    m_prevLastActivate = m_lastActivate;
    m_lastActivate = timeStep;
    if (m_oldZbar < 0) {
      m_oldZbar = 0;
    } else if (m_oldZbar >= 1000) {
      m_oldZbar = 1000;
    }
    while (m_actHistory.size() > 0 &&
           (timeStep - m_actHistory.front()) > maxTimeStep) {
      m_actHistory.pop_front();
    }
    m_actHistory.push_back(static_cast<unsigned int>(timeStep));
  }
}
