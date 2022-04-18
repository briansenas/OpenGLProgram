/*! \file
 * Copyright Brian Sena Simons
 * email: briansena@correo.ugr.es
 */

#ifndef PLYMODEL_H
#define PLYMODEL_H


#include "object3d.h"


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

class _plymodel:public _object3D
{
public:
  _plymodel(string filename="./ply_models/ant.ply");
};



#endif // PLYMODEL_H
