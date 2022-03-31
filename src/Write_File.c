#include "Header.h"

void write_file(double t, double x, double v, double M, short int dripped, FILE* file_out, double Merr){//This function writes the relevant data to a file

    if(dripped == 0){

        fprintf(file_out, "%lg %lg %lg %lg %d %d\n", t, x, v, M, 0, dripped);

    }
    
    else{

        fprintf(file_out, "%lg %lg %lg %lg %lg %d\n", t, x, v, M, Merr, dripped);

    }
    
}