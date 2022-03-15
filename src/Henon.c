#include "Header.h"

int
henonfunc (double x, const double y[], double f[], void* params[]){

    double R = ((double *)params)[6];
    double k = ((double *)params)[2];
    double b = ((double *)params)[3];
    f[0] = 1.0 / y[2];
    f[1] = R / y[2];
    f[2] = g / y[2] - (k*x) / (y[2] * y[1]) - (b + R) / y[1] ;
    return GSL_SUCCESS;

}

int
henonjac (double x, const double y[], double *dfdy, double dfdx[], void* params[]){

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

void henon(void* params[], double* x, double* v, double* M, double* t, short int dripped, FILE* file_out){

    double y[] = {*t, *M, *v};
    const double h = ((double *)params)[4] - *x;

    gsl_odeiv2_system sys = {henonfunc, henonjac, 3, params};

    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-7, 1e-6, 0.0);

    int status = gsl_odeiv2_driver_apply_fixed_step(d, x, h/100.0, 100, y);

       if(status != GSL_SUCCESS){

        printf("error, %s\n", gsl_strerror(status));

        }

    gsl_odeiv2_driver_free (d);

    *t = y[0];
    *M = y[1];
    *v = y[2];

    write_file(*t, *x, *v, *M, dripped, file_out);

    double dM = ((double *)params)[5] * y[1] * y[2];
    double x_new = *x - pow((3 * dM)/(4 * pi), (1.0/3.0)) * (dM/y[1]);

    *x = x_new;
    *M = y[1] - dM;


}