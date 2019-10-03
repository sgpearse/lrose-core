#ifndef SOLOFUNCTIONS_H
#define SOLOFUNCTIONS_H

#include <stdio.h>

#include <vector>
#include <iostream>

using namespace std;

// This is the external interface to the Solo Functions Library

class SoloFunctions
{


public:
  SoloFunctions() {}

  CreateBoundary(short *xpoints, short *ypoints, int npoints, char *name);

  // data is in/out parameter
  RemoveAircraftMotion(short *data, char *boundary_name); 
  /*   //SoloFunctionsApi soloFunctionsApi;                                                       
  int result = se_remove_ac_motion(vert_velocity, ew_velocity, ns_velocity,
				   ew_gndspd_corr, tilt, elevation,
				   field->getDataSi16(), bad, parameter_scale, parameter_bias, dgi_clip_gate,
				   dds_radd_eff_unamb_vel, seds_nyquist_velocity, boundary);
  */

private:

};


#endif
