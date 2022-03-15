#include "Header.h"



int main(){

    Run_Parameters parameters;
    read_file(&parameters);

    double params[8]; //Not entirely sure this array is necessary, but i'm not certain how gsl's functions work, so i'm not taking chances by passing them my struct directly. 

    params[0] = parameters.mass;
    params[1] = parameters.v_0;
    params[2] = parameters.k;
    params[3] = parameters.b;
    params[4] = parameters.x_c;
    params[5] = parameters.alpha;

    params[7] = parameters.x;

    #pragma omp for 

        for(int i = 0; i < 5000 ; i++){

            double R = i * 0.0002 + 0.5;
            //double R = 0.5;
            params[6] = R;
            char fileoutname[100] = {};
            FILE* file_out;

            sprintf(fileoutname, "output_files/data for flowrate = %lf.txt", R);
            puts(fileoutname);

            file_out = fopen(fileoutname, "w");
            fprintf(file_out, "t x v M dripped?\n");

            solver(params, file_out);
            fprintf(file_out, "%lg %lg %lg %lg %lg\n", R, R, R, R, R);
            fclose(file_out);


        }
    
    return 0;
    
}