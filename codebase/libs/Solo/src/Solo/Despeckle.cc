

//int se_despeckle(arg, cmds)             // #despeckle# 
//int arg;
//struct ui_command *cmds;
void se_despeckle(const float *data, float *newData, size_t nGates, float bad, int a_speckle,
		  int dgi_clip_gate, short *boundary_mask) 
{
  int below;
  //  struct ui_command *cmdq=cmds+1; // point to the first argument 
  //  struct solo_edit_stuff *seds, *return_sed_stuff();
  //  struct dd_general_info *dgi, *dd_window_dgi();
  //  struct dds_structs *dds;
  //  char *dst_name;

  int nc, nd, fn, bad, a_speckle;
  int ii, nn, mark;
  float  *ss, *zz, *tt, *nss;
  unsigned short *bnd;


  //  dst_name = (cmdq++)->uc_text;
  //  nd = strlen(dst_name);

  //  seds = return_sed_stuff();
  //  if(seds->finish_up) {
  //    return(1);
  //  }
  //  seds->modified = YES;
  //  a_speckle = seds->a_speckle;
  //  dgi = dd_window_dgi(seds->se_frame);
  //  dds = dgi->dds;
  if (dgi_clip_gate >= nGates) throw new ArgumentException("dgi_clip_gate greater than number of gates");
  nc = dgi_clip_gate+1;
  bnd = boundary_mask;
  //                                                                                                             
  // find the field to be thresholded                                                                            
  //
  //if((fn = dd_find_field(dgi, dst_name)) < 0) {
  // field not found                                                                                         
  //
  //  uii_printf("Field to be thresholded: %s not found\n", dst_name);
  //  seds->punt = YES;
  //  return(-1);
  //}
  //# ifdef NEW_ALLOC_SCHEME
  //  ss = (short *)dds->qdat_ptrs[fn];
  //# else
  //  ss = (short *)((char *)dds->rdat[fn] + sizeof(struct paramdata_d));
  //# endif
  //bad = dds->parm[fn]->bad_data;

  // memcopy data into newData
  memcpy(data, newData, nGates*sizeof(float));

  size_t zzIdx = nGates;
  if (dgi_clip_gate < nGates)
    zzIdx = dgi_clip_gate + 1;
  //                                                                                                             
  // loop through the data                                                                                       
  //
  size_t ssIdx = 0;
  while (ssIdx < zzIdx) {
    // move to first good gate inside a boundary
    while (ssIdx < zzIdx && (data[ssIdx] == bad || !(boundary_mask[ssIdx]))) {
      ssIdx += 1;
    }
    size_t nNotBad = 0;
    size_t ttIdx = ssIdx;
    // now move forward to the next bad flag                                                                   
    //
    while (ssIdx < zzIdx && boundary_mask[ssIdx] && data[ssIdx] != bad) {
      nNotBad += 1;
      ssIdx += 1;
    } 
    //(tt=ss,nn=0; ss < zz && *bnd && *ss != bad; nn++,ss++,bnd++);
    if(boundary_mask[ssIdx] && nNotBad <= a_speckle) {
      while (ttIdx < ssIdx) {
        newData[ttIdx] = bad;      // zap speckle 
        ttIdx+= 1;
      }
    }
    // else,  not a speckle or outside boundary 


  }

  /*
  for(; ss < zz; ss++,bnd++) {
    // move to first good gate inside a boundary                                                               
    //
    for(; ss < zz && (*ss == bad || !(*bnd)); ss++,bnd++);
    if(ss >= zz) break;
    // now move forward to the next bad flag                                                                   
    //
    for(tt=ss,nn=0; ss < zz && *bnd && *ss != bad; nn++,ss++,bnd++);
    if(!(*bnd) || nn > a_speckle)
      continue;         // not a speckle or outside boundary 
    for(;tt < ss;)
      *tt++ = bad;      // zap speckle 
  }
  return(fn);
  */
}
