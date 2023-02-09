#include <iostream>
#include <thread>
#include "KnSequentiell.h"
#include "KnParrallelOMP.h"
using namespace std;
int main()
{
    cout << "Langford!\n";
    int n;


    cout << "Bitte geben Sie eine Sequence zum berechnen an: \nL(2,n)\tn = ";
    cin >> n;

    auto seq = KnSequentiell(n);
    auto start_langford_seq = chrono::high_resolution_clock::now();
    seq.execute();
    auto end_langford_seq = chrono::high_resolution_clock::now() - start_langford_seq;
    long long microseconds_langford_seq = chrono::duration_cast<chrono::microseconds>(end_langford_seq).count();
    cout << "Ausführungszeit sequentiell L(2," << to_string(n) << ") =  " << microseconds_langford_seq << " Mikrosekunden" << endl;
    cout << "Longest found = " << seq.lStep << endl;


    auto parOpenMP = KnParal(n);
    auto start_langford_parOpenMP = chrono::high_resolution_clock::now();
    parOpenMP.execute();
    auto end_langford_parOpenMP = chrono::high_resolution_clock::now() - start_langford_parOpenMP;
    long long microseconds_langford_parOpenMP = chrono::duration_cast<chrono::microseconds>(end_langford_parOpenMP).count();
    cout << "Ausführungszeit parallel L(2," << to_string(n) << ") =  " << microseconds_langford_parOpenMP << " Mikrosekunden" << endl;
    cout << "Gefundene Lösungen = " << parOpenMP.lStep << endl;
    return 0;
}