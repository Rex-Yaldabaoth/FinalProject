#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<gsl/gsl_math.h>
#include<gsl/gsl_odeiv2.h>
#include<gsl/gsl_errno.h>
#include<gsl/gsl_matrix.h>
#include<omp.h>

#define g 980.665
#define pi 3.141592653589793


typedef struct run_parameters{ //Struct containing the properties of an individual planet

    double mass;
    double v_0; //Initial velocity: cgs units ~ 0.1 cm/s
    double k; //Surface tension: cgs units ~ 475 dynes/cm
    double b; //friction: cgs units ~ 1 g/s
    double x_c; //Critical length: cgs units ~ 0.19cm/s
    double alpha;
    double R;
    double tf; //final time (s)
    double x; //current position, can be read in as initial position

} Run_Parameters;

void read_file(Run_Parameters * parameters, int drip_mode);
void solver(void* params[], FILE* file_out, int drip_mode);
void henon(void* params[], double* x, double* v, double* M, double* t, short int dripped, FILE* file_out, double* mass_gain, int drip_mode, double* mass_loss, double* t_prev_drip);
void write_file(double t, double x, double v, double M, short int dripped, FILE* file_out, double Merr);

