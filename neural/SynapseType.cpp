/***************************************************************************
 * SynapseType.cpp
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
#  include "SynapseType.hpp"
#endif

#include <map>
#include <string>

#if !defined(OUTPUT_HPP)
#  include "Output.hpp"
#endif

using std::fstream;
using std::map;
using std::string;

map<string, SynapseType> SynapseType::Member;

SynapseType::SynapseType(const LearningRuleType learningRule, const float mu,
                         const unsigned int NMDArise, const float alpha,
                         const float Ksyn, const float synFailRate,
                         const string& preNeurType, const string& postNeurType)
  : m_learningRule(learningRule), m_mu(mu), m_NMDArise(NMDArise),
    m_alpha(alpha), m_Ksyn(Ksyn), m_synSuccRate(1.0f - synFailRate),
    m_preNeurType(preNeurType), m_postNeurType(postNeurType) {
  setRiseFile(EMPTYSTR);
  setAlphaDecay();
}

SynapseType::SynapseType(const SynapseType& s)
  : m_learningRule(s.m_learningRule), m_mu(s.m_mu), m_NMDArise(s.m_NMDArise),
    m_alpha(s.m_alpha), m_Ksyn(s.m_Ksyn), m_synSuccRate(s.m_synSuccRate),
    m_preNeurType(s.m_preNeurType), m_postNeurType(s.m_postNeurType),
    m_synapticFilter(s.m_synapticFilter) {
  string str(s.getRiseFile().c_str());
  setRiseFile(str);
  for (unsigned int i = 0; i < MAX_RES; i++) {
    for (unsigned int j = 0; j < MAX_NMDA; j++) {
      alphaRiseArray[i][j] = s.alphaRiseArray[i][j];
    }
    for (unsigned int i = 0; i < MAX_TIME_STEP; i++) {
        alphaFallArray[i] = s.alphaFallArray[i];
    }
  }
}

SynapseType& SynapseType::operator=(const SynapseType& s) {
    if (this != &s) {  // make sure it's not the same object
        m_learningRule = s.m_learningRule;
        m_mu = s.m_mu;
        m_NMDArise = s.m_NMDArise;
        m_alpha = s.m_alpha;
        m_Ksyn = s.m_Ksyn;
        m_synSuccRate = s.m_synSuccRate;
        m_preNeurType = s.m_preNeurType;
        m_postNeurType = s.m_postNeurType;
        m_synapticFilter = s.m_synapticFilter;
        string str(s.getRiseFile().c_str());
        setRiseFile(str);
        for (unsigned int i = 0; i < MAX_RES; i++) {
          for (unsigned int j = 0; j < MAX_NMDA; j++) {
                alphaRiseArray[i][j] = s.alphaRiseArray[i][j];
          }
        }
        for (unsigned int i = 0; i < (MAX_TIME_STEP); i++) {
            alphaFallArray[i] = s.alphaFallArray[i];
        }
    }
    return *this;  // Return ref for multiple assignment
}

void SynapseType::addMember(const string& name,
                            const LearningRuleType learningRule, const float mu,
                            const unsigned int NMDArise, const float alpha,
                            const float Ksyn, const float synFailRate,
                            const string& preType, const string& postType) {
  if (Member.find(name) == Member.end()) {
    Member[name] = SynapseType(learningRule, mu, NMDArise, alpha, Ksyn,
                               synFailRate, preType, postType);
  } else {
    CALL_ERROR << "SynapseType " << name << " already exists" << ERR_WHERE;
    exit(EXIT_FAILURE);
  }
}

map<string, SynapseType const*>
SynapseType::findPreSynapticTypes(const string& preNeurType) {
    map<string, SynapseType const*> toReturn;
    for (SynapseTypeMapCIt STMCIt = SynapseType::Member.begin(); STMCIt
            != SynapseType::Member.end(); ++STMCIt) {
        if (STMCIt->second.getPostNeurType() == preNeurType) {
            toReturn[STMCIt->second.getPreNeurType()] = &(STMCIt->second);
        }
    }
    return toReturn;
}
void SynapseType::saveZbar() {  // OUTPUT ALPHA ARRAY HERE
    fstream myfile;
    myfile.open("ZbarRiseArray.txt", fstream::out);
    if (myfile.is_open()) {
        for (unsigned int i = 0; i < MAX_RES; i++) {
            for (unsigned int j = 0; j < MAX_NMDA; j++)
                myfile << alphaRiseArray[i][j] << " ";
            myfile << endl;
        }
        myfile.close();
    }
    // OUTPUT ALPHA FALL ARRAY HERE
    myfile.open("ZbarFallArray.txt", fstream::out);
    if (myfile.is_open()) {
        for (unsigned int i = 0; i < MAX_TIME_STEP; i++)
            myfile << alphaFallArray[i] << " ";
        myfile.close();
    }
}

void SynapseType::setAlphaDecay() {
    memset(alphaRiseArray, 0, sizeof(alphaRiseArray[0]) * MAX_RES * (MAX_NMDA));
    memset(alphaFallArray, 0, sizeof(alphaFallArray[0]) * (MAX_TIME_STEP));

    for (unsigned int i = 0; i < 1001; i++) {
      alphaRiseArray[i][0] = (static_cast<float>(i)) / (1000.0f);

      for (unsigned int j = 0; j < m_NMDArise + 1; j++) {
        // MEGHAN: If using Post Synaptic Rule B set alpha array to ("0" /
        //  "the old zBar") until peak, then decay normally
        if (m_learningRule == LRT_PostSynB) {
          alphaRiseArray[i][j] = (j < m_NMDArise) ? alphaRiseArray[i][0] : 1.0f;
        } else {
          alphaRiseArray[i][j] = (j < m_NMDArise) ? alphaRiseArray[i][0]
            + static_cast<double> (j) / m_NMDArise : 1.0f;
        }
      }
    }
    for (unsigned int j = 0; j < m_NMDArise + 1; j++) {
        alphaRiseArray[1000][j] = 1.0f;
    }

    for (unsigned int i = 0; i < MAX_RES; i++) {
      for (unsigned int j = 0; j < m_NMDArise + 1; j++) {
        if (alphaRiseArray[i][j] > 1.0f) {
          alphaRiseArray[i][j] = 1.0f;
        }
      }
    }

    // USE A FILE TO INPUT zBar during rise
    if (!m_riseFile.empty()) {
        fstream file;
        file.open(m_riseFile.c_str(), fstream::in);
        if (!file.is_open()) {
            CALL_ERROR << "Unable to open " << m_riseFile << ".\n" << ERR_WHERE;
            exit(EXIT_FAILURE);
        }
        char ch[20];
        float rise[MAX_NMDA];
        unsigned int i = 0;
        while (i < m_NMDArise + 1 && !file.eof()) {
            file.getline(ch, 20, ' ');
            if (ch[0] == '\0')
                continue;
            char* ch_end;
            const double val = strtod(ch, &ch_end);
            rise[i++] = val;
            if (i > 1 && (rise[i - 1] < rise[i - 2])) {
                Output::Out() << "\nentry #" << i - 2 << " is: " << rise[i - 2]
                              << " entry #" << i - 1 << " is: " << rise[i - 1]
                              << "\nWARNING: zBarRiseArray is non-monotonic!\n"
                              << "Possible ERROR in " << m_riseFile << "\n\n";
            }
        }
        file.close();

        // THIS IS BAD, USER DID NOT INPUT A CORRECT TXT FILE.
        if (i != m_NMDArise + 1) {
          CALL_ERROR << m_riseFile
                     << " does not agree with NMDArise in script file!\n"
                     << "Possible mismatch between NMDArise and the length of "
                     << m_riseFile << ",\nor an error in the format of "
                     << m_riseFile << endl << ERR_WHERE;
          exit(EXIT_FAILURE);
        }
        if ((rise[0] != 0) || (rise[i - 1] != 1)) {
          CALL_ERROR << m_riseFile << " is not valid.\n"
                     << "First number must be 0 and last number must be 1\n"
                     << "Possible mismatch between NMDArise and the length of "
                     << m_riseFile << ",\nor an error in the format of "
                     << m_riseFile << endl << ERR_WHERE;
            exit(EXIT_FAILURE);
        }
        for (unsigned int j = 0; j < m_NMDArise + 1; j++) {
            alphaRiseArray[0][j] = rise[j];
            for (unsigned int i = 1; i < MAX_RES; i++) {
                alphaRiseArray[i][j] = alphaRiseArray[i - 1][j] + 0.001f;
            }
        }
        for (unsigned int i = 0; i < MAX_RES; i++) {
          for (unsigned int j = 0; j < m_NMDArise + 1; j++) {
            if (alphaRiseArray[i][j] > 1.0f) {
              alphaRiseArray[i][j] = 1.0f;
            }
          }
        }
    }

    // BUILD ALPHA FALL ARRAY HERE:
    m_maxTimeStep = MAX_TIME_STEP;
    for (unsigned int i = 0; i < (MAX_TIME_STEP); i++) {
        alphaFallArray[i] = pow(m_alpha, static_cast<double> (i));
        if (alphaFallArray[i] < 0.001) {
          // TODO(bhocking): verify this still does what we want
          m_maxTimeStep = i;
        }
    }
}
