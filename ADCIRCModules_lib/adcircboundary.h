#ifndef ADCIRCBOUNDARY_H
#define ADCIRCBOUNDARY_H

#include "adcircmodules_global.h"
#include "adcircnode.h"
#include <vector>

class AdcircBoundary {
public:
  explicit AdcircBoundary(int boundaryCode, int boundaryLength);

  int boundaryCode() const;
  void setBoundaryCode(int boundaryCode);

  int boundaryLength() const;
  void setBoundaryLength(int boundaryLength);
  int size() const;
  int length() const;

  double crestElevation(int index) const;
  void setCrestElevation(int index, double crestElevation);

  double subcriticalWeirCoeffient(int index) const;
  void setSubcriticalWeirCoeffient(int index, double subcriticalWeirCoeffient);

  double supercriticalWeirCoefficient(int index) const;
  void setSupercriticalWeirCoefficient(int index,
                                       double supercriticalWeirCoefficient);

  double pipeHeight(int index) const;
  void setPipeHeight(int index, double pipeHeight);

  double pipeDiameter(int index) const;
  void setPipeDiameter(int index, double pipeDiameter);

  double pipeCoefficient(int index) const;
  void setPipeCoefficient(int index, double pipeCoefficient);

  AdcircNode *node1(int index) const;
  void setNode1(int index, AdcircNode *node1);

  AdcircNode *node2(int index) const;
  void setNode2(int index, AdcircNode *node2);

private:
  int m_boundaryCode;
  int m_boundaryLength;
  std::vector<double> m_crestElevation;
  std::vector<double> m_subcriticalWeirCoeffient;
  std::vector<double> m_supercriticalWeirCoefficient;
  std::vector<double> m_pipeHeight;
  std::vector<double> m_pipeDiameter;
  std::vector<double> m_pipeCoefficient;
  std::vector<double> m_averageLongitude;
  std::vector<AdcircNode *> m_node1;
  std::vector<AdcircNode *> m_node2;
};

#endif // ADCIRCBOUNDARY_H
