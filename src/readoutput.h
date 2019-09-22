/*------------------------------GPL---------------------------------------//
// This file is part of ADCIRCModules.
//
// (c) 2015-2019 Zachary Cobell
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
#ifndef ADCMOD_READOUTPUT_H
#define ADCMOD_READOUTPUT_H

#include <fstream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "adcircmodules_global.h"
#include "filetypes.h"
#include "node.h"
#include "outputmetadata.h"
#include "outputrecord.h"

namespace Adcirc {

namespace Output {

class ReadOutput {
 public:
  ReadOutput(const std::string &filename);

  ~ReadOutput();

  void open();

  void close();

  bool exists();

  bool isOpen();

  std::string header() const;
  void setHeader(const std::string &header);

  std::string filename() const;
  void setFilename(const std::string &filename);

  size_t numSnaps() const;
  void setNumSnaps(size_t numSnaps);

  size_t numNodes() const;
  void setNumNodes(size_t numNodes);

  double dt() const;
  void setDt(double dt);

  int dIteration() const;
  void setDiteration(int dit);

  Adcirc::Output::OutputFormat filetype() const;
  void setFiletype(Adcirc::Output::OutputFormat filetype);

  size_t currentSnap() const;
  void setCurrentSnap(const size_t &currentSnap);

  double defaultValue() const;
  void setDefaultValue(double defaultValue);

  static const std::vector<Adcirc::Output::OutputMetadata>
      *adcircFileMetadata();

  Adcirc::Output::OutputMetadata *metadata();
  void setMetadata(const Adcirc::Output::OutputMetadata &metadata);

  double modelDt() const;
  void setModelDt(double modelDt);

  void read(size_t snap = Adcirc::Output::nextOutputSnap());

  Adcirc::Output::OutputRecord *data(size_t snap);
  Adcirc::Output::OutputRecord *data(size_t snap, bool &ok);

  Adcirc::Output::OutputRecord *dataAt(size_t position);
  Adcirc::Output::OutputRecord *dataAt(size_t position, bool &ok);

  void clear();
  void clearAt(size_t position);

 private:
  void setOpen(bool open);

  // variables
  std::fstream m_fid;
  std::vector<std::unique_ptr<Adcirc::Output::OutputRecord>> m_records;
  std::unordered_map<size_t, Adcirc::Output::OutputRecord *> m_recordMap;
  bool m_open;
  Adcirc::Output::OutputFormat m_filetype;
  size_t m_currentSnap;
  size_t m_numSnaps;
  size_t m_numNodes;
  double m_modelDt;
  double m_dt;
  double m_defaultValue;
  int m_dit;
  std::string m_filename;
  std::string m_units;
  std::string m_description;
  std::string m_name;
  std::string m_header;
  Adcirc::Output::OutputMetadata m_metadata;

  // netcdf specific variables
  int m_ncid;
  int m_dimid_time, m_dimid_node;
  int m_varid_time;
  std::vector<double> m_time;
  std::vector<int> m_varid_data;

  // functions
  Adcirc::Output::OutputFormat getFiletype();
  void findNetcdfVarId();
  void rebuildMap();

  void openAscii();
  void openNetcdf();

  void closeAscii();
  void closeNetcdf();

  void readAsciiHeader();
  void readNetcdfHeader();

  void readAsciiRecord(std::unique_ptr<OutputRecord> &record);
  void readNetcdfRecord(size_t snap, std::unique_ptr<OutputRecord> &record);
  int netcdfVariableSearch(size_t variableIndex, OutputMetadata &filetypeFound);
};
}  // namespace Output
}  // namespace Adcirc

#endif  // ADCMOD_READOUTPUT_H
