
// Bad flags can be set using values in one data field,
// then applied to another data field.
// So, the flag "mask" survives from one function to another.
// The bad flag mask is like a boundary mask.
//

class FlagOps {
// need a bad flags mask array; will this be like a boundary mask??
// need to be persistent? static?
// Q: how is bad flag mask used?
// editor/se_for_each.c:      seds->bad_flag_mask = flag = seds->bad_flag_mask_array;
//      for flagged-copy ... consider bad_flag_mask and boundary_mask
// editor/se_defrkl.c:    seds->bad_flag_mask = flag = seds->bad_flag_mask_array;
//      for flag-freckles ... consider bad_flag_mask and boundary_mask
//      for flag-glitches ...    "       "
bool *bad_flag_mask_array;  // unsigned short, originally in Soloii
bool *bad_flag_mask;        // unsigned short, originally in Soloii
bool use_bad_flag_mask;     // int, originally in Soloii



};



/* c------------------------------------------------------------------------ */

void se_do_clear_bad_flags_array(bool *bad_flag_mask, size_t nn)
//  int nn;
{
  //struct solo_edit_stuff *seds, *return_sed_stuff();

  // seds = return_sed_stuff();
  memset(bad_flag_mask, 0, nn * sizeof(bool)); 
  // (nn > 0 ? nn : seds->max_gates) * sizeof(*seds->bad_flag_mask_array));
}
/* c------------------------------------------------------------------------ */

//int se_set_bad_flags(arg, cmds)	/* #set-bad-flags# */
//  int arg;
//  struct ui_command *cmds;
// 
// In params:
// data    pointer to data values
// bad     value to indicate bad value in data

// In/Out params:
// bad_flags_mask send preallocated space, same size as data
//                returns boolean array: 
//                  true if data value meets bad flag criteria AND in inside boundary mask
void se_set_bad_flags(char *where, float scaled_thr1, float scaled_thr2, const float *data, size_t nGates,
		      float bad, size_t dgi_clip_gate, bool *boundary_mask, bool *bad_flag_mask)
{
  //int below;
  //struct ui_command *cmdq=cmds+1; /* point to the first argument */
  //struct solo_edit_stuff *seds, *return_sed_stuff();
  //struct dd_general_info *dgi, *dd_window_dgi();
  //struct dds_structs *dds;
    //char *name, *dst_name, *where;
    //float f_thr1, f_thr2;

    int nc, nd, nchar, bad, thr_bad, fn, fgg;
    int gg, ii, jj, kk, nn, scaled_thr1, scaled_thr2, mark, fthr;
    short *anchor, *ss, *zz; // , *thr=NULL;
    float *thr;
    bool *bnd, *flag; // was unsigned short


    //name = (cmdq++)->uc_text;
    //nchar = strlen(name);
    //where = (cmdq++)->uc_text;
    //f_thr1 = (cmdq++)->uc_v.us_v_float;
    //if(cmdq->uc_ctype != UTT_END)
    //  f_thr2 = cmdq->uc_v.us_v_float;

    //seds = return_sed_stuff();
    //if(seds->finish_up) {
    //	return(1);
    //}
    //seds->modified = YES;
    // which frame is active; i.e. which frame to apply command
    //dgi = dd_window_dgi(seds->se_frame);
    //dds = dgi->dds;
    nc = dgi_clip_gate+1;
    // fgg = seds_first_good_gate; // not used??
    bnd = boundary_mask;
    flag = bad_flag_mask;
    //seds->bad_flag_mask = flag = seds->bad_flag_mask_array;
    //
    // find the thr field; the data are sent as an argument
    //
    //if((fn = dd_find_field(dgi, name)) < 0) {
    // thr field not found
    //
    // uii_printf("Set bad flags field: %s not found\n", name);
    // seds->punt = YES;
    // return(-1);
    //}
    // fn is an index for the data field
    // thr is a pointer to the data
    //# ifdef NEW_ALLOC_SCHEME
    //thr = (short *)dds->qdat_ptrs[fn];
    //# else
    //thr = (short *)((char *)dds->rdat[fn] + sizeof(struct paramdata_d));
    //# endif
    thr = data;
    thr_bad = bad; // dds->parm[fn]->bad_data;
    zz = thr +nc;
    //    scaled_thr1 = DD_SCALE(f_thr1, dds->parm[fn]->parameter_scale
    //		  , dds->parm[fn]->parameter_bias);
    //scaled_thr2 = DD_SCALE(f_thr2, dds->parm[fn]->parameter_scale
    //		  , dds->parm[fn]->parameter_bias);
    //bad = dds->parm[fn]->bad_data;
    se_do_clear_bad_flags_array(nc, bad_flag_mask);
   
    /*
     * loop through the data
     */

    if(strncmp(where, "below", 3) == 0) {
	for(; thr < zz; thr++,bnd++,flag++) {
	    if(!(*bnd) || *thr == bad)
	      *flag = false; // continue;
	    if(*thr < scaled_thr1) {
		*flag = true;
	    }
	}
    }
    else if(strncmp(where, "above", 3) == 0) {
	for(; thr < zz; thr++,bnd++,flag++) {
	    if(!(*bnd) || *thr == bad)
	      *flag = false; // continue;
	    if(*thr > scaled_thr1) {
		*flag = true;
	    }
	}
    }
    else {			/* between */
	for(; thr < zz; thr++,bnd++,flag++) {
	    if(!(*bnd) || *thr == bad)
	      *flag = false; // continue;
	    if(*thr >= scaled_thr1 && *thr <= scaled_thr2) {
		*flag = true;
	    }
	}
    }
    //    return(fn);
}  
/* c------------------------------------------------------------------------ */

