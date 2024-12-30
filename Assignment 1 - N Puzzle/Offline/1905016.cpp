#include<bits/stdc++.h>
using namespace std;
class Puzzle {
public:
    int childcount;
    int dis;
    Puzzle* parent;
    int n;
    int** board;
    int zeroRow;
    int zeroCol;
    int fn;
    Puzzle(int n, int** a, Puzzle* parent, int dis, int direction) {
        this->childcount = 0;
        this->n = n;
        this->parent = parent;
        this->dis = dis;
        this->fn = 0;
        board = new int*[n];
        for (int i=0;i<n;i++) {
            board[i] = new int[n];
        }
        for (int i=0;i<n;i++) {
            for (int j=0;j<n;j++) {
                if (a[i][j]==0) {
                    this->zeroRow = i;
                    this->zeroCol = j;
                }
                board[i][j] = a[i][j];
            }
        }
        if (direction==1) {
            board[zeroRow][zeroCol] = board[zeroRow+1][zeroCol];
            board[zeroRow+1][zeroCol] = 0;
            zeroRow = zeroRow + 1;
        }
        else if (direction==2) {
            board[zeroRow][zeroCol] = board[zeroRow-1][zeroCol];
            board[zeroRow-1][zeroCol] = 0;
            zeroRow = zeroRow - 1;
        }
        else if (direction==3) {
            board[zeroRow][zeroCol] = board[zeroRow][zeroCol+1];
            board[zeroRow][zeroCol+1] = 0;
            zeroCol = zeroCol + 1;
        }
        else if (direction==4) {
            board[zeroRow][zeroCol] = board[zeroRow][zeroCol-1];
            board[zeroRow][zeroCol-1] = 0;
            zeroCol = zeroCol - 1;
        }
    }
    ~Puzzle() {
        for (int i=0;i<n;i++) {
            delete [] board[i];
        }
        delete [] board;
    }
    void setfnHamming() {
        int total = this->dis;
        for (int i=0;i<n*n;i++) {
            if (board[i/n][i%n]==0) continue;
            if (board[i/n][i%n]!=(i+1)%(n*n)) {
                total += 1;
            }
        }
        fn = total;
    }
    void setfnManhattan() {
        int total = this->dis;
        for (int i=0;i<n*n;i++) {
            if (board[i/n][i%n]==0) continue;
            total+= abs(i/n-(board[i/n][i%n]-1)/n)+abs(i%n-(board[i/n][i%n]-1)%n);
        }
        fn = total;
    }
    void setfn(bool isManhattan) {
        if (isManhattan) {
            setfnManhattan();
        }
        else {
            setfnHamming();
        }
    }
};
class Compare {
public:
    bool operator() (Puzzle *a, Puzzle *b) {
        if (a->fn>b->fn) return true;
        return false;
    }
};
void printfromparent(Puzzle* p) {
    if (p==nullptr) return;
    printfromparent(p->parent);
    for (int i=0;i<p->n;i++) {
        for (int j=0;j<p->n;j++) {
            cout<<p->board[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}
void freechain(Puzzle* p) {
    if (p==nullptr) return;
    else if (p->childcount>1) p->childcount--;
    else {
        Puzzle* parent = p->parent;
        delete p;
        freechain(parent);
    }
}
int main ()
{
    int n;
    cin>>n;
    int** a = new int*[n];
    for (int i=0;i<n;i++) a[i] = new int[n];
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            cin>>a[i][j];
        }
    }
    // check if solvable
    int inversion = 0, zero;
    for (int i=0;i<n*n;i++) {
        for (int j=i+1;j<n*n;j++) {
            if (a[i/n][i%n]!=0 && a[j/n][j%n]!=0 && a[i/n][i%n]>a[j/n][j%n]) {
                inversion++;
            }
        }
        if (a[i/n][i%n]==0) {
            zero = n-1-i/n;
        }
    }
    if (n&1 && inversion&1) {
        cout<<"Unsolvable puzzle"<<endl;
    }
    else if (n%2==0 && (inversion+zero)&1) {
        cout<<"Unsolvable puzzle"<<endl;
    }
    else {
        // solve
        int explored, expanded;
        bool isManhattan;
        priority_queue<Puzzle*,vector<Puzzle*>,Compare> q;
        // manhattan
        isManhattan = true;
        q.push(new Puzzle(n,a,nullptr,0,0));
        expanded=0;
        explored=1;
        while (!q.empty()) {
            Puzzle* r = q.top();
            q.pop();
            // check if solved
            bool ok = true;
            for (int i=0;i<n*n;i++) {
                if (r->board[i/r->n][i%r->n]!=(i+1)%(r->n*r->n)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                //cout<<"Explored: "<<explored<<"\tExpanded: "<<expanded<<endl;
                cout<<"Minimum number of moves = "<<r->dis<<endl;
                cout<<endl;
                printfromparent(r);
                freechain(r);
                break;
            }
            if (r->zeroRow+1<r->n && (r->parent==nullptr || r->parent->zeroRow!=r->zeroRow+1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,1);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            if (r->zeroRow-1>=0 && (r->parent==nullptr || r->parent->zeroRow!=r->zeroRow-1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,2);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            if (r->zeroCol+1<r->n && (r->parent==nullptr || r->parent->zeroCol!=r->zeroCol+1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,3);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            if (r->zeroCol-1>=0 && (r->parent==nullptr || r->parent->zeroCol!=r->zeroCol-1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,4);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            expanded+=1;
        }
        while (!q.empty()) {
            Puzzle *p = q.top();
            q.pop();
            freechain(p);
        }

        /*
        // hamming
        q = priority_queue<Puzzle*,vector<Puzzle*>,Compare>();
        isManhattan = false;
        q.push(new Puzzle(n,a,nullptr,0,0));
        expanded=0;
        explored=1;
        while (!q.empty()) {
            Puzzle* r = q.top();
            q.pop();
            // check if solved
            bool ok = true;
            for (int i=0;i<n*n;i++) {
                if (r->board[i/r->n][i%r->n]!=(i+1)%(r->n*r->n)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                //cout<<"Explored: "<<explored<<"\tExpanded: "<<expanded<<endl;
                cout<<"Minimum number of moves = "<<r->dis<<endl;
                cout<<endl;
                printfromparent(r);
                freechain(r);
                break;
            }
            if (r->zeroRow+1<r->n && (r->parent==nullptr || r->parent->zeroRow!=r->zeroRow+1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,1);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            if (r->zeroRow-1>=0 && (r->parent==nullptr || r->parent->zeroRow!=r->zeroRow-1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,2);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            if (r->zeroCol+1<r->n && (r->parent==nullptr || r->parent->zeroCol!=r->zeroCol+1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,3);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            if (r->zeroCol-1>=0 && (r->parent==nullptr || r->parent->zeroCol!=r->zeroCol-1)) {
                Puzzle* temp = new Puzzle(r->n,r->board,r,r->dis+1,4);
                temp->setfn(isManhattan);
                q.push(temp);
                explored+=1;
                r->childcount++;
            }
            expanded+=1;
        }
        while (!q.empty()) {
            Puzzle *p = q.top();
            q.pop();
            freechain(p);
        }
        */
    }
    for (int i=0;i<n;i++) {
        delete [] a[i];
    }
    delete [] a;
    return 0;
}

