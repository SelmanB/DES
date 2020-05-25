#include <iostream>
#include "desimulation.h"

using namespace std;
void warn(){
    cerr<<"Usage: hw2 {-MODE} {n}\n"
        <<"{n}->\tnumber of entities to process\n"<<
        "{MODE}->'r' for real hardware random source\n"<<
        "\t'p' for pseudorandom number generator\n"<<
        "\t'l' for look-up table provided\n"<<flush;
    exit(-1);
}

int main(int argc, char* argv[]) {

/*random test
rndgen g;
for(int i=0;i<10;i++)
cout<<g.random()<<hex<<endl;
*/

/*event comparison test
event e1,e2;
e1.t=30;
e2.t=20;
cout<<(e2<e1)<<endl;
*/

/* imminent event test
event e1(30.3,ARR,1);
event e2(29.8,DPT1,1);
event e3(31.6,DPT2,1);

priority_queue<event> fel;

fel.push(e1);
fel.push(e2);
fel.push(e3);
for(int i=0;i<3;i++) {
    cout << fel.top().type << endl;
    fel.pop();
}*/

 //Interarrival and service time test
 /*
simulation sim(100);
for(int i=0;i<30;i++)
cout<<sim.s2t()<<endl;
*/
/*arrival test
event arr(3,ARR,5);
simulation sim(100);
    sim.log();
sim.arrival(arr);
    sim.log();
*/

//departure test
/*  event dept(3,DPT1,5);
    event arr(2,ARR,5);
    simulation sim(100);
    sim.log();
    sim.arrival(arr);
    sim.log();
    sim.departure1(dept);
    sim.log();
*/
if(argc!=3)warn();


int n=atoi(argv[2]);
if(!n)warn();

rndtype mode;
switch (argv[1][1]){
    case 'r':mode=REAL;break;
    case 'p':mode=PSEUDO;break;
    case 'l':mode=LUT;break;
    default:warn();
}
//cout <<mode<<' '<<n<<endl;
simulation sim(n,mode);

//cout<<"\n\n***POST-SIMULATION STATISTICS***"<<endl
int nqueued;
for(int i=0;i<n;i++) {
    nqueued += sim.table[i].sbegin != sim.table[i].arrival;

}
cout<<endl<<nqueued<<endl;


return 0;
}