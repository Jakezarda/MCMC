#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include "histogram.h"
#include <string>
#include <sstream>

std::string filename(std::string base, int digits, int num, std::string ext){
    std::stringstream file;
    file << base << std::setw(digits) << std::setfill('0') << num << "." << ext;       //creates files with names such that filename0001.extension
    return file.str();
}

int main() {
    int N_s = 1000;
    std::vector<double> pis;
    std::ifstream fin("Pis.dat");
    
    while (!fin.eof()){
        double pi;
        fin >> pi;
        if (!fin.eof()){
            pis.push_back(pi);
            
        }
    }
    fin.close();
    
    std::vector<double> bins;
    std::vector<int> hist;

    double h = histogram(bins, hist, pis);
    
    std::random_device seeder;
    std::mt19937_64 gen(seeder());
    std::uniform_int_distribution<int> dist(0, pis.size() - 1);
    
    std::vector<int> hist_avg(hist.size());
    std::cout << hist.size() << std::endl;
    for (int sample = 0; sample < N_s; ++sample){
        std::vector<double> pis_samp;
        for (int i = 0; i < pis.size(); ++i){
            int index = dist(gen);
            pis_samp.push_back(pis[index]);
            
        }
        
        
        std::vector<double> bins_samp;
        std::vector<int> hist_samp;
        //std::vector<double> avg;
        histogram(bins_samp, hist_samp, h, pis[0], pis[pis.size() - 1], pis_samp);
        
        std::string file = filename("Pis_hist-", 4, sample + 1, "dat");     
        std::ofstream fout(file);
        fout.precision(15);
        double avg = 0;
        for (int i = 0; i < bins_samp.size(); ++i){
            fout << bins_samp[i] << " " << hist_samp[i] << "\n"; 
            
            hist_avg[i] += hist_samp[i];
            
            } 
        fout.close();   
    }
    
    for (int i = 0; i < 157; ++i){
        hist_avg[i] = hist_avg[i]/1000;
    }
    
    std::ofstream fout("Pis_hist.dat");
    fout.precision(15);
    for (int i = 0; i < bins.size(); ++i){
        fout << bins[i] << " " << hist[i] << "\n";
    }
    fout.close();
    
    fout.open("Pis_hist_avg.dat");
    fout.precision(15);
    for (int i = 0; i < bins.size(); ++i){
        fout << bins[i] << " " << hist_avg[i] << "\n";   
    }
    fout.close();
    
    //the following section is experimental
    
    
    
    return 0;

}



