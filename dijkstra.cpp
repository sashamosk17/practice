#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18; // Бесконечность

void solve() {
    int v_count, e_count, start_v;
    // Read number of vertices, edges and start vertex
    if (!(cin >> v_count >> e_count >> start_v)) return;

    // Graph: vector of adjacency lists. pair<to, weight>
    vector<vector<pair<int, long long>>> adj(v_count);

    for (int i = 0; i < e_count; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    // Distance array
    vector<long long> dist(v_count, INF);
    dist[start_v] = 0;

    // Priority queue (min-heap). Stores pair<distance, vertex>
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start_v});

    while (!pq.empty()) {
        pair<long long, int> top = pq.top();
        pq.pop();
        long long current_dist = top.first;
        int u = top.second;

        // If we got an outdated record, skip it
        if (current_dist > dist[u]) continue;

        // Go through all neighbors
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i].first;
            long long weight = adj[u][i].second;

            // Edge relaxation
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    // Output result (distances to all vertices from 0 to V-1)
    for (int i = 0; i < v_count; ++i) {
        if (dist[i] == INF) {
            cout << -1 << " "; // If unreachable
        } else {
            cout << dist[i] << " ";
        }
    }
    cout << "\n";
}

int main() {
    // I/O optimization
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
