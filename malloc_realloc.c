/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: onat
 *
 * Created on December 20, 2016, 9:54 AM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(int argc, char** argv) {
    float ort=0;
    int i=0;
    int *sinifptr;
    sinifptr=(int*) malloc(sizeof(int));
    printf("islemi bitirmek icin -1 girin!!\n");
    for(i=0;1;i++)
    {
        sinifptr=(int*)realloc(sinifptr,(i+1)*sizeof(int));
        printf("%dnci ogrencinin notunu giriniz:\t", i+1);
        scanf("%d", &sinifptr[i]);
        if(sinifptr[i]==-1)
            break;
        ort= ort+sinifptr[i];
    }
    ort=ort/i;
    printf("\n sinif ortalaması %.2f", ort);
    free(sinifptr);

    return (EXIT_SUCCESS);
}

