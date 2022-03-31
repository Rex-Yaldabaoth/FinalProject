#include "Header.h"

void read_file(Run_Parameters * parameters, int drip_mode){

    FILE* file_in;
    char* filename;

    if(drip_mode == 1){
        filename = "run_params1";
    }

    if(drip_mode == 2){
        filename = "run_params2";
    }
    
    file_in = fopen(filename,"r");

    if(file_in == NULL){
        printf("File could not be read. Please ensure your file has the correct name.\n");
        perror("Failed: ");
        exit(EXIT_FAILURE);
    }

    fscanf(file_in, "%*[^\n]\n");

    fscanf(file_in, "%lg %lg %lg %lg %lg %lg %lg %lg", &parameters->mass, &parameters->v_0, &parameters->k, &parameters->b, &parameters->x_c, &parameters->alpha, &parameters->R, &parameters->tf);

    fclose(file_in);

}