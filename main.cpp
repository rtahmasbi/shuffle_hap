


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



void ras_help(void);
bool ras_shuffle_within_inds(std::string file_in, std::string file_out, std::string sep, bool show_iterations);
bool ras_shuffle_between_inds(std::string file_in, std::string file_out, std::string sep, bool show_iterations);




int main(int argc, char ** argv)
{
    int start_time = time(0);
    
    std::cout << "Rasool Tahmasbi; ver " << VER << std::endl;
    
    //proc params
    std::string file_hap="";
    std::string file_out="out";
    std::string sep=" ";
    bool show_iterations=true;
    bool shuffle_between_inds=false;
    
    for (int i=1; i<argc; i++)
    {
        std::string s(argv[i]);
        if (s=="--help" || s=="-h")
        {
            ras_help();
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
        else if (s=="--shuffle_between_inds")
        {
            shuffle_between_inds=true;
        }
        else
        {
            std::cout << "Error: unknown parameter [" << s << "]" << std::endl;
            return 0;
        }
    }
    
    
    /////////////////////////////////////////////////////////
    //
    
    if(shuffle_between_inds)
    {
        ras_shuffle_between_inds(file_hap, file_out, sep, show_iterations);
    }
    else
    {
        ras_shuffle_within_inds(file_hap, file_out, sep, show_iterations);
    }

    
    
    return 0;
    
}









void ras_help(void)
{
    std::cout << std::endl;
    std::cout << "Help" << std::endl;
    std::cout << " shuffle_hap --filehap aa.hap --out [out]" << std::endl;
    std::cout << std::endl;
    
}


bool ras_shuffle_within_inds(std::string file_in, std::string file_out, std::string sep, bool show_iterations)
{
    std::ifstream ifile(file_in.c_str());
    if(!ifile)
    {
        std::cout << "Error: can not open the file ["+ file_in +"] to read." << std::endl;
        return 0;
    }
    int nsnp= CommFunc::ras_FileLineNumber(file_in);
    int nhap= CommFunc::ras_FileColNumber(file_in,sep);
    int nind=nhap/2;
    std::cout << "nsnp=" << nsnp << ", nhap=" << nhap << std::endl;

    
    // out
    std::ofstream outfile;
    outfile.open(file_out.c_str());

    srand (time(NULL));
    
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
        throw("Error: in file ["+file_in+"].");
        return false;
    }
    
    ifile.close();
    outfile.close();

    return true;

}



bool ras_shuffle_between_inds(std::string file_in, std::string file_out, std::string sep, bool show_iterations)
{
    std::ifstream ifile(file_in.c_str());
    if(!ifile)
    {
        std::cout << "Error: can not open the file ["+ file_in +"] to read." << std::endl;
        return 0;
    }
    int nsnp= CommFunc::ras_FileLineNumber(file_in);
    int nhap= CommFunc::ras_FileColNumber(file_in,sep);
    int nind=nhap/2;
    std::cout << "nsnp=" << nsnp << ", nhap=" << nhap << std::endl;
    
    
    // out
    std::ofstream outfile;
    outfile.open(file_out.c_str());
    
    srand (time(NULL));
    
    std::string line;
    unsigned long int j=0,i=0;
    
    std::vector<std::string> vals(nhap);

    // each line is one SNP
    while (std::getline(ifile, line)){
        if (show_iterations && j%1000==0) std::cout << "\r      " << j << " of " << nsnp << " read ..." << std::flush;
        for(i=0; i<nhap-1; i++)// nhap-1, because index starts from 0
        {
            if (line[2*i]=='0')
            vals[i]="0";
            else if (line[2*i]=='1')
            vals[i]="1";
            else
            {
                std::cout << "Error: undefined character in file ["+ file_in +"], line number:" << j << std::endl;
                return false;
            }
            std::random_shuffle(vals.begin(), vals.end() );
        }
        j++;
        
        for(i=0; i<vals.size(); i++)// nhap-1, because index starts from 0
        {
            outfile << vals[i] << sep;
        }
        outfile << std::endl;
    }
    
    if (show_iterations) std::cout << std::endl;
    
    if (j!=nsnp)
    {
        throw("Error: in file ["+file_in+"].");
        return false;
    }
    
    ifile.close();
    outfile.close();
    
    return true;
}


