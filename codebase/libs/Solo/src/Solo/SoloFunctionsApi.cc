#include <Solo/SoloFunctionsApi.hh>
#include <Solo/OneBoundary.hh>
#include <Solo/BoundaryPointMap.hh>
#include <Solo/PointInSpace.hh>
#include <Solo/SoloFunctions.hh>


//void SoloFunctionsApi::CreateBoundary() {
//
//}

SoloFunctionsApi::SoloFunctionsApi() {}


// get the boundary mask for a given:
// - list of boundary vertices
// - ray geometry
//
//short *SoloFunctionsApi::GetBoundaryMask(OneBoundary *boundaryList,
//                                           PointInSpace *radar_origin,
//                                           PointInSpace *boundary_origin,
void SoloFunctionsApi::GetBoundaryMask(long *xpoints, long *ypoints, int npoints,
					 //float radar_origin_x,
                                         //  float radar_origin_y,
                                         //  float radar_origin_z,
                                           float radar_origin_latitude,
                                           float radar_origin_longitude,
                                           float radar_origin_altitude,
                                           float boundary_origin_tilt,
                                           // float boundary_origin_x,
                                           // float boundary_origin_y,
                                           // float boundary_origin_z,
                                           float boundary_origin_latitude,
                                           float boundary_origin_longitude,
                                           float boundary_origin_altitude,
                                           int nGates,
                                           float gateSize,
                                           float distanceToCellNInMeters,
                                           float azimuth,
                                           int radar_scan_mode,
                                           int radar_type,
                                           float tilt_angle,
				       float rotation_angle,
				       bool *boundary_mask_out) {

  /*
  //This test boundary works
  for (int i=0; i<nGates; i++) {
    boundary_mask_out[i] = 0;
  }
  boundary_mask_out[3] = 1;
  boundary_mask_out[4] = 1;
  return;
  */

  /* TODO: test
  boundary_mask_out[0] = 3;
  boundary_mask_out[1] = 2;
  boundary_mask_out[3] = 1;
  return;
  // end test
  */

  // map flat data to internal data structures ...
  PointInSpace *radar_origin = new PointInSpace;
  PointInSpace *boundary_origin = new PointInSpace;

  // insert xy-points into OneBoundary structure
  OneBoundary *boundary = new OneBoundary();
  BoundaryPointMap map;

  // Fill the OneBoundary object with the boundary points
  for (int i=0; i<npoints; i++) {
    map.xse_add_bnd_pt(xpoints[i], ypoints[i], boundary);
  }

  //  radar_origin->x = radar_origin_x;
  //  radar_origin->y = radar_origin_y;
  //  radar_origin->z = radar_origin_z;
  radar_origin->latitude = radar_origin_latitude;
  radar_origin->longitude = radar_origin_longitude;
  radar_origin->altitude = radar_origin_altitude;

  //boundary_origin->x = boundary_origin_x;
  //boundary_origin->y = boundary_origin_y;
  //boundary_origin->z = boundary_origin_z;
  boundary_origin->latitude = boundary_origin_latitude;
  boundary_origin->longitude = boundary_origin_longitude;
  boundary_origin->altitude = boundary_origin_altitude;
  boundary_origin->tilt = boundary_origin_tilt;

  //  send the filled boundary object to the function that will
  // create the mask for the current ray geometry
  map.get_boundary_mask(  
  boundary,
    // bool new_sweep,  // assume new_sweep                                                                       
    //        bool operate_outside_bnd,                                                                           
    //bool shift_bnd,  // always shift                                                                            
    radar_origin,
    boundary_origin,
    nGates,
    gateSize,
    distanceToCellNInMeters,
    azimuth,    // TODO: are azimuth & rotation_angle the same thing? YES                                   
    radar_scan_mode,
    radar_type,
    tilt_angle,
  rotation_angle,
  boundary_mask_out);

  /*
  // copy boundary mask to output array
  for (int i=0; i<nGates; i++)
    boundary_mask_out[i] = boundary_mask[i];
  */

  delete boundary;
  delete radar_origin;
  delete boundary_origin;

  //return boundary_mask;
} 

void SoloFunctionsApi::Despeckle(const float *data, float *newData, size_t nGates, float bad, int a_speckle,
				 size_t dgi_clip_gate, bool *boundary_mask) {
  try {
    se_despeckle(data, newData, nGates, bad, a_speckle, dgi_clip_gate, boundary_mask);
  } catch(...) {
    throw "Something bad happened during script evaluation";
  }
}

  // data is in/out parameter
void SoloFunctionsApi::RemoveAircraftMotion(float vert_velocity, float ew_velocity, float ns_velocity,
					    float ew_gndspd_corr, float tilt, float elevation,
					    const float *data, float *newData, size_t nGates,
					    float bad, size_t dgi_clip_gate,
					    float dds_radd_eff_unamb_vel, float seds_nyquist_velocity,
					    bool *boundary_mask) {

  // TODO: remove parameter_scale and parameter_bias?

  try {

    /// All data are coming in scaled and biased                                              
    se_remove_ac_motion(vert_velocity, ew_velocity, ns_velocity,
			     ew_gndspd_corr, tilt, elevation,
			     data, newData, nGates,
			     bad, dgi_clip_gate,
			     dds_radd_eff_unamb_vel,
			     seds_nyquist_velocity, boundary_mask);


  } catch(...) {
    throw "Something bad happened during script evaluation";
  }

}

void SoloFunctionsApi::BBUnfoldFirstGoodGate(const float *data, float *newData, size_t nGates,
			   float nyquist_velocity, float dds_radd_eff_unamb_vel,
			   int max_pos_folds, int max_neg_folds,
			   size_t ngates_averaged,
			   float *last_good_v0,
			   float bad_data_value, size_t dgi_clip_gate, bool *boundary_mask) {
  try {

    /// All data are coming in scaled and biased
    // last_good_v0 is in/out parameter                                              
    se_BB_unfold_first_good_gate(data, newData, nGates,
				 nyquist_velocity, dds_radd_eff_unamb_vel,
				 max_pos_folds, max_neg_folds,
				 ngates_averaged,
				 last_good_v0,
				 bad_data_value, dgi_clip_gate, boundary_mask);

  } catch(...) {
    throw "Something bad happened during script evaluation";
  }

}



void SoloFunctionsApi::BBUnfoldAircraftWind(const float *data, float *newData, size_t nGates,
			  float nyquist_velocity, float dds_radd_eff_unamb_vel,
			  float azimuth_angle_degrees, float elevation_angle_degrees,
			  float ew_horiz_wind,
			  float ns_horiz_wind,
			  float vert_wind,
			  int max_pos_folds, int max_neg_folds,
			  size_t ngates_averaged,
			  float bad_data_value, size_t dgi_clip_gate, bool *boundary_mask) {
  try {

    /// All data are coming in scaled and biased                                              

    se_BB_unfold_ac_wind(data, newData, nGates,
			    nyquist_velocity, dds_radd_eff_unamb_vel,
			    azimuth_angle_degrees, elevation_angle_degrees,
			    ew_horiz_wind, ns_horiz_wind, vert_wind,
			    max_pos_folds, max_neg_folds,
			    ngates_averaged,
			    bad_data_value, dgi_clip_gate, boundary_mask);


  } catch(...) {
    throw "Something bad happened during script evaluation";
  }

}


void SoloFunctionsApi::BBUnfoldLocalWind(const float *data, float *newData, size_t nGates,
		       float nyquist_velocity, float dds_radd_eff_unamb_vel,
		       float azimuth_angle_degrees, float elevation_angle_degrees,
		       float ew_wind, float ns_wind, float ud_wind,
		       int max_pos_folds, int max_neg_folds,
		       size_t ngates_averaged,
		       float bad_data_value, size_t dgi_clip_gate, bool *boundary_mask) {
  try {

    /// All data are coming in scaled and biased                                              

    se_BB_unfold_local_wind(data, newData, nGates,
			    nyquist_velocity, dds_radd_eff_unamb_vel,
			    azimuth_angle_degrees, elevation_angle_degrees,
			    ew_wind, ns_wind, ud_wind,
			    max_pos_folds, max_neg_folds,
			    ngates_averaged,
			    bad_data_value, dgi_clip_gate, boundary_mask);


  } catch(...) {
    throw "Something bad happened during script evaluation";
  }

}



void SoloFunctionsApi::ZeroInsideBoundary(const float *data, bool *boundaryMask,
					  float *newData, size_t nGates) {


  if (boundaryMask != NULL) {
    for (size_t i=0; i<nGates; i++) {                                                                  
      if (!boundaryMask[i])                                                                                      
        newData[i] = data[i];                                                                                    
      else                                                                                                       
        newData[i] = 0.0;                                                                                        
    }             
  } else {
    for (size_t i=0; i<nGates; i++) {
      newData[i] = 0.0;
    } 
  }
}

