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

  string BBUnfoldFirstGoodGate(string fieldName, RadxVol *vol,
			       int rayIdx, int sweepIdx,
			       float nyquist_velocity,
			       int max_pos_folds,
			       int max_neg_folds,
			       size_t ngates_averaged,
			       size_t clip_gate,
			       float bad_data_value,
			       string newFieldName);


  string SetBadFlagsAbove(string fieldName,  RadxVol *vol,
			  int rayIdx, int sweepIdx,
			  float lower_threshold,
			  size_t clip_gate,
			  float bad_data_value,
			  string badFlagMaskFieldName);

  string SetBadFlagsBelow(string fieldName,  RadxVol *vol,
			  int rayIdx, int sweepIdx,
			  float lower_threshold,
			  size_t clip_gate,
			  float bad_data_value,
			  string badFlagMaskFieldName);

  string SetBadFlagsBetween(string fieldName,  RadxVol *vol,
			  int rayIdx, int sweepIdx,
			  float lower_threshold,
			  float upper_threshold,
			  size_t clip_gate,
			  float bad_data_value,
			  string badFlagMaskFieldName);

  string SetBadFlags(string fieldName,  RadxVol *vol,
		     int rayIdx, int sweepIdx,
                     string where,
		     float lower_threshold, float upper_threshold,
		     size_t clip_gate,
		     float bad_data_value,
		     string badFlagMaskFieldName);


  string AssertBadFlags(string fieldName,  RadxVol *vol,
			int rayIdx, int sweepIdx,
			size_t clip_gate,
			float bad_data_value,
			string badFlagMaskFieldName);

  string ClearBadFlags(string badFlagMaskFieldName,  RadxVol *vol,
		       int rayIdx, int sweepIdx);
		       //size_t clip_gate,
		       //float bad_data_value,
		       //string badFlagMaskFieldName);

  string ComplementBadFlags(string fieldName,  RadxVol *vol,
			    int rayIdx, int sweepIdx);
  //			    size_t clip_gate,
  //		    float bad_data_value,
  //		    string badFlagMaskFieldName);

  /*
  string BBUnfoldAircraftWind(string fieldName, RadxVol *vol,
			      int rayIdx, int sweepIdx,
			      float nyquist_velocity,
			      int max_pos_folds,
			      int max_neg_folds,
			      size_t ngates_averaged,
			      size_t clip_gate,
			      float bad_data_value,
			      string newFieldName);

  */
  /*
const float *data, float *newData, size_t nGates,
                            float nyquist_velocity, float dds_radd_eff_unamb_vel,
                            float azimuth_angle_degrees, float elevation_angle_degrees,
// these come from the platform (asib) information
                            float ew_horiz_wind,
                            float ns_horiz_wind,
                            float vert_wind,

                            int max_pos_folds, int max_neg_folds,
                            size_t ngates_averaged,
                            float bad_data_value, size_t dgi_clip_gate, bool *boundary_mask);
  */
  /*
  string BBUnfoldLocalWind(string fieldName, RadxVol *vol,
			   int rayIdx, int sweepIdx,
			   float nyquist_velocity,
			   float ew_wind, float ns_wind, float ud_wind,
			   int max_pos_folds,
			   int max_neg_folds,
			   size_t ngates_averaged,
			   size_t clip_gate,
			   float bad_data_value,
			   string newFieldName);
  */
  /*
const float *data, float *newData, size_t nGates,
			 float nyquist_velocity, float dds_radd_eff_unamb_vel,
			 float azimuth_angle_degrees, float elevation_angle_degrees,
// these come from the editor (from the script)
			 float ew_wind, float ns_wind, float ud_wind,

			 int max_pos_folds, int max_neg_folds,
			 size_t ngates_averaged,
			 float bad_data_value, size_t dgi_clip_gate, bool *boundary_mask);
  */


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
