// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
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
////////////////////////////////////////////////
//
// Mdvx_typedefs64.hh
//
// Typedefs for Mdvx class - 64-bit upgrade
//
////////////////////////////////////////////////

// This header file can only be included from within Mdvx.hh
#ifdef _in_Mdvx_hh

// Defines for header string and array sizes.
// These should not be changed since that would alter the file format.

#define     MDV64_CHUNK_INFO_LEN      1024
#define     MDV64_INFO_LEN            2048
#define     MDV64_LONG_FIELD_LEN       256
#define     MDV64_MAX_PROJ_PARAMS       16
#define     MDV64_MAX_VLEVELS         2048 
#define     MDV64_NAME_LEN             512
#define     MDV64_SHORT_FIELD_LEN       64
#define     MDV64_TRANSFORM_LEN         64
#define     MDV64_UNITS_LEN             64
#define     MDV64_N_COORD_LABELS         3
#define     MDV64_COORD_UNITS_LEN       64


//////////////////
// MDV64_master_header_t

typedef struct {

  mutable si32  record_len1;       // Fortran record length field (bytes)
  mutable si32  struct_id;         // 1 Magic cookie ID for this struct

  mutable si32  revision_number;   // 2 Minor revision number.
                                   // Major revisions are documented by
                                   // the Magic cookie ID
  
  si64  time_gen;                  // 3 Time data generated.
                                   //   For model data, this is the init time.

  si64  user_time;                 // 4 Time value specific to the dataset.
                                   //   This time value is not used by any
                                   //   of the general MDV code, but can be
                                   //   used by users of specific datasets.

  si64  time_begin;                // 5 Begin time of dataset.  For forecast
                                   //   datasets, this is the begin time of
                                   //   the data used to generate the
                                   //   forecast.
                                   //   Seconds since 1970

  si64  time_end;                  // 6 End time of dataset.  For forecast
                                   //   datasets, this is the end time of the
                                   //   data used to generate the forecast.
                                   //   Seconds since 1970

  mutable si64  time_centroid;     // 7 Midpoint time of dataset.  This time
                                   //   is not necessarily directly in the
                                   //   middle of time_begin and time_end
                                   //   since some datasets have data skewed
                                   //   more towards the beginning or ending
                                   //   data time.  For forecast datasets,
                                   //   this is the forecast valid time,
                                   //   forecast_time, of the first field.

  si64  time_expire;               // 8 Time dataset is no longer valid.
                                   //   Seconds since 1970

  si32  num_data_times;            // 9 Number of data times in 
                                   //   associated data set

  si32  index_number;              // 10 Index of data within data set
                                   //    e.g. used for radar volume number

  mutable si32  data_dimension;    // 11 Dimension of data 
                                   //    (2=flat plane, 3=volume)

  mutable si32  data_collection_type;      // 12 Data type (e.g. DATA_MEASURED)

  si64  user_data;                 // 13 Data specific to the dataset.  This
                                   //    value is not used by any of the
                                   //    general MDV code, but can be used by
                                   //    users of specific datasets.  This
                                   //    field was left here as a placeholder
                                   //    after removing an obsolete field from
                                   //    the header.

  mutable si32 native_vlevel_type; // 14 Native vertical coordinate type 
                                   //    (e.g. VERT_TYPE_SURFACE)

  mutable si32  vlevel_type;       // 15 Current vertical coordinate type 
                                   //    (e.g. VERT_TYPE_SURFACE)

  si32  vlevel_included;           // 16 Flag indicating if specific vlevel 
                                   //    info is included
                                   //    YES(1): each field has a vlevel_hdr,
                                   //    NO(0): no vlevel_hdrs are included

  si32  grid_orientation;          // 17 Ordering of cells in projected grid
                                   //    NOTE: only ORIENT_SN_WE is 
				   //    universally supported. Some software can
				   //    deal with other orientations, most notably,
				   //    ORIENT_NS_WE (Top down) orientations.
				   //
                                   //   NOTE ABOUT GRIDS: Values in a mdv grid typically
				   //   represent data at a single point in space and time.
				   //   People want to know the values between data points
				   //   so they depict this value as a colored box, extending 
				   //   the color of the point to the next "grid box".
				   //   In Mdv Data, the box should be drawn evenly around the
				   //   point in space, described by the grid_minx & y.
				   //   All MDV Grid points are "centered in the box".
				   //   Contours are only possible between grid points
				   //   and do not extend past the points in space.
				   //   Some raw data sets indicate that the
				   //   box is more appropriately rendered where the
				   //   point in space lies somewhere along the outer
				   //   perimiter of the "grid box". Several grids are being described in 
				   //   a more compact form; a reference grid and an offset value.
				   //   "Staggered Grid" is a term sometimes used for this indicator
				   //   and is supposed to mean apply a .5dx and .5dy offset to the
				   //   data for these particular values.
				   //   MDV stores this stagger as explicit and independant grid starting
				   //   positions. If a grid is "staggered", it is unstaggered when
				   //   converted to MDV. No staggering rules need be communicated to
				   //   MDV applications. MDV files containing fields which are
				   //   offset in space from one another will have different
				   //   grid_minx and grid_miny's, and even perhaps different grid_dx & y's. 
				   //   types. This leaves no ambiguity about the position
				   //   and direction of the "stagger" Data grid points are always positioned
				   //   exactly where they apply and are not offset in any way.

  si32  data_ordering;             // 18 Ordering of cells in array 
                                   //    NOTE: only ORDER_XYZ supported
  
  mutable si64  n_fields;          // 19 Number of fields in this data set

  mutable si64  max_nx;            // 20 Max number of array elements in 
                                   //    x direction

  mutable si64  max_ny;            // 21 Max number of array elements in 
                                   //    y direction
  
  mutable si64  max_nz;            // 22 Max number of array elements in 
                                   //    z direction

  mutable si64  n_chunks;          // 23 Number of chunks in this file
    
  // The following offsets are all in bytes from beginning of file

  mutable si64 field_hdr_offset;   // 24 offset to first field header
  mutable si64 vlevel_hdr_offset;  // 25 offset to first vlevel header
  mutable si64 chunk_hdr_offset;   // 26 offset to first chunk header

  mutable si32 field_grids_differ; // 27 Flag indicating if the data grids for
                                   //    all of the fields differ in any way.
                                   //    YES(1): the field grids differ
                                   //    NO(0): the field grids are all the
                                   //    same, so some processing may be
                                   //    simplified

  si64  user_data_si64[8];         // 28-35 User defined data particular to a
                                   //   dataset.  These values are not used
                                   //   by any general MDV processing but
                                   //   are guaranteed to be available to
                                   //   users for their own purposes.

  mutable si64 time_written;       // 36 time file was written to disk

  si64  epoch;                     // 37 Time epoch relative to Jan 1 1970
                                   //    Original epoch is 0
                                   //    time = time in file +
                                   //             epoch * 2**32

  mutable si64 forecast_time;      // 38 Time of forecast the data in this
                                   //    field represents.  For forecst data,
                                   //    this is the same as time_centroid
                                   //    in the master header.
                                   //    For non-forecast data, set to 0.
                                   //    Copy of value in first field.

  mutable si64 forecast_delta;     // 39 Time interval, in seconds, of
                                   //    forecast.  This is the number of
                                   //    seconds since the data midpoint
                                   //    (time_centroid in the master header)
                                   //    this forecast field represents.  This
                                   //    value should be the same as
                                   //    forecast_time - time_centroid. If this
                                   //    field is not a forecast field, this
                                   //    value should be 0.
                                   //    Copy of value in first field.

  si64  unused_si64[2];            // 40-41 To fill out record to 42 si32's
                                   //   (including record_len1)

  fl64  user_data_fl64[6];         // 1-6 User defined data particular to a
                                   //   dataset.  These values are not used
                                   //   by any general MDV processing but
                                   //   are guaranteed to be available to
                                   //   users for their own purposes.

  fl64  sensor_lon;                // 7 origin of sensor (degrees)
  fl64  sensor_lat;                // 8 origin of sensor (degrees)
  fl64  sensor_alt;                // 9 height of sensor (Km)
    
  fl64  unused_fl64[12];           // 10-21 to fill out to 21 fl64's

  char  data_set_info[MDV64_INFO_LEN]; // (512 bytes)
  char  data_set_name[MDV64_NAME_LEN]; // (128 bytes)

  char  data_set_source[MDV64_NAME_LEN]; // Where the data came from
  
  mutable si32  record_len2;           // Fortran record length (in bytes)

}  MDV64_master_header_t;

///////////////////////////////////////////////////////////////////////////
// MDV64_field_header_t
//
// Each field has its own header, 416 bytes in length.
// If you want to add items, replace a spare. Do not change the length.
//
// If a field requires additional vlevel information, then each field must 
// have an associated vlevel header as well.
//
// Supported encoding types are:
//   INT8
//   INT16
//   FLOAT32
//
// Supported compression types are:
//   COMPRESSION_ASIS - no change
//   COMPRESSION_NONE - uncompressed
//   COMPRESSION_RLE - see <toolsa/compress.h>
//   COMPRESSION_LZO - see <toolsa/compress.h>
//   COMPRESSION_GZIP - see <toolsa/compress.h>
//   COMPRESSION_BZIP - see <toolsa/compress.h>
//
// Scaling types apply only to int types (INT8 and INT16)
//
// Supported scaling types are:
//   SCALING_DYNAMIC
//   SCALING_ROUNDED
//   SCALING_INTEGRAL
//   SCALING_SPECIFIED
//
// For SCALING_DYNAMIC, the scale and bias is determined from the
// dynamic range of the data.
//
// For SCALING_ROUNDED, the operation is similar to SCALING_DYNAMIC,
// except that the scale and bias are constrained to round to 0.2, 0.5 or
// 1.0 multiplied by a power of 10.
// 
// For SCALING_INTEGRAL, the operation is similar to SCALING_DYNAMIC,
// except that the scale and bias are constrained to integral values.
// 
// For SCALING_SPECIFIED, the specified scale and bias are used.
//
// Output scale and bias are ignored for conversions to float, and
// for SCALING_DYNAMIC and SCALING_INTEGRAL.
//

typedef struct {

  // 32-bit integers

  mutable si32 record_len1;    // Fortran record length field (in bytes)
  mutable si32 struct_id;      // 1 Magic cookie ID for this struct
     
  si32 field_code;             // 2 MDV Field Code (taken from GRIB,
                               //  defined in <mdv/mdv_field_codes.h>)

  si64 user_time1;             // 3 Time value specific to the field.
                               //   This time value is not used by any
                               //   of the general MDV code, but can be
                               //   used by users of specific datasets.
                               //   This field was left here as a place-
                               //   holder after removing an obsolete
                               //   field from the header.

  si64 forecast_delta;         // 4 Time interval, in seconds, of
                               //   forecast.  This is the number of
                               //   seconds since the data midpoint
                               //   (time_centroid in the master header)
                               //   this forecast field represents.  This
                               //   value should be the same as
                               //   forecast_time - time_centroid. If this
                               //   field is not a forecast field, this
                               //   value should be 0.

  si64 user_time2;             // 5 Time value specific to the field.
                               //   This time value is not used by any
                               //   of the general MDV code, but can be
                               //   used by users of specific datasets.
                               //   This field was left here as a place-
                               //   holder after removing an obsolete
                               //   field from the header.

  si64 user_time3;             // 6 Time value specific to the field.
                               //   This time value is not used by any
                               //   of the general MDV code, but can be
                               //   used by users of specific datasets.
                               //   This field was left here as a place-
                               //   holder after removing an obsolete
                               //   field from the header.

  si64 forecast_time;          // 7 Time of forecast the data in this
                               //   field represents.  For forecst data,
                               //   this is the same as time_centroid
                               //   in the master header.
                               //   For non-forecast data, set to 0.

  si64 user_time4;             // 8 Time value specific to the field.
                               //   This time value is not used by any
                               //   of the general MDV code, but can be
                               //   used by users of specific datasets.
                               //   This field was left here as a place-
                               //   holder after removing an obsolete
                               //   field from the header.

  si64 nx;                     // 9 Number of points in X direction
  si64 ny;                     // 10 Number of points in Y direction
  si64 nz;                     // 11 Number of points in Z direction
 
  si32 proj_type;              // 12 Projection Type (e.g. PROJ_LATLON)
  
  si32 encoding_type;          // 13 Type of data encoding (e.g.
                               //   INT8, FLOAT32)

  si32 data_element_nbytes;    // 14 Size of each data element
                               //   (in bytes)

  mutable si64 field_data_offset;
                               // 15 fseek offset to start of field data
                               //   Field data may be encoded and
                               //   stored in the MDV compression format.
                               //   In this case the offset is to the 
                               //   beginning of nplane offset's and 
                               //   nplanes length's.  Each offset and 
                               //   plane element is a ui64
 
  mutable si64 volume_size;    // 16 Size of data volume in bytes.  Does
                               //   not include Fortran record length
                               //   values that surround the field data.
                               //   If the data is compressed the volume size
                               //   includes the length and offset arrays
                               //   included before the encoded data.
  
  si64 user_data_si64[10];     // 17-26 User defined data particular to a
                               //   field.  These values are not used
                               //   by any general MDV processing but
                               //   are guaranteed to be available to
                               //   users for their own purposes.
  
  mutable 
    si32 compression_type;     // 27: data compression type used
                               //   e.g. COMPRESSION_NONE,
                               //        COMPRESSION_RLE, etc

  si32 transform_type;         // 28: data transform type used
                               //   e.g. DATA_TRANSFORM_NONE,
                               //        DATA_TRANSFORM_LOG, etc
  
  si32 scaling_type;           // 29: data scaling method used
                               //   e.g. SCALING_DYNAMIC,
                               //        SCALING_INTEGRAL, etc
                               //   Only applicable for integral
                               //   encoding types
  
  si32 native_vlevel_type;     // 30 Native vertical coordinate type 
                               //   (e.g. VERT_TYPE_SURFACE)

  si32 vlevel_type;            // 31 Current vertical coordinate type 
                               //   (e.g. VERT_TYPE_SURFACE)

  si32 dz_constant;            // 32 - set true (1) if dz is constant in
                               //   vlevels, false otherwise

  si32 data_dimension;         // 33 Dimension of data 
                               //    (2=flat plane, 3=volume)

  si32 zoom_clipped;           // 34: 0 if there is no clipping between file
                               //       data and the requested zoom
                               //     1 if there is clipping

  si32 zoom_no_overlap;        // 35: 0 if there is some overlap between the
                               //       file data and requested zoom
                               //     1 if no overlap

  si64 unused_si64[4];         // 36-39 Spare, set to 0

  // 64-bit floats
    
  fl64 proj_origin_lat;        // 1 Projection origin 
                               //   (deg, south lat is negative)

  fl64 proj_origin_lon;        // 2 Projection origin 
                               //   (deg, west lon is negative)

  fl64 proj_param[MDV64_MAX_PROJ_PARAMS];
                           // 3-10 Projection information, these
                           //   values are specific to the
                           //   proj_type specified above.
                           //   Note: proj_rotation follows later

			   // PROJ_FLAT:  proj_param[0] = rotation
			   // PROJ_LATLON: not used
			   // PROJ_LAMBERT_CONF: proj_param[0] = lat1
			   //                    proj_param[1] = lat2
			   // PROJ_LATLON: not used
			   // PROJ_POLAR_STEREO: proj_param[0] = tangent_lon
			   //                    proj_param[1] = pole_type
                           //                                    0 = north
                           //                                    1 = south
			   //                    proj_param[2] = central_scale
			   // PROJ_OBLIQUE_STEREO:proj_param[0] = tangent_lat
			   //                     proj_param[1] = tangent_lon
			   //                     proj_param[2] = central_scale
			   // PROJ_MERCATOR: not used
			   // PROJ_TRANS_MERCATOR: proj_params[0] = central_scale
			   // PROJ_ALBERS: proj_param[0] = lat1
			   //              proj_param[1] = lat2
			   // PROJ_LAMBERT_AZIM: not used
                           // PROJ_VERT_PRESP: proj_param[0] = persp_radius in km

                           // proj_param[6] = false_northing
                           // proj_param[7] = false_easting
  
  fl64 vert_reference;         // 11 Vertical coordinate reference value

  fl64 grid_dx;                // 12 X dimension grid spacing 
                               //    units determined by proj_type

  fl64 grid_dy;                // 13 Y dimension grid spacing 
                               //    units determined by proj_type

  fl64 grid_dz;                // 14 Z dimension grid spacing 
                               //    units determined by vlevel_type
                               // If dz is 0.0, then you must use the
                               // vlevel header.

  fl64 grid_minx;              // 15 Starting point of grid 
                               //    units determined by proj_type

  fl64 grid_miny;              // 16 Starting point of grid 
                               //    units determined by proj_type

  fl64 grid_minz;              // 17 Starting point of grid 
                               //    units determined by vlevel_type

  fl64 scale;                  // 18 Scale factor for data values
                               //    Applies to INT8 and INT16 only 
   
  fl64 bias;                   // 19 Bias applied to data values  
                               //    Applies to INT8 and INT16 only

                               // NOTE: To get float data from INT8 and INT16,
                               // float_value = scale * integer_value + bias

  fl32 bad_data_value;         // 20 Data with this value (BEFORE applying
                               //    scale and bias) not valid

  fl32 missing_data_value;     // 21 Data with this value (BEFORE applying
                               //    scale and bias) not measured

  fl64 proj_rotation;          // 22 Projection rotation in degrees
                               // Applies to PROJ_FLAT only

  fl64 user_data_fl64[4];      // 23-26 User defined data particular to a
                               //   field.  These values are not used
                               //   by any general MDV processing but
                               //   are guaranteed to be available to
                               //   users for their own purposes.

  fl32 min_value;              // 27 - min val in data set
  fl32 max_value;              // 28 - max val in data set
  fl32 min_value_orig_vol;     // 29 - min val in original vol before 
                               //      clipping to get data set
  fl32 max_value_orig_vol;     // 30 - max val in original vol before 
                               //      clipping to get data set
  fl64 unused_fl64;            // 31   Spare, to fill out array 
                               //      to 31 fl64's

  // chars

  char field_name_long[MDV64_LONG_FIELD_LEN]; // Long field name (64 bytes)

  char field_name[MDV64_SHORT_FIELD_LEN];     // Short field name (16 bytes)

  char units[MDV64_UNITS_LEN];                // Units label (16 bytes)

  char transform[MDV64_TRANSFORM_LEN];        // Data transformation type 
                                            // (16 bytes)

  char unused_char[MDV64_UNITS_LEN];          // Spare 16 bytes

  mutable si32 record_len2;                 // Fortran record length field

} MDV64_field_header_t;

///////////////////////////////////////////////////////////////////////
// MDV64_vlevel_header
//
// A vlevel_header exists when more information about vertical levels 
// are needed.  If it is used, the master header's vlevel_included 
// flag must be set to 1 and a vlevel_header must be included for every
// field in the file. This structure is 1 kbyte and all vlevel 
// headers should follow the field headers.
//

typedef struct {
  
  mutable si32 record_len1;     // Fortran record length field 
  mutable si32 struct_id;       // 1 Magic cookie ID for this struct
  si32 type[MDV64_MAX_VLEVELS];   // 2-123 (was vlevel_type)
  si32 unused_si32[4];          // 124-127
  fl32 level[MDV64_MAX_VLEVELS];  // 1-122 (was vlevel_params)
  fl64 unused_fl64[5];          // 123-127
  mutable si32 record_len2;     // Fortran record length field

} MDV64_vlevel_header_t;

//////////////////////////////////////////////////////////////////////////////
// MDV64_chunk_header
//
// A chunk header provides information about a "chunk" of data.  Chunk
// data is not designed to conform to any standard so the writer of
// "chunk" data is responsible for knowing how it was written, and
// therefore, how it should be read. Note that there is space for
// ascii text about chunk data.
//
// A chunk header is 512 bytes in length.
//
// The MDV library has access to routines which can interpret certain
// chunk data, identified through the chunk ID.
//

typedef struct {

  mutable si32 record_len1;           // Fortran record length field 
                                      //   (in bytes)

  mutable si32 struct_id;             // 1 Magic cookie ID for this struct
  
  si32 chunk_id;                      // 2 A value identifying the data
                                      //   in this chunk.  Chunk data that
                                      //   we know about are defined in
                                      //   <mdv/mdv_macros.h as
                                      //   MDV64_CHUNK_xxx
  
  mutable si64 chunk_data_offset;     // 3 fseek offset to start of 
                                      //   chunk data 

  si64 size;                          // 4 Chunk size (in bytes)
  
  si64 unused_si64[2];                // 5-6 Unused ints
  
  char info[MDV64_CHUNK_INFO_LEN];      // ascii info about chunk data

  mutable si32 record_len2;           // Fortran record length field

} MDV64_chunk_header_t;

// structures used in Mdvx chunks and DsMdvMsg for storing vertical section
// information 

typedef struct {
  si64 npts;
  si64 spare[3];
} MDV64_chunkVsectWayPtHdr_t;

typedef struct {
  fl64 lat;
  fl64 lon;
} MDV64_chunkVsectWayPt_t;

typedef struct {
  si64 npts;
  fl64 dx_km;
  si64 spare[2];
} MDV64_chunkVsectSamplePtHdr_t;

typedef struct {
  fl64 lat;
  fl64 lon;
  si64 segNum;
  si64 spare;
} MDV64_chunkVsectSamplePt_t;

typedef struct {
  si64 nsegments;
  fl64 total_length;
  si64 spare[2];
} MDV64_chunkVsectSegmentHdr_t;

typedef struct {
  fl64 length;
  fl64 azimuth;
} MDV64_chunkVsectSegment_t;

//////////////////////////////////////////////////////////////////
// projection and coordinates struct
//
// This is an alternative representation of the coord information
// in the field headers.
//
// This struct is not stored in the Mdv files directly.
// It is used to store mdv grid information in related data files,
// which make use of Mdv data but for which it does not make
// sense to store the entire field header.
//
// This struct is also used to pass projection information
// between the DsMdvServer and clients. Its size should be kept
// constant for backwards compatibility.

// flat (radar) projection
  
typedef struct {
  
  fl64 rotation;
  fl64 spare[7];
  
} MDV64_flat_params_t;

// albers conic equal area
  
typedef struct {
  
  fl64 lat1;
  fl64 lat2;
  fl64 spare[6];
  
} MDV64_albers_params_t;

// lambert conformal
  
typedef struct {
  
  fl64 lat1;
  fl64 lat2;
  fl64 spare[6];
  
} MDV64_lc2_params_t;

// oblique stereographic
  
typedef struct {
  
  fl64 tan_lat;
  fl64 tan_lon;
  fl64 central_scale;
  fl64 spare[5];
  
} MDV64_os_params_t;

// polar stereographic
  
typedef struct {
  
  fl64 tan_lon;
  fl64 central_scale;
  si32 pole_type; // 0 - POLE_NORTH, 1 - POLE_SOUTH
  fl64 spare[5];
  
} MDV64_ps_params_t;
  
// transverse mercator
  
typedef struct {
  
  fl64 central_scale;
  fl64 spare[7];
  
} MDV64_trans_merc_params_t;

typedef struct {
  
  fl64 persp_radius; // perspective point radius (km)
  fl64 spare[7];
  
} MDV64_vert_persp_params_t;

// coordinate structure
    
typedef struct {

  // posn of origin

  fl64 proj_origin_lat;
  fl64 proj_origin_lon;
    
  union {
    flat_params_t flat;
    lc2_params_t lc2;
    os_params_t os;
    ps_params_t ps;
    trans_merc_params_t tmerc;
    albers_params_t albers;
    vert_persp_params_t vp;
  } proj_params;

  // false easting and northing
  // false_easting is added to X to keep it positive

  fl64 false_easting;
  fl64 false_northing;

  // start value, SW corner,
  // bottom plane (* scale)
  // minz set to -1 if dz_constant is FALSE

  fl64 minx, miny, minz;
    
  // cartesian spacing in each dirn (* scale)
  // dz set to -1 if dz_constant is FALSE

  fl64 dx, dy, dz;

  // sensor coords    

  fl64 sensor_x;
  fl64 sensor_y;
  fl64 sensor_z;
  fl64 sensor_lat;
  fl64 sensor_lon;
    
  si64 spare[11];
    
  // type of projection used for grid
  // MDV_PROJ_FLAT, MDV_PROJ_LATLON etc.

  si32 proj_type;
    
  // flag to indicate regularly spaced planes.
  // Set to 1 if regularly-spaced planes
  // (constant dz), 0 otherwise. 

  si32 dz_constant;
    
  // number of points in each dirn    

  si64 nx, ny, nz;
    
  // number of bytes of character data
  // at end of this struct

  si64 nbytes_char;
  
  char unitsx[MDV64_COORD_UNITS_LEN]; // units in x dirn
  char unitsy[MDV64_COORD_UNITS_LEN]; // units in y dirn
  char unitsz[MDV64_COORD_UNITS_LEN]; // units in z dirn
    
} MDV64_coord_t;
  
#endif