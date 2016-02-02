#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/****************************************************************************************************
                            SUPPORT FILES
****************************************************************************************************/
#include "structures.h"
#include "functions.h"
#include "readWrite.h"

int main(int argc, char *argv[])
{  
  int i, j, k, l, index, indexaux, Np, idPart;
  int ii, jj, kk;
  double xc, yc, zc; // Positions of the cells
  double xp, yp, zp, vxp, vyp, vzp; // Positions and velocities of the particles
  double Window_fn; //Window function
  double norm_factor; //Normalization factor for the momentum computation
  char *inFile=NULL;
  FILE *outfile=NULL, *outfile1=NULL, *outfile2=NULL;
  /*----- For verifications -----*/
  double totalMass=0.0;
  double totMassCIC=0.0;
  int sumaPart = 0;

  
  if(argc < 2)
    {
      printf("Error: Incomplete number of parameters. Execute as follows:\n");
      printf("%s Parameters_file\n", argv[0]);
      exit(0);
    }//if 

  inFile = argv[1];


  /*************************************************
             READING DATA FILE
  *************************************************/

  /*+++++  Reading parameters +++++*/
  printf("Reading parameters file\n");
  printf("--------------------------------------------------\n\n");
  read_parameters( inFile );

  printf("Parameters file read!\n");
  printf("--------------------------------------------------\n\n");

  printf("Reading data file\n");
  printf("--------------------------------------------------\n\n");

  
#ifdef BINARYDATA  
  /*+++++ Reading binary data file +++++*/
  read_binary();

  GV.NGRID3 = GV.NGRID * GV.NGRID * GV.NGRID;
  GV.dx = GV.L / (1.0*GV.NGRID);
  GV.volCell = GV.dx*GV.dx*GV.dx;
#endif

  
#ifdef ASCIIDATA 
  /*+++++ Allocating memory +++++*/
  part = (struct particle *) calloc((size_t) GV.NpTot,sizeof(struct particle));
  
  /*+++++ Reading data file +++++*/
  read_ascii( GV.FILENAME );

  /*+++++ Simulation parameters +++++*/  
  GV.NGRID3 = GV.NGRID * GV.NGRID * GV.NGRID;
  GV.dx = GV.L / ((double) GV.NGRID);
  GV.volCell = GV.dx*GV.dx*GV.dx;
#endif

  printf("Data file read!\n");
  printf("--------------------------------------------------\n\n");


  /*+++++ Computing mean density +++++*/
  printf("Computing mean density\n");
  printf("--------------------------------------------------\n\n");
  for(i=0; i<GV.NpTot; i++)
    {
      totalMass += part[i].mass;
    }//for i
  GV.rhoMean = totalMass / pow(GV.L, 3.0); // Mean density in 1e10M_sun/Mpc^3


  printf("-----------------------------------------------\n");
  printf("Cosmological parameters:\n");
  printf("OmegaM0=%lf OmegaL0=%lf redshift=%lf HubbleParam=%lf\n",
	 GV.OmegaM0,
	 GV.OmegaL0,
	 GV.zRS,
	 GV.HubbleParam);
  printf("-----------------------------------------------\n");
  printf("Simulation parameters:\n");
  printf("NGRID=%d NGRID3=%d Particle_Mass=%lf NpTotal=%ld \nrhoMean=%lf L=%lf volCell=%lf dx=%lf \nFilename=%s\n",
	 GV.NGRID,
	 GV.NGRID3,
	 GV.mass,
	 GV.NpTot,
	 GV.rhoMean,
	 GV.L,
	 GV.volCell,
	 GV.dx,
	 GV.FILENAME);
  printf("-----------------------------------------------\n");


  /*************************************************
                FROM PARTICLES TO GRID
  *************************************************/
  
  /*+++++ Array of structure Cell, size NGRID^3 +++++*/
  printf("Allocating memory\n");
  printf("-----------------------------------------------\n");
  cells = (struct Cell *) calloc( GV.NGRID3, sizeof( struct Cell) );
  printf("Memory allocated\n");
  printf("-----------------------------------------------\n");

  /*----- Setting values to zero at the beggining -----*/
  for(i=0; i<GV.NGRID3; i++){
    cells[i].Np_cell = 0;
    cells[i].denCon = 0.0;
    cells[i].rho = 0.0;
  }//for i
  
  printf("Locating cells\n");
  printf("-----------------------------------------------\n");

  /*++++ Locating cells +++++*/
  for(i=0; i<GV.NpTot; i++)    
    {
      locateCell(part[i].pos[X], part[i].pos[Y], part[i].pos[Z], i, cells);
    }//for i
  
  printf("Particles located in the grid\n");
  printf("-----------------------------------------------\n");


  printf("Performing the mass assignment\n");
  printf("-----------------------------------------------\n");
  
  
  /*+++++ Distribution scheme +++++*/
  for(i=0; i<GV.NGRID; i++)
    {
      for(j=0; j<GV.NGRID; j++)
	{
	  for(k=0; k<GV.NGRID; k++)
	    {
	      
	      /*----- Index of the cell  -----*/
	      index = INDEX(i,j,k); // C-order
	      
	      /*----- Coordinates in the center of the cell -----*/
	      xc = GV.dx*(0.5 + i);
	      yc = GV.dx*(0.5 + j);
	      zc = GV.dx*(0.5 + k);
	
	      /*----- Number of particles in the cell -----*/
	      Np = cells[index].Np_cell;
	      
	      for(l=0; l<Np; l++)
		{
		  /*::::: Particle ID :::::*/
		  idPart = cells[index].id_part[l];
		  
		  /*::::: Coordinates of the particle  :::::*/
		  xp = part[idPart].pos[X];
		  yp = part[idPart].pos[Y];
		  zp = part[idPart].pos[Z];
		  
	  
		  /*::::: Mass and momentum assignment to neighbour cells (CIC) :::::*/
		  for(ii=-1; ii<=1; ii++)
		    {
		      for(jj=-1; jj<=1; jj++)
			{
			  for(kk=-1; kk<=1; kk++)
			    {
		
			      indexaux = INDEX(mod(i+ii,GV.NGRID),mod(j+jj,GV.NGRID),mod(k+kk,GV.NGRID));
			      xc = GV.dx*(0.5 + i+ii);
			      yc = GV.dx*(0.5 + j+jj);
			      zc = GV.dx*(0.5 + k+kk);
			      
			      /*----- Mass with CIC assignment scheme ------*/
			      Window_fn = W(xc-xp, yc-yp, zc-zp, GV.dx);
			      cells[indexaux].rho += part[idPart].mass * Window_fn;
			    }//for kk
			}//for jj
		    }//for ii
	   	  
		}//for l	      	      
	      
	    }//for k
	}//for j
    }//for i
  
  free(part);
  
  /*+++++ Saving output file in ASCII format +++++*/
#ifdef ASCIIDATA
  outfile = fopen(strcat(GV.FILENAME,"_DenCon_CIC.dat"),"w");
  fprintf(outfile, "%s%9s %12s %12s %12s %12s %12s\n", 
	  "#", "Index", "NumberOfPars",
	  "x", "y", "z", "DenCon");
  
  for(i=0; i<GV.NGRID; i++)
    {
      for(j=0; j<GV.NGRID; j++)
	{
	  for(k=0; k<GV.NGRID; k++)
	    {
	
	      index = INDEX(i,j,k); // C-order
	      
	      /*----- coordinates of the centre of the cell -----*/
	      xc = GV.dx * (0.5 + i);
	      yc = GV.dx * (0.5 + j);
	      zc = GV.dx * (0.5 + k);
	
	      /*----- Calculating the final density in the cell -----*/
	      totMassCIC += cells[index].rho; /* We have not divided by the volume yet. 
						This is still the mass */
	      cells[index].rho = cells[index].rho / GV.volCell; //This is the actual density
	      
	      /*----- Verification of number of particles -----*/
	      sumaPart += cells[index].Np_cell;
	      
	      
	      /*----- Calculating the final density contrast in the cell -----*/
	      cells[index].denCon = (cells[index].rho/GV.rhoMean) - 1.0;
		
	      fprintf(outfile,
		      "%10d %10d %12.6lf %12.6lf %12.6lf %12.6lf\n", 
		      index, cells[index].Np_cell,
		      xc, yc, zc, cells[index].denCon);


	    }//for k
	}// for j
    }// for i

  fclose(outfile);
#endif


  /*+++++ Writing binary file +++++*/
#ifdef BINARYDATA
  for(i=0; i<GV.NGRID; i++)
    {
      for(j=0; j<GV.NGRID; j++)
	{
	  for(k=0; k<GV.NGRID; k++)
	    {
	      
	      index = INDEX(i,j,k); // C-order
	      
	      /*----- coordinates of the centre of the cell -----*/
	      cells[index].pos[X] = GV.dx * (0.5 + i);
	      cells[index].pos[Y] = GV.dx * (0.5 + j);
	      cells[index].pos[Z] = GV.dx * (0.5 + k);
	      
	      /*----- Calculating the final density in the cell -----*/
	      totMassCIC += cells[index].rho; /* We have not divided by the volume yet. 
						 This is still the mass */

	      cells[index].rho = cells[index].rho / GV.volCell; //This is the actual density
	      

	      /*----- Verification of number of particles -----*/
	      sumaPart += cells[index].Np_cell;
	      
	      
	      /*----- Calculating the final density contrast in the cell -----*/
	      cells[index].denCon = (cells[index].rho/GV.rhoMean) - 1.0;	      	      
	    }//for k
	}// for j
    }// for i

  write_binary();
#endif
  
    
  printf("Total number of particles:%10d\n", sumaPart);
  printf("Mass CIC = %lf\n",totMassCIC);
  printf("Mass Simulation = %lf\n", totalMass);
  
  /*+++++ Freeing up memory allocation +++++*/ 
  free(cells);
  
  printf("Code has finished successfully\n");

  return 0;
}//main
