#include<bits/stdc++.h>
#include<algorithm>
#include<random>
#include<chrono>
#include<fstream>
using namespace std;
class Node {
public:
    bool isLeaf;
    int testAttributeNumber;
    int classValue;
    vector<Node*> branches;
    Node(bool isLeaf, int testAttributeNumber, int classValue) {
        this->isLeaf = isLeaf;
        this->testAttributeNumber = testAttributeNumber;
        this->classValue = classValue;
    }
};
int popularityValue(vector<vector<string>>& examples, vector<string>& classValues) {
    vector<int> sum(classValues.size());
    for (int i=0;i<sum.size();i++) {
        sum[i] = 0;
    }
    for (int i=0;i<examples.size();i++) {
        for (int j=0;j<classValues.size();j++) {
            if (examples[i][examples[i].size()-1]==classValues[j]) {
                sum[j]++;
                break;
            }
        }
    }
    int index = -1;
    int mx = -1;
    for (int i=0;i<sum.size();i++) {
        if (mx<sum[i]) {
            mx = sum[i];
            index = i;
        }
    }
    return index;
}
bool sameClassification(vector<vector<string>>& examples, vector<string>& classValues, int& classification) {
    vector<int> sum(classValues.size());
    for (int i=0;i<sum.size();i++) {
        sum[i] = 0;
    }
    for (int i=0;i<examples.size();i++) {
        for (int j=0;j<classValues.size();j++) {
            if (examples[i][examples[i].size()-1]==classValues[j]) {
                sum[j]++;
                break;
            }
        }
    }
    int cnt = 0;
    for (int i=0;i<sum.size();i++) {
        if (sum[i]>0) {
            cnt++;
            classification = i;
        }
    }
    bool r = false;
    if (cnt==1) r = true;
    return r;
}
bool isAttributeAvailable(vector<bool>& attributeAvailable) {
    int cnt = 0;
    for (int i=0;i<attributeAvailable.size();i++) {
        if (attributeAvailable[i]) cnt++;
    }
    bool r = false;
    if (cnt>0) r = true;
    return r;
}
int getImportantAttribute(vector<bool>& attributeAvailable, vector<vector<string>>& examples, vector<vector<string>>& values) {
    vector<double> informationGain(attributeAvailable.size());
    for (int i=0;i<informationGain.size();i++) {
        informationGain[i] = 0;
    }
    vector<double> sum(values[values.size()-1].size());
    for (int i=0;i<sum.size();i++) {
        sum[i] = 0;
    }
    for (int i=0;i<examples.size();i++) {
        for (int j=0;j<values[values.size()-1].size();j++) {
            if (examples[i][examples[i].size()-1]==values[values.size()-1][j]) {
                sum[j]++;
                break;
            }
        }
    }
    double total = 0;
    for (int i=0;i<sum.size();i++) {
        total += sum[i];
    }
    double entropy = 0;
    for (int i=0;i<values[values.size()-1].size();i++) {
        if (sum[i]/total==0) entropy -= 0;
        else entropy -= ((sum[i]/total)*(log2(sum[i]/total)));
    }
    for (int i=0;i<informationGain.size();i++) {
        if (attributeAvailable[i]) {
            vector<vector<vector<string>>> branchExample(values[i].size());
            for (int j=0;j<examples.size();j++) {
                for (int k=0;k<values[i].size();k++) {
                    if (examples[j][i]==values[i][k]) {
                        branchExample[k].push_back(examples[j]);
                        break;
                    }
                }
            }
            informationGain[i] = entropy;
            for (int j=0;j<branchExample.size();j++) {
                vector<double> sumChild(values[values.size()-1].size());
                for (int k=0;k<sumChild.size();k++) {
                    sumChild[k] = 0;
                }
                for (int k=0;k<branchExample[j].size();k++) {
                    for (int l=0;l<values[values.size()-1].size();l++) {
                        if (branchExample[j][k][branchExample[j][k].size()-1]==values[values.size()-1][l]) {
                            sumChild[l]++;
                            break;
                        }
                    }
                }
                double entropyChild = 0;
                if (branchExample[j].size()>0) {
                    for (int k=0;k<values[values.size()-1].size();k++) {
                        if ((sumChild[k]/((double)branchExample[j].size()))==0) entropyChild -= 0;
                        else entropyChild -= ((sumChild[k]/((double)branchExample[j].size()))*(log2(sumChild[k]/((double)branchExample[j].size()))));
                    }
                }
                informationGain[i] -= entropyChild*((double)branchExample[j].size()/total);
            }
        }
    }
    double mx = -1;
    int index = -1;
    for (int i=0;i<informationGain.size();i++) {
        if (attributeAvailable[i]) {
            if (informationGain[i]>mx) {
                mx = informationGain[i];
                index = i;
            }
        }
    }
    return index;
}
Node* decisionTreeLearning(vector<bool>& attributeAvailable, vector<vector<string>>& examples, vector<vector<string>>& parentExamples, vector<vector<string>>& values) {
    int classification = -1;
    if (examples.size()==0) {
        return new Node(true,-1,popularityValue(parentExamples,values[values.size()-1]));
    }
    else if (sameClassification(examples,values[values.size()-1],classification)) {
        return new Node(true,-1,classification);
    }
    else if (!isAttributeAvailable(attributeAvailable)) {
        return new Node(true,-1,popularityValue(examples,values[values.size()-1]));
    }
    else {
        int attribute = getImportantAttribute(attributeAvailable,examples,values);
        Node* tree = new Node(false,attribute,-1);
        for (int i=0;i<values[attribute].size();i++) {
            vector<bool> newAttributeAvailable = attributeAvailable;
            newAttributeAvailable[attribute] = false;
            vector<vector<string>> exs;
            for (int j=0;j<examples.size();j++) {
                if (examples[j][attribute]==values[attribute][i]) {
                    exs.push_back(examples[j]);
                }
            }
            tree->branches.push_back(decisionTreeLearning(newAttributeAvailable,exs,examples,values));
        }
        return tree;
    }
}
int evaluate(Node* tree, vector<vector<string>>& values, vector<string>& test) {
    if (tree->isLeaf) return tree->classValue;
    int index = -1;
    for (int i=0;i<values[tree->testAttributeNumber].size();i++) {
        if (test[tree->testAttributeNumber]==values[tree->testAttributeNumber][i]) {
            index = i;
            break;
        }
    }
    return evaluate(tree->branches[index],values,test);
}
int main ()
{
    ifstream in("car.data");
    vector<vector<string>> data;
    string s;
    while (!in.eof()) {
        vector<string> v;
        in>>s;
        string b = s;
        unsigned int pos= b.find(",");
        while (pos!=string::npos) {
            v.push_back(b.substr(0,pos));
            b = b.substr(pos+1,b.size()-pos-1);
            pos = b.find(",");
        }
        v.push_back(b);
        data.push_back(v);
    }
    in.close();
    int mx = -1;
    for (int i=0;i<data.size();i++) {
        mx = max((int)data[i].size(),mx);
    }
    vector<set<string>> uniqueValues(mx);
    for (int i=0;i<data.size();i++) {
        for (int j=0;j<data[i].size();j++) {
            uniqueValues[j].insert(data[i][j]);
        }
    }
    vector<vector<string>> values;
    for (int i=0;i<uniqueValues.size();i++) {
        vector<string> val;
        for (auto str: uniqueValues[i]) {
            val.push_back(str);
        }
        values.push_back(val);
    }
    /*
    values.push_back({"vhigh","high","med","low"});
    values.push_back({"vhigh","high","med","low"});
    values.push_back({"2","3","4","5more"});
    values.push_back({"2","4","more"});
    values.push_back({"small","med","big"});
    values.push_back({"low","med","high"});
    values.push_back({"unacc","acc","good","vgood"});
    */
    auto rng = default_random_engine(chrono::system_clock::now().time_since_epoch().count());
    int experimentNumber = 20;
    vector<double> accuracy(experimentNumber);
    double mean = 0;
    for (int k=0;k<experimentNumber;k++) {
        shuffle(data.begin(),data.end(),rng);
        vector<vector<string>> train,test;
        for (int i=0;i<data.size();i++) {
            if (i<((double)data.size())*.8) {
                train.push_back(data[i]);
            }
            else {
                test.push_back(data[i]);
            }
        }
        vector<bool> attributeAvailable(values.size()-1);
        for (int i=0;i<attributeAvailable.size();i++) {
            attributeAvailable[i] = true;
        }
        Node* tree = decisionTreeLearning(attributeAvailable,train,train,values);
        int correct = 0;
        for (int i=0;i<test.size();i++) {
            if (values[values.size()-1][evaluate(tree,values,test[i])]==test[i][test[i].size()-1]) {
                correct++;
            }
        }
        accuracy[k] = ((double)correct/(double)test.size())*100.0;
        mean += accuracy[k];
    }
    mean = mean/(double)experimentNumber;
    double standardDeviation;
    standardDeviation = 0;
    for (int i=0;i<experimentNumber;i++) {
        standardDeviation += (accuracy[i]-mean)*(accuracy[i]-mean);
    }
    standardDeviation = sqrt(standardDeviation/(double)experimentNumber);
    cout<<"Mean accuracy: "<<mean<<"%"<<endl;
    cout<<"Standard deviation of accuracy: "<<standardDeviation<<"%"<<endl;
    return 0;
}
