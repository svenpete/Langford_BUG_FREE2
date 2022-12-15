#include <iostream>
#include <thread>
#include "LangfordParrallel.h"
#include "LangfordSequentiell.h"
#include "LangfordParrallelOMP.h"
using namespace std;
int main()
{
    cout << "Langford!\n";
    int n;


    cout << "Bitte geben Sie eine Sequence zum berechnen an: \nL(2,n)\tn = ";
    cin >> n;

    //prüfen ob die Eingabe kongruent zu 3 4 ist
    while ((n % 4 != 0) && ((n+1) % 4 != 0)) {
        cout << "L(2," << n << ") nicht lösbar.\n Bitte geben Sie ein neues n an:\n";
        cin >> n;
    }

//    auto seq = LangfordSequentiell(n);
//    auto start_langford_seq = chrono::high_resolution_clock::now();
//    seq.execute();
//    auto end_langford_seq = chrono::high_resolution_clock::now() - start_langford_seq;
//    long long microseconds_langford_seq = chrono::duration_cast<chrono::microseconds>(end_langford_seq).count();
//    cout << "Ausführungszeit sequentiell L(2," << to_string(n) << ") =  " << microseconds_langford_seq << " Mikrosekunden" << endl;
//    cout << "Gefundene Lösungen = " << seq.solutions << endl;

    auto par = LangfordParrallel(n);
    auto start_langford_par = chrono::high_resolution_clock::now();
    par.execute();
    auto end_langford_par = chrono::high_resolution_clock::now() - start_langford_par;
    long long microseconds_langford_par = chrono::duration_cast<chrono::microseconds>(end_langford_par).count();
    cout << "Ausführungszeit parallel L(2," << to_string(n) << ") =  " << microseconds_langford_par << " Mikrosekunden" << endl;
    cout << "Gefundene Lösungen = " << par.solutions << endl;

    auto parOpenMP = LangfordParrallelOpenMP(n);
    auto start_langford_parOpenMP = chrono::high_resolution_clock::now();
    parOpenMP.execute();
    auto end_langford_parOpenMP = chrono::high_resolution_clock::now() - start_langford_parOpenMP;
    long long microseconds_langford_parOpenMP = chrono::duration_cast<chrono::microseconds>(end_langford_parOpenMP).count();
    cout << "Ausführungszeit parallel L(2," << to_string(n) << ") =  " << microseconds_langford_parOpenMP << " Mikrosekunden" << endl;
    cout << "Gefundene Lösungen = " << parOpenMP.solutions << endl;
    return 0;
}