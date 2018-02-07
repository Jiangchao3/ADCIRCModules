//------------------------------GPL---------------------------------------//
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
//------------------------------------------------------------------------//
#ifndef NODALATTRIBUTES_H
#define NODALATTRIBUTES_H

#include "adcirc/geometry/mesh.h"
#include "adcirc/modelparameters/attribute.h"
#include "adcirc/modelparameters/attributemetadata.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace Adcirc {

namespace ModelParameters {

class NodalAttributes {
public:
  explicit NodalAttributes();

  explicit NodalAttributes(string filename,
                           Adcirc::Geometry::Mesh *mesh = nullptr);
  explicit NodalAttributes(const char *filename,
                           Adcirc::Geometry::Mesh *mesh = nullptr);

  int read();

  void setFilename(string filename);
  void setFilename(const char *filename);
  string filename();

  void setMesh(Adcirc::Geometry::Mesh *mesh);
  Adcirc::Geometry::Mesh *mesh();

  int write(string outputFilename);
  int write(const char *filename);

  string attributeNames(int index);
  const char *attributeNamesChar(int index);

  int locateAttribute(string attributeName);
  int locateAttribute(const char *attributeName);

  string header() const;
  void setHeader(const string &title);

  int numParameters() const;
  void setNumParameters(int numParameters);

  int numNodes() const;
  void setNumNodes(int numNodes);

  Attribute *attribute(int parameter, int node);
  Attribute *attribute(string parameter, int node);
  Attribute *attribute(const char *parameter, int node);

private:
  int _readFort13Header(std::fstream &fid);
  int _readFort13Defaults(std::fstream &fid);
  int _readFort13Body(std::fstream &fid);
  void _fillDefaultValues();
  void _mapNodes();

  /// Mapping function between the name of a nodal parameter and its position in
  /// the nodalParameters vector
  std::unordered_map<string, int> m_attributeLocations;

  /// Filename of the file that will be read or was read
  string m_filename;

  /// Title/header found at the beginning of the nodal attributes file
  string m_header;

  /// Number of nodal attributes found within this file
  int m_numParameters;

  /// Number of nodes in the mesh that this nodal attribute files references
  int m_numNodes;

  /// Underlying adcirc_mesh object (if necessary)
  Adcirc::Geometry::Mesh *m_mesh;

  /// Vector of objects containing the nodal parameters read from the file
  vector<AttributeMetadata> m_nodalParameters;

  /// Vector of objects for the nodal parameters
  vector<vector<Attribute> > m_nodalData;
};
} // namespace ModelParameters
} // namespace Adcirc
#endif // NODALATTRIBUTES_H
