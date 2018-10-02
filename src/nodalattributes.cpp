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
#include "nodalattributes.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <utility>
#include "attribute.h"
#include "boost/format.hpp"
#include "error.h"
#include "io.h"
#include "stringconversion.h"

using namespace Adcirc::ModelParameters;
using namespace std;

#define CHECK_RETURN_AND_CLOSE(ierr)                                           \
  if (ierr != Adcirc::NoError) {                                               \
    fid.close();                                                               \
    adcircmodules_throw_exception("NodalAttributes: Error reading file data"); \
    return Adcirc::HasError;                                                   \
  }

#define CHECK_FILEREAD_RETURN(ok)                                              \
  if (!ok) {                                                                   \
    fid.close();                                                               \
    adcircmodules_throw_exception("NodalAttributes: Error reading file data"); \
    return Adcirc::HasError                                                    \
  }

NodalAttributes::NodalAttributes() : m_filename(string()) {
  this->m_mesh = nullptr;
  this->m_numParameters = 0;
  this->m_numNodes = 0;
}

NodalAttributes::NodalAttributes(string filename, Adcirc::Geometry::Mesh *mesh)
    : m_filename(filename) {
  this->m_mesh = mesh;
  if (this->m_mesh != nullptr) {
    this->m_numNodes = mesh->numNodes();
  }
  this->m_numParameters = 0;
}

void NodalAttributes::setFilename(string filename) {
  this->m_filename = std::move(filename);
}

string NodalAttributes::filename() { return this->m_filename; }

void NodalAttributes::setMesh(Adcirc::Geometry::Mesh *mesh) {
  this->m_mesh = mesh;
}

Adcirc::Geometry::Mesh *NodalAttributes::mesh() { return this->m_mesh; }

size_t NodalAttributes::locateAttribute(const string &attributeName) {
  assert(this->m_attributeLocations.find(attributeName) !=
         this->m_attributeLocations.end());
  if (this->m_attributeLocations.find(attributeName) ==
      this->m_attributeLocations.end()) {
    return -1;
  } else {
    return this->m_attributeLocations[attributeName];
  }
}

void NodalAttributes::read() {
  std::fstream fid(this->filename(), std::fstream::in);

  this->_readFort13Header(fid);
  this->_readFort13Defaults(fid);
  this->_fillDefaultValues();
  this->_readFort13Body(fid);

  fid.close();

  if (this->m_mesh != nullptr) {
    this->_mapNodes();
  }

  return;
}

void NodalAttributes::_readFort13Header(std::fstream &fid) {
  string tempLine;
  bool ok;

  std::getline(fid, tempLine);

  this->setHeader(tempLine);

  std::getline(fid, tempLine);

  size_t numnodes = StringConversion::stringToSizet(tempLine, ok);
  if (!ok) {
    adcircmodules_throw_exception("NodalAttributes: Error reading file data");
  }

  std::getline(fid, tempLine);

  size_t numparams = StringConversion::stringToSizet(tempLine, ok);
  if (!ok) {
    adcircmodules_throw_exception("NodalAttributes: Error reading file data");
  }

  this->setNumParameters(numparams);

  if (this->m_mesh != nullptr) {
    if (this->m_mesh->numNodes() != numnodes) {
      adcircmodules_throw_exception(
          "NodalAttributes: Number of nodes does not match provided mesh.");
    }
  } else {
    this->setNumNodes(numnodes);
  }

  this->m_nodalParameters.resize(this->numParameters());
  this->m_nodalData.resize(this->numParameters());
  for (size_t i = 0; i < this->numParameters(); ++i) {
    this->m_nodalData[i].resize(this->numNodes());
    for (size_t j = 0; j < this->numNodes(); ++j) {
      if (this->m_mesh != nullptr) {
        this->m_nodalData[i][j].setId(this->mesh()->node(j)->id());
      } else {
        this->m_nodalData[i][j].setId(j + 1);
      }
    }
  }

  return;
}

void NodalAttributes::_readFort13Defaults(std::fstream &fid) {
  string name, units, tempLine;
  vector<string> tempList;
  double defaultValue;
  vector<double> defaultValueVector;

  for (size_t i = 0; i < this->numParameters(); ++i) {
    std::getline(fid, name);
    name = StringConversion::sanitizeString(name);

    std::getline(fid, units);

    bool ok;
    std::getline(fid, tempLine);
    int nValues = StringConversion::stringToSizet(tempLine, ok);
    if (!ok) {
      adcircmodules_throw_exception("NodalAttributes: Error reading file data");
    }

    if (nValues == 1) {
      std::getline(fid, tempLine);
      defaultValue = StringConversion::stringToDouble(tempLine, ok);
      assert(ok);
      if (!ok) {
        adcircmodules_throw_exception(
            "NodalAttributes: Error reading file data");
      }

      this->m_nodalParameters[i] = AttributeMetadata(name, units, nValues);
      this->m_nodalParameters[i].setDefaultValue(defaultValue);
      this->m_attributeLocations[name] = i;

    } else {
      std::getline(fid, tempLine);
      int ierr = IO::splitString(tempLine, tempList);
      if (ierr != 0) {
        adcircmodules_throw_exception(
            "NodalAttributes: Error reading file data");
      }

      defaultValueVector.resize(nValues);
      for (size_t j = 0; j < nValues; ++j) {
        defaultValueVector[j] =
            StringConversion::stringToDouble(tempList[j], ok);
        assert(ok);
        if (!ok) {
          adcircmodules_throw_exception(
              "NodalAttributes: Error reading file data");
        }
      }

      this->m_nodalParameters[i] = AttributeMetadata(name, units, nValues);
      this->m_nodalParameters[i].setDefaultValue(defaultValueVector);
      this->m_attributeLocations[name] = i;
    }
  }
  return;
}

void NodalAttributes::_fillDefaultValues() {
  for (size_t i = 0; i < this->numParameters(); ++i) {
    if (this->m_nodalParameters[i].numberOfValues() == 1) {
      for (size_t j = 0; j < this->numNodes(); ++j) {
        this->m_nodalData[i][j].resize(1);
        this->m_nodalData[i][j].setValue(
            this->m_nodalParameters[i].getDefaultValue());
      }
    } else {
      for (size_t j = 0; j < this->numNodes(); ++j) {
        this->m_nodalData[i][j].resize(
            this->m_nodalParameters[i].numberOfValues());
        this->m_nodalData[i][j].setValue(
            this->m_nodalParameters[i].getDefaultValues());
      }
    }
  }
}

void NodalAttributes::_mapNodes() {
  for (size_t i = 0; i < this->numParameters(); ++i) {
    for (size_t j = 0; j < this->numNodes(); ++j) {
      this->m_nodalData[i][j].setNode(
          this->mesh()->nodeById(this->m_nodalData[i][j].id()));
    }
  }
  return;
}

void NodalAttributes::_readFort13Body(std::fstream &fid) {
  string tempLine, name;
  int ierr;
  size_t node;
  double value;
  bool ok;
  vector<double> values;
  vector<string> tempList;
  values.resize(12);

  for (size_t i = 0; i < this->numParameters(); ++i) {
    std::getline(fid, name);
    name = StringConversion::sanitizeString(name);
    size_t index = this->m_attributeLocations[name];

    std::getline(fid, tempLine);
    size_t numNonDefault = StringConversion::stringToSizet(tempLine, ok);
    assert(ok);
    if (!ok) {
      adcircmodules_throw_exception("NodalAttributes: Error reading file data");
    }

    size_t nValues = this->m_nodalParameters[index].numberOfValues();

    for (size_t j = 0; j < numNonDefault; ++j) {
      std::getline(fid, tempLine);

      if (nValues == 1) {
        ierr = IO::splitStringAttribute1Format(tempLine, node, value);
        assert(ierr == 0);
        if (ierr != 0) {
          adcircmodules_throw_exception(
              "NodalAttributes: Error reading file data");
        }

        Attribute *a;
        if (this->m_mesh != nullptr) {
          a = &this->m_nodalData[i][this->mesh()->nodeIndexById(node)];
        } else {
          a = &this->m_nodalData[i][node - 1];
        }
        a->setValue(value);
        a->setId(node);

      } else if (nValues == 12) {
        ierr = IO::splitStringAttribute12Format(tempLine, node, values);
        assert(ierr == 0);
        if (ierr != 0) {
          adcircmodules_throw_exception(
              "NodalAttributes: Error reading file data");
        };

        Attribute *a;
        if (this->m_mesh != nullptr) {
          a = &this->m_nodalData[i][this->mesh()->nodeIndexById(node)];
        } else {
          a = &this->m_nodalData[i][node - 1];
        }
        a->setValue(values);
        a->setId(node);

      } else {
        //...Generic routine here is slower than above, but there is
        //   no reason it will ever be used in ADCIRC's current state.
        //   Placed here as future-proofing.
        IO::splitString(tempLine, tempList);
        node = StringConversion::stringToSizet(tempList[0], ok);
        assert(ok);
        if (!ok) {
          adcircmodules_throw_exception(
              "NodalAttributes: Error reading file data");
        }

        for (size_t j = 1; j < tempList.size(); ++j) {
          value = StringConversion::stringToDouble(tempList[j], ok);
          assert(ok);
          if (!ok) {
            adcircmodules_throw_exception(
                "NodalAttributes: Error reading file data");
          }

          if (this->m_mesh != nullptr) {
            this->m_nodalData[i][this->mesh()->nodeIndexById(node)].setValue(
                j, value);
          } else {
            this->m_nodalData[i][node].setValue(j, value);
          }
        }
      }
    }
  }
  return;
}

size_t NodalAttributes::numNodes() const { return this->m_numNodes; }

void NodalAttributes::setNumNodes(size_t numNodes) {
  this->m_numNodes = numNodes;
}

size_t NodalAttributes::numParameters() const { return this->m_numParameters; }

void NodalAttributes::setNumParameters(size_t numParameters) {
  this->m_numParameters = numParameters;
}

string NodalAttributes::header() const { return this->m_header; }

void NodalAttributes::setHeader(const string &header) {
  this->m_header = header;
}

Attribute *NodalAttributes::attribute(size_t parameter, size_t node) {
  assert(node < this->numNodes());
  assert(parameter < this->numParameters());

  if (node < this->numNodes() && parameter < this->numParameters()) {
    return &this->m_nodalData[parameter][node];
  }
  adcircmodules_throw_exception(
      "NodalAttributes: Attribute could not be located");
  return nullptr;
}

Attribute *NodalAttributes::attribute(string parameter, size_t node) {
  size_t index = this->locateAttribute(std::move(parameter));
  return this->attribute(index, node);
}

string NodalAttributes::attributeNames(size_t index) {
  assert(index < this->m_nodalParameters.size());
  if (index < this->m_nodalParameters.size()) {
    return this->m_nodalParameters[index].name();
  }
  adcircmodules_throw_exception("NodalAttributes: Request out of bounds.");
  return string();
}

void NodalAttributes::write(const string &outputFilename) {
  std::ofstream outputFile;
  outputFile.open(outputFilename);
  this->_writeFort13Header(outputFile);
  this->_writeFort13Body(outputFile);
  outputFile.close();
  return;
}

void NodalAttributes::_writeFort13Header(std::ofstream &fid) {
  fid << this->m_header << "\n";
  fid << boost::str(boost::format("%11i\n") % this->numNodes());
  fid << boost::str(boost::format("%11i\n") % this->numParameters());
  for (auto &v : this->m_nodalParameters) {
    fid << v.headerString();
  }
  return;
}

void NodalAttributes::_writeFort13Body(std::ofstream &fid) {
  for (size_t i = 0; i < this->numParameters(); ++i) {
    size_t ndefault =
        this->_countDefault(this->m_nodalParameters[i], this->m_nodalData[i]);
    fid << this->m_nodalParameters[i].name() << "\n";
    fid << boost::str(boost::format("%11i\n") % ndefault);

    for (size_t j = 0; j < this->numNodes(); ++j) {
      if (!this->m_nodalParameters[i].checkIfDefaultValue(
              this->m_nodalData[i][j])) {
        fid << this->m_nodalData[i][j].write();
      }
    }
  }
  return;
}

size_t NodalAttributes::_countDefault(
    Adcirc::ModelParameters::AttributeMetadata &metadata,
    std::vector<Adcirc::ModelParameters::Attribute> &values) {
  size_t n = 0;
  for (const auto &v : values) {
    if (!metadata.checkIfDefaultValue(v)) {
      n++;
    }
  }
  return n;
}
