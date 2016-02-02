/****************************************************************************************************           
NAME: conf2dump                                                                                                 
FUNCTION: Reads the input file with parameters                                                                  
INPUT: Parameters file                                                                                          
RETURN: 0                                                                                                       
****************************************************************************************************/

int conf2dump( char filename[] )
{
  int nread; 
  char cmd[1000];
  sprintf( cmd, 
	   "grep -v \"#\" %s | grep -v \"^$\" | gawk -F\"=\" '{print $2}' > %s.dump", 
	   filename, filename );
  nread = system( cmd );
  
  return 0;
}


/****************************************************************************************************           
NAME: read_parameters                                                                                           
FUNCTION: Reads the parameters                                                                                  
INPUT: Parameters file                                                                                          
RETURN: 0                                                                                                       
****************************************************************************************************/

int read_parameters( char filename[] )
{
  int nread;
  char cmd[1000], filenamedump[1000];
  FILE *file;
  
  /*+++++ Loading the file +++++*/
  file = fopen( filename, "r" );
  if( file==NULL )
    {
      printf( "  * The file '%s' doesn't exist!\n", filename );
      return 1;
    }
  fclose(file);
  
  /*+++++ Converting to plain text +++++*/
  conf2dump( filename );
  sprintf( filenamedump, "%s.dump", filename );
  file = fopen( filenamedump, "r" );
  
  /*+++++ Parameters for binary data +++++*/
#ifdef BINARYDATA
  nread = fscanf(file, "%d", &GV.NGRID);
  nread = fscanf(file, "%s", GV.FILENAME);

  /*+++++ Cosmological parameters +++++*/  
  nread = fscanf(file, "%lf", &GV.OmegaM0);
  nread = fscanf(file, "%lf", &GV.OmegaL0);
  nread = fscanf(file, "%lf", &GV.zRS);
  nread = fscanf(file, "%lf", &GV.HubbleParam);    
  GV.aSF = 1.0/(1.0 + GV.zRS);   
#endif


  /*+++++ Parameters for ASCII data +++++*/
#ifdef ASCIIDATA
  /*+++++ Simulation parameters +++++*/
  nread = fscanf(file, "%d", &GV.NGRID);
  nread = fscanf(file, "%lf", &GV.L);
  nread = fscanf(file, "%s", GV.FILENAME);
  nread = fscanf(file, "%ld", &GV.NpTot);
  
  /*+++++ Cosmological parameters +++++*/  
  nread = fscanf(file, "%lf", &GV.OmegaM0);
  nread = fscanf(file, "%lf", &GV.OmegaL0);
  nread = fscanf(file, "%lf", &GV.zRS);
  nread = fscanf(file, "%lf", &GV.HubbleParam);    
  GV.aSF = 1.0/(1.0 + GV.zRS); 
#endif

    fclose( file );
    
    printf( "  * The file '%s' has been loaded!\n", filename );

    sprintf( cmd, "rm -rf %s.dump", filename );
    nread = system( cmd );
    
    return 0;
}


/****************************************************************************************************           
NAME: read_ascii                                                                                          
FUNCTION: Reads the parameters                                                                                  
INPUT: Parameters file                                                                                          
RETURN: 0                                                                                                       
****************************************************************************************************/
#ifdef ASCIIDATA
int read_ascii(char *filename)
{
  int i, nread;
  double x_aux, y_aux, z_aux;  
  FILE *inFile = NULL;
  char buff[1000];
  
  inFile = fopen(filename, "r");
  
  /*+++++ Ignoring first line +++++*/
  printf("Ignoring first line\n");
  printf("--------------------------------------------------\n\n");
  nread = fgets(buff, 1000, inFile);
  

  /*+++++ Reading from second line +++++*/
  printf("Reading from second line\n");
  printf("--------------------------------------------------\n\n");

  for(i=0; i<GV.NpTot; i++)
    {                    
      nread = fscanf(inFile, "%d %d %lf %lf %lf %lf", 
		     &part[i].id, &part[i].tag, 
		     &part[i].pos[X], &part[i].pos[Y], &part[i].pos[Z], 
		     &part[i].mass);
      /*      
      if(i%10000==0)
	{
	  printf("Reading i=%d x=%lf mass=%lf\n", i, part[i].pos[X], part[i].mass);
	}//if
      */
 
    }//for i
  fclose(inFile);
    
  return 0;
}//read_ascii
#endif


/****************************************************************************************************           
NAME: read_binary
FUNCTION: Reads the parameters in the binary file
INPUT: Parameters file                                                                                          
RETURN: 0                                                                                                       
****************************************************************************************************/

int read_binary(void)
{
  int i, nread;
  double pos_aux[3];
  FILE *inFile=NULL;
  
  inFile = fopen(GV.FILENAME, "r");

  /*+++++ Saving Simulation parameters +++++*/
  nread = fread(&GV.L, sizeof(double), 1, inFile);  //Box Size
  nread = fread(&GV.NpTot, sizeof(long int), 1, inFile);  //Number of particles
  nread = fread(&GV.OmegaM0, sizeof(double), 1, inFile);  //Matter density parameter
  nread = fread(&GV.OmegaL0, sizeof(double), 1, inFile);  //Cosmological constant density parameter
  nread = fread(&GV.zRS, sizeof(double), 1, inFile);  //Redshift
  nread = fread(&GV.HubbleParam, sizeof(double), 1, inFile);  //Hubble parameter


  printf("-----------------------------------------------\n");
  printf("Cosmological parameters:\n");
  printf("OmegaM0=%lf OmegaL0=%lf redshift=%lf HubbleParam=%lf\n",
	 GV.OmegaM0,
	 GV.OmegaL0,
	 GV.zRS,
	 GV.HubbleParam);
  printf("-----------------------------------------------\n");

  printf("Simulation parameters:\n");
  printf("NpTotal=%ld L=%lf\n",
	 GV.NpTot,
	 GV.L);
  printf("-----------------------------------------------\n");



  /*+++++ Allocating memory +++++*/
  part = (struct particle *) calloc((size_t) GV.NpTot,sizeof(struct particle));

  for(i=0; i<GV.NpTot; i++ )
    { 
      nread = fread(&part[i].id, sizeof(long int), 1, inFile);
      nread = fread(&part[i].tag, sizeof(int), 1, inFile);

      nread = fread(&pos_aux[0], sizeof(double), 3, inFile);
      
      /*----- Positions -----*/
      part[i].pos[X] = pos_aux[X];
      part[i].pos[Y] = pos_aux[Y];
      part[i].pos[Z] = pos_aux[Z];

      nread = fread(&part[i].mass, sizeof(double), 1, inFile);
      /*
      if(i%10000==0)
	{
	  printf("Reading i=%d x=%lf mass=%lf\n", i, part[i].pos[X], part[i].mass);
	}//if
      */
    }//for i

  fclose(inFile);
  return 0;
}//read_binary


/****************************************************************************************************           
NAME: write_binary
FUNCTION: Writes binary data file
INPUT: none                                                                
RETURN: 0                                                                                                       
****************************************************************************************************/

int write_binary(void)
{
  int i, nread;
  double pos_aux[3];
  FILE *outFile=NULL;
  outFile = fopen("./../Processed_data/CIC_DenCon_field.bin", "w");

  /*+++++ Saving Simulation parameters +++++*/
  fwrite(&GV.L, sizeof(double), 1, outFile);  //Box Size
  fwrite(&GV.OmegaM0, sizeof(double), 1, outFile);  //Matter density parameter
  fwrite(&GV.OmegaL0, sizeof(double), 1, outFile);  //Cosmological constant density parameter
  fwrite(&GV.zRS, sizeof(double), 1, outFile);  //Redshift
  fwrite(&GV.HubbleParam, sizeof(double), 1, outFile);  //Hubble parameter


  for(i=0; i<GV.NGRID3; i++ )
    { 
      fwrite(&i, sizeof(int), 1, outFile);
      fwrite(&cells[i].Np_cell, sizeof(int), 1, outFile);
      
      /*----- Positions -----*/
      pos_aux[X] = cells[i].pos[X];
      pos_aux[Y] = cells[i].pos[Y];
      pos_aux[Z] = cells[i].pos[Z];
      
      fwrite(&pos_aux[0], sizeof(double), 3, outFile);
      fwrite(&cells[i].denCon, sizeof(double), 1, outFile);
    }//for i

  fclose(outFile);  
  return 0;  
}//write_binary
