/*
This C++ program was written to detect motion in video frames.
author: Stephen Schneider */

/*********************************************************************/
/* vsdif   Compute thresholded image sequence to detect image change over time (using buffer method)	*/
/*********************************************************************/

#include "VisXV4.h"      	/* VisionX structure include file   	*/
#include "Vutil.h"       	/* VisionX utility header files     	*/

VXparam_t par[] =        	/* command line structure           	*/
{
{	"if=",	0,   " input file vssum: compute temporal mean"},
{	"of=",	0,   " output file "},
{	"n=", 	0,   " number of frames "},
{	"th=",	0,   " difference threshold"},
{ 	0,   	0,	0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  NVAL   par[2].val
#define  TH 	par[3].val

int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);
int    	x,y,z;       	/* index counters             	*/
int    	n;           	/* Number of frames to average	*/
int    	sum;
int    	thresh;    	 /* inputted threshold value */
int    	diff;             	/* difference between consecutive image frames */

	VXparse(&argc, &argv, par); /* parse the command line	*/

	n = (NVAL ? atoi(NVAL) : 1); /* read n, default is n=1   */

if (TH !=0){
   	 thresh = atoi(TH); 	/*set thresh to parameter TH as an integer */
   	 }
else{
    thresh = 60;   /* or set thresh to a default threshold   */
}

fprintf (stdout, "Threshold is %d\n", thresh); /* Prints threshold */

	while (Vbfread( &im, IVAL, n)) {
    		if ( im.type != VX_PBYTE || im.chan != 1) { /* check format  */
       			fprintf (stderr, "image not byte type\n");
       			exit (1);
    		 }
    		for (y = im.ylo; y <= im.yhi; y++) {
       			for (x = im.xlo; x <= im.xhi; x++) {
          				for (z = (im.zlo+1); z <= im.zhi; z++) {
               				diff = 0;   			 /* diff variable reset to zero after each loop */
               				diff = abs(im.u[z][y][x] - im.u[z-1][y][x]); /*absolute value of pixel difference over time */

     					if (diff > thresh) {
   	   					im.u[0][y][x] = 255; /* if diff exceeds the threshold, label the pixels as value 255 */
   	   				}
   					else{
		   				im.u[0][y][x] = 128; /*else label the pixels as value 128. */
   	   				}   	 
    				}
}
}

    	V3fwrite (&im, OVAL); /* write the oldest frame */
	}
	exit(0);
}
