#include<bits/stdc++.h>
#include<fstream>
#include<ctime>
#include<cstdlib>
using namespace std;
int maxcut(vector<int>& added, vector<vector<pair<int,int>>>& adj) {
    int cutval = 0;
    for (int i=0;i<added.size();i++) {
        if (added[i]==1) {
            for (int j=0;j<adj[i].size();j++) {
                if (added[adj[i][j].first]==2) {
                    cutval+=adj[i][j].second;
                }
            }
        }
    }
    return cutval;
}
int semigreedy(double alpha, vector<int>& added, vector<vector<pair<int,int>>>& adj) {
    int n = adj.size();
    int mn = INT_MAX;
    int mx = -INT_MAX;
    for (int i=0;i<n;i++) {
        for (int j=0;j<adj[i].size();j++) {
            if (i<adj[i][j].first) {
                mn = min(mn,adj[i][j].second);
                mx = max(mx,adj[i][j].second);
            }
        }
    }
    vector<pair<int,int>> rlist;
    double cutoff = mn + alpha*(mx-mn);
    for (int i=0;i<n;i++) {
        for (int j=0;j<adj[i].size();j++) {
            if (i<adj[i][j].first && adj[i][j].second>=cutoff) {
                rlist.push_back(make_pair(i,adj[i][j].first));
            }
        }
    }
    int choice = rand()%rlist.size();
    for (int i=0;i<n;i++) added[i] = 0;
    int cnt = 2;
    added[rlist[choice].first] = 1;
    added[rlist[choice].second] = 2;
    while (cnt<n) {
        vector<pair<int,int>> sigmaX,sigmaY;
        for (int i=0;i<n;i++) {
            if (added[i]==0) {
                int val = 0;
                for (int j=0;j<adj[i].size();j++) {
                    if (added[adj[i][j].first]==2) {
                        val+=adj[i][j].second;
                    }
                }
                sigmaX.push_back(make_pair(val,i));
                val = 0;
                for (int j=0;j<adj[i].size();j++) {
                    if (added[adj[i][j].first]==1) {
                        val+=adj[i][j].second;
                    }
                }
                sigmaY.push_back(make_pair(val,i));
            }
        }
        mn = INT_MAX;
        mx = -INT_MAX;
        for (int i=0;i<sigmaX.size();i++) {
            mn = min(mn,min(sigmaX[i].first,sigmaY[i].first));
            mx = max(mx,max(sigmaX[i].first,sigmaY[i].first));
        }
        cutoff = mn + alpha*(mx-mn);
        vector<pair<int,int>> nrlist;
        for (int i=0;i<sigmaX.size();i++) {
            if (max(sigmaX[i].first,sigmaY[i].first)>=cutoff) {
                if (sigmaX[i].first>sigmaY[i].first) {
                    nrlist.push_back(make_pair(1,sigmaX[i].second));
                }
                else {
                    nrlist.push_back(make_pair(2,sigmaY[i].second));
                }
            }
        }
        choice = rand()%nrlist.size();
        if (nrlist[choice].first==1) {
            added[nrlist[choice].second] = nrlist[choice].first;
        }
        else if (nrlist[choice].first==2) {
            added[nrlist[choice].second] = nrlist[choice].first;
        }
        cnt++;
    }
    return maxcut(added,adj);
}
int localsearch(vector<int>& added, vector<vector<pair<int,int>>>& adj,int &iterationCount) {
    int n = adj.size();
    bool change = true;
    iterationCount = 0;
    while (change) {
        change = false;
        for (int i=0;i<n;i++) {
            int sigX,sigY;
            sigX = sigY = 0;
            for (int j=0;j<adj[i].size();j++) {
                if (adj[i][j].first==i) continue;
                if (added[adj[i][j].first]==2) {
                    sigX+=adj[i][j].second;
                }
                else if (added[adj[i][j].first]==1) {
                    sigY+=adj[i][j].second;
                }
            }
            if (added[i]==1 && sigY-sigX>0) {
                added[i] = 2;
                change = true;
                break;
            }
            else if (added[i]==2 && sigX-sigY>0) {
                added[i] = 1;
                change = true;
                break;
            }
        }
        iterationCount++;
    }
    return maxcut(added,adj);
}
int grasp(int max_iteration, double alpha, vector<int>& best_added, vector<vector<pair<int,int>>>& adj, int &localSearchAvgIteration,int &localSearchAvgBestValue) {
    int n = adj.size();
    int best_maxcut = 0;
    localSearchAvgIteration = 0;
    localSearchAvgBestValue = 0;
    for (int i=0;i<max_iteration;i++) {
        vector<int> added(n);
        int cnt = 0;
        semigreedy(alpha,added,adj);
        int r = localsearch(added,adj,cnt);
        if (i==1) {
            best_maxcut = r;
            best_added = added;
        }
        else {
            if (r>best_maxcut) {
                best_maxcut = r;
                best_added = added;
            }
        }
        localSearchAvgIteration += cnt;
        localSearchAvgBestValue += r;
    }
    localSearchAvgBestValue/=max_iteration;
    localSearchAvgIteration/=max_iteration;
    return best_maxcut;
}
int main ()
{
    srand(time(0));
    bool bestValueKnown[54];
    int knownBestValue[54];
    for (int i=0;i<54;i++) bestValueKnown[i] = false;
    bestValueKnown[0] = bestValueKnown[1] = bestValueKnown[2] = bestValueKnown[10] = bestValueKnown[11] =bestValueKnown[12] = true;
    bestValueKnown[13] = bestValueKnown[14] = bestValueKnown[15] = bestValueKnown[21] = bestValueKnown[22] = bestValueKnown[23] = true;
    bestValueKnown[31] = bestValueKnown[32] = bestValueKnown[33] = bestValueKnown[34] = bestValueKnown[35] = bestValueKnown[36] = true;
    bestValueKnown[42] = bestValueKnown[43] = bestValueKnown[44] = bestValueKnown[47] = bestValueKnown[48] = bestValueKnown[49] = true;

    knownBestValue[0] = 12078; knownBestValue[1] = 12084; knownBestValue[2] = 12077; knownBestValue[10] = 627; knownBestValue[11] = 621; knownBestValue[12] = 645;
    knownBestValue[13] = 3187; knownBestValue[14] = 3169; knownBestValue[15] = 3172; knownBestValue[21] = 14123; knownBestValue[22] = 14129; knownBestValue[23] = 14131;
    knownBestValue[31] = 1560; knownBestValue[32] = 1537; knownBestValue[33] = 1541; knownBestValue[34] = 8000; knownBestValue[35] = 7996; knownBestValue[36] = 8009;
    knownBestValue[42] = 7027; knownBestValue[43] = 7022; knownBestValue[44] = 7020; knownBestValue[47] = 6000; knownBestValue[48] = 6000; knownBestValue[49] = 5988;
    ofstream out("summary.csv");
    cout.rdbuf(out.rdbuf());
    cout<<"Problem,,,,Constructive algorithm,,,,Local search,,GRASP,,Known best solution or upper bound"<<endl;
    cout<<"Name,|V| or n,|E| or m,Weight Range,Simple Randomized,Simple Greedy,Semi Greedy,,Simple Local,,GRASP-1,,"<<endl;
    cout<<",,,,,,alpha = 0.6,alpha = 0.8,No. of iterations,Best value,No. of iterations,Best value,"<<endl;
    for(int number=1;number<=54;number++) {
        string filename = "set1/g"+to_string(number)+".rud";
        ifstream in(filename);
        if (!in.is_open()) continue;
        cin.rdbuf(in.rdbuf());
        string name = "G"+to_string(number);
        int n,m;
        cin>>n>>m;
        vector<vector<pair<int,int>>> adj(n);
        int u,v,c;
        for (int i=0;i<m;i++) {
            cin>>u>>v>>c;
            u--;v--;
            adj[u].push_back(make_pair(v,c));
            adj[v].push_back(make_pair(u,c));
        }
        double alpha = 1;
        int mn = INT_MAX;
        int mx = -INT_MAX;
        for (int i=0;i<n;i++) {
            for (int j=0;j<adj[i].size();j++) {
                if (i<adj[i][j].first) {
                    mn = min(mn,adj[i][j].second);
                    mx = max(mx,adj[i][j].second);
                }
            }
        }
        cout<<name<<","<<n<<","<<m<<",\"["<<mn<<","<<mx<<"]\",";
        vector<int> added(n);
        // randomized
        int step = 50;
        int total = 0;
        for (int i=0;i<step;i++) {
            for (int i=0;i<added.size();i++) {
                added[i] = rand()%2+1;
            }
            total += maxcut(added,adj);
        }
        cout<<total/step<<",";
        //greedy
        alpha = 1;
        cout<<semigreedy(alpha,added,adj)<<",";
        //semi greedy
        alpha = 0.6;
        step = 20;
        total = 0;
        for (int i=0;i<step;i++) {
            total += semigreedy(alpha,added,adj);
        }
        cout<<total/step<<",";
        alpha = 0.8;
        step = 20;
        total = 0;
        for (int i=0;i<step;i++) {
            total += semigreedy(alpha,added,adj);
        }
        cout<<total/step<<",";
        // local search and GRASP
        int max_iteration = 50;
        int localSearchAvgIteration, localSearchAvgBestValue;
        localSearchAvgBestValue = localSearchAvgIteration = 0;
        alpha = 0.6;
        int val = grasp(max_iteration,alpha,added,adj,localSearchAvgIteration,localSearchAvgBestValue);
        cout<<localSearchAvgIteration<<","<<localSearchAvgBestValue<<",";
        cout<<max_iteration<<","<<val<<",";
        if (!bestValueKnown[number-1]) cout<<"Unknown"<<endl;
        else cout<<knownBestValue[number-1]<<endl;
        in.close();
    }
    out.close();
    return 0;
}
