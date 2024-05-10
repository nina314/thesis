#include "staticKcenter/gonzales.hpp"
#include "staticKcenter/distanceRIndependent.hpp"
#include "staticKcenter/randomCenters.hpp"
#include "staticKcenter/baselineGreedy.hpp"
#include "staticKcenter/bottleneck.hpp"
#include "dynamicKcenter/IncrementalAlgo.hpp"
#include "utils/common.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <unordered_set>

int main(int argc, char *argv[]) {
    int k = stoi(argv[1]); // Number of centers to select
    int alpha = stoi(argv[2]); // Approximation factor
    double eps = stod(argv[3]);
    string name = argv[4];

    ofstream runtimes("results/" + name + "-staticKcenterRuntimes.txt");
    ofstream costs("results/" + name + "-staticKcenterCosts.txt");

    runtimes << "RIndependent Gonzalez BaselineGreedy Bottleneck Random Dynamic" << endl;
    costs << "RIndependent Gonzalez BaselineGreedy Bottleneck Random Dynamic" << endl;

    int maxWeight = 1;

    auto graph = getGraph("testingData/cleanedFiles/" + name + "-Edges.txt");
    auto edgesToRemove = getQueries("testingData/cleanedFiles/" + name + "-Queries.txt");

    auto ia = IncrementalAlgo(graph, k, eps);

    for (auto [s, d] : edgesToRemove) {
        graph[s].erase({d, 1});
        graph[d].erase({s, 1});
    }
    
    for (auto [s, d] : edgesToRemove) {
        graph[s].insert({d, 1});
        graph[d].insert({s, 1});

        auto start = high_resolution_clock::now();
        auto centers1 = distanceRIndependent(graph, k, maxWeight);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        auto runtime1 = duration.count();
        auto cost1 = cost(graph, centers1);
        cout<<"1 "<<cost1<<endl;
        
        start = high_resolution_clock::now();
        auto centers2 = gonzalezAlpha(graph, k, alpha, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime2 = duration.count();
        auto cost2 = cost(graph, centers2);
        cout<<"2 "<<cost2<<endl;
        
        start = high_resolution_clock::now();
        auto centers3 = baselineGreedy(graph, k, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime3 = duration.count();
        auto cost3 = cost(graph, centers3);
        cout<<"3 "<<cost3<<endl;
        
        start = high_resolution_clock::now();
        auto centers4 = bottleneck(graph, k, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime4 = duration.count();
        auto cost4 = cost(graph, centers4);
        cout<<"4 "<<cost4<<endl;
        
        start = high_resolution_clock::now();
        ia.insertEdge(s, d);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime5 = duration.count();
        auto cents = ia.getCenters();
        
        for(auto c: cents) cout<<c<<" ";
        cout<<endl;
        auto cost5 = cost(graph, ia.getCenters());
        cout<<"5 "<<cost5<<endl;

        costs << cost1 << " " << cost2 << " " << cost3 << " " << cost4 << " " << cost5 << endl;
        
        runtimes << runtime1 << " " << runtime2 << " " << runtime3 << " " << runtime4 << " " << runtime5 << endl;
        
    }
    return 0;
}