#include "Header.h"

//This is made using modified example code from GSL's documentation, in particular, the page about solving ODEs. The URL is: https://www.gnu.org/software/gsl/doc/html/ode-initval.html
int func(double t,  const double y[], double f[], void* params[]){ //function containing the mathematical function we wish to simulate

    //In all cases in this file, y[0] is position x, y[1] is mass M, and y[2] is velocity v

    (void)(t); /* avoid unused parameter warning */
    double R = ((double *)params)[6];
    double k = ((double *)params)[2];
    double b = ((double *)params)[3];
    f[0] = y[2];
    f[1] = R;
    f[2] = g - ((k * y[0]) / y[1]) - ((y[2] * (R + b))/(y[1]));
    return GSL_SUCCESS;

}

int jac(double t, const double y[], double *dfdy, double dfdt[], void* params[]){//Function containing the jacobian matrix of the system being simulated

    (void)(t); /* avoid unused parameter warning */
    double R = ((double *)params)[6];
    double k = ((double *)params)[2];
    double b = ((double *)params)[3];
    gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, 3, 3);
    gsl_matrix * m = &dfdy_mat.matrix;
    gsl_matrix_set (m, 0, 0, 0.0); //These set the elements of the jacobian matrix
    gsl_matrix_set (m, 0, 1, 1.0);
    gsl_matrix_set (m, 0, 2, 0.0);
    gsl_matrix_set (m, 1, 0, 0.0);
    gsl_matrix_set (m, 1, 1, 0.0);
    gsl_matrix_set (m, 1, 2, 0.0);
    gsl_matrix_set (m, 2, 0, -k / (y[1]));
    gsl_matrix_set (m, 2, 1, -(R + b) / (y[1]));
    gsl_matrix_set (m, 2, 2, (k * y[0]) / (y[1] * y[1]) + (y[2] * (R + b) / (y[1] * y[1])));
    dfdt[0] = 0.0; //Derivates of the functions with respect to time
    dfdt[1] = 0.0;
    dfdt[2] = 0.0;
    return GSL_SUCCESS;

}

void solver(void* params[], FILE* file_out, int drip_mode){

    double y[] = {0.0, ((double *)params)[0], 0.0};
    double t = 0;
    double t_prev_drip = 0;
    double dt = 5e-3; //Timestep
    double ti = 0; //New time
    double tmax = 50; //total run time
    int iteration = 0;
    int iteration_max = 1000000; //maximum number of iterations, as a safeguard
    short int dripped = 0; //Will be either 1 or 0, used to check if the tap has dripped
    double mass_gain = 0; //Mass gained due to flow rate R
    double mass_loss = 0; //Mass lost due to dripping

    write_file(t, y[0], y[2], y[1], dripped, file_out, 0);

    gsl_odeiv2_system sys = {func, jac, 3, params}; //Define the system for GSL
    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-6, 1e-6, 0.0); //Define the type of solver to use

    while(t < tmax){
        
        iteration++;//Count the number of iterations
        if (iteration > iteration_max){

            printf("Max iterations reached, stopping simulation.\n");
            break;

        }

        ti = t + dt; //Next time is current time + timestep

        int status = gsl_odeiv2_driver_apply(d, &t, ti, y);

        if(status != GSL_SUCCESS){

            printf("error, return value=%d\n", status);
            break;

        }

        t = ti; //Advance the time variable to the newly calculated time
        
        if( iteration % 100 == 0){//Write every 100 iterations

            write_file(t, y[0], y[2], y[1], dripped, file_out, 0); 

        }

        if (y[0] > ((double *)params)[4]){//Check to see if position has moved past the critical point, causing a drip

            dripped = 1; //Set dripped to 1

            gsl_odeiv2_driver_free (d);//Free the solver

            double x = y[0]; //Set variable to be passed into the henon reverse solver
            double M = y[1];
            double v = y[2];

            henon(params, &x, &v, &M, &t, dripped, file_out, &mass_gain, drip_mode, &mass_loss, &t_prev_drip); //Does the henon step
            d = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rk4, 1e-6, 1e-6, 0.0);//Reallocate the solver so the simulation can continue

            y[0] = x; //Set the y array to the new values produced by henon
            y[1] = M;
            y[2] = v;

            dripped = 0;//Tell loop that the drip is finished
            t_prev_drip = t;

        }

    }

    gsl_odeiv2_driver_free (d);

}