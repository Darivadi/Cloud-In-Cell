/*
 * Function:  readGADGETBinaryFile
 * --------------------
 * Reads a GADGET binary file and store the information in the data structure
 * variable *part* it also returns the total number of particles.
 *
 *  There are no arguments in the routiene.
 *
 *  returns: Total number of particles stored in the binare file.
 */
#ifdef BINARYGADGET
int readGADGETBinaryFile(){
  FILE *fdata = NULL;
  int i, j;
  int N_tot, N_min, N_max, dummy, nread=0;
  float Maux, faux[3];
  unsigned int uintaux;

  printf("############################################################\n");
  printf("Reading snapshot %s\n", GV.FILENAME);
  fdata = fopen(GV.FILENAME,"r");
  if(fdata == NULL){
    printf("File %s cannot be open\n", GV.FILENAME);
  }

  nread = fread(&dummy, sizeof(dummy), 1, fdata);
  nread = fread(&Header, sizeof(struct gadget_head), 1, fdata);
  nread = fread(&dummy, sizeof(dummy), 1, fdata);

  N_tot = 0;

  printf("----------------------------------------\n");
  printf("Reading snapshot with:\n");
  printf("----------------------------------------\n");
  for(i = 0; i<6; i++){
    N_tot += Header.npartTotal[i];
    printf("Type %d has Npart=%12d NpartTotal=%12d with mass %16.8lf\n", i,
	   Header.Npart[i], Header.npartTotal[i], Header.mass[i]);
  }//for i 

  printf(" There is a total %d particles in the snapshot\n\n", N_tot);
  printf("----------------------------------------\n");
  printf(" * Frame's Time... %16.8f\n",Header.time);
  printf(" * Redshift... %16.8f\n",Header.redshift);
  printf(" * Flagsfr... %d\n",Header.flag_sfr);
  printf(" * Flagfed... %d\n",Header.flag_feedback);
  printf(" * Flagcool... %d\n",Header.flag_cooling);
  printf(" * numfiles... %d\n",Header.num_files);
  printf(" * Boxsize... %16.8f\n",Header.BoxSize);
  printf(" * Omega0... %16.8f\n",Header.Omega0);
  printf(" * OmageLa... %16.8f\n",Header.OmegaLambda);
  printf(" * Hubbleparam... %16.8f\n",Header.HubbleParam);

  // Memory allocation for part variable
  part = (struct particle *) calloc((size_t) N_tot,sizeof(struct particle));

  if(part == NULL){
    printf("Structure particles could not be allocated\n");
    exit(0);
  }//if

  /*********************/
  /* Getting positions */
  /*********************/
  nread=fread(&dummy, sizeof(dummy), 1, fdata);
  for(i=0; i<N_tot; i++){
    nread=fread(&faux[0], sizeof(float), 3, fdata);
    part[i].pos[X] = faux[0];
    part[i].pos[Y] = faux[1];
    part[i].pos[Z] = faux[2];
  }//for i
  nread=fread(&dummy, sizeof(dummy), 1, fdata);
  if(dummy != (3*N_tot*sizeof(float))){
    printf(" Can not read properly ids %d %lu\n",dummy,3*N_tot*sizeof(float));
    exit(0);
  }//if

  /**********************/
  /* Getting velocities */
  /**********************/
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  for(i=0; i<N_tot; i++){
    nread=fread(&faux[0],sizeof(float),3,fdata);
    part[i].vel[X] = faux[0];
    part[i].vel[Y] = faux[1];
    part[i].vel[Z] = faux[2];
  }//for i

  
  nread=fread(&dummy, sizeof(dummy), 1, fdata);
  if(dummy != (3*N_tot*sizeof(float))){
    printf(" Can not read properly ids %d %lu\n", dummy, 3*N_tot*sizeof(float));
    exit(0);
  }//if

  /****************/
  /* Getting ID's */
  /****************/
  nread=fread(&dummy, sizeof(dummy), 1, fdata);
  for(i=0; i<N_tot; i++){
    nread=fread(&uintaux, sizeof(unsigned int), 1, fdata);
    part[i].id = uintaux;
  }//for i
  
  nread=fread(&dummy, sizeof(dummy), 1, fdata);
  if(dummy != (N_tot*sizeof(unsigned int))){
    printf(" Can not read properly ids %d %lu\n", dummy, N_tot*sizeof(unsigned int));
    exit(0);
  }//if 

  /******************/
  /* Getting masses */
  /******************/
  nread=fread(&dummy, sizeof(dummy),1,fdata);
  N_min = N_max=0;
  for(j=0; j<=5; j++){
    N_max=N_max+Header.npartTotal[j];
    if((Header.mass[j]==0) && (Header.npartTotal[j]!=0)){
      for(i=N_min;i<N_max;i++){
	nread=fread(&Maux,sizeof(float),1,fdata);
	part[i].mass = Maux;
      }//for i
    }//if
    if((Header.mass[j]!=0) && (Header.npartTotal[j]!=0)){
      for(i=N_min;i<N_max;i++){
	part[i].mass = Header.mass[j];
      }//for i
    }//if
    N_min=N_max;
  }//for j
  nread=fread(&dummy,sizeof(dummy),1,fdata);
  fclose(fdata);
  return N_tot;
}
#endif

/*
 * Function:  W
 * --------------------
 * Computes the distribution of particle, given an distribution scheme.
 * Distribution scheme could be Nearest Grid Point (NGP), Cloud in Cell (CIC)
 * and Triangular Shaped Cloud (TSC). Distribution schemes are selected by
 * preprocessor directives in the header of the code.
 *
 *  x, y, z: position in x, y and z.
 *
 *  H: separation of grids ( H = L / N_grid ).
 *
 *  returns: value of the window function according to the distribution scheme.
 */

double W(double x, double y, double z, double H){
  double Wx, Wy, Wz;

  // CHECK IT!!!!!!
#ifdef NGP
  /* Nearest Grid Point */
  // One dimensional window function in the X-axis
  if( fabs(x) < H*0.5 )
    {
      Wx = 1.0;
    }
  else if( fabs(x) == H*0.5  )
    {
      Wx = 0.5;
    }
  else
    {
      Wx = 0.0;
    }
  // One dimensional window function in the Y-axis
  if( fabs(y) < H*0.5 )
    {
      Wy = 1.0;
  }
  else if( fabs(y) == H*0.5  )
    {
      Wy = 0.5;
    }
  else
    {
      Wy = 0.0;
    }
  // One dimensional window function in the Z-axis
  if( fabs(z) < H*0.5 ){
    Wz = 1.0;
  }else if( fabs(z) == H*0.5  ){
    Wz = 0.5;
  }else{
    Wz = 0.0;
  }
#endif

#ifdef CIC
  /* Cloud In Cell */
  // One dimensional window function in the X-axis
  if( fabs(x) < H )
    {
      Wx = 1.0 - fabs(x)/H;
    }
  else
    {
      Wx = 0.0;
    }
  // One dimensional window function in the Y-axis
  if( fabs(y) < H )
    {
      Wy = 1.0 - fabs(y)/H;
    }
  else
    {
      Wy = 0.0;
    }
  // One dimensional window function in the Z-axis
  if( fabs(z) < H )
    {
      Wz = 1.0 - fabs(z)/H;
    }
  else
    {
      Wz = 0.0;
    }
#endif

  return Wx * Wy * Wz; /* As we use the regular cubic grid, the three 
			  dimensional window function is given as the 
			  multiplication of three one dimensional window 
			  functions.
			  WindowFunction = W(x) * W(y) * W(z).
		       */
}

/*
 * Function:  locateCell
 * --------------------
 * Locate the index in C-Order of the cell in wich should be the particle 
 * according to the coordinate of the particle.
 *
 *  xp, yp, zp: Coordinates x, y and z of the particle.
 *  indexPartArray: index of the particle in the array of struct particle.
 *  cells: array of struct Cell.
 *
 *  returns: The function does not return nothing.
 */

void locateCell(double xp, double yp, double zp, int indexPartArray, struct Cell *cells){
  
  //Numbers in order to find the cell
  //Counters in x,y and z
  int i,j,k,n;
  
  // x-axis
  i = floor( (xp / GV.L) * GV.NGRID );
  // Y-axis
  j = floor( (yp / GV.L) * GV.NGRID );
  // Z-axis
  k = floor( (zp / GV.L) * GV.NGRID );
  
  n = INDEX(i,j,k);
    
  // Storing particles in cell
  /*if(cells[n].Np_cell == 0)
    {
      cells[n].id_part = (long int*) malloc(1*sizeof(long int) ); 
    }//of
  */
  cells[n].Np_cell = cells[n].Np_cell + 1;  
  cells[n].id_part = (long int*) realloc(cells[n].id_part, cells[n].Np_cell*sizeof(long int)); 
  cells[n].id_part[cells[n].Np_cell-1] = indexPartArray;
  
}

/*
 * Function:  mod
 * --------------------
 * Calculate the modulo operation for two numbers a and b (a%b) 
 * includding negative numbers.
 *
 *  a: Numerator of the division.
 *  b: Denominator of the division.
 *
 *  returns: The module a%b including the option for negative numbers.
 */

int mod(int a, int b){
  int mod = a%b;
  while(mod<0){
    mod += b;
  }
  return mod;
}
