#ifndef LOAD_H_INCLUDED
#define LOAD_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>

void read_imageset(vector<string> &img_set,string fileset)
{
    string temp;
    ifstream in_file;
    in_file.open(fileset,ios::in);

    while(getline(in_file,temp))
    {
        img_set.push_back(temp);
    }
    in_file.close();
}

#endif // LOAD_H_INCLUDED
