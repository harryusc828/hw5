#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool schedule_help(size_t day, size_t num_day, DailySchedule& sched, const AvailabilityMatrix& avail, vector<size_t> num_shifts, const size_t dailyNeed, const size_t maxShifts);
void form_all_comb(vector<Worker_T> the_vec, vector<vector<Worker_T>>& big_vec, const size_t dailyNeed, size_t idx);
bool can_schedule(vector<Worker_T> the_vec, size_t idx, vector<size_t> num_shifts, const AvailabilityMatrix& avail, const size_t maxShifts, size_t day); 
void increment(vector<size_t>& num_shifts, vector<vector<Worker_T>> big_vec, size_t idx, size_t count);
void decrement(vector<size_t>& num_shifts, vector<vector<Worker_T>> big_vec, size_t idx, size_t count); 
// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    size_t num_worker = avail[0].size();
    size_t num_day = avail.size(); 
    vector<size_t> num_shifts(num_worker, 0); 
    //vector<vector<Worker_T>> the_vec(num_day , vector<Worker_T> (dailyNeed, -1)); 

    //cout << "I am here" << endl; 
    return schedule_help(0, num_day, sched, avail, num_shifts, dailyNeed, maxShifts); 

}

bool schedule_help(size_t day, size_t num_day, DailySchedule& sched, const AvailabilityMatrix& avail, vector<size_t> num_shifts, const size_t dailyNeed, const size_t maxShifts){
    if(day == num_day){
        return true;
    }
    vector<vector<Worker_T>> big_vec; 
    vector<Worker_T> the_vec; 
    for(size_t i = 0; i < avail[day].size(); i++){
        if(avail[day][i] == true){
            the_vec.push_back(i); 
        }
    }
    form_all_comb(the_vec, big_vec, dailyNeed, 0); 
    // cout <<"printing sched here" << endl; 
    // for(size_t i = 0; i < sched.size(); i++){
    //     for(size_t j = 0; j < sched[i].size(); j++){
    //         cout << sched[i][j]; 
    //     }
    //     cout << endl; 
    // }

    for(size_t i = 0; i < big_vec.size(); i++){
        //cout << "loop big_vec size is " << big_vec.size() << endl; 
        sched.push_back(big_vec[i]); 
        increment(num_shifts, big_vec, i, 0);
        if(can_schedule(big_vec[i], 0, num_shifts, avail, maxShifts, day)){
        //    cout << "true 1" << endl; 
            if(schedule_help(day + 1, num_day, sched, avail, num_shifts, dailyNeed, maxShifts)){
                return true;
            }
        }
        sched.pop_back(); 
        decrement(num_shifts, big_vec, i, 0); 
    }
    //cout << "false here" << endl; 
    return false; 
}

void increment(vector<size_t>& num_shifts, vector<vector<Worker_T>> big_vec, size_t idx, size_t count){
    if(count == big_vec[idx].size()){
        return; 
    }
    num_shifts[big_vec[idx][count]] ++; 
    increment(num_shifts, big_vec, idx, count+1); 
}

void decrement(vector<size_t>& num_shifts, vector<vector<Worker_T>> big_vec, size_t idx, size_t count){
    if(count == big_vec[idx].size()){
        return; 
    }
    num_shifts[big_vec[idx][count]] --;  
    decrement(num_shifts, big_vec, idx, count+1); 
}


void form_all_comb(vector<Worker_T> the_vec, vector<vector<Worker_T>>& big_vec, const size_t dailyNeed, size_t idx){

    // if(the_vec.size() < dailyNeed){
    //     return false; 
    // }
    if(the_vec.size() == dailyNeed){
        big_vec.push_back(the_vec);
        return; 
    }
    for(size_t i = idx; i < the_vec.size(); i++){
        vector<Worker_T> new_positions = the_vec; 
        new_positions.erase (new_positions.begin() + i); 
        form_all_comb(new_positions, big_vec, dailyNeed, i); 
    }
    return; 
}

bool can_schedule(vector<Worker_T> the_vec, size_t idx, vector<size_t> num_shifts, const AvailabilityMatrix& avail, const size_t maxShifts, size_t day){

    if(idx == the_vec.size()){
        return true; 
    }
    if(num_shifts[the_vec[idx]] <= maxShifts ){
        return can_schedule(the_vec, idx + 1, num_shifts, avail, maxShifts, day); 
    }
    else {
        return false; 
    }

}
