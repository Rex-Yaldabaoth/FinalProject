#include "Header.h"

void write_file(double t, double x, double v, double M, short int dripped, FILE* file_out){//This function writes the relevant data to a file

    //printf("%lg %lg %lg %lg %d\n", t, x, v, M, dripped);
    fprintf(file_out, "%lg %lg %lg %lg %d\n", t, x, v, M, dripped);
    
}