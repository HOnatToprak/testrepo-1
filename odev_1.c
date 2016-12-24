/* 
 * File:   main.c
 * Author: onat
 *
 * Created on December 22, 2016, 9:02 AM
 */

/*
 * 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include<time.h>

int asciitotal(char word[10]);
int differ(int x, int y);
void randomize(char word[10], int x);
int main(int argc, char** argv) {
    char word[10];
    int i;//kelime sayısı
    int x,y;//iki sayı arasındaki fark
    printf("Enter a word with maximum 10 characters:   ");
    fgets(word, sizeof(word), stdin);
    for(i=0;word[i]!=('\0');i++)//harf büyültme
        word[i]=toupper(word[i]);
    printf("The word with uppercases:   %s ", word);
    printf("The word contains %d characters.\n", i-1);
    printf("Total ASCII values of the characters:   %d\n", asciitotal(word));
    printf("Now enter an integer:  ");
    scanf("%d", &x);
    printf("Enter another one:   ");
    scanf("%d", &y);
    printf("The difference between them is %d\n", differ(x,y));
    printf("---Randomizing The Word---\n");
    randomize(word, i-1);
    
    return (EXIT_SUCCESS);
}
int asciitotal(char word[10])
{
    int sum=0;
    for(int i=0;word[i]!=('\0');i++)
        sum+=word[i];
    return sum-10;
}
int differ(int x, int y)
{
    if(x>=y)
        return x-y;
    else
        return y-x;
}
void randomize(char word[10], int i)
{
    srand(time(NULL));
    int x=rand()%i;//değişecek karakter sayısı
    printf("\nNumber of changes:%d\n", x);
    int y;
    for(x;x!=0;x--)
    {
        y=rand()%90+1;
        while(y<65)//ascii range için
            y=(rand()%90+1);
        word[(rand()%i)]=y;
    }
    printf("New word is %s\n", word);
           
}
    

