HEADERS += \
  colors.h \
  basic_object3d.h \
  cone.h \
  cylinder.h \
  object3d.h \
  axis.h \
  operate.h \
  plymodel.h \
  plysweep.h \
  sphere.h \
  tetrahedron.h \
  glwidget.h \
  window.h \
  cube.h \
  file_ply_stl.h \
  wheel.h \
  wheels_axis.h \
  tooth.h\
  teeth.h \
  forearm.h \
  hydraulic.h \
  excavator.h \
  tablero.h

SOURCES += \
  basic_object3d.cc \
  cone.cpp \
  cylinder.cc \
  object3d.cc \
  axis.cc \
  plymodel.cc \
  plysweep.cpp \
  sphere.cc \
  tetrahedron.cc \
  main.cc \
  glwidget.cc \
  window.cc \
  cube.cc \
  operate.cc \
  file_ply_stl.cc \
  wheel.cc \
  wheels_axis.cc \
  tooth.cc\
  teeth.cc \
  forearm.cc \
  hydraulic.cc \
  excavator.cc \
  tablero.cc

LIBS += -L/usr/X11R6/lib64 -lGL -lGLU -lGLEW


CONFIG += c++11
QT += widgets
