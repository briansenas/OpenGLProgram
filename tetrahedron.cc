/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019
 * GPL 3
 */


#include "tetrahedron.h"


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

_tetrahedron::_tetrahedron(float Size)
{
  Vertices.clear();

  Vertices.push_back(_vertex3f(-Size/2,-Size/2,-Size/2));
  Vertices.push_back(_vertex3f(0,-Size/2,Size/2));
  Vertices.push_back(_vertex3f(Size/2,-Size/2,-Size/2));
  Vertices.push_back(_vertex3f(0,Size/2,0));

  Triangles.clear();

  Triangles.push_back(_vertex3ui(0,1,3));
  Triangles.push_back(_vertex3ui(1,2,3));
  Triangles.push_back(_vertex3ui(2,0,3));
  Triangles.push_back(_vertex3ui(0,2,1));

}

