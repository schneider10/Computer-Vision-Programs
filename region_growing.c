/* The following is a program written in C that can perform a region growing algorithm on an image for segmentation purposes. 
author: Stephen Schneider */

#include "VisXV4.h"       	/* VisionX structure include file	*/
#include "Vutil.h"        	/* VisionX utility header files  	*/

VXparam_t par[] =         	/* command line structure        	*/
{ /* prefix, value,   description                     	*/   
{	"if=",	0,   " input file  vtemp: local max filter "},
{	"of=",	0,   " output file "},
{	"r=", 	0,   "region pixel range"},
{	"-p", 	0,   "if set, label value will be first pixel in region. Sequential otherwise"},
{ 	0,   	0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  RANGE  par[2].val
#define  first  par[3].val

void setlabel(int, int, int, int);

Vfstruct (im);                  	/* i/o image structure      	*/
Vfstruct (tm);                  	/* temp image structure     	*/


main(argc, argv)
int argc;
char *argv[];
{
int            	y,x,l,d,r,first_pix;                 	/* index counters           	*/
  VXparse(&argc, &argv, par);   	/* parse the command line   	*/
  Vfread(&im, IVAL);            	/* read image file          	*/
  Vfembed(&tm, &im, 1,1,1,1);   	/* image structure with border  */
 
  if ( im.type != VX_PBYTE ) {  	/* check image format       	*/
 	fprintf(stderr, "vtemp: no byte image data in input file\n");
 	exit(-1);
  }

  r = 10;    /* default r value */
  if (RANGE)  r = atoi(RANGE);  /* if r= was specified, get value */
  if (r < 0 || r > 255) {
    fprintf(stderr, "r= must be between 0 and 255\nUsing r=10\n");
    	r = 10;
 	}

  for (y = im.ylo ; y <= im.yhi ; y++) {
 	for (x = im.xlo; x <= im.xhi; x++)  {
       	im.u[y][x] = 0;               	/* Set the entire output image to zeros */
 	}
   }

   fprintf(stdout, "range is %d\n",r);
   fprintf(stdout, "first is %d\n",first);
   first_pix = 0;

   for (y = tm.ylo ; y <= tm.yhi ; y++) {
  	for (x = tm.xlo; x <= tm.xhi; x++)  {

  	/* If pixel is unlabeled and p label is set */
  	 
      if (first!=0 && tm.u[y][x] != 0 && im.u[y][x] == 0) {    	 
        	first_pix = tm.u[y][x];   
             	/* Sets global variable "first" to pixel value. */
     	fprintf(stdout, "x,y pixel coordinate is %d , %d\n", x,y);
     	fprintf(stdout, "pixel value is %d\n",first_pix);
     setlabel(x, y, first_pix,r);  
       	}
    	 
     	 
   	/*If pixel is unlabeled and p label is not set */


     	if (first == 0 && tm.u[y][x] != 0 && im.u[y][x] == 0) {  	 
    	first_pix++;                           	/* Sets global variable "first" to first number in sequence */
    	if (first_pix == 256) {                	/* If label "first" goes past 255, it is reset back to 1. */
       	first_pix = 1;        	 
    	}   	 
       	fprintf(stdout, "the label value is %d\n", first_pix);    
       	setlabel(x, y, first_pix,r);   
   	}
 	}
   }

   Vfwrite(&im, OVAL);         	/* write image file            	*/
   exit(0);
}

/* function to label unlabeled objects based on their grey level value */

void setlabel(int x, int y, int l, int r)
{
   im.u[y][x] = l;
   fprintf(stdout, "pixel is labeled as %d\n", im.u[y][x]);

if ( tm.u[y+1][x] != 0 && im.u[y+1][x] == 0 && abs((tm.u[y+1][x] - tm.u[y][x])) < r) {

   setlabel(x,y+1,l,r);
   /*fprintf(stdout, "pixel %d,%d is */
}

if ( tm.u[y-1][x] != 0 && im.u[y-1][x] == 0 && abs((tm.u[y-1][x] - tm.u[y][x])) < r) {

   setlabel(x,y-1,l,r);
}

if ( tm.u[y][x-1] != 0 && im.u[y][x-1] == 0 && abs((tm.u[y][x-1] - tm.u[y][x])) < r) {

   setlabel(x-1,y,l,r);
}

if ( tm.u[y][x+1] != 0 && im.u[y][x+1] == 0 && abs((tm.u[y][x+1] - tm.u[y][x])) < r) {

   setlabel(x+1,y,l,r);
}

}
