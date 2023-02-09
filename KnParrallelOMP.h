//
// Created by svenpete@id.hsos.de on 15.12.22.
//
#ifndef __LANGFORD_PAR_H
#define __LANGFORD_PAR_H

#include <fstream>
#include <sstream>
#include <tbb/tbb.h>


using namespace std;
class KnParal
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

    KnParal(int n)
    {
        this -> N = n;
        this -> M = n;
        vector<int> bufStep; //=  {2, 3};
    };

    void execute(){
        #pragma omp parallel //task parallel ausführen
        #pragma omp single // ein task pro thread
        Step({0,0}, StepNum, StepIterate);
    };

    void Step(vector<int> pos, int StepNum, vector<vector<int>> StepIterate) {
        StepNum += 1;
        int i = 0;

        StepIterate.push_back(pos);

        if ((StepIterate.size() > 1) && (pos[0] == 0) && (pos[1] == 0)) {
            if (lStep < StepIterate.size()) {
                //lPath = StepIterate;
                lStep = StepNum ;//StepIterate.size();
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

                }
            }

            if(isValid(NewY, NewX, N) && vorhanden == false && DoesntIntersect(StepIterate, bufStep, StepNum)){
                pos = bufStep;
                #pragma omp task // diese funktionsobjekt soll in der Task ausgeführt werden
                Step(pos, StepNum, StepIterate);
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
};

#endif // __LANGFORD_PAR_H