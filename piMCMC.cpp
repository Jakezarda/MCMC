#include <iostream>
#include <fstream>
#include <vector>
#include "mcmc.hpp"

int main() {
    std::vector<double> bins, hist;
    std::ifstream fin("Pis_hist.dat");
    while (!fin.eof()){
     double bin, val;
     fin >> bin >> val;
     if (!fin.eof()){
      bins.push_back(bin);
      hist.push_back(val);
     }
    }
    fin.close();
    
    std::vector<std::vector<double>> C(hist.size(), std::vector<double>(hist.size()));
    std::vector<double>(hist.size());
    fin.open("Pis_cov.dat");
    for (int i = 0; i < hist.size(); ++i) {
        for (int j = 0; j < hist.size(); ++j) {
            double val;
            fin >> val;
            C[i][j] = val;
        }
    }
    fin.close();
    
    mcmc chain(bins, hist, C, 3);
    
    std::vector<double> theta = {10.6166, 3.14057, 0.00182755};           //got from plotting on gnuplot, it returned these values theta[0]=A theta[1] = mu theta[2] = sigma
    std::vector<double> var = {10.6166*0.05, 3.14057*0.05, 0.00182755*0.05};
    
    chain.burn_in(1000, theta, var);
    chain.tune_acceptance(1000);
    chain.run_chain(1000000, "Pis_MCMC.dat");                  //doing 10,000 for now, may want to increase later.
    
    return 0;
}


//for compiling the object code

//g++ -march=native -lgsl -lgslcblas -O3 piMCMC.cpp mcmc.cpp /usr/lib/x86_64-linux-gnu/libgsl.a /usr/lib/x86_64-linux-gnu/libgslcblas.a -o piMCMC
