#include "../maximalndependentSet/FastMIS.hpp"
#include "../maximalndependentSet/KboundedMIS.hpp"
#include "../maximalndependentSet/GreedyMIS.hpp"
#include "../utils/common.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <chrono>

using namespace chrono;

int main(int argc, char *argv[]) {
    string name = argv[1];
    int k = INF;

    ofstream runtimesInc("results/short/IncrementalMIS/runtimes/" + name + "-incrementalMISRuntimes.txt");
    runtimesInc << "FastMIS KBoundedMIS GreedyMIS" << endl;

    ofstream costsInc("results/short/IncrementalMIS/costs/" + name + "-incrementalMISRuntimes.txt");
    costsInc << "FastMIS KBoundedMIS GreedyMIS" << endl;

    auto adj = getGraph("testingData/cleanedFiles/short/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/short/" + name + "-Queries.txt");

    unordered_set<int> VV; 
    for(int i=0; i<adj.size(); i++) VV.insert(i);
    
    auto start = high_resolution_clock::now();
    FastMIS fastMIS(adj, VV);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    start = high_resolution_clock::now();
    KboundedMIS kMIS(adj, k);
    stop = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop - start);
    
    
    start = high_resolution_clock::now();
    GreedyMIS greedyMIS(adj, k);
    stop = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop - start);
    
    runtimesInc << duration.count() << " " << duration1.count() << " " << duration2.count() <<endl;

    for (auto [s, p] : edgesToAdd) {
        auto [d, w] = p;
        
        start = high_resolution_clock::now();
        fastMIS.update(s, d);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        
        start = high_resolution_clock::now();
        auto kRes = kMIS.update(s, d);
        stop = high_resolution_clock::now();
        duration1 = duration_cast<microseconds>(stop - start);
        
        start = high_resolution_clock::now();
        auto gRes = greedyMIS.update(s, d);
        stop = high_resolution_clock::now();
        duration1 = duration_cast<microseconds>(stop - start);
        
        runtimesInc << duration.count() << " " << duration1.count() << " " << duration2.count() <<endl;
        costsInc << fastMIS.mis.size() << " " << kRes.size()<<" "<< gRes.size() <<endl;
    }
}
