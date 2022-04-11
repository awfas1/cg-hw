//
// copyright 2018 Palestine Polytechnic Univeristy
//
// This software can be used and/or modified for academich use as long as 
// this commented part is listed
//
// Last modified by: Zein Salah, on 24.04.2018
//


#include "PPUViewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  PPUViewer tPPUViewer;
  tPPUViewer.show();

  return app.exec();
}