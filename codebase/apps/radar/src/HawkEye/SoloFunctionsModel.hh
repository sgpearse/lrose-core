#ifndef SOLOFUNCTIONSMODEL_H
#define SOLOFUNCTIONSMODEL_H

#include <stdio.h>

#include <vector>
#include <iostream>
#include <string>

#include "Radx/RadxVol.hh"

using namespace std;


class SoloFunctionsModel
{


public:
  SoloFunctionsModel() {}

  vector<double> RemoveAircraftMotion(vector<double>, RadxVol *vol);
  vector<double> RemoveAircraftMotion(string fieldName, RadxVol *vol);
  vector<double> RemoveAircraftMotion(string fieldName, RadxVol *vol,
				      int rayIdx, int sweepIdx);


  string ZeroMiddleThird(string fieldName,  RadxVol *vol,
			 int rayIdx, int sweepIdx,
			 string newFieldName);

  string ZeroInsideBoundary(string fieldName,  RadxVol *vol,
			 int rayIdx, int sweepIdx,
			 string newFieldName);

  void SetBoundaryMask(RadxVol *vol, int rayIdx, int sweepIdx);
  void SetBoundaryMaskOriginal(RadxVol *vol, int rayIdx, int sweepIdx);
 
  void printBoundaryMask();

private:

  //  SpreadSheetModel *dataModel;
  short *_boundaryMask;
  size_t _boundaryMaskLength;
  bool _boundaryMaskSet;
};


#endif
