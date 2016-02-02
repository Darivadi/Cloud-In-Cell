#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
//#include <gsl/gsl_histogram.h>

#define CIC /* Preprocessor directive for the selection of the distribution
	       scheme in the routine windowFunction. 
	       Distribution schemes could be Nearest Grid Point (NGP), 
	       Cloud in Cell (CIC) and Triangular Shaped Cloud (TSC).
	    */

/* Index preprocessor for the C-Order */
#define INDEX(i,j,k) (k)+GV.NGRID*((j)+GV.NGRID*(i))

#include "structures.h"
#include "functions.h"
#include "readWrite.h"

int main(){
  
  int i, j, k, l, index, indexaux, Np, idPart;
  int ii, jj, kk;
  double xc, yc, zc; // Positions of the cells
  double xp, yp, zp, vxp, vyp, vzp; // Positions and velocities of the particles
  double Window_fn; //Window function
  double norm_factor; //Normalization factor for the momentum computation
  FILE *outfile, *outfile1, *outfile2;
  
  //--- For verifications ---
  double totalMass=0.0;
  int sumaPart = 0;
  double sumaMom[3], sumaVel[3];
  double sumaMomPart[3], sumaVelPart[3];
  double auxMom[3], auxVel[3];
  double maxDenCon = 0.0;

  char buffer[50];

  //////////////////////////////////
  //* READING GADGET BINARY FILE *//
  //////////////////////////////////
  read_parameters("./parameters_file.dat");
  GV.NpTot = readGADGETBinaryFile();

  /* Cosmological parameters */  
  GV.OmegaM0 = Header.Omega0;
  GV.OmegaL0 = Header.OmegaLambda;
  GV.zRS = Header.redshift;
  GV.HubbleParam = Header.HubbleParam;
  GV.aSF = 1.0/(1.0 + GV.zRS);

  /* Simulation parameters */
  GV.L = Header.BoxSize;
  GV.NGRID3 = GV.NGRID * GV.NGRID * GV.NGRID;
  GV.mass = Header.mass[1];
  GV.rhoMean = (GV.mass * GV.NpTot) / pow(GV.L,3); // Mean density in 1e10M_sun/Mpc^3
  GV.dx = GV.L / ((double) GV.NGRID);
  GV.volCell = GV.dx*GV.dx*GV.dx;

  printf("-----------------------------------------------\n");
  printf("Cosmological parameters:\n");
  printf("OmegaM0=%lf OmegaL0=%lf redshift=%lf HubbleParam=%lf\n",
	 GV.OmegaM0,
	 GV.OmegaL0,
	 GV.zRS,
	 GV.HubbleParam);
  printf("-----------------------------------------------\n");
  printf("Simulation parameters:\n");
  printf("NGRID=%d NGRID3=%d Particle_Mass=%lf NpTotal=%lf rhoMean=%lf L=%lf volCell=%lf dx=%lf Filename=%s\n",
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


  //////////////////////////////
  //* FROM PARTICLES TO GRID *//
  //////////////////////////////

  /* Array of structure Cell, size NGRID^3 */
  cells = (struct Cell *)calloc( GV.NGRID3, sizeof( struct Cell) );

  // Setting values to zero at the beggining
  for(i=0; i<GV.NGRID3; i++){
    cells[i].Np_cell = 0;
    cells[i].denCon = 0.0;
    cells[i].rho = 0.0;
    cells[i].velx = 0.0;
    cells[i].vely = 0.0;
    cells[i].velz = 0.0;
    cells[i].momentum_p[0] = 0.0;
    cells[i].momentum_p[1] = 0.0;
    cells[i].momentum_p[2] = 0.0;
  }//for i
  
  /* Locating cells */
  for(i=0; i<GV.NpTot; i++){
    locateCell(part[i].posx, part[i].posy, part[i].posz, i, cells);
  }

  printf("Particles located in the grid\n");
  printf("-----------------------------------------------\n");

  
  /* Verifying Momentum conservation of all particles */  
  sumaMomPart[0] = sumaMomPart[1] = sumaMomPart[2]= 0.0;
  sumaVelPart[0] = sumaVelPart[1] = sumaVelPart[2]= 0.0;
  for(i = 0; i<GV.NpTot ; i++)
    {
      sumaMomPart[0] += GV.mass*part[i].velx;
      sumaMomPart[1] += GV.mass*part[i].vely;
      sumaMomPart[2] += GV.mass*part[i].velz;
      sumaVelPart[0] += part[i].velx;
      sumaVelPart[1] += part[i].vely;
      sumaVelPart[2] += part[i].velz;
    }//for i

  sumaMomPart[0] /= GV.NpTot;
  sumaMomPart[1] /= GV.NpTot;
  sumaMomPart[2] /= GV.NpTot;
  sumaVelPart[0] /= GV.NpTot;
  sumaVelPart[1] /= GV.NpTot;
  sumaVelPart[2] /= GV.NpTot;

  printf("Total momentum from particles in x:%12.6lf\n", sumaMomPart[0]);
  printf("Total momentum from particles in y:%12.6lf\n", sumaMomPart[1]);
  printf("Total momentum from particles in z:%12.6lf\n", sumaMomPart[2]);
  printf("Mean velocity from particles in x:%12.6lf\n", sumaVelPart[0]);
  printf("Mean velocity from particles in y:%12.6lf\n", sumaVelPart[1]);
  printf("Mean velocity from particles in z:%12.6lf\n", sumaVelPart[2]);

  
  /* Distribution scheme  */
  for(i=0; i<GV.NGRID; i++){
    for(j=0; j<GV.NGRID; j++){
      for(k=0; k<GV.NGRID; k++){
	
	/* Index of the cell  */
	index = INDEX(i,j,k); // C-order
	
	/* coordinates in the center of the cell */
	xc = GV.dx*(0.5 + i);
	yc = GV.dx*(0.5 + j);
	zc = GV.dx*(0.5 + k);
	
	/* Number of particles in the cell */
	Np = cells[index].Np_cell;
	
	/* //This works only in NGP
	if(Np == 0){
	  cells[index].velx = 0.0;
	  cells[index].vely = 0.0;
	  cells[index].velz = 0.0;
	  continue;  
	}
	*/
	
	for(l=0; l<Np; l++){
	  /* Particle ID */
	  idPart = cells[index].id_part[l];
	  
	  /* Coordinates of the particle  */
	  xp = part[idPart].posx;
	  yp = part[idPart].posy;
	  zp = part[idPart].posz;
	  
	  /* Velocities of the particle  */
	  vxp = part[idPart].velx;
	  vyp = part[idPart].vely;
	  vzp = part[idPart].velz;
	  
	  /* 
	     Cell velocity assignment:
	     At the end of the for at l-index we are going 
	     to toke the ratio with respect to Np, in order 
	     to get the mean of the volocities in each cell.
	   */

	  //--- NGP velocity Assignment ---
	  cells[index].velx += vxp;
	  cells[index].vely += vyp;
	  cells[index].velz += vzp;

	  
	  //--- Mass and momentum assignment to neighbour cells (CIC) ---
	  for(ii=-1; ii<=1; ii++){
	    for(jj=-1; jj<=1; jj++){
	      for(kk=-1; kk<=1; kk++){
		
		indexaux = INDEX(mod(i+ii,GV.NGRID),mod(j+jj,GV.NGRID),mod(k+kk,GV.NGRID));
		xc = GV.dx*(0.5 + i+ii);
		yc = GV.dx*(0.5 + j+jj);
		zc = GV.dx*(0.5 + k+kk);

		//--- Mass with CIC assignment scheme ---
		Window_fn = W(xc-xp, yc-yp, zc-zp, GV.dx);
		cells[indexaux].rho += GV.mass * Window_fn;

		//--- Momentum and velocity with CIC assignment scheme ---
		/*
		cells[indexaux].momentum_p[0] += GV.mass * vxp * Window_fn;
		cells[indexaux].momentum_p[1] += GV.mass * vyp * Window_fn;
		cells[indexaux].momentum_p[2] += GV.mass * vzp * Window_fn;
		cells[index].velx += vxp * Window_fn;
		cells[index].vely += vyp * Window_fn;
		cells[index].velz += vzp * Window_fn;
		*/
	      }//for kk
	    }//for jj
	  }//for ii
	   	  
	}//for l
	
	/* 
	   Cell velocity assignment:
	   At the end of the for at l-index we are going 
	   to toke the ratio with respect to Np, in order 
	   to get the mean of the volocities in each cell.
	*/
	cells[index].velx = cells[index].velx / ((double) Np);
	cells[index].vely = cells[index].vely / ((double) Np);
	cells[index].velz = cells[index].velz / ((double) Np);
	
      }//for k
    }//for j
  }//for i

  
  //--- Normalization of momentum ---
  /*
  //norm_factor = (GV.NGRID3 / GV.NpTot); //Normalization for NGP
  norm_factor = GV.aSF * (GV.NGRID3 / GV.NpTot); //Normalization for CIC
  for(i=0; i<GV.NGRID; i++){
    for(j=0; j<GV.NGRID; j++){
      for(k=0; k<GV.NGRID; k++){
	// Index of the cell
	index = INDEX(i,j,k); // C-order

	//NGP Momentum normalization
	//cells[index].momentum_p[0] = norm_factor * cells[index].velx;
	//cells[index].momentum_p[1] = norm_factor * cells[index].vely;
	//cells[index].momentum_p[2] = norm_factor * cells[index].velz;

	//CIC Momentum normalization
	cells[index].momentum_p[0] = norm_factor * cells[index].momentum_p[0];
	cells[index].momentum_p[1] = norm_factor * cells[index].momentum_p[1];
	cells[index].momentum_p[2] = norm_factor * cells[index].momentum_p[2];
      }//for k
    }//for j
  }//for i
  printf("Normalization for momentum: aSF*Ngrid^3/NTotalParts=%lf\n", norm_factor);
  */

  //--- Saving output file ---  
  outfile = fopen(strcat(GV.FILENAME,"_densNGP.dat"),"w");
  fprintf(outfile,
	  "%s%9s %12s %12s %12s %12s %12s %12s %12s %12s %12s\n", 
	  "#", "Index", "NumberOfPars",
	  "x", "y", "z",
	  "px", "py", "pz",
	  "rho", "DenCon");

  //outfile1 = fopen("DenCon_vs_z.dat", "w");
  
  sumaMom[0] = sumaMom[1] = sumaMom[2] = 0.0;
  sumaVel[0] = sumaVel[1] = sumaVel[2] = 0.0;
  
  for(i=0; i<GV.NGRID; i++){
    for(j=0; j<GV.NGRID; j++){


      maxDenCon = 0.0;
      
      for(k=0; k<GV.NGRID; k++){
	
	index = INDEX(i,j,k); // C-order
	
	/* coordinates of the cell (in the center of) */
	xc = GV.dx * (0.5 + i);
	yc = GV.dx * (0.5 + j);
	zc = GV.dx * (0.5 + k);
	
	/* Calculating the final density in the cell */
	totalMass += cells[index].rho; /* We have not divided by the volume yet. 
					  This is still the mass */
	cells[index].rho = cells[index].rho / GV.volCell; //This is the actual density

	/* Verification of number of particles and momentum conservation */
	sumaPart += cells[index].Np_cell;
	sumaVel[0] += cells[index].velx;
	sumaVel[1] += cells[index].vely;
	sumaVel[2] += cells[index].velz;
	sumaMom[0] += cells[index].momentum_p[0];
	sumaMom[1] += cells[index].momentum_p[1];
	sumaMom[2] += cells[index].momentum_p[2];     
      
	
	/* Calculating the final density contrast in the cell */
	cells[index].denCon = (cells[index].rho/GV.rhoMean) - 1.0;

	/* Computing the maximum density along z-axis */
	if(cells[index].denCon > maxDenCon )
	  {
	    maxDenCon = cells[index].denCon;
	  }//if
	
	
	/* Writting in the file  */
	
	fprintf(outfile,
		"%10d %10d  %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf\n", 
		index, cells[index].Np_cell,
		xc, yc, zc, 
		cells[index].velx, cells[index].vely, cells[index].velz, 
		cells[index].rho, cells[index].denCon);
	
	/*
	fprintf(outfile,
		"%10d %10d  %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf\n", 
		index, cells[index].Np_cell,
		xc, yc, zc, 
		cells[index].momentum_p[0], cells[index].momentum_p[1], cells[index].momentum_p[2], 
		cells[index].rho, cells[index].denCon);
	*/
      }//for k


      /*  
      if( (i%32==0) && (j%32==0) )
	{
	  snprintf(buffer, sizeof(char)*50, "DenCon_VS_k_i%d_j%d.txt", i, j);
	  
	  outfile2 = fopen(buffer, "w");
	  
	  for(k=-GV.NGRID; k<GV.NGRID; k++)
	    {
	      if(k<0)
		l = k+GV.NGRID;
	      else
		l = k;
	      
	      index = INDEX(i,j,l); // C-order
	      
	      fprintf(outfile2,
		      "%10d %12.6lf\n", 
		      k, cells[index].denCon/(maxDenCon));
	      
	    }//for k
	  fclose(outfile2);
	  
	}//if
      */
    }// for j
  }// for i
  
  fclose(outfile);
  fclose(outfile1);

  
  printf("Total number of particles:%10d\n", sumaPart);
  printf("Mass CIC = %lf\n",totalMass);
  printf("Mass Simulation = %lf\n",GV.NpTot*GV.mass);
  printf("Total momentum from cells in x:%16.8lf\n", sumaMom[0]);
  printf("Total momentum from cells in y:%16.8lf\n", sumaMom[1]);
  printf("Total momentum from cells in z:%16.8lf\n", sumaMom[2]);
  printf("Mean velocity from cells in x:%16.8lf\n", sumaVel[0]);
  printf("Mean velocity from cells in y:%16.8lf\n", sumaVel[1]);
  printf("Mean velocity from cells in z:%16.8lf\n", sumaVel[2]);
  
  /* Freeing up memory allocation */
  free(part);
  free(cells);
  
  return 0;
}//main
