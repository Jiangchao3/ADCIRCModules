/*-------------------------------GPL-------------------------------------//
//
// QADCModules - A library for working with ADCIRC models
// Copyright (C) 2016  Zach Cobell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------*/
/**
 * \class adcirc_mesh
 *
 * \brief Class that describes an ADCIRC mesh
 *
 * \author Zach Cobell
 *
 * The ADCIRC mesh class is a set of routines that is designed to read/write
 * ADCIRC format meshes. The setup is such that it can be easily manipulated
 * so that codes/routines can be written using this class to manipulate meshes
 *
 * Contact: zcobell@gmail.com
 *
 */
#ifndef ADCIRC_MESH_H
#define ADCIRC_MESH_H

#include <QPointer>
#include <QTextStream>
#include <QObject>
#include <QVector>
#include <QFile>
#include <QMap>
#include <QList>
#include <QCryptographicHash>
#include <qmath.h>

#include "QADCModules_global.h"
#include "adcirc_forward.h"

class QADCMODULESSHARED_EXPORT adcirc_mesh : public QObject
{
    Q_OBJECT

public:

    explicit adcirc_mesh(QObject *parent = 0);

    explicit adcirc_mesh(QADCModules_errors *error, QObject *parent = 0);

    //...Public variables...//

    ///Filename for this ADCIRC mesh
    QString                            filename;

    ///Title, or header, from the mesh file
    QString                            title;

    ///Number of elements contained within the mesh file
    int                                numElements;

    ///Number of nodes contained with the mesh file
    int                                numNodes;

    ///Vector made up of the class adcirc_node with x, y, and z information for nodes in this mesh
    QVector<adcirc_node*>              nodes;

    ///Vector made up of the class adcirc_element with connectivity information
    QVector<adcirc_element*>           elements;

    ///Number of open boundary conditions contained within the mesh
    int                                numOpenBoundaries;

    ///Total number of open boundary nodes in this mesh
    int                                totNumOpenBoundaryNodes;

    ///Number of land boundary conditions within the mesh
    int                                numLandBoundaries;

    ///Total number of land boundary nodes in this mesh
    int                                totNumLandBoundaryNodes;

    ///Vector made up of the class adcirc_boundary with the open boundary information
    QVector<adcirc_boundary*>          openBC;

    ///Vector made up of the class adcirc_boundary with the land boundary information
    QVector<adcirc_boundary*>          landBC;

    ///QADCModules_errors class to track the error codes generated by routines in this class
    QADCModules_errors*                error;

    ///adcirc_node_table class to keep track of the elements that are around a node
    QVector<adcirc_node_table*>        node_table;

    ///Variable that lets other portions of the code know if the mesh is in a geographic coordinate system
    bool isLatLon;

    ///Variable that determines the EPSG coordinate system reference. Default is geographic (WGS84, EPSG: 4326)
    int epsg;

    ///Map function between a node ID and its position in the node vector
    QMap<int,int> nodeToPositionMapping;

    ///Map function between array position and the node id
    QMap<int,int> nodeToIdMapping;

    ///Map function between an element ID and its position in the element vector
    QMap<int,int> elementToPositionMapping;

    ///Map function between array position and the element id
    QMap<int,int> elementToIdMapping;

    ///Unique hash that describes the state of this mesh
    QString hash;


    //...Public functions...///

    int read();
    int read(QString inputFile);

    int readNetCDF();
    int readNetCDF(QString inputFile);

    int write(QString outputFile);

    int setIgnoreMeshNumbering(bool value);

    int renumber();

    int buildElementTable();

    int buildNodalSearchTree();

    int buildElementalSearchTree();

    int checkLeveeHeights(qreal minAbovePrevailingTopo = 0.20);

    int raiseLeveeHeights(int &numLeveesRaised, qreal &maximumAmountRaised,
                          qreal minAbovePrevailingTopo = 0.20, qreal minRaise = 0.01,
                          QString diagnosticFile = QString());

    int checkDisjointNodes(int &numDisjointNodes, QList<adcirc_node *> &disjointNodeList);

    int eliminateDisjointNodes(int &numDisjointNodes);

    int checkOverlappingElements(int &numOverlappingElements, QList<adcirc_element*> &overlappingElementList);

    int checkOverlappingBoundaries(int &numOverlappingBoundaries, QList<adcirc_node *> &overlappingBoundaryNodeList);

    int checkBoundariesOnBoundary(int &numberOfBadBCs, QList<adcirc_node *> &boundaryConditionList);

    int checkClosedIslandBoundaries(int &numUnclosedIslandBoundaries);

    int setProjection(int epsg);

    int project(int epsg);

    int findNearestNode(QPointF pointLocation, adcirc_node* &nearestNode);
    int findNearestNode(qreal x, qreal y, adcirc_node* &nearestNode);

    int findXNearestNodes(QPointF pointLocation, int nn, QList<adcirc_node *> &nodeList);
    int findXNearestNodes(qreal x, qreal y, int nn, QList<adcirc_node *> &nodeList);

    int findXNearestElements(QPointF pointLocation, int nn, QList<adcirc_element *> &elementList);
    int findXNearestElements(qreal x, qreal y, int nn, QList<adcirc_element *> &elementList);

    int findElement(QPointF pointLocation, adcirc_element* &nearestElement, bool &found, QVector<qreal> &weights);
    int findElement(QPointF pointLocation, adcirc_element* &nearestElement, bool &found);
    int findElement(qreal x, qreal y, adcirc_element *&nearestElement, bool &found, QVector<qreal> &weights);
    int findElement(qreal x, qreal y, adcirc_element* &nearestElement, bool &found);

    int toShapefile(QString outputFile);

    int setHashAlgorithm(QCryptographicHash::Algorithm hashType);

    int hashMesh();

private:

    //...PRIVATE VARIABLES...//

    ///Variable that is checked to decide if non-sequential mesh numbering is a fatal error. Default = false
    bool ignoreMeshNumbering;

    ///Variable that lets other portions of the code know if the mesh is correctly numbered or not
    bool meshNeedsNumbering;

    ///Instance of class used for coordinate system transformation
    proj4 *coordinateSystem;

    ///Pointer that holds a kd-tree search tree for this mesh's nodes
    QPointer<qKdtree2> nodalSearchTree;

    ///Pointer that holds a kd-tree search tree for this mesh's elements
    QPointer<qKdtree2> elementalSearchTree;

    ///Hash algorithm that should be used when developing any hashes for this mesh (Default: SHA1)
    QCryptographicHash::Algorithm hashAlgorithm;


protected:

    //...PROTECTED FUNCTIONS...//

    int readMesh();

    int readMeshFromNetCDF();

    int writeMesh(QString filename);

    int allocateNodes();

    int allocateElements();

    int readOpenBoundaries(int &position, QStringList &fileData);

    int readLandBoundaries(int &position, QStringList &fileData);

    int findAdcircElement(QPointF location, adcirc_element* &nearestElement, bool &found, QVector<qreal> &weights);

};

#endif // ADCIRC_MESH_H
