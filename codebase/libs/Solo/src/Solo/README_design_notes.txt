extracting code from Soloii ...
remove the use of seds, dgi data structures; instead, pass args
break dgi into separate chunks of info

pass around the boundary structures? rather than keep them in a global, like seds?

remove the command structure?
keep the boundary data structures ... 
- BoundaryStuff *sebs 
- boundary point management
- OneBoundary
- BoundaryPoint ...
- PointInSpace 


NOTE: many pieces of info are passed around inside global structures
For example, a value inside a global structure is set in one
function, then in another function, the value is retrieved.
This is done frequently instead of passing the info as 
arguments and return values.

10/3/2019  -------------

From calling program ...
// create boundary (points, name)
// get boundary mask(name) ?? may not need this; just create mask when boundary is first used
// set current boundary (name) ?? may not need this; if we reference boundaries by name.
// call f(short *data, string boundary_name, etc.)
// Internally: call f(short *data, short *boundary_mask, etc.)
// return short *out_data
//

create_boundary(xypoints, string name) {

// TEST(SoloBoundary, get_boundary_mask_same_origin_zero_distance_to_gates) {

  // all data must be gridded, so x,y coordinates, plus the value                                                       

  float boundaryX[] = {};
  float boundaryY[] = {};
  float oneRayData[] = {4.0, 5.0};
  float *outData;
  float expectedValue[] = {5.0, 6.0};

  int nBoundaryPoints = 4;
  float gateSizeInMeters = 10;
  float rayAzimuth = 0.0;

  short *boundaryMask; //  = new short(nBoundaryPoints);                                                                
  // Where it is now ...                                                                                                
  OneBoundary *boundaryList = new OneBoundary();
    BoundaryPointMap map;

    map.xse_add_bnd_pt(2, 2, boundaryList);
    map.xse_add_bnd_pt(13, 2, boundaryList);
    map.xse_add_bnd_pt(13, 11, boundaryList);
    map.xse_add_bnd_pt(2, 11, boundaryList);

    return boundaryList;
}


  OneBoundary boundaryList = create_boundary(xypoints); 

  PointInSpace radar_origin;
  PointInSpace boundary_origin;
  float gateSize = 1.5;
  int nGates = 15;
  float distanceToCellNInMeters = 0;
  float azimuth = 45.0;
  int radar_scan_mode = PPI;
  int radar_type = GROUND;

  // TODO: there is a current_tilt AND a boundary_radar->tilt in se_shift_bnd                                           
  // What is the difference between these?                                                                              
  //                                                                                                                    
  float tilt_angle = 0;  // TODO: are tilt_angle & rotation_angle the same thing?                                       
  float rotation_angle = azimuth;  // TODO: is this azimuth?                                                            
  radar_origin.latitude = 40.0;
  radar_origin.longitude = 40.0;
  radar_origin.altitude = 0.0;

  boundary_origin.latitude = 40.0;
  boundary_origin.longitude = 40.0;
  boundary_origin.altitude = 0.0;
  boundary_origin.tilt = 0.0;

  BoundaryPointMap bpm;

  boundaryMask = bpm.get_boundary_mask(boundaryList,
                                           &radar_origin,
                                            &boundary_origin,
                                             nGates,
                                             gateSize,
                                             distanceToCellNInMeters,
                                             azimuth,
                                             radar_scan_mode,
                                             radar_type,
                                             tilt_angle,
                                             rotation_angle);

  boundaryList->print();

// TODO: set boundaryList as current boundary; all calls to f(x) use the current boundary

// call f(short *data, short *boundary_mask, etc.)
  se_remove_ac_motion(short *data_copy, short *boundaryMask, etc.);

  return data_copy;
}
