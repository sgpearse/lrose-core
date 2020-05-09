/*
# define NEW_ALLOC_SCHEME

# include <dorade_headers.h>
# include <solo_editor_structs.h>
# include <dd_math.h>
# include <ui.h>
# include <ui_error.h>
# include <stdio.h>
# include <function_decl.h>
# include <dgi_func_decl.h>
# include <glib.h>

extern GString *gs_complaints;


# define         BB_USE_FGG 0
# define     BB_USE_AC_WIND 1
# define  BB_USE_LOCAL_WIND 2
*/

/* c------------------------------------------------------------------------ */

int alt_gecho(double min_grad,
	      int *zmax_cell,
	      double elev,
	      float *data,
	      size_t nGates,
	      double dds_asib_rotation_angle,
	      double dds_asib_roll,
	      double dds_cfac_rot_angle_corr)

/*
(dgi, min_grad, zmax_cell)
  DGI_PTR dgi;
  double min_grad;
  int *zmax_cell;
*/
{
  //    DDS_PTR dds=dgi->dds;
    int pn, ii, jj, kk, mm, mark, zmax = 0, ival;
    size_t nn;
    int mark_max = -1, mark_grad = -1, ng_grad;
    float *ss;
    short scaled30;
    double gs, slope, smin_grad, grad; //  elev = dds->ra->elevation;
    double rot_angle;

    if((pn = dd_find_field(dgi, "DBZ")) < 0) {	
	uii_printf("Source parameter %s not found for surface removal\n"
		  , "DBZ");
	return(-1);
    }
    /* assume this routine is not called unless the antenna is
     * in a position to see the ground
     */
    // TODO: is gs the distance between gates?
    gs = dds->celvc->dist_cells[1] - dds->celvc->dist_cells[0];
    ss = data; // (short *)dds->qdat_ptrs[pn];
    nn = nGates; // dgi->dds->celvc->number_cells;
    // TODO: what to do about this scaled30?  just set it to 30.0 ?
    scaled30 = (int)(dgi->dds->parm[pn]->parameter_scale * 30.);
    // TODO: what to do about this ??
    smin_grad = min_grad * dgi->dds->parm[pn]->parameter_scale * gs * 2.;
    rot_angle = dds_asib_rotation_angle + dds_asib_roll +
      dds_cfac_rot_angle_corr;

    /* scaled change in dBz over two gates */
    /* min_grad = .08 => ~20 dBz over two gates. */
       
    ng_grad = (int)(1.075 * (1./fabs (sin (elev)) +1.));
    if (ng_grad < 3 || ng_grad > 60)
      { mark = 0; }

    if (ng_grad < 2)
      { ng_grad = 2; }
    else if (ng_grad > 77)
      { ng_grad = 77; }

    for (ii=ng_grad+1; ii < nn; ii++) {
       ival = ss[ii];
       if (ival < scaled30)
	 { continue; }
       
       if (mark_grad < 0) {
	  for (grad = 0, jj = 0; jj < ng_grad; jj++) {
	     grad += ss[ii-jj] - ss[ii-jj-1];
	     if (grad > smin_grad) {
		mark_grad = ii-jj-1;
		break;
	     }
	  }
       }
       if (ival > scaled30 && ival > zmax) {
	  mark_max = ii; zmax = ival;
       }
    }
				/*
    printf ("%8.2f %8.4f %4d %4d %4d %.1f\n"
	    , rot_angle, elev, ng_grad, mark_grad, mark_max
	    , smin_grad *.01);
				 */
    *zmax_cell = mark_max;
    return (mark_grad);
}

/* c------------------------------------------------------------------------ */

void se_ac_surface_tweak(
			 float optimal_beamwidth,
			 float vert_beam_width,
			 float asib_altitude_agl,
			 float dds_ra_elevation,
			 size_t clip_gate,
			 bool getenv_ALTERNATE_GECHO,
			 bool *boundary_mask,
)
{
    /* 
     * #remove-surface#
     * #remove-only-surface#
     * #remove-only-second-trip-surface#
     */
    struct ui_command *cmdq=cmds+1; /* point to the first argument */
    int ii, nc, nn, mark, navg, first_cell=YES, pn, sn;
    int g1, g2, gx, gate_shift, surface_only = NO, only_2_trip = NO;
    int no_footprint = NO, zmax_cell,  alt_g1;
    bool alt_gecho_flag = false;
    char *name, *aa;
    short *ss, *tt, *zz, *bnd, v0, v4, vx, bad;
    double rot_ang, earthr, deg_elev, bmwidth, elev_limit = -.0001;
    double range_val, min_range, max_range, alt, range1;
    double ground_intersect, footprint, surf_shift, fudge=1.;
    double d, elev, tan_elev, half_vbw, min_grad = .08;	/* dBz/meter */
    struct dd_general_info *dgi, *dd_window_dgi();
    struct dds_structs *dds;
    struct platform_i *asib;
    struct radar_angles *ra;
    struct solo_edit_stuff *seds, *return_sed_stuff();


    seds = return_sed_stuff();	/* solo editing struct */

    if(seds->finish_up) {
	return(1);
    }
    if(seds->process_ray_count == 1) {
	/*
	 * 
	 */
	mark = 0;
    }
    //    name = (cmdq++)->uc_text;
    //    sn = strlen(name);

    surface_only = strstr( cmds->uc_text
			  , "ly-sur") != NULL; /* only-surface */
    only_2_trip = strstr( cmds->uc_text
			 , "ly-sec") != NULL; /* only-second-trip-surface */

    //    dgi = dd_window_dgi(seds->se_frame);
    bnd = (short *) seds->boundary_mask;
    //dds = dgi->dds;
    //asib = dds->asib;
    /*
     * we probably need to make a calculation to determine if
     * the ground echo goes above the aircraft i.e. is the distance
     * (arc length) defined by a ray of length max_range rotated
     * through half the beam width greater than or equal to the
     * altitude of the aircraft?
     */
    bmwidth = RADIANS(seds->optimal_beamwidth ? seds->optimal_beamwidth :
	  dgi->dds->radd->vert_beam_width);
    half_vbw = .5 * bmwidth;

    alt = (asib->altitude_agl)*1000.;
    max_range = dds->celvc->dist_cells[dgi->clip_gate];
    elev = dds->ra->elevation;

    if (surface_only && (aa = getenv ("ALTERNATE_GECHO"))) {
       if( elev > -.002)	/* -.10 degrees */
	 { return(1); }
       alt_gecho_flag = true;
       d = atof (aa);
       if (d > 0)
	 { min_grad = d; }	/* dbz per meter */
    }

    if( !surface_only && (d = max_range * fudge * bmwidth) >= alt) {
	d -= alt;
	elev_limit = atan2(d, (double)max_range);

	if( elev > elev_limit)
	    return(1);

	if(d >= 0 && elev > -fudge * bmwidth) {
	    only_2_trip = YES;
	    g1 = 0;
	}
    }

    if( elev > elev_limit)
	  return(1);

    if( !only_2_trip ) {
        earthr = dd_earthr(dd_latitude(dgi));
	elev -= half_vbw;
	tan_elev = tan(elev);
	range1 = ground_intersect = (-(alt)/sin(elev))*
	      (1.+alt/(2.*earthr*1000.*
		       tan_elev*tan_elev));
	if(ground_intersect > max_range || ground_intersect < 0 )
	      return(1);
	/*
	 */
	g1 = dd_cell_num(dgi->dds, 0, range1);
    }
    gate_shift = seds->surface_gate_shift;

    if (alt_gecho_flag) {
      ii = alt_gecho (min_grad, &zmax_cell, elev, data, nGates,
		      dds_asib_rotation_angle, dds_asib_roll, dds_cfac_rot_angle_corr);
       /*
(double min_grad,
              int *zmax_cell,
              double elev,
              float *data,
              size_t nGates,
              double dds_asib_rotation_angle,
              double dds_asib_roll,
              double dds_cfac_rot_angle_corr)
       */
       if (ii > 0)
	 { g1 = ii; gate_shift = 0; }
       else
	 { return (1); }
    }

    if((pn = dd_find_field(dgi, name)) < 0) {	
	uii_printf("Source parameter %s not found for surface removal\n"
		  , name);
	seds->punt = YES;
	return(-1);
    }
    g1 += gate_shift;
    if(g1 < 0) g1 = 0;

    ss = (short *)dds->qdat_ptrs[pn];
    zz = ss + dgi->clip_gate +1;
    ss += g1;
    bad = dds->parm[pn]->bad_data;

    for(; ss < zz;)
	  *ss++ = bad;

    return(1);
}

/* c------------------------------------------------------------------------ */

int se_remove_storm_motion(arg, cmds)	/* #remove-storm-motion# */
  int arg;
  struct ui_command *cmds;	
{
    /* remove the aircraft motion from velocities
     */
  //    struct ui_command *cmdq=cmds+1; /* point to the first argument */
    int ii, nc, nn, mark, pn, sn;
    int scaled_vel;
    //char *name;
    short *ss, *tt, *zz, *bnd, vx, bad;
    float speed, wind, scale, bias;
    double d, az, cosEl, rcp_cosEl, theta, cosTheta, adjust, scaled_adjust;
    //struct dd_general_info *dgi, *dd_window_dgi();
    //struct dds_structs *dds;
    //struct solo_edit_stuff *seds, *return_sed_stuff();
    double d_angdiff();


    seds = return_sed_stuff();	/* solo editing struct */

    if(seds->finish_up) {
	return(1);
    }
    seds->modified = YES;
    name = (cmdq++)->uc_text;
    sn = strlen(name);
    bnd = (short *) seds->boundary_mask;
    dgi = dd_window_dgi(seds->se_frame);
    dds = dgi->dds;

    if((pn = dd_find_field(dgi, name)) < 0) {	
	uii_printf("Source parameter %s not found for copy\n", name);
	seds->punt = YES;
	return(-1);
    }
# ifdef NEW_ALLOC_SCHEME
    ss = (short *)dds->qdat_ptrs[pn];
# else
    ss = (short *)((char *)dds->rdat[pn] +sizeof(struct paramdata_d));
# endif
    wind = (cmdq++)->uc_v.us_v_float; /* angle */
    speed = (cmdq++)->uc_v.us_v_float;
    wind = FMOD360 (wind +180.); /* change to wind vector */
    az = dd_rotation_angle (dgi);
    cosEl = cos (RADIANS (dd_elevation_angle (dgi)));
    if (fabs (cosEl) < .0001)
      { return (1); }
    rcp_cosEl = 1./cosEl;
    theta = d_angdiff (az, wind); /* clockwise from az to wind */
    adjust = cos (RADIANS (theta)) * speed;

    scale = dds->parm[pn]->parameter_scale;
    bias = dds->parm[pn]->parameter_bias;
    scaled_adjust = DD_SCALE(adjust, scale, bias);
    bad = dds->parm[pn]->bad_data;
    nc = dgi->clip_gate +1;
    zz = ss +nc;

    for(; ss < zz; ss++,bnd++) {
	if(!(*bnd) || *ss == bad)
	      continue;
	d = (*ss * cosEl -scaled_adjust) * rcp_cosEl;
	*ss = (short)d;
    }
    return(1);
}
/* c------------------------------------------------------------------------ */
