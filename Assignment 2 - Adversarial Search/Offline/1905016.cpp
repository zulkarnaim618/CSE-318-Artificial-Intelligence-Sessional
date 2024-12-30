#include<bits/stdc++.h>
using namespace std;
class Mancala {
public:
    int* board;
    int size;
    int initialPiece;
    int playerCount;
    int currentPlayer;
    int* additionalMovesEarned;
    int* stonesCaptured;
    Mancala() {
        size = 6;
        initialPiece = 4;
        playerCount = 2;
        currentPlayer = 0;
        additionalMovesEarned = new int[playerCount];
        stonesCaptured = new int[playerCount];
        board = new int[playerCount*size+playerCount];
        for (int i=0;i<playerCount*size+playerCount;i++) {
            if (i%(size+1)==size) board[i] = 0;
            else board[i] = initialPiece;
        }
        for (int i=0;i<playerCount;i++) {
            additionalMovesEarned[i] = 0;
            stonesCaptured[i] = 0;
        }
    }
    Mancala(Mancala& mancala) {
        this->size = mancala.size;
        this->initialPiece = mancala.initialPiece;
        this->playerCount = mancala.playerCount;
        this->currentPlayer = mancala.currentPlayer;
        this->additionalMovesEarned = new int[playerCount];
        this->stonesCaptured = new int[playerCount];
        this->board = new int[playerCount*size+playerCount];
        for (int i=0;i<playerCount*size+playerCount;i++) {
            this->board[i] = mancala.board[i];
        }
        for (int i=0;i<playerCount;i++) {
            this->additionalMovesEarned[i] = mancala.additionalMovesEarned[i];
            this->stonesCaptured[i] = mancala.stonesCaptured[i];
        }
    }
    Mancala(Mancala* mancala) {
        this->size = mancala->size;
        this->initialPiece = mancala->initialPiece;
        this->playerCount = mancala->playerCount;
        this->currentPlayer = mancala->currentPlayer;
        this->additionalMovesEarned = new int[playerCount];
        this->stonesCaptured = new int[playerCount];
        this->board = new int[playerCount*size+playerCount];
        for (int i=0;i<playerCount*size+playerCount;i++) {
            this->board[i] = mancala->board[i];
        }
        for (int i=0;i<playerCount;i++) {
            this->additionalMovesEarned[i] = mancala->additionalMovesEarned[i];
            this->stonesCaptured[i] = mancala->stonesCaptured[i];
        }
    }
    ~Mancala() {
        delete [] board;
        delete [] additionalMovesEarned;
        delete [] stonesCaptured;
    }
    bool isGameOver() {
        for (int i=0;i<playerCount;i++) {
            bool over = true;
            for (int j=0;j<size;j++) {
                if (board[i*(size+1)+j]!=0) {
                    over = false;
                    break;
                }
            }
            if (over) return over;
        }
        return false;
    }
    void move(int cell) {
        // cell from 1 to size
        cell--;
        if (cell<0 || cell>size-1 || board[currentPlayer*(size+1)+cell]==0) {
            cout<<"Not a valid cell to move"<<endl;
            return;
        }
        int temp = board[currentPlayer*(size+1)+cell];
        board[currentPlayer*(size+1)+cell] = 0;
        int i=0;
        int t=0;
        while (temp>0) {
            i++;
            t = (currentPlayer*(size+1)+cell+i)%(playerCount*size+playerCount);
            if (t%(size+1)==size && t/(size+1)!=currentPlayer) continue;
            board[t]++;
            temp--;
        }
        if (t/(size+1)!=currentPlayer) currentPlayer=(currentPlayer+1)%playerCount;
        else if (t%(size+1)!=size) {
            if (board[t]==1) {
                int sum = 1;
                for (int j=0;j<playerCount;j++) {
                    if (j==currentPlayer) continue;
                    sum += board[j*(size+1)+(size-1-(t%(size+1)))];
                    board[j*(size+1)+(size-1-(t%(size+1)))] = 0;
                }
                if (sum>1) {
                    stonesCaptured[currentPlayer] += sum;
                    board[currentPlayer*(size+1)+size] += sum;
                    board[t] = 0;
                }
            }
            currentPlayer=(currentPlayer+1)%playerCount;
        }
        else {
            additionalMovesEarned[currentPlayer]++;
        }
    }
    int h(int htype) {
        if (htype==1) return h1();
        else if (htype==2) return h2();
        else if (htype==3) return h3();
        else if (htype==4) return h4();
    }
    int h1() {
        return board[0*(size+1)+size]-board[1*(size+1)+size];
    }
    int h2() {
        int s1 = 0;
        int player = 0;
        for (int i=0;i<size;i++) s1+=board[player*(size+1)+i];
        int s2 = 0;
        player = 1;
        for (int i=0;i<size;i++) s2+=board[player*(size+1)+i];
        int w1,w2;
        w1 = 5;
        w2 = 2;
        return w1*(board[0*(size+1)+size]-board[1*(size+1)+size])+w2*(s1-s2);
    }
    int h3() {
        int s1 = 0;
        int player = 0;
        for (int i=0;i<size;i++) s1+=board[player*(size+1)+i];
        int s2 = 0;
        player = 1;
        for (int i=0;i<size;i++) s2+=board[player*(size+1)+i];
        int w1,w2,w3;
        w1 = 5;
        w2 = 2;
        w3 = 3;
        return w1*(board[0*(size+1)+size]-board[1*(size+1)+size])+w2*(s1-s2)+w3*additionalMovesEarned[0];
    }
    int h4() {
        int s1 = 0;
        int player = 0;
        for (int i=0;i<size;i++) s1+=board[player*(size+1)+i];
        int s2 = 0;
        player = 1;
        for (int i=0;i<size;i++) s2+=board[player*(size+1)+i];
        int w1,w2,w3,w4;
        w1 = 5;
        w2 = 2;
        w3 = 3;
        w4 = 4;
        return w1*(board[0*(size+1)+size]-board[1*(size+1)+size])+w2*(s1-s2)+w3*additionalMovesEarned[0]+w4*stonesCaptured[0];
    }
    friend ostream& operator<<(ostream& out, Mancala& mancala);
};
ostream& operator<<(ostream& out, Mancala& mancala) {
    out<<"\t\t\tPlayer 2"<<endl;
    out<<"Index:\t";
    for (int i=mancala.size;i>0;i--) out<<i<<"\t";
    out<<endl;
    out<<"\t";
    for (int i=mancala.size-1;i>=0;i--) out<<mancala.board[1*(mancala.size+1)+i]<<"\t";
    out<<endl;
    out<<mancala.board[1*(mancala.size+1)+mancala.size]<<"\t";
    for (int i=mancala.size;i>0;i--) out<<"\t";
    out<<mancala.board[0*(mancala.size+1)+mancala.size]<<endl;
    out<<"\t";
    for (int i=0;i<mancala.size;i++) out<<mancala.board[0*(mancala.size+1)+i]<<"\t";
    out<<endl;
    out<<"Index:\t";
    for (int i=1;i<=mancala.size;i++) out<<i<<"\t";
    out<<endl;
    out<<"\t\t\tPlayer 1"<<endl;
    return out;
}
int adversarial(int depthLimit, int alpha, int beta, Mancala* mancala, int htype) {
    if (depthLimit==0 || mancala->isGameOver()) {
        return mancala->h(htype);
    }
    if (mancala->currentPlayer==0) {
        for (int i=0;i<mancala->size;i++) {
            if (mancala->board[mancala->currentPlayer*(mancala->size+1)+i]!=0) {
                Mancala* temp = new Mancala(mancala);
                temp->move(i+1);
                alpha = max(alpha,adversarial(depthLimit-1,alpha,beta,temp,htype));
                delete temp;
                if (alpha>=beta) {
                    return alpha;
                }
            }
        }
        return alpha;
    }
    else if (mancala->currentPlayer==1) {
        for (int i=0;i<mancala->size;i++) {
            if (mancala->board[mancala->currentPlayer*(mancala->size+1)+i]!=0) {
                Mancala* temp = new Mancala(mancala);
                temp->move(i+1);
                beta = min(beta,adversarial(depthLimit-1,alpha,beta,temp,htype));
                delete temp;
                if (beta<=alpha) {
                    return beta;
                }
            }
        }
        return beta;
    }
}
int main () {
    Mancala* mancala = new Mancala();
    int player1depth,player2depth,player1heuristic,player2heuristic;
    player1depth = 9;
    player2depth = 9;
    player1heuristic = 3;
    player2heuristic = 3;
    while (!mancala->isGameOver()) {
        cout<<*mancala<<endl;
        cout<<"Player "<<mancala->currentPlayer+1<<" move: ";
        if (mancala->currentPlayer==1) {
            int index = -1;
            int bestVal = INT_MAX;
            int currentVal;
            for (int i=0;i<mancala->size;i++) {
                if (mancala->board[mancala->currentPlayer*(mancala->size+1)+i]!=0) {
                    Mancala* temp = new Mancala(mancala);
                    temp->move(i+1);
                    currentVal = adversarial(player2depth,-INT_MAX,bestVal,temp,player2heuristic);
                    delete temp;
                    if (currentVal<bestVal) {
                        bestVal = currentVal;
                        index = i+1;
                    }
                }
            }
            cout<<index<<endl;
            mancala->move(index);
        }
        else {
            int index = -1;
            int bestVal = -INT_MAX;
            int currentVal;
            for (int i=0;i<mancala->size;i++) {
                if (mancala->board[mancala->currentPlayer*(mancala->size+1)+i]!=0) {
                    Mancala* temp = new Mancala(mancala);
                    temp->move(i+1);
                    currentVal = adversarial(player1depth,bestVal,INT_MAX,temp,player1heuristic);
                    delete temp;
                    if (currentVal>bestVal) {
                        bestVal = currentVal;
                        index = i+1;
                    }
                }
            }
            cout<<index<<endl;
            mancala->move(index);
        }
    }
    cout<<*mancala<<endl;
    int total1 = 0;
    for (int i=0;i<mancala->size+1;i++) total1+=mancala->board[0*(mancala->size+1)+i];
    int total2 = 0;
    for (int i=0;i<mancala->size+1;i++) total2+=mancala->board[1*(mancala->size+1)+i];
    if (total1>total2) {
        cout<<"Player 1 won"<<endl;
        cout<<"Player 1 score: "<<total1<<endl;
        cout<<"Player 2 score: "<<total2<<endl;
    }
    else if (total1<total2) {
        cout<<"Player 2 won"<<endl;
        cout<<"Player 1 score: "<<total1<<endl;
        cout<<"Player 2 score: "<<total2<<endl;
    }
    else {
        cout<<"Game draw"<<endl;
        cout<<"Player 1 score: "<<total1<<endl;
        cout<<"Player 2 score: "<<total2<<endl;
    }
    delete mancala;
    return 0;
}
