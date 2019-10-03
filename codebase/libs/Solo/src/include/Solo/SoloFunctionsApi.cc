#include "SoloFunctions.hh"

void SoloFunctionsApi::CreateBoundary() {

}

SoloFunctionsApi::SoloFunctions() {}

SoloFunctionsApi::CreateBoundary(short *xpoints, short *ypoints, int npoints, char *name) {



}

// get the boundary mask for a given:
// - list of boundary vertices
// - ray
//
SoloFunctionsApi::GetBoundaryMask(boundaryList,
				       &radar_origin,
				       &boundary_origin,
				       nGates,
				       gateSize,
				       distanceToCellNInMeters,
				       azimuth,
				       radar_scan_mode,
				       radar_type,
				       tilt_angle,
			       rotation_angle) {

} 



  // data is in/out parameter
SoloFunctionsApi::RemoveAircraftMotion(vert_velocity, ew_velocity, ns_velocity,
				   ew_gndspd_corr, tilt, elevation,
				   short *data, bad, parameter_scale, parameter_bias, dgi_clip_gate,
				   dds_radd_eff_unamb_vel, seds_nyquist_velocity, short *boundary_mask); 
  /*   //SoloFunctionsApi soloFunctionsApi;                                                       
  int result = se_remove_ac_motion(vert_velocity, ew_velocity, ns_velocity,
				   ew_gndspd_corr, tilt, elevation,
				   field->getDataSi16(), bad, parameter_scale, parameter_bias, dgi_clip_gate,
				   dds_radd_eff_unamb_vel, seds_nyquist_velocity, boundary);
  */
{

  // lookup the boundary by name



}
