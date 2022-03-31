#include "Header.h"

/* The things done by this function are done as in M.Henon 1982, cited by the paper I am working from.
* THe point of it is to integrate back to the precise point at which the solver exceeded the critical value, to avoid any errors that might occur from overshooting the critical value
*/
int henonfunc (double x, const double y[], double f[], void* params[]){//new functions for this one step

    double R = ((double *)params)[6];
    double k = ((double *)params)[2];
    double b = ((double *)params)[3];
    f[0] = 1.0 / y[2];
    f[1] = R / y[2];
    f[2] = g / y[2] - (k*x) / (y[2] * y[1]) - (b + R) / y[1] ;
    return GSL_SUCCESS;

}

int henonjac (double x, const double y[], double *dfdy, double dfdx[], void* params[]){//new jacobian for this one step

    double R = ((double *)params)[6];
    double k = ((double *)params)[2];
    double b = ((double *)params)[3];
    gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, 3, 3);
    gsl_matrix * m = &dfdy_mat.matrix;
    gsl_matrix_set (m, 0, 0, 0.0);
    gsl_matrix_set (m, 0, 1, -1.0 / (y[2] * y[2]));
    gsl_matrix_set (m, 0, 2, 0.0);
    gsl_matrix_set (m, 1, 0, 0.0);
    gsl_matrix_set (m, 1, 1, -1.0*(R+b) / (y[2] * y[2]));
    gsl_matrix_set (m, 1, 2, 0.0);
    gsl_matrix_set (m, 2, 0, 0.0);
    gsl_matrix_set (m, 2, 1, (k * x / y[1] - g)/ (y[2] * y[2]));
    gsl_matrix_set (m, 2, 2, ((k * x) / y[2] + b + R) / (y[1] * y[1]));
    dfdx[0] = 0.0;
    dfdx[1] = -k / (y[2] * (y[1]));
    dfdx[2] = 0.0;
    return GSL_SUCCESS;

}

void henon(void* params[], double* x, double* v, double* M, double* t, short int dripped, FILE* file_out, double* mass_gain, int drip_mode, double* mass_loss, double* t_prev_drip){

    double y[] = {*t, *M, *v}; //Set y to be the parameters passed in from the main solver
    const double h = ((double *)params)[4] - *x; //the difference in position between the current position, and the critical length


    gsl_odeiv2_system sys = {henonfunc, henonjac, 3, params};//New henon system

    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-7, 1e-6, 0.0);//new driver for the henon system

    int status = gsl_odeiv2_driver_apply_fixed_step(d, x, h/100.0, 100, y);//Breaks the interval of size h into 100 steps, and then integrates through them

        if(status != GSL_SUCCESS){

            printf("error, %s\n", gsl_strerror(status));

        }

    gsl_odeiv2_driver_free (d);//Free the driver

    *t = y[0]; //Extract the values of the exact drip point to put back into the solver
    *M = y[1];
    *v = y[2];

    double dM = 0;

    if(drip_mode == 1){
        dM = ((double *)params)[5] * y[1] * y[2]; //Change in mass caused by the drip
    }
    
    if(drip_mode == 2){
        dM = ((double *)params)[5] * y[2]; 
    }

    double x_new = *x - pow((3 * dM)/(4 * pi), (1.0/3.0)) * (dM/y[1]); //Change in position caused by the drip

    *mass_gain += ((*t - *t_prev_drip) * ((double *)params)[6]); //The difference between the accumulated mass up to this time, and the mass removed the drip will be the error in mass
    *mass_loss += dM;
    double Merr_percent = ((*mass_gain - *mass_loss) / *mass_gain) * 100;

    write_file(*t, *x, *v, *M, dripped, file_out, Merr_percent);//write the data for the exact drip point

    *x = x_new; //Set the mass and position to the new, post drip positions
    *M = y[1] - dM;

    


}