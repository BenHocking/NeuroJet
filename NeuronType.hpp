#if !defined(NEURONTYPE_HPP)
#  define NEURONTYPE_HPP

#  if !defined(DATATYPES_HPP)
#    include "DataTypes.hpp"
#  endif
#  if !defined(FILTER_HPP)
#    include "Filter.hpp"
#  endif
#  if !defined(SYNAPSETYPE_HPP)
#    include "SynapseType.hpp"
#  endif
#  if !defined(SYSTEMVAR_HPP)
#    include "SystemVar.hpp"
#  endif
#  include <map>
#  include <string>

enum ThresholdType { TT_Undef, TT_Simple, TT_E, TT_Log, TT_Rational };

class NeuronType {
public:
   NeuronType(): m_isExc(true), m_isInhDiv(false), m_thresholdType(TT_Simple), m_name("") {};
   NeuronType(const NeuronType& n);
   NeuronType(bool isExc, bool isInhDiv, const ThresholdType thresholdType, const std::string& name):
      m_isExc(isExc), m_isInhDiv(isInhDiv), m_thresholdType(thresholdType), m_name(name) {};
   NeuronType& operator=(const NeuronType& n);
   void convolveFilters();
   bool forceExt() const;
   unsigned int getFilterSize() const { return m_convolvedFilter.size(); }
   Filter getFilter() const { return m_convolvedFilter; }
   std::string getName() const { return m_name; }
   template<class T>
   T getParameter(const std::string& param, const T defValue) const {
      // m_parameter[param] isn't const friendly, hence this:
     return (hasParameter(param)) ? from_string<T>(m_parameter.find(param)->second) : defValue;
   }
   ThresholdType getThresholdType() const { return m_thresholdType; }
   bool hasParameter(const std::string& param) const {
      return (m_parameter.find(param) != m_parameter.end());
   }
   bool isExcType() const { return m_isExc; }
   bool isInhDivType() const { return m_isInhDiv; }
   void loadDTSFilterValues(const DataList &filterVals) {
      m_dendriteToSomaFilter.setFilter(filterVals);
      convolveFilters();
   }
   void loadSynapseFilterValues(const DataList &filterVals) {
      m_synapseType.setFilter(filterVals);
      convolveFilters();
   }
   void setParameter(const std::string& param, const std::string& val) { m_parameter[param] = val; }
   bool useIzh() const;
   void setThresholdType(const ThresholdType thresholdType) { m_thresholdType=thresholdType; }
   ////////////////////
   // Static members //
   ////////////////////
   // This map will only be accessed during CreateNetwork
   static std::map<std::string, NeuronType> Member;
   static void addMember(const std::string& name, bool isExc, bool isInhDiv, const ThresholdType thresholdType);
private:
   Filter m_dendriteToSomaFilter;
   // dendrite-to-soma filter convolved with synapse filter
   Filter m_convolvedFilter;
   SynapseType m_synapseType;
   std::map<const std::string, std::string> m_parameter;
   bool m_isExc;
   bool m_isInhDiv;
   ThresholdType m_thresholdType;
   std::string m_name;
};

typedef std::map<std::string, NeuronType>::const_iterator NeuronTypeMapCIt;
typedef std::map<std::string, NeuronType>::iterator NeuronTypeMapIt;

#endif // NEURONTYPE_HPP
