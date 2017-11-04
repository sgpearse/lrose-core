// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
// ** Copyright UCAR (c) 1992 - 2017
// ** University Corporation for Atmospheric Research(UCAR)
// ** National Center for Atmospheric Research(NCAR)
// ** Boulder, Colorado, USA
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
////////////////////////////////////////////
// Params.hh
//
// TDRP header file for 'Params' class.
//
// Code for program Iq2Dsr
//
// This header file has been automatically
// generated by TDRP, do not modify.
//
/////////////////////////////////////////////

/**
 *
 * @file Params.hh
 *
 * This class is automatically generated by the Table
 * Driven Runtime Parameters (TDRP) system
 *
 * @class Params
 *
 * @author automatically generated
 *
 */

#ifndef Params_hh
#define Params_hh

using namespace std;

#include <tdrp/tdrp.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cfloat>

// Class definition

class Params {

public:

  // enum typedefs

  typedef enum {
    DEBUG_OFF = 0,
    DEBUG_NORM = 1,
    DEBUG_VERBOSE = 2,
    DEBUG_EXTRA_VERBOSE = 3
  } debug_t;

  typedef enum {
    ARCHIVE = 0,
    REALTIME = 1,
    SIMULATE = 2,
    FMQ = 3
  } mode_t;

  typedef enum {
    SCAN_MODE_UNKNOWN = 0,
    SCAN_MODE_SECTOR = 1,
    SCAN_MODE_COPLANE = 2,
    SCAN_MODE_RHI = 3,
    SCAN_MODE_VERTICAL_POINTING = 4,
    SCAN_MODE_IDLE = 7,
    SCAN_MODE_SURVEILLANCE = 8,
    SCAN_MODE_SUNSCAN = 11,
    SCAN_MODE_POINTING = 12,
    SCAN_MODE_MANUAL_PPI = 15,
    SCAN_MODE_MANUAL_RHI = 16
  } scan_mode_t;

  typedef enum {
    SINGLE_POL = 0,
    DP_ALT_HV_CO_ONLY = 1,
    DP_ALT_HV_CO_CROSS = 2,
    DP_ALT_HV_FIXED_HV = 3,
    DP_SIM_HV_FIXED_HV = 4,
    DP_SIM_HV_SWITCHED_HV = 5,
    DP_H_ONLY_FIXED_HV = 6,
    DP_V_ONLY_FIXED_HV = 7
  } xmit_rcv_mode_t;

  typedef enum {
    WINDOW_RECT = 0,
    WINDOW_VONHANN = 1,
    WINDOW_BLACKMAN = 2,
    WINDOW_BLACKMAN_NUTTALL = 3
  } window_t;

  typedef enum {
    PROC_FLAGS_NONE = 0,
    PROC_FLAGS_DUAL_PRT = 1
  } processing_flags_t;

  typedef enum {
    BEAM_SPECIFY_N_SAMPLES = 0,
    BEAM_SPECIFY_ANGLE = 1
  } beam_method_t;

  typedef enum {
    WIDTH_METHOD_R0R1 = 0,
    WIDTH_METHOD_R1R2 = 1,
    WIDTH_METHOD_HYBRID = 2
  } spectrum_width_method_t;

  typedef enum {
    RX_CHANNEL_HC = 0,
    RX_CHANNEL_VC = 1,
    RX_CHANNEL_HX = 2,
    RX_CHANNEL_VX = 3
  } rx_channel_t;

  typedef enum {
    ATMOS_ATTEN_NONE = 0,
    ATMOS_ATTEN_CONSTANT = 1,
    ATMOS_ATTEN_CRPL = 2
  } atmos_atten_method_t;

  typedef enum {
    FIR_LEN_125 = 0,
    FIR_LEN_60 = 1,
    FIR_LEN_40 = 2,
    FIR_LEN_30 = 3,
    FIR_LEN_20 = 4,
    FIR_LEN_10 = 5
  } fir_filter_len_t;

  typedef enum {
    NOISE_RAY_BY_RAY = 0,
    NOISE_RUNNING_MEDIAN = 1
  } noise_method_t;

  typedef enum {
    CENSORING_NONE = 0,
    CENSORING_BY_NOISE_FLAG = 1,
    CENSORING_BY_SNR_AND_NCP = 2
  } censoring_mode_t;

  typedef enum {
    PHASE_DECODE_NONE = 0,
    PHASE_DECODE_RANDOM = 1,
    PHASE_DECODE_SZ = 2,
    PHASE_DECODE_SZ1 = 3
  } phase_decoding_t;

  typedef enum {
    DBZ = 0,
    DBZ_NO_ATMOS_ATTEN = 1,
    DBZHC = 2,
    DBZVC = 3,
    DBZHX = 4,
    DBZVX = 5,
    VEL = 6,
    VEL_ALT = 7,
    VEL_HV = 8,
    VEL_H_ONLY = 9,
    VEL_V_ONLY = 10,
    VEL_ALT_FOLD_INTERVAL = 11,
    VEL_ALT_FOLD_CONFIDENCE = 12,
    VEL_CORRECTED = 13,
    VEL_PRT_SHORT = 14,
    VEL_PRT_LONG = 15,
    VEL_DIFF = 16,
    VEL_UNFOLD_INTERVAL = 17,
    WIDTH = 18,
    WIDTH_R0R1 = 19,
    WIDTH_R1R2 = 20,
    WIDTH_R1R3 = 21,
    WIDTH_PPLS = 22,
    WIDTH_H_ONLY = 23,
    WIDTH_V_ONLY = 24,
    WIDTH_PRT_LONG = 25,
    WIDTH_PRT_SHORT = 26,
    NCP = 27,
    NCP_H_ONLY = 28,
    NCP_V_ONLY = 29,
    NCP_H_MINUS_V = 30,
    NCP_TRIP1 = 31,
    NCP_TRIP2 = 32,
    NCP_TRIP3 = 33,
    NCP_TRIP4 = 34,
    NCP_PRT_LONG = 35,
    NCP_PRT_SHORT = 36,
    NCP_TRIP_FLAG = 37,
    NOISE_BIAS_DB_HC = 38,
    NOISE_BIAS_DB_HX = 39,
    NOISE_BIAS_DB_VC = 40,
    NOISE_BIAS_DB_VX = 41,
    SNR = 42,
    SNRHC = 43,
    SNRHX = 44,
    SNRVC = 45,
    SNRVX = 46,
    DBM = 47,
    DBMHC = 48,
    DBMHX = 49,
    DBMVC = 50,
    DBMVX = 51,
    DBMHC_NS = 52,
    DBMHX_NS = 53,
    DBMVC_NS = 54,
    DBMVX_NS = 55,
    ZDRM = 56,
    ZDR = 57,
    ZDR_BIAS = 58,
    LDR = 59,
    LDRHM = 60,
    LDRH = 61,
    LDRVM = 62,
    LDRV = 63,
    LDR_DIFF = 64,
    LDR_MEAN = 65,
    RHOHV = 66,
    RHOHV_NNC = 67,
    PHIDP0 = 68,
    PHIDP = 69,
    PHIDP_COND = 70,
    PHIDP_FILT = 71,
    PHIDP_SDEV_4KDP = 72,
    PHIDP_JITTER_4KDP = 73,
    ZDR_SDEV_4KDP = 74,
    KDP = 75,
    PSOB = 76,
    KDP_HB = 77,
    RHO_HC_VX = 78,
    RHO_VC_HX = 79,
    RHO_VX_HX = 80,
    RHO_PHIDP = 81,
    CPR_MAG = 82,
    CPR_PHASE = 83,
    CPR_LDR = 84,
    DBZ_ATTEN_CORRECTION = 85,
    ZDR_ATTEN_CORRECTION = 86,
    DBZ_ATTEN_CORRECTED = 87,
    ZDR_ATTEN_CORRECTED = 88,
    CPA = 89,
    TDBZ = 90,
    SPIN = 91,
    MAX_TDBZ_SPIN = 92,
    ZDR_SDEV = 93,
    PHIDP_SDEV = 94,
    DBZ_DIFF_SQ = 95,
    DBZ_SPIN_CHANGE = 96,
    CMD = 97,
    CMD_FLAG = 98,
    TDBZ_INTEREST = 99,
    SPIN_INTEREST = 100,
    CPA_INTEREST = 101,
    ZDR_SDEV_INTEREST = 102,
    PHIDP_SDEV_INTEREST = 103,
    CLUT = 104,
    CLUT_2_WX_RATIO = 105,
    SPECTRAL_NOISE = 106,
    SPECTRAL_SNR = 107,
    AIQ_HC = 108,
    NIQ_HC = 109,
    AIQ_VC = 110,
    NIQ_VC = 111,
    SZ_TRIP_FLAG = 112,
    SZ_LEAKAGE = 113,
    CENSORING_FLAG = 114,
    LAG0_HC_DB = 115,
    LAG0_HX_DB = 116,
    LAG0_VC_DB = 117,
    LAG0_VX_DB = 118,
    LAG0_HC_SHORT_DB = 119,
    LAG0_VC_SHORT_DB = 120,
    LAG0_HC_LONG_DB = 121,
    LAG0_VC_LONG_DB = 122,
    LAG0_VCHX_DB = 123,
    LAG0_VCHX_PHASE = 124,
    LAG0_HCVX_DB = 125,
    LAG0_HCVX_PHASE = 126,
    LAG1_HC_DB = 127,
    LAG1_HC_PHASE = 128,
    LAG1_VC_DB = 129,
    LAG1_VC_PHASE = 130,
    LAG1_HCVC_DB = 131,
    LAG1_HCVC_PHASE = 132,
    LAG1_VCHC_DB = 133,
    LAG1_VCHC_PHASE = 134,
    LAG1_VXHX_DB = 135,
    LAG1_VXHX_PHASE = 136,
    LAG1_HC_LONG_DB = 137,
    LAG1_HC_LONG_PHASE = 138,
    LAG1_VC_LONG_DB = 139,
    LAG1_VC_LONG_PHASE = 140,
    LAG1_HC_SHORT_DB = 141,
    LAG1_HC_SHORT_PHASE = 142,
    LAG1_VC_SHORT_DB = 143,
    LAG1_VC_SHORT_PHASE = 144,
    LAG1_HC_LONG_TO_SHORT_DB = 145,
    LAG1_HC_LONG_TO_SHORT_PHASE = 146,
    LAG1_VC_LONG_TO_SHORT_DB = 147,
    LAG1_VC_LONG_TO_SHORT_PHASE = 148,
    LAG1_HC_SHORT_TO_LONG_DB = 149,
    LAG1_HC_SHORT_TO_LONG_PHASE = 150,
    LAG1_VC_SHORT_TO_LONG_DB = 151,
    LAG1_VC_SHORT_TO_LONG_PHASE = 152,
    LAG2_HC_DB = 153,
    LAG2_HC_PHASE = 154,
    LAG2_VC_DB = 155,
    LAG2_VC_PHASE = 156,
    LAG3_HC_DB = 157,
    LAG3_HC_PHASE = 158,
    LAG3_VC_DB = 159,
    LAG3_VC_PHASE = 160,
    RVVHH0_DB = 161,
    RVVHH0_PHASE = 162,
    RVVHH0_LONG_DB = 163,
    RVVHH0_LONG_PHASE = 164,
    RVVHH0_SHORT_DB = 165,
    RVVHH0_SHORT_PHASE = 166,
    SDEV_VV = 167,
    PRT = 168,
    NUM_PULSES = 169,
    TEST = 170,
    TEST2 = 171,
    TEST3 = 172,
    TEST4 = 173,
    TEST5 = 174
  } field_id_t;

  typedef enum {
    TRANSITION_FLAG_AT_CENTER = 0,
    TRANSITION_FLAG_AT_BOTH_ENDS = 1,
    TRANSITION_FLAG_AT_EITHER_END = 2,
    TRANSITION_FLAG_MISSING = 3
  } transition_method_t;

  // struct typedefs

  typedef struct {
    tdrp_bool_t check_scan_mode;
    scan_mode_t scan_mode;
    tdrp_bool_t check_prf;
    double prf_lower_limit;
    double prf_upper_limit;
    tdrp_bool_t check_antenna_rate;
    double antenna_rate_lower_limit;
    double antenna_rate_upper_limit;
    beam_method_t beam_method;
    int beam_n_samples;
    double beam_angle_deg;
    tdrp_bool_t index_the_beams;
    double indexed_resolution;
    double min_antenna_rate_for_indexing;
    window_t window;
    tdrp_bool_t switching_receiver;
    xmit_rcv_mode_t xmit_rcv_mode;
    tdrp_bool_t apply_clutter_filter;
    tdrp_bool_t apply_sz;
    tdrp_bool_t change_velocity_sign;
    processing_flags_t proc_flags;
    tdrp_bool_t check_scan_name;
    char* scan_name;
  } moments_params_t;

  typedef struct {
    double pulse_width_us;
    char* cal_dir;
    tdrp_bool_t check_xmit_rcv_mode;
    xmit_rcv_mode_t xmit_rcv_mode;
    double zdr_correction_db;
    double system_phidp_deg;
  } pulse_width_cal_t;

  typedef struct {
    rx_channel_t rx_channel;
    double slope;
    double intercept;
  } rx_gain_correction_t;

  typedef struct {
    double value;
    double interest;
  } interest_map_point_t;

  typedef struct {
    int length;
    double min_valid_cmd;
  } speckle_threshold_t;

  typedef struct {
    field_id_t id;
    char* name;
    char* units;
    double scale;
    double bias;
    tdrp_bool_t write_unfiltered;
    tdrp_bool_t write_filtered;
  } output_field_t;

  typedef struct {
    double min_el;
    double max_el;
    double min_az;
    double max_az;
    double min_range;
    double max_range;
  } selected_region_t;

  ///////////////////////////
  // Member functions
  //

  ////////////////////////////////////////////
  // Default constructor
  //

  Params ();

  ////////////////////////////////////////////
  // Copy constructor
  //

  Params (const Params&);

  ////////////////////////////////////////////
  // Destructor
  //

  ~Params ();

  ////////////////////////////////////////////
  // Assignment
  //

  void operator=(const Params&);

  ////////////////////////////////////////////
  // loadFromArgs()
  //
  // Loads up TDRP using the command line args.
  //
  // Check usage() for command line actions associated with
  // this function.
  //
  //   argc, argv: command line args
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   char **params_path_p:
  //     If this is non-NULL, it is set to point to the path
  //     of the params file used.
  //
  //   bool defer_exit: normally, if the command args contain a 
  //      print or check request, this function will call exit().
  //      If defer_exit is set, such an exit is deferred and the
  //      private member _exitDeferred is set.
  //      Use exidDeferred() to test this flag.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int loadFromArgs(int argc, char **argv,
                   char **override_list,
                   char **params_path_p,
                   bool defer_exit = false);

  bool exitDeferred() { return (_exitDeferred); }

  ////////////////////////////////////////////
  // loadApplyArgs()
  //
  // Loads up TDRP using the params path passed in, and applies
  // the command line args for printing and checking.
  //
  // Check usage() for command line actions associated with
  // this function.
  //
  //   const char *param_file_path: the parameter file to be read in
  //
  //   argc, argv: command line args
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   bool defer_exit: normally, if the command args contain a 
  //      print or check request, this function will call exit().
  //      If defer_exit is set, such an exit is deferred and the
  //      private member _exitDeferred is set.
  //      Use exidDeferred() to test this flag.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int loadApplyArgs(const char *params_path,
                    int argc, char **argv,
                    char **override_list,
                    bool defer_exit = false);

  ////////////////////////////////////////////
  // isArgValid()
  // 
  // Check if a command line arg is a valid TDRP arg.
  //

  static bool isArgValid(const char *arg);

  ////////////////////////////////////////////
  // load()
  //
  // Loads up TDRP for a given class.
  //
  // This version of load gives the programmer the option to load
  // up more than one class for a single application. It is a
  // lower-level routine than loadFromArgs, and hence more
  // flexible, but the programmer must do more work.
  //
  //   const char *param_file_path: the parameter file to be read in.
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   expand_env: flag to control environment variable
  //               expansion during tokenization.
  //               If TRUE, environment expansion is set on.
  //               If FALSE, environment expansion is set off.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int load(const char *param_file_path,
           char **override_list,
           int expand_env, int debug);

  ////////////////////////////////////////////
  // loadFromBuf()
  //
  // Loads up TDRP for a given class.
  //
  // This version of load gives the programmer the option to
  // load up more than one module for a single application,
  // using buffers which have been read from a specified source.
  //
  //   const char *param_source_str: a string which describes the
  //     source of the parameter information. It is used for
  //     error reporting only.
  //
  //   char **override_list: A null-terminated list of overrides
  //     to the parameter file.
  //     An override string has exactly the format of an entry
  //     in the parameter file itself.
  //
  //   const char *inbuf: the input buffer
  //
  //   int inlen: length of the input buffer
  //
  //   int start_line_num: the line number in the source which
  //     corresponds to the start of the buffer.
  //
  //   expand_env: flag to control environment variable
  //               expansion during tokenization.
  //               If TRUE, environment expansion is set on.
  //               If FALSE, environment expansion is set off.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int loadFromBuf(const char *param_source_str,
                  char **override_list,
                  const char *inbuf, int inlen,
                  int start_line_num,
                  int expand_env, int debug);

  ////////////////////////////////////////////
  // loadDefaults()
  //
  // Loads up default params for a given class.
  //
  // See load() for more detailed info.
  //
  //  Returns 0 on success, -1 on failure.
  //

  int loadDefaults(int expand_env);

  ////////////////////////////////////////////
  // sync()
  //
  // Syncs the user struct data back into the parameter table,
  // in preparation for printing.
  //
  // This function alters the table in a consistent manner.
  // Therefore it can be regarded as const.
  //

  void sync() const;

  ////////////////////////////////////////////
  // print()
  // 
  // Print params file
  //
  // The modes supported are:
  //
  //   PRINT_SHORT:   main comments only, no help or descriptions
  //                  structs and arrays on a single line
  //   PRINT_NORM:    short + descriptions and help
  //   PRINT_LONG:    norm  + arrays and structs expanded
  //   PRINT_VERBOSE: long  + private params included
  //

  void print(FILE *out, tdrp_print_mode_t mode = PRINT_NORM);

  ////////////////////////////////////////////
  // checkAllSet()
  //
  // Return TRUE if all set, FALSE if not.
  //
  // If out is non-NULL, prints out warning messages for those
  // parameters which are not set.
  //

  int checkAllSet(FILE *out);

  //////////////////////////////////////////////////////////////
  // checkIsSet()
  //
  // Return TRUE if parameter is set, FALSE if not.
  //
  //

  int checkIsSet(const char *param_name);

  ////////////////////////////////////////////
  // arrayRealloc()
  //
  // Realloc 1D array.
  //
  // If size is increased, the values from the last array 
  // entry is copied into the new space.
  //
  // Returns 0 on success, -1 on error.
  //

  int arrayRealloc(const char *param_name,
                   int new_array_n);

  ////////////////////////////////////////////
  // array2DRealloc()
  //
  // Realloc 2D array.
  //
  // If size is increased, the values from the last array 
  // entry is copied into the new space.
  //
  // Returns 0 on success, -1 on error.
  //

  int array2DRealloc(const char *param_name,
                     int new_array_n1,
                     int new_array_n2);

  ////////////////////////////////////////////
  // freeAll()
  //
  // Frees up all TDRP dynamic memory.
  //

  void freeAll(void);

  ////////////////////////////////////////////
  // usage()
  //
  // Prints out usage message for TDRP args as passed
  // in to loadFromArgs().
  //

  static void usage(ostream &out);

  ///////////////////////////
  // Data Members
  //

  char _start_; // start of data region
                // needed for zeroing out data
                // and computing offsets

  debug_t debug;

  int beam_count_for_debug_print;

  char* instance;

  tdrp_bool_t use_multiple_threads;

  int n_compute_threads;

  mode_t mode;

  char* input_fmq;

  tdrp_bool_t position_fmq_at_start;

  char* input_dir;

  tdrp_bool_t use_ldata_info_file;

  int max_realtime_valid_age;

  tdrp_bool_t invert_hv_flag;

  tdrp_bool_t swap_receiver_channels;

  tdrp_bool_t cohere_iq_to_burst_phase;

  tdrp_bool_t prt_is_for_previous_interval;

  tdrp_bool_t compute_prt_from_interpulse_periods;

  tdrp_bool_t change_velocity_sign_staggered;

  tdrp_bool_t use_pulse_width_from_ts_proc;

  tdrp_bool_t override_primary_prt;

  double primary_prt_secs;

  double georef_time_margin_secs;

  tdrp_bool_t check_radar_id;

  int radar_id;

  double nsecs_for_antenna_rate;

  tdrp_bool_t interpolate_antenna_angles;

  double angle_interp_max_change;

  int angle_interp_max_queue_size;

  tdrp_bool_t angle_interp_adjust_for_latency;

  double angle_interp_latency;

  tdrp_bool_t angle_interp_debug;

  tdrp_bool_t override_radar_name;

  char* radar_name;

  tdrp_bool_t override_radar_location;

  double radar_latitude_deg;

  double radar_longitude_deg;

  double radar_altitude_meters;

  tdrp_bool_t override_gate_geometry;

  double gate_spacing_meters;

  double start_range_meters;

  tdrp_bool_t override_radar_wavelength;

  double radar_wavelength_cm;

  tdrp_bool_t apply_azimuth_offset;

  double azimuth_offset;

  tdrp_bool_t apply_elevation_offset;

  double elevation_offset;

  moments_params_t *_moments_params;
  int moments_params_n;

  int min_n_samples;

  int max_n_samples;

  tdrp_bool_t control_n_samples_from_time_series;

  tdrp_bool_t control_xmit_rcv_mode_from_time_series;

  tdrp_bool_t control_beam_indexing_from_time_series;

  tdrp_bool_t discard_non_indexed_beams;

  tdrp_bool_t adjust_dbz_for_measured_xmit_power;

  tdrp_bool_t compute_zdr_using_snr;

  tdrp_bool_t adjust_zdr_for_measured_xmit_power;

  tdrp_bool_t check_for_missing_pulses;

  tdrp_bool_t discard_beams_with_missing_pulses;

  tdrp_bool_t correct_for_system_phidp;

  int zdr_median_filter_len;

  int rhohv_median_filter_len;

  int staggered_prt_median_filter_len;

  tdrp_bool_t compute_velocity_corrected_for_platform_motion;

  spectrum_width_method_t spectrum_width_method;

  char* startup_cal_file;

  tdrp_bool_t set_cal_by_pulse_width;

  pulse_width_cal_t *_pulse_width_cals;
  int pulse_width_cals_n;

  int cal_recheck_period;

  tdrp_bool_t use_cal_from_time_series;

  tdrp_bool_t override_cal_dbz_correction;

  double dbz_correction;

  tdrp_bool_t override_cal_zdr_correction;

  double zdr_correction_db;

  tdrp_bool_t override_cal_ldr_corrections;

  double ldr_correction_db_h;

  double ldr_correction_db_v;

  tdrp_bool_t override_cal_system_phidp;

  double system_phidp_deg;

  tdrp_bool_t change_phidp_sign;

  tdrp_bool_t correct_receiver_gains_for_temperature;

  rx_gain_correction_t *_rx_gain_correction;
  int rx_gain_correction_n;

  char* temperature_tags_in_status_xml;

  tdrp_bool_t apply_precip_attenuation_correction;

  tdrp_bool_t specify_coefficients_for_attenuation_correction;

  double dbz_attenuation_coefficient;

  double dbz_attenuation_exponent;

  double zdr_attenuation_coefficient;

  double zdr_attenuation_exponent;

  atmos_atten_method_t atmos_atten_method;

  double atmos_atten_db_per_km;

  tdrp_bool_t apply_residue_correction_in_adaptive_filter;

  double min_snr_db_for_residue_correction;

  tdrp_bool_t use_polynomial_regression_clutter_filter;

  int regression_filter_polynomial_order;

  tdrp_bool_t regression_filter_interp_across_notch;

  tdrp_bool_t use_simple_notch_clutter_filter;

  double simple_notch_filter_width_mps;

  tdrp_bool_t use_h_only_for_alt_mode_clutter_vel;

  fir_filter_len_t KDP_fir_filter_len;

  int KDP_n_filt_iterations_unfolded;

  int KDP_n_filt_iterations_conditioned;

  tdrp_bool_t KDP_use_iterative_filtering;

  double KDP_phidp_difference_threshold;

  int KDP_ngates_for_stats;

  double KDP_phidp_sdev_max;

  double KDP_phidp_jitter_max;

  tdrp_bool_t KDP_check_snr;

  double KDP_snr_threshold;

  tdrp_bool_t KDP_check_rhohv;

  double KDP_rhohv_threshold;

  tdrp_bool_t KDP_check_zdr_sdev;

  double KDP_zdr_sdev_max;

  double KDP_min_valid_abs_kdp;

  tdrp_bool_t KDP_debug;

  tdrp_bool_t KDP_write_ray_files;

  char* KDP_ray_files_dir;

  tdrp_bool_t KDP_compute_using_hubbert_bringi_method;

  fir_filter_len_t KDP_HB_fir_filter_len;

  double KDP_HB_phidp_difference_threshold;

  double KDP_HB_phidp_sdev_threshold;

  double KDP_HB_zdr_sdev_threshold;

  double KDP_HB_rhohv_threshold;

  tdrp_bool_t use_estimated_noise_for_noise_subtraction;

  int noise_ngates_kernel;

  int noise_min_ngates_for_ray_median;

  noise_method_t noise_method;

  int noise_ngates_for_running_median;

  tdrp_bool_t set_equal_noise_bias_in_all_channels;

  interest_map_point_t *_phase_change_error_for_noise_interest_map;
  int phase_change_error_for_noise_interest_map_n;

  double phase_change_error_for_noise_interest_weight;

  interest_map_point_t *_dbm_sdev_for_noise_interest_map;
  int dbm_sdev_for_noise_interest_map_n;

  double dbm_sdev_for_noise_interest_weight;

  interest_map_point_t *_ncp_mean_for_noise_interest_map;
  int ncp_mean_for_noise_interest_map_n;

  double ncp_mean_for_noise_interest_weight;

  double interest_threshold_for_noise;

  interest_map_point_t *_phase_change_error_for_signal_interest_map;
  int phase_change_error_for_signal_interest_map_n;

  double phase_change_error_for_signal_interest_weight;

  interest_map_point_t *_dbm_sdev_for_signal_interest_map;
  int dbm_sdev_for_signal_interest_map_n;

  double dbm_sdev_for_signal_interest_weight;

  double interest_threshold_for_signal;

  censoring_mode_t censoring_mode;

  double censoring_snr_threshold;

  double censoring_ncp_threshold;

  tdrp_bool_t change_aiq_sign;

  phase_decoding_t phase_decoding;

  int phase_decoding_ntrips_check;

  int phase_decoding_ntrips_save;

  double phase_decoding_ncp_threshold;

  double phase_decoding_snr_threshold;

  double phase_decoding_notch_width;

  tdrp_bool_t sz1_negate_phase_codes;

  double sz1_strong_to_weak_power_ratio_threshold;

  double sz1_out_of_trip_power_ratio_threshold;

  int sz1_out_of_trip_power_n_replicas;

  int cmd_kernel_ngates_tdbz;

  int cmd_kernel_ngates_spin;

  int cmd_kernel_ngates_zdr_sdev;

  int cmd_kernel_ngates_phidp_sdev;

  double cmd_spin_dbz_threshold;

  double cmd_snr_threshold;

  int cpa_median_filter_len;

  tdrp_bool_t cpa_compute_using_alternative;

  double cmd_threshold_for_clutter;

  tdrp_bool_t cmd_check_for_offzero_weather;

  double cmd_threshold_for_offzero_weather;

  double min_snr_for_offzero_weather;

  double notch_width_for_offzero_snr;

  tdrp_bool_t cmd_check_for_windfarm_clutter;

  double min_spectral_snr_for_windfarm_clutter;

  double min_snr_for_windfarm_clutter_check;

  double min_cpa_for_windfarm_clutter_check;

  tdrp_bool_t apply_db_for_db_correction;

  double db_for_db_ratio;

  double db_for_db_threshold;

  tdrp_bool_t apply_cmd_speckle_filter;

  speckle_threshold_t *_cmd_speckle_filter_thresholds;
  int cmd_speckle_filter_thresholds_n;

  tdrp_bool_t apply_cmd_gap_filter;

  int cmd_gap_filter_len;

  double cmd_gap_filter_threshold;

  tdrp_bool_t apply_nexrad_spike_filter_after_cmd;

  interest_map_point_t *_tdbz_interest_map;
  int tdbz_interest_map_n;

  double tdbz_interest_weight;

  interest_map_point_t *_spin_interest_map;
  int spin_interest_map_n;

  double spin_interest_weight;

  double max_of_tdbz_and_spin_interest_weight;

  interest_map_point_t *_cpa_interest_map;
  int cpa_interest_map_n;

  double cpa_interest_weight;

  interest_map_point_t *_zdr_sdev_interest_map;
  int zdr_sdev_interest_map_n;

  double zdr_sdev_interest_weight;

  interest_map_point_t *_phidp_sdev_interest_map;
  int phidp_sdev_interest_map_n;

  double phidp_sdev_interest_weight;

  tdrp_bool_t apply_rhohv_test_after_cmd;

  double rhohv_test_min_rhohv;

  double rhohv_test_max_rhohv;

  double rhohv_improvement_factor_threshold;

  char* output_fmq_url;

  int output_fmq_size;

  int output_fmq_nslots;

  tdrp_bool_t output_fmq_compress;

  int nbeams_for_params_and_calib;

  tdrp_bool_t write_blocking;

  int beam_wait_msecs;

  output_field_t *_output_fields;
  int output_fields_n;

  tdrp_bool_t use_sweep_info_from_time_series;

  tdrp_bool_t delay_tilt_start_msg_during_ant_trans;

  tdrp_bool_t set_end_of_sweep_when_antenna_changes_direction;

  double required_delta_angle_for_antenna_direction_change;

  int min_rays_in_sweep_for_antenna_direction_change;

  int max_sweeps_in_vol_for_antenna_direction_change;

  tdrp_bool_t use_volume_info_from_time_series;

  tdrp_bool_t set_end_of_vol_from_elev_angle;

  tdrp_bool_t vol_starts_at_bottom;

  double elev_change_for_end_of_vol;

  int min_beams_per_vol;

  tdrp_bool_t set_end_of_vol_on_prf_change;

  tdrp_bool_t set_end_of_vol_on_pulse_width_change;

  transition_method_t transition_method;

  tdrp_bool_t check_transition_from_fixed_angle_error;

  double max_fixed_angle_error_ppi;

  double max_fixed_angle_error_rhi;

  selected_region_t selected_region;

  tdrp_bool_t do_selected_print;

  char* spectra_dir;

  tdrp_bool_t write_individual_spectra_files;

  tdrp_bool_t write_combined_spectra_file;

  double min_snr_for_combined_spectra_file;

  int data_mapper_report_interval;

  char _end_; // end of data region
              // needed for zeroing out data

private:

  void _init();

  mutable TDRPtable _table[254];

  const char *_className;

  bool _exitDeferred;

};

#endif

