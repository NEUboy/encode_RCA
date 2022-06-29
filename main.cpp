#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "image.h"
#include "test.h"
#include "load.h"

using namespace std;

int main()
{

    vector<string> image_set;
    read_imageset(image_set,"fileset.txt");
    string key="EFC796D47FDFFF69AB7DF3DFFF3CE7AFDEFEFC6977757FC9DA69D93F4D76FC7F";
    test te;
    image img1;
    for(int i=0;i<image_set.size();i++)
    {
        cout<<"****************************************";
        cout<<image_set[i];
        cout<<"****************************************"<<endl;

        img1.readFile(image_set[i]);
        te.robust_test(img1,key);
        te.diff_attack_test(img1,key);
        te.encode_key_sensitivity(img1,key);
        te.decode_key_sensitivity(img1,key);
        te.pix_cov(img1);
        img1.encrypt(key,"");
        img1.saveFile();
        te.pix_cov(img1);
        te.Chi_square_test(img1);
        te.entropy_test(img1);
    }

    /*
    image img2;
    test te;
    string key="EFC796D47FDFFFE9AB7DF3DFFF3CE7AFDEFEFC6977757FC9DA69D93F4D76FC7F";
    img2.readFile("lena.bmp");

    img2.encrypt(key,"");
    img2.saveFile();

    img2.decrypt(key,"");
    img2.saveFile();

    img2.readFile("lena_encrypted.bmp");
    img2.pixel_loss(0.10);
    img2.saveFile();
    img2.decrypt("000");
    img2.saveFile();

    img2.readFile("lena_encrypted.bmp");
    img2.saltPepper_noise(0.001);
    img2.saveFile();
    img2.decrypt("000");
    img2.saveFile();
    */
    return 0;
}
