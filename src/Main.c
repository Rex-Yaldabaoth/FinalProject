#include "Header.h"



int main(){

    int drip_mode = 0;
    printf("Welcome to the dripping tap simulator! What dripping mode would you like?\nEnter 1 for drop size to be proportional to the current mass, and 2 for it to not be.\n");
    scanf("%d", &drip_mode);

    if(drip_mode == 1 || drip_mode == 2){
        
    }

    else{
        printf("That is not a valid option. Please run the program again and enter a valid option.\n");
        return -1;
    }

    Run_Parameters parameters; //Struct containing the initial parameters
    read_file(&parameters, drip_mode);

    double params[8]; //Parameters array, for use with GSL's solver. 

    params[0] = parameters.mass;
    params[1] = parameters.v_0; //initial velocity
    params[2] = parameters.k; //Surface tension of the drop
    params[3] = parameters.b; //friction
    params[4] = parameters.x_c; //critical length at which the water will drip
    params[5] = parameters.alpha; //

    params[7] = parameters.x; //setting initial position
    char fileoutname[100] = {}; //character array containing the name of the output file

    #pragma omp for //openmp parallel for loop

        for(int i = 0; i < 5000 ; i++){

            double R = i * 0.0002 + 0.5; //Flow rate, increasing for each run of the simulation. Starts at 0.5
            //double R = 0.885;
            params[6] = R;
            memset(fileoutname, 0, sizeof fileoutname);
            FILE* file_out;

            sprintf(fileoutname, "output_files/data for flowrate = %lf.txt", R); //output file name based on the flow rate of the current run
            puts(fileoutname);

            file_out = fopen(fileoutname, "w");
            fprintf(file_out, "t x v M Merr dripped?\n"); //first line of output file

            solver(params, file_out, drip_mode); //Contains the simulation solver
            fprintf(file_out, "%lg %lg %lg %lg %lg %lg\n", R, R, R, R, R, R); //Sets the elements of the final line of the file to be the flow rate of the current run, for reading by python
            fclose(file_out);


        }
    
    return 0;
    
}