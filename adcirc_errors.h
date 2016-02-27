/*-------------------------------GPL-------------------------------------//
//
// ADCMESH - A library for working with ADCIRC models
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
 * \class adcirc_erros
 *
 * \brief Class to relay error messages to the user
 *
 * \author Zach Cobell
 *
 * The adcirc_errors class is designed to be used by all other classes
 * in this suite of classes to relay error messages. Error messages are
 * mapped to a set of descriptions on startup so that the user can easily
 * understand where a particular function has failed. All routines must
 * return an error code as defined by this class or no error (ERROR_NOERROR)
 * to achieve successful completion
 *
 * Contact: zcobell@gmail.com
 *
 */
#ifndef ADCIRC_ERRORS_H
#define ADCIRC_ERRORS_H

#include "QADCModules_global.h"
#include <QObject>
#include <QMap>

//----------------------------------------------------//
//...Define a set of errors for use with this
//   class
#define ERROR_NOERROR      -999900000
#define ERROR_FILEOPENERR  -999901001
#define ERROR_NULLFILENAM  -999901002
#define ERROR_FILENOEXIST  -999901003

#define ERROR_MESHREAD_HEADER -999902001
#define ERROR_MESHREAD_NODERR -999902002
#define ERROR_MESHREAD_ELEMER -999902003
#define ERROR_MESHREAD_BNDERR -999902004
#define ERROR_MESHREAD_NODNUM -999902005
#define ERROR_MESHREAD_ELENUM -999902006
#define ERROR_MESHREAD_BNDUNK -999902007
#define ERROR_MESHREAD_BNDNUL -999902008

#define ERROR_NODALPARAM_DEFAULTVALUESIZE -999903001
#define ERROR_NODALPARAM_NOTENOUGHVALUES  -999903002
#define ERROR_NODALPARAM_READERROR        -999903003
#define ERROR_NODALPARAM_OUTOFRANGE       -999903004

//----------------------------------------------------//


class QADCMODULESSHARED_EXPORT adcirc_errors : public QObject
{
    Q_OBJECT
public:

    explicit adcirc_errors(QObject *parent = 0);

    ///Stores the current error code for retrieval later
    int errorCode;

    QString getErrorString();

protected:

    ///Map function between error codes and their descriptions
    QMap<int,QString> errorMapping;

    int initializeErrors();


};

#endif // ADCIRC_ERRORS_H
