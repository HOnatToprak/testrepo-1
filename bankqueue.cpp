#include <iostream>

using namespace std;
#define EMP 6//employee num
#define CST 10//customer num
int main()
{
    int time = 1;
    int empiter = 0;
    bool debug = false;

    int inputs[][3] = {
        {122,1,2},
        {128,1,1},
        {113,1,2},
        {118,2,3},
        {119,2,5},
        {115,2,9},
        {127,3,1},
        {156,3,4},
        {112,3,5},
        {121,3,7}
    };
    int waittime[] = {0,0,0,0,0,0,0,0,0,0};
    int resttime[] = {0,0,0,0,0,0};

    for(int i = 0;i < CST;i++){
        empiter = 0;
        while(inputs[i][1]>time){
            time++;
            for(int j=0;j<EMP;j++){
                if(resttime[j]>0){
                    resttime[j]--;
                    if(debug)
                        cout << j <<"  resttime decreased to  "<< resttime[j]<<endl;
                }
            }
        }
        while(resttime[empiter]>0){
            empiter++;
            if(empiter == EMP){
                inputs[i][1]++;
                time++;
                for(int j =0;j<EMP;j++)
                    if(resttime[j]>0)
                        resttime[j]--;
                empiter = 0;
                waittime[i]++;
            }
        }
        resttime[empiter] = inputs[i][2];
        if(debug)
            cout << "resttime assigned  " << resttime[empiter] << endl;
        cout << inputs[i][0]<<"  " << EMP-empiter <<"  "<< time <<"  "<< time+inputs[i][2] <<"  "<< waittime[i] <<endl;

    }
    return 0;
}
