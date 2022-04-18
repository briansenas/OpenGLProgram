/*! \file
 * Copyright Domingo Martín Perandres
 * email: dmartin@ugr.es
 * web: http://calipso.ugr.es/dmartin
 * 2003-2019
 * GPL 3
 */


#ifndef BASIC_OBJECT3D_H
#define BASIC_OBJECT3D_H

#include <vector>
#include <GL/gl.h>
#include "colors.h"
#include "vertex.h"


/*****************************************************************************//**
 *
 *
 *
 *****************************************************************************/

class _basic_object3D
{
  public:

  vector<_vertex3f> Vertices;

  void draw_point();
};

#endif
