// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2020                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 

#ifndef SOLOFUNCTIONSAPI_H
#define SOLOFUNCTIONSAPI_H

#include <stdio.h>
#include <vector>
#include <iostream>

#include <Solo/PointInSpace.hh>
#include <Solo/OneBoundary.hh>

using namespace std;

// This is the external interface to the Solo Functions Library

class SoloFunctionsApi
{


public:
  SoloFunctionsApi();

  void CreateBoundary(short *xpoints, short *ypoints, int npoints, char *name);


  // 
  // call BoundaryPointMap::get_boundary_mask to do the work
  // 
  short *GetBoundaryMask(OneBoundary *boundaryList,
					   PointInSpace *radar_origin,
					   PointInSpace *boundary_origin,
					   int nGates,
					   float gateSize,
					   float distanceToCellNInMeters,
					   float azimuth,
					   int radar_scan_mode,
					   int radar_type,
					   float tilt_angle,
					   float rotation_angle); 


  // TODO: use Radx::SI16, etc? to standardize the numberic sizes?  Ask Mike about this.
  // Q: send a pointer to the boundary mask? or the name of the boundary mask?
  // name of the boundary mask, 1st.  Then, translate the name to the list of
  // boolean mask; The API performs the translation.

  // data is in/out parameter
  void RemoveAircraftMotion(float vert_velocity, float ew_velocity, float ns_velocity,
			    float ew_gndspd_corr, float tilt, float elevation,
			    short *data, short bad, float parameter_scale, 
			    float parameter_bias, int dgi_clip_gate,
			    short dds_radd_eff_unamb_vel, int seds_nyquist_velocity, char *boundary_name); 
  /*   //SoloFunctionsApi soloFunctionsApi;                                                       
  int result = se_remove_ac_motion(vert_velocity, ew_velocity, ns_velocity,
				   ew_gndspd_corr, tilt, elevation,
				   field->getDataSi16(), bad, parameter_scale, parameter_bias, dgi_clip_gate,
				   dds_radd_eff_unamb_vel, seds_nyquist_velocity, boundary);
  */

private:

};


#endif
