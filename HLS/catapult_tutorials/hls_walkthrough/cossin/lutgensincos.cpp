#include<iostream> 
#include<cstring>
#include<cstdlib>
#include<stdio.h>
#include<math.h>

//==========================================================================================
// Note: 
//      This file is used to generate a lookup table for the ac_sincos_lut.h library header.
//      In this file, a lookup table has been generated for an input width of 12 bits and 
//      input integer width of 0 bits. So for example, if a lookup table has to be generated 
//      for an input width of 14 bits and input integer width of 2 bits, then the variables 
//      input_width and input_int have to be replaced accordingly.
//------------------------------------------------------------------------------------------

int main()
{

	FILE *f = fopen("file.txt", "w");
	if (f == NULL)
	{
		  printf("Error opening file!\n");
		  exit(1);
	}

  const int input_width = 12;
  const int input_int = 0;

  unsigned int NTE = 1<<(input_width - input_int -3); //No of table entries
  //double sine[NTE];
  //double cosine[NTE];
  double step = M_PI/(4*NTE);                         //Interval between angles
  double y = 0;
  double scaled_angle = 0;

  fprintf(f, "static const luttype sincos[%d] = { \n", NTE);

  for (unsigned int i=0; i < NTE; i++) 
  {
	  fprintf(f, "  {%23.22f, %23.22f}, //index = %d, scaled angle = %13.12f \n", cos(y), sin(y), i, scaled_angle);
    y += step;
    scaled_angle = y/(2*M_PI);
  }

  fclose(f);

	return 0;

}
