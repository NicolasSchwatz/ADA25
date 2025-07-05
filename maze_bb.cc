// OpenAI - 00000000A
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int r, c;
};

// Directions: N, NE, E, SE, S, SW, W, NW
const int dr[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dc[8] = {0, 1, 1, 1, 0, -1, -1, -1};

vector<pair<int,int>> maze_bb(const vector<vector<int>>& grid,
                              vector<unsigned long long>& stats) {
    int n = grid.size();
    int m = grid[0].size();
    stats.assign(8, 0);

    if (grid[0][0] == 0 || grid[n-1][m-1] == 0)
        return {};

    queue<Node> q;
    vector<vector<bool>> visited(n, vector<bool>(m,false));
    vector<vector<pair<int,int>>> parent(n, vector<pair<int,int>>(m,{-1,-1}));

    q.push({0,0});
    visited[0][0] = true;
    stats[1]++; // nexplored

    bool found=false;
    while(!q.empty() && !found){
        Node cur = q.front();
        q.pop();
        stats[0]++; // nvisit
        bool has_child=false;
        if(cur.r==n-1 && cur.c==m-1){
            stats[2]++; // nleaf
            stats[6]++; // best from leaf
            found=true;
            break;
        }
        for(int k=0;k<8;k++){
            int nr=cur.r+dr[k];
            int nc=cur.c+dc[k];
            if(nr<0 || nr>=n || nc<0 || nc>=m || grid[nr][nc]==0 || visited[nr][nc]){
                stats[3]++; // nunfeasible
                continue;
            }
            has_child=true;
            visited[nr][nc]=true;
            parent[nr][nc]={cur.r,cur.c};
            q.push({nr,nc});
            stats[1]++; // nexplored
        }
        if(!has_child) stats[2]++; // nleaf
    }

    vector<pair<int,int>> path;
    if(found){
        int r=n-1,c=m-1;
        while(r!=-1 && c!=-1){
            path.push_back({r,c});
            auto p=parent[r][c];
            r=p.first; c=p.second;
        }
        reverse(path.begin(), path.end());
    }
    return path;
}

int main(int argc, char* argv[]){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool opt_p=false, opt_p2D=false;
    string filename="";
    for(int i=1;i<argc;i++){
        string arg=argv[i];
        if(arg=="-p") opt_p=true;
        else if(arg=="--p2D") opt_p2D=true;
        else if(arg=="-f" && i+1<argc) { filename=argv[++i]; }
        else { cerr << "Usage: maze_bb [ -p] [ --p2D] -f fichero_entrada\n"; return 1; }
    }
    if(filename.empty()){ cerr << "Usage: maze_bb [ -p] [ --p2D] -f fichero_entrada\n"; return 1; }

    ifstream fin(filename);
    if(!fin){ cerr << "Error al abrir el fichero\n"; return 1; }
    int n,m; if(!(fin>>n>>m)){ cerr << "Error en el fichero\n"; return 1; }
    vector<vector<int>> grid(n, vector<int>(m));
    for(int i=0;i<n;i++) for(int j=0;j<m;j++) fin>>grid[i][j];

    auto start=chrono::high_resolution_clock::now();
    vector<unsigned long long> stats;
    vector<pair<int,int>> path = maze_bb(grid, stats);
    auto end=chrono::high_resolution_clock::now();
    double time_ms = chrono::duration<double, milli>(end-start).count();

    size_t length = path.empty()?0:path.size();
    cout<<length<<"\n";
    for(size_t i=0;i<stats.size();i++){ if(i) cout<<' '; cout<<stats[i]; }
    cout<<"\n"<<fixed<<setprecision(3)<<time_ms<<"\n";

    if(opt_p2D){
        if(path.empty()){
            cout<<"0\n";
        }else{
            vector<string> board(n,string(m,'0'));
            for(int i=0;i<n;i++) for(int j=0;j<m;j++) board[i][j]=grid[i][j]?'1':'0';
            for(auto [r,c]: path) board[r][c]='*';
            for(int i=0;i<n;i++) cout<<board[i]<<"\n";
        }
    }
    if(opt_p){
        if(path.empty()){
            cout<<"<0>\n";
        }else{
            string seq="<";
            for(size_t i=1;i<path.size();i++){
                int drp=path[i].first-path[i-1].first;
                int dcp=path[i].second-path[i-1].second;
                for(int k=0;k<8;k++) if(dr[k]==drp && dc[k]==dcp){ seq+=char('1'+k); break; }
            }
            seq+='>';
            cout<<seq<<"\n";
        }
    }

    return 0;
}

