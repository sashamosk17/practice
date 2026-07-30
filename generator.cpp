#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <random>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <string>

using namespace std;

// Generate connected graph with given number of vertices and edges
void generateGraph(int V, int E, int testNum, const string& filename) {
    mt19937 rng(testNum); // seed = test number for reproducibility
    uniform_int_distribution<int> vertexDist(0, V - 1);
    uniform_int_distribution<long long> weightDist(1, 100000);

    set<pair<int, int>> edges; // to avoid duplicate edges
    vector<pair<pair<int, int>, long long>> edgeList;

    // Step 1: create spanning tree for connectivity (V-1 edges)
    for (int i = 1; i < V; ++i) {
        int parent = uniform_int_distribution<int>(0, i - 1)(rng);
        edges.insert({parent, i});
        edgeList.push_back({{parent, i}, weightDist(rng)});
    }

    // Step 2: add remaining edges randomly
    int remainingEdges = E - (V - 1);
    while (remainingEdges > 0 && edges.size() < (long long)V * (V - 1)) {
        int u = vertexDist(rng);
        int v = vertexDist(rng);

        if (u != v && edges.find({u, v}) == edges.end()) {
            edges.insert({u, v});
            edgeList.push_back({{u, v}, weightDist(rng)});
            remainingEdges--;
        }
    }

    // Shuffle edges for random order
    shuffle(edgeList.begin(), edgeList.end(), rng);

    // Write to file
    ofstream out(filename);
    out << V << " " << edgeList.size() << " " << 0 << "\n"; // start vertex = 0
    for (size_t i = 0; i < edgeList.size(); ++i) {
        out << edgeList[i].first.first << " " << edgeList[i].first.second << " " << edgeList[i].second << "\n";
    }
    out.close();
}

// Generate chain (path)
void generateChain(int V, int testNum, const string& filename) {
    mt19937 rng(testNum);
    uniform_int_distribution<long long> weightDist(1, 100000);

    ofstream out(filename);
    out << V << " " << (V - 1) << " " << 0 << "\n";
    for (int i = 0; i < V - 1; ++i) {
        out << i << " " << (i + 1) << " " << weightDist(rng) << "\n";
    }
    out.close();
}

// Generate star
void generateStar(int V, int testNum, const string& filename) {
    mt19937 rng(testNum);
    uniform_int_distribution<long long> weightDist(1, 100000);

    ofstream out(filename);
    out << V << " " << (V - 1) << " " << 0 << "\n";
    for (int i = 1; i < V; ++i) {
        out << 0 << " " << i << " " << weightDist(rng) << "\n";
    }
    out.close();
}

// Generate graph with disconnected components
void generateDisconnected(int V, int testNum, const string& filename) {
    mt19937 rng(testNum);
    uniform_int_distribution<long long> weightDist(1, 100000);

    // Split into 3 components
    int comp1 = V / 3;
    int comp2 = V / 3;
    int comp3 = V - comp1 - comp2;

    vector<pair<pair<int, int>, long long>> edgeList;

    // Component 1: contains vertex 0
    for (int i = 1; i < comp1; ++i) {
        edgeList.push_back({{0, i}, weightDist(rng)});
    }

    // Component 2: disconnected
    for (int i = comp1 + 1; i < comp1 + comp2; ++i) {
        edgeList.push_back({{comp1, i}, weightDist(rng)});
    }

    // Component 3: disconnected
    for (int i = comp1 + comp2 + 1; i < V; ++i) {
        edgeList.push_back({{comp1 + comp2, i}, weightDist(rng)});
    }

    ofstream out(filename);
    out << V << " " << edgeList.size() << " " << 0 << "\n";
    for (size_t i = 0; i < edgeList.size(); ++i) {
        out << edgeList[i].first.first << " " << edgeList[i].first.second << " " << edgeList[i].second << "\n";
    }
    out.close();
}

// Generate complete bipartite graph
void generateBipartite(int V, int testNum, const string& filename) {
    mt19937 rng(testNum);
    uniform_int_distribution<long long> weightDist(1, 100000);

    int part1 = V / 2;
    int part2 = V - part1;

    vector<pair<pair<int, int>, long long>> edgeList;

    for (int i = 0; i < part1; ++i) {
        for (int j = part1; j < V; ++j) {
            edgeList.push_back({{i, j}, weightDist(rng)});
        }
    }

    ofstream out(filename);
    out << V << " " << edgeList.size() << " " << 0 << "\n";
    for (size_t i = 0; i < edgeList.size(); ++i) {
        out << edgeList[i].first.first << " " << edgeList[i].first.second << " " << edgeList[i].second << "\n";
    }
    out.close();
}

// Run Dijkstra algorithm and save answer
void runDijkstra(const string& inputFile, const string& outputFile) {
    string cmd = "./dijkstra.exe < " + inputFile + " > " + outputFile;
    system(cmd.c_str());
}

int main() {
    // Create tests folder
    system("mkdir -p tests");

    // Vertex sizes for first 75 tests
    vector<int> sizes = {
        100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
        1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000,
        3500, 4000, 4500, 5000, 5000
    };

    int testNum = 1;

    // Group 1: Sparse graphs (tests 1-25)
    cout << "Generating group 1: Sparse graphs (1-25)..." << endl;
    for (int i = 0; i < 25; ++i) {
        int V = sizes[i];
        int E = V + (rand() % (V / 10 + 1));
        string inputFile = "tests/" + to_string(testNum);
        string outputFile = "tests/" + to_string(testNum) + ".a";

        generateGraph(V, E, testNum, inputFile);
        runDijkstra(inputFile, outputFile);

        cout << "  Test " << testNum << ": V=" << V << ", E=" << E << endl;
        testNum++;
    }

    // Group 2: Medium density (tests 26-50)
    cout << "Generating group 2: Medium density (26-50)..." << endl;
    for (int i = 0; i < 25; ++i) {
        int V = sizes[i];
        int E = (int)(V * log2(V) * 0.9) + (rand() % (int)(V * log2(V) * 0.2 + 1));
        E = min(E, V * (V - 1));
        string inputFile = "tests/" + to_string(testNum);
        string outputFile = "tests/" + to_string(testNum) + ".a";

        generateGraph(V, E, testNum, inputFile);
        runDijkstra(inputFile, outputFile);

        cout << "  Test " << testNum << ": V=" << V << ", E=" << E << endl;
        testNum++;
    }

    // Group 3: Dense graphs (tests 51-75)
    cout << "Generating group 3: Dense graphs (51-75)..." << endl;
    for (int i = 0; i < 25; ++i) {
        int V = sizes[i];
        long long maxE = (long long)V * (V - 1);
        int E = min(maxE, (long long)(V * V / 10));
        string inputFile = "tests/" + to_string(testNum);
        string outputFile = "tests/" + to_string(testNum) + ".a";

        generateGraph(V, E, testNum, inputFile);
        runDijkstra(inputFile, outputFile);

        cout << "  Test " << testNum << ": V=" << V << ", E=" << E << endl;
        testNum++;
    }

    // Group 4: Special cases (tests 76-100)
    cout << "Generating group 4: Special cases (76-100)..." << endl;

    // 76-80: Chains
    vector<int> chainSizes = {100, 500, 1000, 2500, 5000};
    for (size_t idx = 0; idx < chainSizes.size(); ++idx) {
        int size = chainSizes[idx];
        string inputFile = "tests/" + to_string(testNum);
        string outputFile = "tests/" + to_string(testNum) + ".a";

        generateChain(size, testNum, inputFile);
        runDijkstra(inputFile, outputFile);

        cout << "  Test " << testNum << " (chain): V=" << size << endl;
        testNum++;
    }

    // 81-85: Stars
    vector<int> starSizes = {100, 500, 1000, 2500, 5000};
    for (size_t idx = 0; idx < starSizes.size(); ++idx) {
        int size = starSizes[idx];
        string inputFile = "tests/" + to_string(testNum);
        string outputFile = "tests/" + to_string(testNum) + ".a";

        generateStar(size, testNum, inputFile);
        runDijkstra(inputFile, outputFile);

        cout << "  Test " << testNum << " (star): V=" << size << endl;
        testNum++;
    }

    // 86-90: Disconnected components
    vector<int> disconnectedSizes = {150, 600, 1200, 3000, 4500};
    for (size_t idx = 0; idx < disconnectedSizes.size(); ++idx) {
        int size = disconnectedSizes[idx];
        string inputFile = "tests/" + to_string(testNum);
        string outputFile = "tests/" + to_string(testNum) + ".a";

        generateDisconnected(size, testNum, inputFile);
        runDijkstra(inputFile, outputFile);

        cout << "  Test " << testNum << " (disconnected): V=" << size << endl;
        testNum++;
    }

    // 91-95: Complete bipartite graphs
    vector<int> bipartiteSizes = {100, 200, 300, 400, 500};
    for (size_t idx = 0; idx < bipartiteSizes.size(); ++idx) {
        int size = bipartiteSizes[idx];
        string inputFile = "tests/" + to_string(testNum);
        string outputFile = "tests/" + to_string(testNum) + ".a";

        generateBipartite(size, testNum, inputFile);
        runDijkstra(inputFile, outputFile);

        cout << "  Test " << testNum << " (bipartite): V=" << size << endl;
        testNum++;
    }

    // 96-100: Edge cases
    cout << "  Generating edge cases (96-100)..." << endl;

    // Test 96: One vertex
    {
        ofstream out("tests/96");
        out << "1 0 0\n";
        out.close();
        runDijkstra("tests/96", "tests/96.a");
        cout << "  Test 96 (one vertex)" << endl;
    }

    // Test 97: Two vertices, one edge
    {
        ofstream out("tests/97");
        out << "2 1 0\n";
        out << "0 1 50000\n";
        out.close();
        runDijkstra("tests/97", "tests/97.a");
        cout << "  Test 97 (two vertices)" << endl;
    }

    // Test 98: Maximum weights
    {
        ofstream out("tests/98");
        out << "10 9 0\n";
        for (int i = 0; i < 9; ++i) {
            out << i << " " << (i + 1) << " 100000\n";
        }
        out.close();
        runDijkstra("tests/98", "tests/98.a");
        cout << "  Test 98 (max weights)" << endl;
    }

    // Test 99: Minimum weights
    {
        ofstream out("tests/99");
        out << "10 20 0\n";
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (i != j && (i + j) % 2 == 0) {
                    out << i << " " << j << " 1\n";
                }
            }
        }
        out.close();
        runDijkstra("tests/99", "tests/99.a");
        cout << "  Test 99 (min weights)" << endl;
    }

    // Test 100: Maximum size
    {
        generateGraph(5000, 50000, 100, "tests/100");
        runDijkstra("tests/100", "tests/100.a");
        cout << "  Test 100 (max size): V=5000, E=50000" << endl;
    }

    cout << "\nGeneration complete! Created 100 tests and 100 answer files." << endl;

    return 0;
}
