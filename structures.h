////////////////////////////////////////////////////
// HEADER WITH ALL DATA SRUCTURES FOR THE PROGRAM //
////////////////////////////////////////////////////

/****** Global variales ******/
struct globalVariables
{
  int NGRID;      // Number of cell in one axis.
  double L;       // Lenght of the simulation in Mpc.
  long int NpTot;   // Total number of particles
  int NGRID3;     // Total number of cells
  double rhoMean; // Mean density of ALL the simulation
  double mass;    // Mass of each particle
  double volCell; // Volume of each cell
  double dx;      // Also known as H : Size of the cell
  char FILENAME[1000]; //Path of the gadget file

  /* COSMOLOGICAL PARAMETERS*/
  double OmegaM0;     //Omega matter at present time
  double OmegaL0;     //Omega Lambda at present time
  double zRS;         //Redshift of the simulation
  double aSF;         //Scale factor of the simulation
  double HubbleParam; //Hubble parameter of the simulation

}GV;


/****** Structure particle ******/
struct particle
{
  unsigned int id;
  double pos[3]; // Position
  double mass; // Mass
  int tag; // Tag indicating if its simulated (0) or observational (1)
}*part;


/****** Structure gadget_head ******/
struct gadget_head
{
  int      Npart[6];
  double   mass[6];
  double   time;
  double   redshift;
  int      flag_sfr;
  int      flag_feedback;
  int      npartTotal[6];
  int      flag_cooling;
  int      num_files;
  double   BoxSize;
  double   Omega0;
  double   OmegaLambda;
  double   HubbleParam; 
  char     fill[256- 6*4- 6*8- 2*8- 2*4- 6*4- 2*4 - 4*8];  /* fills to 
							      256 Bytes */
}Header;

struct Cell
{
  int       Np_cell; // Number of particles in the Cell.
  long int* id_part; // Array with the ID of the particles inside the cell.
  double     denCon; // Density contrast of the cell
  double        rho; // Density in the cell
  double     pos[3]; // Position
}*cells;


/****************************************************************************************************           
                       PRE-PROCESSOR DEFINITIONS                                                                
****************************************************************************************************/
#define X 0
#define Y 1
#define Z 2
#define INDEX(i,j,k) (k)+GV.NGRID*((j)+GV.NGRID*(i)) // Index preprocessor for the C-Order
