/***************************************************************************
 * SynapseType.hpp
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
#if !defined(SYNAPSETYPE_HPP)
#  define SYNAPSETYPE_HPP

#  include <map>
#  include <string>

#  if !defined(FILTER_HPP)
#    include "Filter.hpp"
#  endif

#  if !defined(STRINGUTILS_HPP)
#    include "utils/StringUtils.hpp"
#  endif

using neurojet::stringutils::EMPTYSTR;

enum LearningRuleType {
  LRT_Undef,
  LRT_PostSyn,
  LRT_MvgAvg,
  LRT_PostSynB,
  LRT_MultiActPS
};

class SynapseType {
 public:
  explicit SynapseType(const std::string& prePostNeurType = "default")
    : m_learningRule(LRT_PostSyn), m_mu(0.0f), m_NMDArise(1), m_alpha(0.0f),
      m_Ksyn(1.0f), m_synSuccRate(1.0f), m_riseFile(EMPTYSTR),
      m_preNeurType(prePostNeurType),
      m_postNeurType(prePostNeurType), m_maxTimeStep(0) {}
  SynapseType(const SynapseType& s);
  SynapseType(const LearningRuleType learningRule, const float mu,
              const unsigned int NMDArise, const float alpha, const float Ksyn,
              const float synFailRate, const std::string& preNeurType,
              const std::string& postNeurType);
  SynapseType& operator=(const SynapseType& s);
  static const unsigned int MAX_TIME_STEP = 500;
  static const unsigned int MAX_NMDA = 21;
  static const unsigned int MAX_RES = 1001;
  float alphaRiseArray[MAX_RES][MAX_NMDA];
  float alphaFallArray[MAX_TIME_STEP];
  float getAlpha() const { return m_alpha; }
  LearningRuleType getLearningRule() const { return m_learningRule; }
  Filter* getFilter() { return &m_synapticFilter; }
  float getKsyn() const { return m_Ksyn; }
  unsigned int getMaxTimeStep() const { return m_maxTimeStep; }
  string getRiseFile() const { return m_riseFile; }
  unsigned int getNMDArise() const { return m_NMDArise; }
  std::string getPreNeurType() const { return m_preNeurType; }
  std::string getPostNeurType() const { return m_postNeurType; }
  float getSynModRate() const { return m_mu; }
  float getSynSuccRate() const { return m_synSuccRate; }
  void setAlpha(const float alpha) {
    m_alpha = alpha;
    setAlphaDecay();
  }
  void setFilter(const DataList &filterVals) {
    m_synapticFilter.setFilter(filterVals);
  }
  void setKsyn(const float Ksyn) {
    m_Ksyn = Ksyn;
  }
  void setRiseFile(const string file) {
    m_riseFile = file;
    setAlphaDecay();
  }
  void setLearningRule(const LearningRuleType learningRule) {
    m_learningRule = learningRule;
  }
  void setNMDArise(const unsigned int NMDArise) {
    m_NMDArise = NMDArise;
    setAlphaDecay();
  }
  void setSynModRate(const float mu) {
    m_mu = mu;
  }
  void setSynFailRate(const float synFailRate) {
    m_synSuccRate = 1 - synFailRate;
  }
  void setSave(const int saveIt) {
    if (saveIt == 1) {
      saveZbar();
    }
  }

  ////////////////////
  // Static members //
  ////////////////////
  // Using a map here is fine, since this public method will only be accessed
  // once per neuron type - while creating the network. From then on, each
  // neuron type will have a copy of the SynapseType referenced here.
  static std::map<std::string, SynapseType> Member;
  static void addMember(const std::string& name,
                        const LearningRuleType learningRule,
                        const float mu, const unsigned int NMDArise,
                        const float alpha, const float Ksyn,
                        const float synFailRate, const std::string& preType,
                        const std::string& postType);
  static std::map<std::string, SynapseType const*>
  findPreSynapticTypes(const std::string& preNeurType);

 private:
  LearningRuleType m_learningRule;
  float m_mu;
  unsigned int m_NMDArise;
  float m_alpha;
  float m_Ksyn;
  float m_synSuccRate;
  string m_riseFile;
  std::string m_preNeurType;
  std::string m_postNeurType;
  Filter m_synapticFilter;
  unsigned int m_maxTimeStep;
  void setAlphaDecay();
  void saveZbar();
};

typedef std::map<std::string, SynapseType>::const_iterator SynapseTypeMapCIt;
typedef std::map<std::string, SynapseType>::iterator SynapseTypeMapIt;

#endif
