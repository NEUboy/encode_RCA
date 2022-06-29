#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED
#include <iostream>
#include<cmath>
#include <stdlib.h>
#include "image.h"

using namespace std;
static void key_to_bit(string key, unsigned char bit_buff[256]);
static double bit_to_val(unsigned char bit_buff[256],int start, int stop);
static unsigned int bit_to_int(unsigned char bit_buff[256],int start, int stop);

static void get_blocks(unsigned char ** blocks[8] ,unsigned char ** pixelbuff,int id_x,int id_y);
static void block_shuffle(unsigned char **pixelbuff,int * order,int length,int img_size[2]);
static void RCA_func(unsigned char ** block, unsigned char height, unsigned char width);
static void block_iterate(unsigned char ** blocks[8], unsigned char order[8],int times);
static void block_to_pixels(unsigned char ** blocks[8],unsigned char ** pixelbuff,int id_x,int id_y);

void randNumCreate(double ** chaos,int length,string key,int round);
void pretreatment(image *img,double ** chaos_x);
void permutate(image *img, double ** chaos_x,int round);
void diffusion(image *img, double ** chaos_y,int round);

#endif // ENCRYPT_H_INCLUDED
