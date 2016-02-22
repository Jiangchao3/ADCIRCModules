/*-------------------------------GPL-------------------------------------//
//
// QADCModules - A library for working with ADCIRC models
// Copyright (C) 2015  Zach Cobell
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
 * \class adcirc_node
 *
 * \addtogroup QADCModules
 *
 * \brief Class that describes an ADCIRC node
 *
 * \author Zachary Cobell
 * \version Version 0.1
 * \date 02/21/2016
 *
 * Contact: zcobell@gmail.com
 *
 * Created on: 02/21/2016
 *
 */
#ifndef ADCIRC_NODE_H
#define ADCIRC_NODE_H

#include <QObject>

class adcirc_node : public QObject
{
    Q_OBJECT
public:
    explicit adcirc_node(QObject *parent = 0);

    double x;
    double y;
    double z;
    int    id;

};

#endif // ADCIRC_NODE_H
