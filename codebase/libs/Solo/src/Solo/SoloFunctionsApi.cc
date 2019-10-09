#include <Solo/SoloFunctionsApi.hh>

//void SoloFunctionsApi::CreateBoundary() {
//
//}

SoloFunctionsApi::SoloFunctionsApi() {}

void SoloFunctionsApi::CreateBoundary(short *xpoints, short *ypoints, int npoints, char *name) {



}

// get the boundary mask for a given:
// - list of boundary vertices
// - ray
//
short *SoloFunctionsApi::GetBoundaryMask(OneBoundary *boundaryList,
                                           PointInSpace *radar_origin,
                                           PointInSpace *boundary_origin,
                                           int nGates,
                                           float gateSize,
                                           float distanceToCellNInMeters,
                                           float azimuth,
                                           int radar_scan_mode,
                                           int radar_type,
                                           float tilt_angle,
                                           float rotation_angle) {

   return NULL;
} 



  // data is in/out parameter
void SoloFunctionsApi::RemoveAircraftMotion(float vert_velocity, float ew_velocity, float ns_velocity,
						   float ew_gndspd_corr, float tilt, float elevation,
						   short *data, short bad, float parameter_scale,
						   float parameter_bias, int dgi_clip_gate,
					    short dds_radd_eff_unamb_vel, int seds_nyquist_velocity, char *boundary_name) {

}


/*
RemoveAircraftMotion(vert_velocity, ew_velocity, ns_velocity,
				   ew_gndspd_corr, tilt, elevation,
				   short *data, bad, parameter_scale, parameter_bias, dgi_clip_gate,
				   dds_radd_eff_unamb_vel, seds_nyquist_velocity, short *boundary_mask); 
				   */
  /*   //SoloFunctionsApi soloFunctionsApi;                                                       
  int result = se_remove_ac_motion(vert_velocity, ew_velocity, ns_velocity,
				   ew_gndspd_corr, tilt, elevation,
				   field->getDataSi16(), bad, parameter_scale, parameter_bias, dgi_clip_gate,
				   dds_radd_eff_unamb_vel, seds_nyquist_velocity, boundary);
  
{

  // lookup the boundary by name



}
*/
