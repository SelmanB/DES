//
// Created by selman on 17.10.2019.
//

#ifndef HW2_DESIMULATION_H
#define HW2_DESIMULATION_H

#include<queue>
#include <iostream>
#include <iomanip>
//#include <set>
#include "event.h"
#include "selmanrnd.h"





class simulation{
public:
    simulation(int maxentities,rndtype mode);
    void init();
    void execute();
    void log();
    void printtable();
 //   void stat();


    /******STATE***********/
    bool s1=1,s2=1;
    std::queue<index>checkout;     //entities in the queue
    std::priority_queue<event>fel;  //future event list
    timeUnit clock=0;               //simulation clock


    /*****STATISTICS*******/
    timeUnit tle=0;             //time of last event
    timeUnit totq=0;            //total time spent in the queue
    timeUnit tots1=0, tots2=0;  //total time spent in service
    int departed1=0, departed2=0;//number of people served
    int arrivals=0;             //arrivals realized
    int max;
    entity *table;
    //std::set<event> eventlog;


//private:
    void arrival(event evn);
    void departure1(event evn);
    void departure2(event evn);
    rndgen source;
    timeUnit iat(){
        return (timeUnit)(source.random()*11+5);
    }
    timeUnit s1t(){
        return (timeUnit)(source.random()*8+12);
    }
    timeUnit s2t(){
        double a=source.random();
        if(a<0.12)return 5.0;
        if(a<0.52)return 15.0;
        if(a<0.88)return 25.0;
        if(a<0.95)return 35.0;
        else return 45.0;
    }
};




simulation::simulation(int maxentities,rndtype mode):source(mode) {
    max=maxentities;
    table=new entity[maxentities];
    std::cout <<std::fixed<< std::setw(10)<<std::setprecision(5)<<std::right<<std::setfill('0');
    init();
    execute();
}


void simulation::arrival(event evn) {
    clock=evn.t;
    table[evn.customerID].ID=evn.customerID;//create new entity in stat base
    table[evn.customerID].arrival=clock;//update status
    if(arrivals<max){
        event nextarr(clock+iat(),ARR,arrivals);//schedule next arrival
        fel.push(nextarr);
    }
    if(!s1 || !s2){                     //server empty
        table[evn.customerID].sbegin=clock;    //update status
        if(!s1){                        //server 1 selected
            table[evn.customerID].server=1;
            s1=true;
            event dept1(clock+s1t(),DPT1,evn.customerID);
            fel.push(dept1);
        }else{                          //server 2 selected
            table[evn.customerID].server=2;
            s2=true;
            event dept2(clock+s2t(),DPT2,evn.customerID);
            fel.push(dept2);
        }
    }else{                              //enqueue the customer
        checkout.push(evn.customerID);
    }
}

void simulation::departure1(event evn) {
    clock=evn.t;
    table[evn.customerID].send=clock;//depart the customer
    table[evn.customerID].duration=table[evn.customerID].send-table[evn.customerID].sbegin;
    if(checkout.empty()){
        s1=false;
    }else{
        index id=checkout.front();//get the customer from queue
        checkout.pop();
        table[id].server=1;//start serving the customer
        table[id].sbegin=clock;
        event next(clock+s1t(),DPT1,id);//schedule next departure
        fel.push(next);
    }
}
void simulation::departure2(event evn) {
    clock=evn.t;
    table[evn.customerID].send=clock;//depart the customer
    table[evn.customerID].duration=table[evn.customerID].send-table[evn.customerID].sbegin;
    if(checkout.empty()){
        s2=false;
    }else{
        index id=checkout.front();//get the customer from queue
        checkout.pop();
        table[id].server=2;//start serving the customer
        table[id].sbegin=clock;
        event next(clock+s2t(),DPT2,id);//schedule next departure
        fel.push(next);
    }
}




void simulation::execute(){
    using namespace std;
    cout<<"\n\n***STATE LOGS****\n"<<endl;

    while(fel.size()){

        log();
        event imm=fel.top();
        //eventlog.insert(imm);
        totq+=(imm.t-clock)*checkout.size();
        tots1+=(imm.t-clock)*s1;
        tots2+=(imm.t-clock)*s2;

        if(imm.type==ARR){
            arrival(imm);
            arrivals++;
        }if(imm.type==DPT1){
            departure1(imm);
            departed1++;
        }if(imm.type==DPT2){
            departure2(imm);
            departed2++;
        }

        //TODO: add maximum waiting time using the queue(see commented out lines using set ADT)
        fel.pop();
    }log();

    cout<<"\n\n***ENTITIES PROCESSED***\n"<<endl;
    printtable();
    //cout<<"\n\n***EVENT LOGS***\n"<<endl;
    //printevents();
    cout<<"\n\n***STATISTICS***\n"<<endl;
    cout<<"totq:\t\t"<<totq<<"\ntots1:\t\t"<<tots1<<"\ntots2:\t\t"<<tots2<<"\ndeparted1:\t"
    <<departed1<<"\ndeparted2:\t"<<departed2<<endl;
}


void simulation::init() {
    arrivals=3;
    table[0].ID=0;
    table[0].arrival=0;
    table[0].sbegin=0;
    table[0].server=1;
    table[1].ID=1;
    table[1].arrival=0;
    table[1].sbegin=0;
    table[1].server=2;
    event arr(iat(),ARR,2);
    event dpt1(s1t(),DPT1,0);
    event dpt2(s2t(),DPT2,1);
    fel.push(arr);
    fel.push(dpt1);
    fel.push(dpt2);
}

void simulation::log() {    //print summary of the current state to stdout
    using namespace std;
    cout<<"clock:"<<clock<<'\t'<<flush;
    cout<<"checkout:"<<checkout.size()<<'\t'<<flush;
    cout<<"server1:"<<s1<<'\t'<<flush;
    cout<<"server2:"<<s2<<'\t'<<flush;
if(!fel.empty())
    cout<<"Imminent event:"<<etn[fel.top().type]<<" at time "<<fel.top().t<<" for "<<fel.top().customerID<<flush;

    cout<<endl;
 //   cout<<'\t'<<

}

void simulation::printtable() { //print the entity-based simulation table
    using namespace std;
    for(int i=0;i<max;i++)
        if(table[i].ID!=-1){
                cout<<"ID:"<<table[i].ID<<"\tArrival:"<<table[i].arrival<<
                "\tServer:"<<table[i].server<<"\tService begin:"<<table[i].sbegin<<
                "\tDuration:"<<table[i].duration<<"\tEnd:"<<table[i].send<<endl;
        }
        cout<<"Note that duration is calculated when service begins but recorded at departure, also the customer ID is assigned upon arrival."<<endl;

}



#endif //HW2_DESİMULATİON_H
