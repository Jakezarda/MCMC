#include <iostream>
#include <fstream>
#include <ctime>
#include "mcmc.hpp"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <random>
#include "model.hpp"

void mcmc::getParams() {
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for(int i = 0; i < this->N_params; ++i) {
        this->theta_i[i] = this->theta_0[i] + dist(this->gen)*var[i];
    }
}

double mcmc::getChisq() {
    double chisq = 0;
    
    for (int i = 0; i < this->data.size(); ++i) {
        for (int j = i; j < this->data.size(); ++j) {
            chisq += (this->data[i] - this->model[i])*this->Psi[i][j]*(this->data[j] - this->model[j]);
        }
    }
    return chisq;
}

bool mcmc::trial() {
    bool move = false;
    mcmc::getParams();
    f(this->x, this->theta_i, this->model);
    this->chisq_i = getChisq();
    
    double likelihoodRatio = std::exp(0.5*(this->chisq_0 - this->chisq_i));
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double test = dist(this->gen);
    
    if (likelihoodRatio > test) {
        this->theta_0 = this->theta_i;
        this->chisq_0 = this->chisq_i;
        move = true;
    }
    return move;
}

mcmc::mcmc(std::vector<double> &x, std::vector<double> &data, 
           std::vector<std::vector<double>> &C, int N_params) {
    this->N_data = data.size();
    this->N_params = N_params;
    this->theta_0 = std::vector<double>(N_params);
    this->theta_i = std::vector<double>(N_params);
    this->var = std::vector<double>(N_params);
    for (int i = 0; i < this->N_data; ++i) {
        this->data.push_back(data[i]);
        this->x.push_back(x[i]);
    }
    
    this->model = std::vector<double>(this->N_data);
    
    gsl_matrix *cov = gsl_matrix_alloc(this->N_data, this->N_data);
    
    for (int i = 0; i < this->N_data; ++i) {
        for (int j = 0; j < this->N_data; ++j) {
            gsl_matrix_set(cov, i, j, C[i][j]);
        }
    }
    
    gsl_linalg_cholesky_decomp1(cov);
    gsl_linalg_cholesky_invert(cov);
    
    this->Psi = std::vector<std::vector<double>>(this->N_data, std::vector<double>(this->N_data));
    
    for (int i = 0; i < this->N_data; ++i) {
        for (int j = 0; j < this->N_data; ++j) {
            Psi[i][j] = gsl_matrix_get(cov, i, j);
        }
    }
    
    gsl_matrix_free(cov);
    
    this->gen.seed(time(0));
}

void mcmc::burn_in(int N_burn, std::vector<double> theta, 
                   std::vector<double> var) {
    for (int i = 0; i < this->N_params; ++i) {
        this->theta_0[i] = theta[i];
        this->var[i] = var[i];
    }
    
    this->chisq_0 = getChisq();
    
    f(this->x, this->theta_0, this->model);
    
    for (int i = 0; i < N_burn; ++i) {
        mcmc::trial();
    }
}

void mcmc::tune_acceptance(int N_trials) {
    double acceptance = 1.0;
    while (acceptance > 0.235 or acceptance < 0.233) {
        int moved = 0;
        for (int i = 0; i < N_trials; ++i) {
            bool move = mcmc::trial();
            if (move) {
                moved++;
            }
        }
        acceptance = double(moved)/double(N_trials);
        
        if (acceptance > 0.235) {
            for (int i = 0; i < this->var.size(); ++i) {
                this->var[i] *= 1.01;
            }
        } else if (acceptance < 0.233) {
            for (int i = 0; i < this->var.size(); ++i) {
                this->var[i] *= 0.99;
            }
        }
    }
}

void mcmc::run_chain(int N_trials, std::string file) {
    std::ofstream fout(file);
    fout.precision(15);
    
    int moved = 0;
    for (int i = 0; i < N_trials; ++i) {
        bool move = mcmc::trial();
        if (move) moved++;
        for (int j = 0; j < this->theta_0.size(); ++j) {
            fout << theta_0[j] << " ";
        }
        fout << "\n";
    }
    fout.close();
    
    std::cout << "Final acceptance of the chain: " << double(moved)/double(N_trials) << "\n";
    
    //below this is experimental
    double avg = 0;
    double sum = 0;
    for (int i = 0; i < theta_0.size(); ++i){
        sum += theta_0[i];
        avg = sum/theta_0.size();
        
    }
    
    std::cout << "The average is " << avg << "\n";
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
