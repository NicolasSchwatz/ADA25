// Autor: [TU NOMBRE] - [TU DNI/NIE]
// maze_bb.cc - Práctica ADA - Ramificación y poda
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int r, c;
    int cost;
    int f;
    bool operator<(const Node& o) const {
        return f > o.f; // menor f (coste + heurística) tiene prioridad
    }
};

const int dr[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dc[8] = {0, 1, 1, 1, 0, -1, -1, -1};

vector<pair<int,int>> maze_bb(const vector<vector<int>>& grid, vector<unsigned long long>& stats) {
    int n = grid.size();
    int m = grid[0].size();
    stats.assign(8, 0);

    if (grid[0][0] == 0 || grid[n-1][m-1] == 0) return {};

    auto heuristic = [&](int r, int c) {
        return max(abs(r - (n - 1)), abs(c - (m - 1))); // distancia Chebyshev
    };

    vector<vector<int>> visited(n, vector<int>(m, INT_MAX));
    vector<vector<pair<int,int>>> parent(n, vector<pair<int,int>>(m, {-1, -1}));
    priority_queue<Node> pq;
    pq.push({0, 0, 1, 1 + heuristic(0, 0)});
    visited[0][0] = 1;
    stats[1]++; // nexplored

    int best_cost = INT_MAX;
    bool found = false;

    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        stats[0]++; // nvisit

        if (cur.r == n-1 && cur.c == m-1) {
            if (cur.cost < best_cost) {
                best_cost = cur.cost;
                stats[6]++; // best updated from leaf
            }
            found = true;
            continue;
        }

        bool has_child = false;
        for (int k = 0; k < 8; k++) {
            int nr = cur.r + dr[k];
            int nc = cur.c + dc[k];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m || grid[nr][nc] == 0) {
                stats[3]++; // nunfeasible
                continue;
            }
            int new_cost = cur.cost + 1;
            int est_total = new_cost + heuristic(nr, nc);
            if (est_total >= best_cost) {
                stats[4]++; // not promising
                continue;
            }
            if (new_cost >= visited[nr][nc]) {
                stats[5]++; // promising but discarded
                continue;
            }
            has_child = true;
            visited[nr][nc] = new_cost;
            parent[nr][nc] = {cur.r, cur.c};
            pq.push({nr, nc, new_cost, est_total});
            stats[1]++; // nexplored
        }
        if (!has_child) stats[2]++; // nleaf
    }

    vector<pair<int,int>> path;
    if (found) {
        int r = n - 1, c = m - 1;
        while (r != -1 && c != -1) {
            path.emplace_back(r, c);
            tie(r, c) = parent[r][c];
        }
        reverse(path.begin(), path.end());
    }
    return path;
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool opt_p = false, opt_p2D = false;
    string filename = "";
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-p") opt_p = true;
        else if (arg == "--p2D") opt_p2D = true;
        else if (arg == "-f" && i + 1 < argc) filename = argv[++i];
        else { cerr << "Usage: maze_bb [ -p] [ --p2D] -f fichero_entrada\n"; return 1; }
    }
    if (filename.empty()) {
        cerr << "Usage: maze_bb [ -p] [ --p2D] -f fichero_entrada\n";
        return 1;
    }

    ifstream fin(filename);
    if (!fin) { cerr << "Error al abrir el fichero\n"; return 1; }
    int n, m; fin >> n >> m;
    vector<vector<int>> grid(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fin >> grid[i][j];

    auto start = chrono::high_resolution_clock::now();
    vector<unsigned long long> stats;
    vector<pair<int,int>> path = maze_bb(grid, stats);
    auto end = chrono::high_resolution_clock::now();
    double time_ms = chrono::duration<double, milli>(end - start).count();

    size_t length = path.empty() ? 0 : path.size();
    cout << length << "\n";
    for (size_t i = 0; i < stats.size(); i++) {
        if (i) cout << ' ';
        cout << stats[i];
    }
    cout << "\n" << fixed << setprecision(3) << time_ms << "\n";

    if (opt_p2D) {
        if (path.empty()) cout << "0\n";
        else {
            vector<string> board(n, string(m, '0'));
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    board[i][j] = grid[i][j] ? '1' : '0';
            for (auto [r, c] : path) board[r][c] = '*';
            for (int i = 0; i < n; i++) cout << board[i] << "\n";
        }
    }

    if (opt_p) {
        if (path.empty()) cout << "<0>\n";
        else {
            string seq = "<";
            for (size_t i = 1; i < path.size(); i++) {
                int drp = path[i].first - path[i - 1].first;
                int dcp = path[i].second - path[i - 1].second;
                for (int k = 0; k < 8; k++)
                    if (dr[k] == drp && dc[k] == dcp) { seq += char('1' + k); break; }
            }
            seq += '>';
            cout << seq << "\n";
        }
    }

    return 0;
}

