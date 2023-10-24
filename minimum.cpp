#include <bits/stdc++.h>
using namespace std;

class bank
{
public:
    string name;
    int netAmount;
    set<string> types;
};

// function to calculate the minimum flow
void minimizeCashFlow(int numBanks,bank input[],unordered_map<string,int>& indexOf,int numTransactions,vector<vector<int>>& graph,int maxNumTypes){
    
    //Find net amount of each bank has
    bank listOfNetAmounts[numBanks];
    
    for(int b=0;b<numBanks;b++){
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].types = input[b].types;
        
        int amount = 0;
        //incoming edges
        //column travers
        for(int i=0;i<numBanks;i++){
            amount += (graph[i][b]);
        }
        
        //outgoing edges
        //row traverse
        for(int j=0;j<numBanks;j++){
            amount += ((-1) * graph[b][j]);
        }
        
        listOfNetAmounts[b].netAmount = amount;
    }
    
    vector<vector<pair<int,string>>> ansGraph(numBanks,vector<pair<int,string>>(numBanks,{0,""}));//adjacency matrix
    
    
    //find min and max net amount
    int numZeroNetAmounts=0;
    
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }
    while(numZeroNetAmounts!=numBanks){
        
        int minIndex=getMinIndex(listOfNetAmounts, numBanks);
        pair<int,string> maxAns = getMaxIndex(listOfNetAmounts, numBanks, minIndex,input,maxNumTypes);
        
        int maxIndex = maxAns.first;
        
        if(maxIndex == -1){
            
            (ansGraph[minIndex][0].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[minIndex][0].second) = *(input[minIndex].types.begin());
            
            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numBanks);
            (ansGraph[0][simpleMaxIndex].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[0][simpleMaxIndex].second) = *(input[simpleMaxIndex].types.begin());
            
            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;
            
            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;
            
        }
        else{
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);
            
            (ansGraph[minIndex][maxIndex].first) += (transactionAmount);
            (ansGraph[minIndex][maxIndex].second) = maxAns.second;
            
            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;
            
            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }
        
    }
    
    printAns(ansGraph,numBanks,input);
    // cout<<"HI\n";
}
// correct
int main()
{
    cout << "\n\t\t\t\t********************* Welcome to CASH FLOW MINIMIZER SYSTEM ***********************\n\n\n";
    cout << "This system minimizes the number of transactions among multiple banks in the different corners of the world that use different modes of payment.There is one world bank (with all payment modes) to act as an intermediary between banks that have no common mode of payment. \n\n";
    cout << "Enter the number of banks participating in the transactions.\n";
    int numBanks;
    cin >> numBanks;

    bank input[numBanks];
    unordered_map<string, int> indexOf; // stores index of a bank

    cout << "Enter the details of the banks and transactions as stated:\n";
    cout << "Bank name ,number of payment modes it has and the payment modes.\n";
    cout << "Bank name and payment modes should not contain spaces\n";

    int maxNumTypes;
    for (int i = 0; i < numBanks; i++)
    {
        if (i == 0)
        {
            cout << "World Bank : ";
        }
        else
        {
            cout << "Bank " << i << " : ";
        }
        cin >> input[i].name;
        indexOf[input[i].name] = i;
        int numTypes;
        cin >> numTypes;

        if (i == 0)
            maxNumTypes = numTypes;

        string type;
        while (numTypes--)
        {
            cin >> type;

            input[i].types.insert(type);
        }
    }

    cout << "Enter number of transactions.\n";
    int numTransactions;
    cin >> numTransactions;

    vector<vector<int>> graph(numBanks, vector<int>(numBanks, 0)); // adjacency matrix

    cout << "Enter the details of each transaction as stated:";
    cout << "Debtor Bank , creditor Bank and amount\n";
    cout << "The transactions can be in any order\n";
    for (int i = 0; i < numTransactions; i++)
    {
        cout << (i) << " th transaction : ";
        string s1, s2;
        int amount;
        cin >> s1 >> s2 >> amount;

        graph[indexOf[s1]][indexOf[s2]] = amount;
    }

    // settle
    minimizeCashFlow(numBanks, input, indexOf, numTransactions, graph, maxNumTypes);
    return 0;
}

/*
5
A 2 t1 t2
B 1 t1
C 1 t1
D 1 t2
E 1 t2
4
B A 300
C A 700
D B 500
E B 500

--------
5
World_Bank 2 Google_Pay PayTM
Bank_B 1 Google_Pay
Bank_C 1 Google_Pay
Bank_D 1 PayTM
Bank_E 1 PayTM
4
Bank_B World_Bank 300
Bank_C World_Bank 700
Bank_D Bank_B 500
Bank_E Bank_B 500

--------------------

6
B 3 1 2 3
C 2 1 2
D 1 2
E 2 1 3
F 1 3
G 2 2 3
9
G B 30
G D 10
F B 10
F C 30
F D 10
F E 10
B C 40
C D 20
D E 50
*/
