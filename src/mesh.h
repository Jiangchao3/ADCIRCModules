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
/**
 * @class Mesh
 * @author Zachary Cobell
 * @brief Class that handles operations using Adcirc mesh files
 * @copyright Copyright 2018 Zachary Cobell. All Rights Reserved.
 * @license This project is released under the terms of the GNU General Public
 * License v3
 *
 * The Mesh class handles functions related to reading an
 * adcirc mesh into memory and provides some facilities for
 * manipulation. The code is designed to be functional
 * with the python interface to the code.
 *
 * The code is able to handle meshes that are traditional, that
 * is they contained order indicies. When the code detects unordered
 * indicies, a translation table is automatically generated that allows
 * the code to continue to function appropriately and the id column of
 * the mesh will be considered a label.
 *
 * Performance is considered to be of the greatest importance and
 * therefore the Boost libraries have been utilized for reading
 * and writing of mesh data over the standard C++ functions.
 * All edits to this code should be benchmarked against a prior
 * version to ensure that memory usage and cpu time is not adversely
 * affected.
 *
 *
 */
#ifndef MESH_H
#define MESH_H

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "adcircmodules_global.h"
#include "boundary.h"
#include "element.h"
#include "node.h"
#include "point.h"

class QKdtree2;

namespace Adcirc {
namespace Geometry {

class Mesh {
 public:
  explicit Mesh();
  explicit Mesh(const std::string &filename);

  ~Mesh();

  std::vector<double> x();
  std::vector<double> y();
  std::vector<double> z();
  std::vector<std::vector<double>> xyz();
  std::vector<std::vector<size_t>> connectivity();

  void read();

  void write(const std::string &outputFile);

  std::string filename() const;
  void setFilename(const std::string &filename);

  std::string meshHeaderString() const;
  void setMeshHeaderString(const std::string &meshHeaderString);

  size_t numNodes() const;
  void setNumNodes(size_t numNodes);

  size_t numElements() const;
  void setNumElements(size_t numElements);

  size_t numOpenBoundaries() const;
  void setNumOpenBoundaries(size_t numOpenBoundaries);

  size_t numLandBoundaries() const;
  void setNumLandBoundaries(size_t numLandBoundaries);

  size_t totalOpenBoundaryNodes();

  size_t totalLandBoundaryNodes();

  int projection();
  void defineProjection(int epsg, bool isLatLon);
  void reproject(int epsg);
  bool isLatLon();

  void cpp(double lambda, double phi);
  void inverseCpp(double lambda, double phi);

  void toNodeShapefile(std::string outputFile);
  void toConnectivityShapefile(std::string outputFile);

  void buildNodalSearchTree();
  void buildElementalSearchTree();

  bool nodalSearchTreeInitialized();
  bool elementalSearchTreeInitialized();

  bool nodeOrderingIsLogical();
  bool elementOrderingIsLogical();

  size_t findNearestNode(Point location);
  size_t findNearestNode(double x, double y);
  size_t findNearestElement(Point location);
  size_t findNearestElement(double x, double y);
  size_t findElement(Point location);
  size_t findElement(double x, double y);

  Adcirc::Geometry::Node *node(size_t index);
  Adcirc::Geometry::Element *element(size_t index);
  Adcirc::Geometry::Boundary *openBoundary(size_t index);
  Adcirc::Geometry::Boundary *landBoundary(size_t index);

  Adcirc::Geometry::Node *nodeById(size_t id);
  Adcirc::Geometry::Element *elementById(size_t id);

  size_t nodeIndexById(size_t id);
  size_t elementIndexById(size_t id);

  void resizeMesh(size_t numNodes, size_t numElements, size_t numOpenBoundaries,
                  size_t numLandBoundaries);

  void addNode(size_t index, Adcirc::Geometry::Node &node);
  void deleteNode(size_t index);

  void addElement(size_t index, Adcirc::Geometry::Element &element);
  void deleteElement(size_t index);

  QKdtree2 *nodalSearchTree() const;
  QKdtree2 *elementalSearchTree() const;

 private:
  void _readMeshHeader(std::fstream &fid);
  void _readNodes(std::fstream &fid);
  void _readElements(std::fstream &fid);
  void _readOpenBoundaries(std::fstream &fid);
  void _readLandBoundaries(std::fstream &fid);
  void _init();

  std::string m_filename;
  std::string m_meshHeaderString;
  std::vector<Adcirc::Geometry::Node> m_nodes;
  std::vector<Adcirc::Geometry::Element> m_elements;
  std::vector<Adcirc::Geometry::Boundary> m_openBoundaries;
  std::vector<Adcirc::Geometry::Boundary> m_landBoundaries;
  std::unordered_map<size_t, size_t> m_nodeLookup;
  std::unordered_map<size_t, size_t> m_elementLookup;
  size_t m_numNodes;
  size_t m_numElements;
  size_t m_numOpenBoundaries;
  size_t m_numLandBoundaries;
  size_t m_totalOpenBoundaryNodes;
  size_t m_totalLandBoundaryNodes;
  int m_epsg;
  bool m_isLatLon;

  bool m_nodeOrderingLogical;
  bool m_elementOrderingLogical;

  std::unique_ptr<QKdtree2> m_nodalSearchTree;
  std::unique_ptr<QKdtree2> m_elementalSearchTree;
};
}  // namespace Geometry
}  // namespace Adcirc

#endif  // MESH_H
