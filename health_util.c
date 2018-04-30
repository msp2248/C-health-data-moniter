#include <stdio.h>
#include <stdlib.h>
#include "health.h"
#include <string.h>

/*
* health_util.c - Helper functions for the Health Monitoring System
*
* Add any optional helper function declarations here, define 
* the functions after the required functions below.
*
* static void myOptionalFunc();   // EXAMPLE 
*
*/

/*
* addPatient: check-in a new patient
*   (1) allocate a new Chart for the patient
*   (2) initialize the chart with the passed patientID
*   (3) new patients are inserted at the start of the patient list
*
* (note that the variable patientList is globally accessible)
*/
void addPatient( int patientID ){
	Chartptr cp = malloc(sizeof(Chart));
	cp->id = patientID;    // create a new chart node at the beginning of the list
	cp->next = patientList;
	cp->buffer = NULL;
	patientList = cp;
}

/*
* addHealthType: add a new health type buffer for a given patient
*	(1) allocate a new CircularBuffer
*	(2) initialize the buffer
* 	(3) link it to the existing patient's Chart
*/
void addHealthType( int patientID, int newType ){
	if(getChart(patientID) != NULL){      // create a new buffer node at the beginning of the list, check if there a chart first
		Chartptr foundchart = getChart(patientID);
		CBuffptr bufftype = malloc(sizeof(CircularBuffer));
		bufftype -> type = newType;
		bufftype -> end = 0; //initialize start and end to 0
		bufftype -> start = 0;
		bufftype -> next = foundchart -> buffer;
		foundchart -> buffer = bufftype;
	}
}
/*
*  getChart: given a patientID, return a pointer to their Chart
*/

Chartptr getChart( int patientID ){
	Chartptr foundChart = patientList;
	while (foundChart && foundChart -> id != patientID){ // iterate to the end of the linked list
		foundChart = foundChart -> next;
	}
	return foundChart; //null if this chart doesnt exist
}

/*
*  getHealthType: given a patientID & healthType, return a pointer 
*  to the Circul
arBuffer for that type. If the health type does NOT exist 
*  for that patient, return NULL
*/
CBuffptr getHealthType( int patientID, int healthType ){
	CBuffptr foundType = getChart(patientID) -> buffer;
	while (foundType && foundType -> type != healthType){ // iterate to the end of the linked list
		foundType = foundType -> next;
        }
	return foundType; // null if this type deosnt exist

}

/*
*  addHealthReading: given a pointer to CircularBuffer, add the passed
*  timestamp and health data type reading to the buffer
*/
void addHealthReading( CBuffptr buffer, char* timestamp, int reading ){
	static int i = 0;
        if (buffer -> start == MAXREADINGS){
                buffer -> start == 0;  // reset start of the buffer
        }
	if (buffer -> end == MAXREADINGS){
                buffer -> end = 0; // if end is maxreadings, start and end should be reset
                i = 1;
        }
        strcpy(buffer -> reading[buffer -> end].timestamp, timestamp);
        buffer -> reading[buffer -> end].value = reading;
        if (i!=0){
                buffer -> start++;
                buffer -> end++;
        }
        else{
                buffer -> end++;
        }
}

/*
*  removePatient: check-out an existing patient
*	  (1) delete the patient's Chart & accompanying 
*         health data readings.
*     (2) update the list of current patients
*/
void removePatient( int patientID ){
	Chartptr prevChart  = patientList;
	Chartptr removeChart = getChart(patientID);
	Chartptr nextChart = removeChart -> next;
	while (prevChart != removeChart && prevChart-> next != removeChart){ // iterate to end of the list to the node before the node being removed
		prevChart = prevChart -> next;
	}
	CBuffptr curr;
	while (removeChart->buffer != NULL){
		curr=removeChart->buffer; //set curr to the buffer
		removeChart->buffer=removeChart->buffer->next; // iterate after that
		free(curr); //now you can free curr while still being able to iterate
	}
	if(prevChart!=removeChart){
		prevChart->next=nextChart; //If list size is greater than 1
	}
	else{
		prevChart=nextChart; //if size of the list is 1
	}
	free(removeChart); //free the chart finally
}

/*
* Optional helper functions defined starting here:
*
* static void myOptionalFunc(){ }  // EXAMPLE
* 
*/

void freeChart(int id){
	Chartptr removeChart = getChart(id); //iterate to the chart you want to remove the buffers from
	CBuffptr curr;
        while (removeChart->buffer != NULL){
                curr=removeChart->buffer; //free all of the buffers
                removeChart->buffer=removeChart->buffer->next;
                free(curr);
        }


}



