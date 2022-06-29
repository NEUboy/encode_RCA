#include "decrypt.h"

static void key_to_bit(string key, unsigned char bit_buff[256])
{
    char hex_key[64];
    memset(hex_key,0,64);
    strcpy(hex_key,key.c_str());
    for(int i=0;i<64;i++)
    {
        if((hex_key[i]>=48)&&(hex_key[i]<=57))
        {
            char temp=0x08;
            for(int j=0;j<4;j++)
            {
                bit_buff[i*4+j]=(((hex_key[i]-48)&temp)==temp);
                temp=temp>>1;
            }
        }
        else
        {
            char temp=0x08;
            for(int j=0;j<4;j++)
            {
                bit_buff[i*4+j]=(((hex_key[i]-55)&temp)==temp);
                temp=temp>>1;
            }
        }
    }
}

static double bit_to_val(unsigned char bit_buff[256],int start, int stop)
{
    double val=0;
    for(int i=start;i<stop;i++)
    {
        val+=bit_buff[i]*pow(0.5,i-start+1);
    }
    return val;
}

static unsigned int bit_to_int(unsigned char bit_buff[256],int start, int stop)
{
    unsigned int rec=0;
    for(int i=start;i<stop;i++)
    {
        rec=(rec<<1)+bit_buff[i];
    }
    return rec;
}

static void get_blocks(unsigned char ** blocks[8] ,unsigned char ** pixelbuff,int id_x,int id_y)
{
    for(int row=id_x;row<id_x+8;row++)
    {
        for(int col=id_y;col<id_y+8;col++)
        {
            unsigned char temp=0b00000001;
            for (int i=0;i<8;i++)
            {
                blocks[i][row-id_x][col-id_y]=((pixelbuff[row][col]&temp)==temp);
                temp=temp<<1;
            }
        }
    }
}

static void block_deshuffle(unsigned char **pixelbuff,int * order,int length,int img_size[2])
{
    unsigned char ** tempbuff;
    tempbuff= new unsigned char* [img_size[0]];
    for(int i=0;i<img_size[0];i++)
    {
        tempbuff[i]= new unsigned char [img_size[1]];
    }
    for(int i=0;i<length;i++)
    {
        int read_x=i/(img_size[1]/8)*8;
        int read_y=i%(img_size[1]/8)*8;
        int write_x=order[i]/(img_size[1]/8)*8;
        int write_y=order[i]%(img_size[1]/8)*8;
        for(int row=0;row<8;row++)
        {
            for(int col=0;col<8;col++)
            {
                tempbuff[write_x+row][write_y+col]=pixelbuff[read_x+row][read_y+col];
            }
        }
    }
    for(int i=0;i<img_size[0];i++)
    {
        memcpy(pixelbuff[i],tempbuff[i],img_size[1]);
        delete []tempbuff[i];
    }
    delete []tempbuff;
}

static void block_to_pixels(unsigned char ** blocks[8],unsigned char ** pixelbuff,int id_x,int id_y)
{
    for(int row=id_x;row<id_x+8;row++)
    {
        for(int col=id_y;col<id_y+8;col++)
        {
            unsigned char val=0;
            for(int i=7;i>=0;i--)
            {
                val=(val<<1)+blocks[i][row-id_x][col-id_y];
            }
            pixelbuff[row][col]=val;
        }
    }
}

static void RCA_func(unsigned char ** block, unsigned char height, unsigned char width)
{
    unsigned char ** new_block= new unsigned char *[height];
    for (int i=0;i<height;i++)
    {
        new_block[i]= new unsigned char [width];
        memset(new_block[i],0,width);
    }

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            unsigned char cnt=0;
            unsigned char up= i-1; unsigned char down=i+1;
            unsigned char left=j-1; unsigned char right=j+1;
            if(i==0)
                up=height-1;
            if(i==height-1)
                down=0;
            if(j==0)
                left=width-1;
            if(j==width-1)
                right=0;

            cnt= block[up][left]+block[up][j]+block[up][right]
                +block[i][left]+block[i][right]
                +block[down][left]+block[down][j]+block[down][right];

            if(block[i][j]==0)
                if (cnt==7||cnt==5||cnt==3||cnt==1)
                    new_block[i][j]=1;
            if(block[i][j]==1)
                if (cnt==7||cnt==5||cnt==3||cnt==1)
                    new_block[i][j]=0;
        }
    }

    for (int i=0;i<height;i++)
    {
        memcpy(block[i],new_block[i],width);
        delete []new_block[i];
    }
    delete []new_block;
}

static void block_iterate(unsigned char ** blocks[8], unsigned char order[8],int times)
{
    unsigned char ** temp_blocks[8];
    for(int t=0;t<times;t++)
    {
        //先做备份
        for (int i=0;i<8;i++)
        {
            temp_blocks[i]=new unsigned char*[8];
            for(int j=0;j<8;j++)
            {
                temp_blocks[i][j]=new unsigned char [8];
                memcpy(temp_blocks[i][j],blocks[i][j],8);
            }
        }

        //迭代下一个状态
        for(int i=0;i<7;i++)
        {
            RCA_func(blocks[order[i]],8,8);
        }
        //处理order第一张
        for (int row=0;row<8;row++)
        {
             for (int col=0;col<8;col++)
             {
                 int sum=0;
                 for(int i=0;i<8;i++)
                 {
                     sum+=blocks[order[i]][row][col];
                 }
                 blocks[order[0]][row][col]=sum%2;
             }
        }
        //其余的依次拷贝
        for(int id=1;id<8;id++)
        {
            for(int row=0;row<8;row++)
            {
                memcpy(blocks[order[id]][row],temp_blocks[order[id-1]][row],8);
            }
        }
        //空间释放
        for (int id=0;id<8;id++)
        {
            for (int row=0;row<8;row++)
            {
               delete []temp_blocks[id][row];
            }
            delete []temp_blocks[id];
        }
    }
}

static void swap_double(double *a, double *b)
{
    double temp;
    temp=*a;*a=*b; *b=temp;
}

static void swap_int(int *a,int*b)
{
    int temp;
    temp=*a;*a=*b; *b=temp;
}

static void swap_char(unsigned char *a, unsigned char *b)
{
    char temp;
    temp=*a;*a=*b;*b=temp;
}

void derandNumCreate(double ** chaos,int length,string key,int round)
{
    double pi=3.1415926536;
    unsigned char bit_buff[256];
    key_to_bit(key,bit_buff);
    double xi=bit_to_val(bit_buff,0,32)*bit_to_val(bit_buff,32,64);
    double yi=bit_to_val(bit_buff,64,96)*bit_to_val(bit_buff,96,128);
    double theta=bit_to_val(bit_buff,128,160)*bit_to_val(bit_buff,160,192);
    unsigned int alpha= bit_to_int(bit_buff,192,224);
    unsigned int beta = bit_to_int(bit_buff,224,256);
    unsigned int mu;

    if(round==0)
        mu=alpha^beta;
    else
        mu=alpha+beta;

    theta=theta*mu-floor(theta*mu);

    for (int i=0;i<30;i++)
    {
        xi=sin(pi*((4*theta*xi*(1-xi))+(1-theta)*sin(pi*yi)));
        yi=sin(pi*((4*theta*yi*(1-yi))+(1-theta)*sin(pi*xi)));
    }
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<10;j+=2)
        {
            xi=sin(pi*((4*theta*xi*(1-xi))+(1-theta)*sin(pi*yi)));
            yi=sin(pi*((4*theta*yi*(1-yi))+(1-theta)*sin(pi*xi)));
            if(j<8)
            {
                chaos[i][j]=xi;chaos[i][j+1]=xi;
            }
            else
            {
                chaos[i][j]=(xi+yi)/2;
            }

        }
    }
}

void depretreatment(image *img,double ** chaos_x)
{
    unsigned char ** pixelbuff;
    int img_size[2];
    img->getSize(img_size);
    pixelbuff = new unsigned char* [img_size[0]];
    for(int i=0;i< img_size[1];i++)
    {
        pixelbuff[i]= new unsigned char[img_size[1]];
    }
    img->getPixels(pixelbuff);

    for(int id=0;id<(img_size[0]/8*img_size[1]/8);id++)
    {
        int id_x=id/(img_size[1]/8)*8;
        int id_y=id%(img_size[1]/8)*8;

        int rand_pos[4]; unsigned char rand_pix[4];
        for(int i=0;i<4;i++)
        {
            rand_pos[i]=(long long)(chaos_x[id][1+i]*1.0e+14)%64;
            rand_pix[i]=(long long)(chaos_x[id][5+i]*1.0e+14)%256;
        }
        for(int i=0;i<4;i++)
        {
            int x=rand_pos[i]/8;
            int y=rand_pos[i]%8;
            pixelbuff[id_x+x][id_y+y]=pixelbuff[id_x+x][id_y+y]-rand_pix[i];
        }
    }
    img->writePixels(pixelbuff);
}

void depermutate(image *img,double ** chaos_x,int round)
{
    unsigned char ** pixelbuff;
    int img_size[2];
    img->getSize(img_size);
    pixelbuff = new unsigned char* [img_size[0]];
    for(int i=0;i< img_size[1];i++)
    {
        pixelbuff[i]= new unsigned char[img_size[1]];
    }
    img->getPixels(pixelbuff);
    int length=img_size[0]/8*img_size[1]/8;
    int * order= new int [length];
    double * chaos_array;
    chaos_array=new double[length];
    for(int i=0;i<length;i++)
    {
        chaos_array[i]=chaos_x[i][0];
        order[i]=i;
    }
    chaos_array[length-1]=0;
    for(int i=0;i<length;i++)
    {
        for(int j=0;j<length-1-i;j++)
        {
            if(chaos_array[j]>chaos_array[j+1])
            {
                swap_double(&chaos_array[j],&chaos_array[j+1]);
                swap_int(&order[j],&order[j+1]);
            }
        }
    }
    block_deshuffle(pixelbuff,order,length,img_size);
    img->writePixels(pixelbuff);
}

void dediffusion(image *img,double ** chaos_y,int round)
{
    unsigned char ** blocks[8];
    unsigned char ** pixelbuff;
    unsigned char ** pre_block;
    unsigned char ** cur_block;
    int img_size[2];
    img->getSize(img_size);

    for(int i=0;i<8;i++)
    {
        blocks[i]= new unsigned char* [8];
        for(int j=0;j<8;j++)
        {
            blocks[i][j]= new unsigned char [8];
        }
    }
    pixelbuff = new unsigned char* [img_size[0]];
    for(int i=0;i< img_size[1];i++)
    {
        pixelbuff[i]= new unsigned char[img_size[1]];
    }
    img->getPixels(pixelbuff);

    pre_block= new unsigned char* [8];
    for(int i=0;i<8;i++)
    {
        pre_block[i]=new unsigned char[8];
    }
    cur_block= new unsigned char* [8];
    for(int i=0;i<8;i++)
    {
        cur_block[i]=new unsigned char[8];
    }

    for(int id=0;id<(img_size[0]/8*img_size[1]/8);id++)
    {
        int id_x; int id_y;
        id_x=id/(img_size[1]/8)*8;
        id_y=id%(img_size[1]/8)*8;
        double chaos_array[9];
        memcpy(chaos_array,chaos_y[id],9*sizeof(double));
        unsigned char order[8]={0,1,2,3,4,5,6,7};
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<7-i;j++)
            {
                if(chaos_array[j]>chaos_array[j+1])
                {
                    swap_double(&chaos_array[j],&chaos_array[j+1]);
                    swap_char(&order[j],&order[j+1]);
                }
            }
        }
        int times= 10+((long long)(chaos_array[8]*1.0e+14)%8);

        //先存一下当前的密文块
        if(id==0)
        {
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    pre_block[i][j]=pixelbuff[id_x+i][id_y+j];
                }
            }
            get_blocks(blocks,pixelbuff,id_x,id_y);
            block_iterate(blocks,order,times);
            block_to_pixels(blocks,pixelbuff,id_x,id_y);

        }
        else
        {
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    cur_block[i][j]=pixelbuff[id_x+i][id_y+j];
                }
            }
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    pixelbuff[i+id_x][j+id_y]=pixelbuff[i+id_x][j+id_y]^pre_block[i][j];
                }
            }
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    pre_block[i][j]=cur_block[i][j];
                }
            }
            get_blocks(blocks,pixelbuff,id_x,id_y);
            block_iterate(blocks,order,times);
            block_to_pixels(blocks,pixelbuff,id_x,id_y);
        }
    }
    img->writePixels(pixelbuff);
    for (int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            delete []blocks[i][j];
        }
        delete []blocks[i];
    }
    for(int i=0;i< img_size[0];i++)
    {
        delete []pixelbuff[i];
    }
    delete []pixelbuff;
    for(int i=0;i<8;i++)
    {
        delete []pre_block[i];
    }
    delete []pre_block;
    for(int i=0;i<8;i++)
    {
        delete []cur_block[i];
    }
    delete []cur_block;
}
