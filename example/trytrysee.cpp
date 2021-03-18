
#define LIBSVM_TOOL_H
#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "svm.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
int main(){
   // struct svm_problem prob;
    std::ifstream infile("/home/dev/libsvm-3.23/tools/test2.txt");
    std::string modelFile = "/home/dev/libsvm-3.23/tools/train2.txt.model";
    struct svm_model* model= svm_load_model(modelFile.c_str());
   // prob.x = new svm_node * [1];
    vector<string> ftr;
    std::string a,b,c,d;
    double count=0;
    double count1=0;
    while (infile>>a>>b>>c>>d)
    {
        double x=atof(b.substr(b.find(":") + 1).c_str());
            double y=atof(c.substr(c.find(":") + 1).c_str());
            double r=atof(d.substr(d.find(":") + 1).c_str());

            //std::string x=std::to_string(obj.x);
            //std::string y=std::to_string(obj.y);
            //std::string a=std::to_string(obj.width*obj.height);
            svm_node xnode[4];
            double scalex=(x-177)/1815.8750000000002*2-1;
                        double scaley=(y-27)/794.2070230607967*2-1;
                        double scalea=(r-156)/15396*2-1;
                        xnode[0].index = 1;
                        xnode[0].value = scalex ;
                        xnode[1].index = 2;
                        xnode[1].value = scaley;
                        xnode[2].index = 3;
                        xnode[2].value = scalea;
                        xnode[3].index = -1;
            double label = svm_predict(model, xnode);
           // int   var_y = static_cast<int>(label);
            std::stringstream stream;
            stream << std::fixed << std::setprecision(0) << label;
            std::string s = stream.str();

            //double diff=abs(s-atof(a.c_str()));
            if (s==a)
            {count++;
            //cout<<"correct"<<endl;
            }
            else
            {count1++;
           //cout<<s<<endl;
            printf("prediction:%s x: %s ,y: %s \n",s.c_str(),b.substr(b.find(":") + 1).c_str(),c.substr(c.find(":") + 1).c_str());
            }
            //cout<<label<<endl;



}
     double acc=count/(count+count1 );      //std::string data1="1:"+x+" 2:"+y+" 3:"+a;
           //double *cs= new double[2];
cout<<"accuracy:"<<acc<<endl;
int ddd=1;
double re=ddd;
std::vector<int> detection_results;
int i;
//for (i=0;i<4;i++) {
 //   detection_results.push_back(2*i);
//}
//cout<<detection_results[ddd]<<endl;
}
