//
// Created by svenpete@id.hsos.de on 13.12.22.
//

#ifndef ASS4_LANGFORDCOPY_H
#define ASS4_LANGFORDCOPY_H

#include <tbb/tbb.h>

tbb::enumerable_thread_specific<int> Loesungen{0};
int N_Doppelt;
int N;

using namespace std;
class LangfordParrallel
{
public:
    int n;
    string tag;
    int solutions;
    vector<int> langford_seq;

    int laenge_seq;
    int Anz_Werte;

    LangfordParrallel(int n){
        this -> n = n;
        langford_seq = vector<int>(2 * n, 0);
        laenge_seq = 2 * n;
        Anz_Werte = n;
    }

    void execute()
    {
        langford(langford_seq, this -> n);
    }

    void langford(vector<int> sequence, int idx)
    {
        N = n;
        N_Doppelt = 2 * n;
        tbb::task_group tg;
        tg.run_and_wait(LangfordTask(tg, idx, vector<int>(sequence)));
        for(auto it = Loesungen.begin(); it != Loesungen.end(); it++) solutions += *it;
    }

    class LangfordTask
    {
    public:
        tbb::task_group& task_group;

        int index;
        vector<int> sequence;
        LangfordTask(tbb::task_group& g, int index2, const vector<int>& sequence)
                : task_group(g), index(index2), sequence(sequence) {}

        void operator()() const
        {
            vector<int> sequence = this->sequence;
            int limit = N_Doppelt;
            int Anz_Werte = N;


            if (index == Anz_Werte){
                if((index % 2) == 0 ){
                    limit = Anz_Werte / 2;
                } else {
                    limit = (Anz_Werte - 1) / 2;
                }
            } else if((index == Anz_Werte - 1) && (sequence[(Anz_Werte / 2) - 1] == Anz_Werte) && (Anz_Werte % 2 == 0)  ){
                limit = Anz_Werte / 2;
            }

            for(int i = 0; i < limit; i++) {
                if(i > 0 && sequence[i - 1] == index) {
                    sequence[i - 1] = 0;
                    sequence[i + index] = 0;
                }
                if(i + index + 1 < sequence.size() && sequence[i] == 0 && sequence[i + index + 1] == 0) {
                    sequence[i] = index;
                    sequence[i + index + 1] = index;
                    if(index > 1)
                        task_group.run(LangfordTask(task_group, index - 1, sequence));
                    else if(index == 1) {
                        Loesungen.local()++;
                        break;
                    }
                }
            }
        }
    };
};


#endif //ASS4_LANGFORDCOPY_H
