


#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <iostream>
#include <ctime>
#include <vector>
#include <string> // str1.compare(str2)
#include <sstream> //std::istringstream
#include <cmath> //pow
#include <algorithm> //find
#include "CommFunc.h"
#include <cstdlib>


#define VER "1.0"







int main(int argc, char ** argv)
{
    int start_time = time(0);
    
    std::cout << "Rasool Tahmasbi; ver " << VER << std::endl;
    
    //proc params
    std::string file_hap="";
    std::string file_out="out";
    std::string sep=" ";
    bool show_iterations=true;
    
    for (int i=1; i<argc; i++)
    {
        std::string s(argv[i]);
        if (s=="--help")
        {
            //ras_help();
            return 0;
        }
        else if (s=="--filehap")
        {
            file_hap=std::string(argv[++i]);
        }
        else if (s=="--out")
        {
            file_out=std::string(argv[++i]);
        }
        else
        {
            std::cout << "Error: unknown parameter [" << s << "]" << std::endl;
            return 0;
        }
    }
    
    
    /////////////////////////////////////////////////////////
    //
    
    std::string file_name=file_hap;
    std::ifstream ifile(file_name.c_str());
    if(!ifile)
    {
        std::cout << "Error: can not open the file ["+ file_name +"] to read." << std::endl;
        return 0;
    }
    int nsnp= CommFunc::ras_FileLineNumber(file_hap);
    int nhap= CommFunc::ras_FileColNumber(file_hap,sep);
    int nind=nhap/2;
    std::cout << "nsnp=" << nsnp << ", nhap=" << nhap << std::endl;
    
    
    
    // out
    std::ofstream outfile;
    outfile.open(file_out.c_str());
    

    
    srand (time(NULL));

    std::vector<std::string> st1;
    
    std::string line;
    unsigned long int j=0,i=0;
    
    // each line is one SNP
    while (std::getline(ifile, line)){
        if (show_iterations && j%1000==0) std::cout << "\r      " << j << " of " << nsnp << " read ..." << std::flush;
        for(i=0; i<nind; i++)
        {
            char temp1=line[4*i];
            char temp2=line[4*i+2];
            int r = rand() % 2;
            if (r==1)
            {
                outfile << temp1 << sep << temp2 << sep;
            }
            else
            {
                outfile << temp2 << sep << temp1 << sep;
            }
        }
        j++;
        outfile << std::endl;
    }
    
    if (show_iterations) std::cout << std::endl;
    
    if (j!=nsnp)
    {
        throw("Error: in file ["+file_name+"].");
        return false;
    }
    
    ifile.close();
    outfile.close();
    
    return 0;
    
}






