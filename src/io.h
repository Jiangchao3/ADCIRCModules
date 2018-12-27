/*------------------------------GPL---------------------------------------//
// This file is part of ADCIRCModules.
//
// (c) 2015-2018 Zachary Cobell
//
// ADCIRCModules is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ADCIRCModules is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ADCIRCModules.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------*/
#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "adcircmodules_global.h"

class IO {
 public:
  IO() = default;

  static int readFileData(const std::string &filename,
                          std::vector<std::string> &data);

  static int splitString(std::string &data, std::vector<std::string> &result);

  static bool splitStringNodeFormat(std::string &data, size_t &id, double &x,
                                    double &y, double &z);

  static bool splitStringElemFormat(std::string &data, size_t &id,
                                    std::vector<size_t> &nodes);

  static bool splitStringBoundary0Format(std::string &data, size_t &node1);

  static bool splitStringBoundary23Format(std::string &data, size_t &node1,
                                          double &crest, double &supercritical);

  static bool splitStringBoundary24Format(std::string &data, size_t &node1,
                                          size_t &node2, double &crest,
                                          double &subcritical,
                                          double &supercritical);

  static bool splitStringBoundary25Format(std::string &data, size_t &node1,
                                          size_t &node2, double &crest,
                                          double &subcritical,
                                          double &supercritical,
                                          double &pipeheight, double &pipecoef,
                                          double &pipediam);

  static bool splitStringAttribute1Format(std::string &data, size_t &node,
                                          double &value);

  static bool splitStringAttribute2Format(std::string &data, size_t &node,
                                          double &value1, double &value2);

  static bool splitStringAttributeNFormat(std::string &data, size_t &node,
                                          std::vector<double> &values);

  static bool splitStringHarmonicsElevationFormat(std::string &data,
                                                  double &amplitude,
                                                  double &phase);

  static bool splitStringHarmonicsVelocityFormat(std::string &data,
                                                 double &u_magnitude,
                                                 double &u_phase,
                                                 double &v_magnitude,
                                                 double &v_phase);

  static bool splitString2dmNodeFormat(std::string &data, size_t &id, double &x,
                                       double &y, double &z);

  static bool splitString2dmElementFormat(std::string &data, size_t &id,
                                          std::vector<size_t> &nodes);

  static std::string getFileExtension(const std::string &filename);

  static bool fileExists(const std::string &filename);
};

#endif  // IO_H
