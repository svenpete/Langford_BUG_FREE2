//
// Created by svenpete@id.hsos.de on 15.12.22.
//
#ifndef __LANGFORD_SEQ_H
#define __LANGFORD_SEQ_H


#include <fstream>
#include <sstream>
#include <tbb/tbb.h>
using namespace std;
class LangfordSequentiell
{
public:
    int n;
    string tag;
    int solutions = 0; // nicht ändern falls wird +16 auf die Lösung gerechnet
    vector<int> langford_seq;

    int laenge_seq;
    int Anz_Werte;

    LangfordSequentiell(int n)
    {
        this -> n = n;
        langford_seq = vector<int>(2 * n, 0);
        laenge_seq = 2 * n;
        Anz_Werte = n;

    }

    void execute()
    {
        langford(langford_seq, this -> n);
    }

    void langford(vector<int> langford_seq, int index)
    {

        int laenge_seq = this->laenge_seq;
        int Anz_Werte = this->Anz_Werte;
        //In erster Iteration den Zaehler für die schleife auf die hälfte (für gerade) bzw. die hälfte -1^für krumme Zahlen
        //weil bei Parameter 3 bsp. der erste Index maximal in stelle 2 stehen kann bsp. 3 1 2 1 3 2 geht mit 3 in pos 3;  2 1 3 1 2 x 3
        if (index == Anz_Werte){
            if((index % 2) == 0 ){
                laenge_seq = Anz_Werte / 2;
            } else {
                laenge_seq = (Anz_Werte - 1) / 2;
            } //in der zweiten Iteration können bei geraden N auch Spiegelungen auftreten. Diese werden hier rausgenommen
        } else if((index == Anz_Werte - 1) && (langford_seq[(Anz_Werte / 2) - 1] == Anz_Werte) && (Anz_Werte % 2 == 0)  ){
            laenge_seq = Anz_Werte / 2;
        }

        //
        for(int i = 0; i < laenge_seq; i++) {
            if(i > 0 && langford_seq[i - 1] == index) {
                langford_seq[i - 1] = 0;
                langford_seq[i + index] = 0;
            }
            // prüfen auf outofbounds exception
            if(i + index + 1 < langford_seq.size()) {
                //püfen ob die Stellen im Vektor frei sind
                if(langford_seq[i] == 0 && langford_seq[i + index + 1] == 0) {
                    // Wenn frei dann setzen wir diese
                    langford_seq[i] = index;
                    langford_seq[i + index + 1] = index;
                    //solange wir noch nicht fertig sind machen wir dies nochmal
                    if(index > 1) {
                        this->langford_seq = langford_seq;
                        langford(vector<int>(langford_seq), index - 1);
                    }
                        // wenn der index = 1 ist wird der Lösungzähler incrementiert
                    else if(index == 1) {
                        solutions++;
                        break;
                    }
                }
            }
        }
    }
};

#endif // __LANGFORD_SEQ_H