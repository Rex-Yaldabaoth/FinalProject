#include "Header.h"

//This is made using m,odified example code from GSL's documentation, in particular, the page about solving ODEs
int
func(double t,  const double y[], double f[], void* params[]){

    (void)(t); /* avoid unused parameter warning */
    double R = ((double *)params)[6];
    double k = ((double *)params)[2];
    double b = ((double *)params)[3];
    f[0] = y[2];
    f[1] = R;
    f[2] = g - ((k * y[0]) / y[1]) - ((y[2] * (R + b))/(y[1]));
    return GSL_SUCCESS;

}

int
jac(double t, const double y[], double *dfdy, double dfdt[], void* params[]){

    (void)(t); /* avoid unused parameter warning */
    double R = ((double *)params)[6];
    double k = ((double *)params)[2];
    double b = ((double *)params)[3];
    gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, 3, 3);
    gsl_matrix * m = &dfdy_mat.matrix;
    gsl_matrix_set (m, 0, 0, 0.0);
    gsl_matrix_set (m, 0, 1, 1.0);
    gsl_matrix_set (m, 0, 2, 0.0);
    gsl_matrix_set (m, 1, 0, 0.0);
    gsl_matrix_set (m, 1, 1, 0.0);
    gsl_matrix_set (m, 1, 2, 0.0);
    gsl_matrix_set (m, 2, 0, -k / (y[1]));
    gsl_matrix_set (m, 2, 1, -(R + b) / (y[1]));
    gsl_matrix_set (m, 2, 2, (k * y[0]) / (y[1] * y[1]) + (y[2] * (R + b) / (y[1] * y[1])));
    dfdt[0] = 0.0;
    dfdt[1] = 0.0;
    dfdt[2] = 0.0;
    return GSL_SUCCESS;

}

void solver(void* params[], FILE* file_out){

    double y[] = {0.0, ((double *)params)[0], 0.0};
    double t = 0;
    double dt = 5e-3; //Timestep
    double ti = 0; //New time
    double tmax = 50;
    int iteration = 0;
    int iteration_max = 1000000;
    short int dripped = 0;

    write_file(t, y[0], y[2], y[1], dripped, file_out);

    gsl_odeiv2_system sys = {func, jac, 3, params};
    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-6, 1e-6, 0.0);

    while(t < tmax){
        
        iteration++;
        if (iteration > iteration_max){

            printf("Max iterations exceeded, stopping...\n");
            break;

        }

        ti = t + dt; //Next time is current time + timestep

        int status = gsl_odeiv2_driver_apply(d, &t, ti, y);

        if(status != GSL_SUCCESS){

            printf("error, return value=%d\n", status);
            break;

        }

        t = ti; //Current time has now advanced to ti

        if( iteration % 100 == 0){

            write_file(t, y[0], y[2], y[1], dripped, file_out);

        }

        if (y[0] > ((double *)params)[4]){

            dripped = 1;

            gsl_odeiv2_driver_free (d);

            double x = y[0];
            double M = y[1];
            double v = y[2];

            henon(params, &x, &v, &M, &t, dripped, file_out);
            d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-6, 1e-6, 0.0);

            y[0] = x;
            y[1] = M;
            y[2] = v;

            dripped = 0;

        }

    }

    gsl_odeiv2_driver_free (d);

}