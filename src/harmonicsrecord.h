#ifndef HARMONICSRECORD_H
#define HARMONICSRECORD_H

#include <string>
#include <vector>
#include "adcircmodules_global.h"

class HarmonicsRecordImpl;

namespace Adcirc {
namespace Harmonics {
class HarmonicsRecord {
 public:
  ADCIRCMODULES_EXPORT HarmonicsRecord();
  ADCIRCMODULES_EXPORT ~HarmonicsRecord();

  std::string ADCIRCMODULES_EXPORT name() const;
  void ADCIRCMODULES_EXPORT setName(const std::string& name);

  double ADCIRCMODULES_EXPORT value(size_t index);
  std::vector<double> ADCIRCMODULES_EXPORT values();

  void ADCIRCMODULES_EXPORT set(size_t index, double value);
  void ADCIRCMODULES_EXPORT set(const std::vector<double>& value);

  void ADCIRCMODULES_EXPORT resize(size_t numNodes);

  double ADCIRCMODULES_EXPORT frequency() const;
  void ADCIRCMODULES_EXPORT setFrequency(double frequency);

  double ADCIRCMODULES_EXPORT nodalFactor() const;
  void ADCIRCMODULES_EXPORT setNodalFactor(double nodalFactor);

  double ADCIRCMODULES_EXPORT equilibriumArg() const;
  void ADCIRCMODULES_EXPORT setEquilibriumArg(double equilibriumArg);

 private:
  HarmonicsRecordImpl* m_impl;
};
}  // namespace Harmonics
}  // namespace Adcirc

#endif  // HARMONICSRECORD_H
