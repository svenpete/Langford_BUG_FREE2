//
// Created by svenpete@id.hsos.de on 15.12.22.
//
#ifndef __LANGFORD_SEQ_H
#define __LANGFORD_SEQ_H

#include <fstream>
#include <sstream>
#include <tbb/tbb.h>


using namespace std;
class KnSequentiell
{
public:

    vector<int> row = {2, 2, -2, -2, 1, 1, -1, -1 };
    vector<int> col = {-1, 1, 1, -1, 2, -2, 2, -2 };
    vector<vector<int>> StepIterate;

/*  vector<int> bufStep; //=  {2, 3};
    vector<vector<int>> lPath = {{0, 0}};
    vector<vector<int>> StepIterate;*/
    int lStep = 0;
    vector<vector<int>> lPath = {{0, 0}};

// Feldgröße
    int N = 5;
    int M = 5;
    int StepNum = 0;
    int solutions = 0;

    KnSequentiell(int n)
    {
        this -> N = n;
        this -> M = n;
        vector<int> bufStep; //=  {2, 3};
    };

    void execute(){
        Step({0,0}, StepNum);
    };

    void Step(vector<int> pos, int StepNum) {
        StepNum += 1;
        int i = 0;

        StepIterate.push_back(pos);

        if ((StepIterate.size() > 1) && (pos[0] == 0) && (pos[1] == 0)) {
            if (lStep < StepIterate.size()) {
                lPath = StepIterate;
                lStep = StepIterate.size();
            }
            return;
        };


        while (i < 8) {
            if (StepIterate.size() > StepNum) {
                while (StepIterate.size() > StepNum) {
                    StepIterate.pop_back();
                }
            }


            pos = StepIterate[StepNum - 1];
            int NewY = pos[1] + row[i];
            int NewX = pos[0] + col[i];
            vector<int> bufStep = {NewX, NewY};

            bool vorhanden = false;
            for (int j = 0; j < StepIterate.size(); ++j) {
                bool xTrue = StepIterate[j][0] == bufStep[0] ? true : false;
                bool yTrue = StepIterate[j][1] == bufStep[1] ? true : false;

                if (xTrue && yTrue) {
                    vorhanden = bufStep[0] == 0 && bufStep[1] == 0 ? false : true;
                    break;
                }
            }

            if(isValid(NewY, NewX, N) && vorhanden == false && DoesntIntersect(StepIterate, bufStep, StepNum)){
                pos = bufStep;
                Step(pos, StepNum);
            }

            i += 1;
        }
    }


    // hier wird zunächst die Koeffizientendeterminante errechnet, wenn der Wert gleich 0 ist sind die Geraden ohne Schnittpunkt
    float det(vector<auto>a, vector<auto>b){
        return a[0] * b[1] - a[1] * b[0];
    }

    bool line_intersection(vector<vector<int>> line1, vector<vector<int>> line2){
        //die ganze Upper/Lower geschichte wird gebraucht um den Ort des Schnittpunkts einzudämmen auf den Bereich der Geraden.
        //Aufgrund der unendlichen Längen der Geraden würde man sonst Schnittpunkte erhalten die garnicht mehr jucken
        int upperX = line1[1][0] > line1[0][0] ? line1[1][0] : line1[0][0];
        int lowerX = line1[1][0] < line1[0][0] ? line1[1][0] : line1[0][0];
        int upperY = line1[1][1] > line1[0][1] ? line1[1][1] : line1[0][1];
        int lowerY = line1[1][1] < line1[0][1] ? line1[1][1] : line1[0][1];

        int upperXl2 = line2[1][0] > line2[0][0] ? line2[1][0] : line2[0][0];
        int lowerXl2 = line2[1][0] < line2[0][0] ? line2[1][0] : line2[0][0];
        int upperYl2 = line2[1][1] > line2[0][1] ? line2[1][1] : line2[0][1];
        int lowerYl2 = line2[1][1] < line2[0][1] ? line2[1][1] : line2[0][1];

        vector<int> xdiff = {line1[0][0] - line1[1][0], line2[0][0] - line2[1][0]}; //#-2 , 2
        vector<int> ydiff = {line1[0][1] - line1[1][1], line2[0][1] - line2[1][1]}; //# -1, -1

        // hier wird zunächst die Koeffizientendeterminante errechnet, wenn der Wert gleich 0 ist sind die Geraden ohne Schnittpunkt
        float  div = det(xdiff, ydiff);
        if (div == 0)
            return true;

        // im späteren Verlauf werden nun die Zählerdeterminanten für x und y ausgerechnet und durch die Koeffizientendeterminante geteilt
        //weiterführendes findet man unter cramersche-regel
        vector<float> d = {det(line1[0],line1[1]), det(line2[0],line2[1])};
        float x = det(d, xdiff) / div;
        float y = det(d, ydiff) / div;

        //cout << x << y << endl;


        //Falls der ermittelte Schnittpunkt nun außerhalb des x order y Intervalls einer der beiden geraden liegt soll nichts passieren
        //wichtig und richtig so wie das hier ist, da um ein Schnittpunkt beider gerade zu sein der x und y wert im bereich von beiden geraden sein muss
        //wenn nur für l 1 geprüft werden würde dann könnte ein falsches positiv entstehen, weil l2 ja sonst unendlich ist und quer über das feld einen punkt
        //auf l1 treffen kann der dann valide ist aber gar kein schnittpunkt
        if(x > upperX || x < lowerX || y > upperY || y < lowerY || x > upperXl2 || x < lowerXl2 || y > upperYl2 || y < lowerYl2)
            return true;

        if(x == 0 && y == 0)
            return true;

        return false;
    };


    //der ganze Bums steuert die Intersection geschichte so ein bisschen. Im Prinzip wird für jeden neuen Punkt den man einsetzten will geprüft,
    //ob er mit einer der vorherigen Punkte einen Schnittpunkt hat, Als Ausnahme ist noch hinterlegt, dass für den Punkt 0,0 die erste überprüfung nicht gemacht wird,
    //weil der Schnittpunkt bei 0,0 sonst immer sagen würde, dass es nicht zulässig ist
    //hier sollte im fertigen Programm noch erarbeitet werden, dass der Punkt sich mit dem Startpunkt ändert und der Schnittpunkt == exakter Startpunkt = kein Schnittpunkt
    bool DoesntIntersect(auto StepIterate, auto bufStep, auto StepNum){

        int CntInters = 0;

        for (int i = 0; i < StepNum-2; ++i) {
            bool test = line_intersection({StepIterate[i], StepIterate[i + 1]}, {StepIterate[StepNum - 1 ], bufStep });

            if(test == false ){
                CntInters += 1;
            }

        }
        if (CntInters == 0)
            return true;

        return false;
    };


    bool isValid(int y, int x, int N, int M = 99){
        // prüfen ob das Schachfeld quadratisch ist
        M = M == 99 ? N : M;

        if ((y >= 0) && (y < N) && (x >= 0) && (x < M))
            return true;

        if (y == 0 && x == 0)
            return true;

        return false;
    };

/*    void langford(vector<int> langford_seq, int index)
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
    }*/
};

#endif // __LANGFORD_SEQ_H