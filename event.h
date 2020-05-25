//
// Created by selman on 17.10.2019.
//

#ifndef HW2_EVENT_H
#define HW2_EVENT_H

typedef double timeUnit ;
typedef int index;
enum eventType{ARR,DPT1,DPT2};
const char* etn[]={"     Arrival","Dept. from 1","Dept. from 2"};


class entity{
public:
    index ID=-1;//non-initialized entities in the table have indices -1
   timeUnit arrival=0;
   timeUnit sbegin=0;
   timeUnit send=0;
   timeUnit duration=0;
   short server=0;
};

class event{
public:
    event(timeUnit _t, eventType _type,index _customerID){
       t=_t;
       type=_type;
       customerID=_customerID;
    }

    timeUnit t;
    eventType type;
    index customerID;

   //This is the comparison function used to find the imminent event
   friend bool operator <(const event& left, const event& right) { return left.t > right.t;}


};

#endif //HW2_EVENT_H
