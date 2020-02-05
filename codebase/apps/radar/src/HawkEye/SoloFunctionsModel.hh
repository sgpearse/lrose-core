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
  SoloFunctionsModel();

  vector<double> RemoveAircraftMotion(vector<double>, RadxVol *vol);
  vector<double> RemoveAircraftMotion(string fieldName, RadxVol *vol);
  vector<double> RemoveAircraftMotion(string fieldName, RadxVol *vol,
				      int rayIdx, int sweepIdx);

  string RemoveAircraftMotion(string fieldName, RadxVol *vol,
			      int rayIdx, int sweepIdx,
			      float nyquist_velocity,
			      size_t clip_gate,
			      float bad_data_value,
			      string newFieldName);


  string ZeroMiddleThird(string fieldName,  RadxVol *vol,
			 int rayIdx, int sweepIdx,
			 string newFieldName);

  string ZeroInsideBoundary(string fieldName,  RadxVol *vol,
			 int rayIdx, int sweepIdx,
			 string newFieldName);

  string Despeckle(string fieldName,  RadxVol *vol,
		   int rayIdx, int sweepIdx,
		   size_t speckle_length,
		   size_t clip_gate,
		   float bad_data_value,
		   string newFieldName);

  void SetBoundaryMask(RadxVol *vol, int rayIdx, int sweepIdx);
  void SetBoundaryMaskOriginal(RadxVol *vol, int rayIdx, int sweepIdx);
  void CheckForDefaultMask(RadxVol *vol, int rayIdx, int sweepIdx);
  void SetDefaultMask(RadxVol *vol, int rayIdx, int sweepIdx);

  void DetermineBoundaryMask(RadxVol *vol, int rayIdx, int sweepIdx); 
  void printBoundaryMask();

private:

  //  SpreadSheetModel *dataModel;
  bool *_boundaryMask;
  size_t _boundaryMaskLength;
  bool _boundaryMaskSet;
};


#endif
