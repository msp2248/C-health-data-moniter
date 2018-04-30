#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "health.h"

/*
* Main function for the Health Monitoring System. Primarily responsible for
* processing input of csv lines and printing as required. Data structures are
* maintained using the helper functions in health_util.c
* 
*/

Chartptr patientList = NULL;    /* Define global variable patientList (declared in health.h) */
								/* patientList is globaaly accessible in health_util.c    */
void main(){

printf("Welcome to the Health Monitoring System\n\n");

/*
*  YOUR CODE GOES HERE:
*  (1) Read an input csv line from stdin 
*  (2) Parse csv line into appropriate fields
*  (3) Take action based on input type:
*  		 - Check-in or check-out a patient with a given ID
*        - Add a new health data type for a given patient
*        - Store health data in patient record or print if requested
*  (4) Continue (1)-(3) until EOF
*/

	int type;
	int id;
	int value;
	char time[MAXTIME];
	while (scanf("%d, %8s, %d, %d", &id, time, &type, &value) >0){
		if (type < 6 && type > 0){
			Chartptr tempptr = getChart(id);
			if (tempptr){
				CBuffptr tempbuff = getHealthType(id, type);
				if (tempbuff){
					addHealthReading(tempbuff, time, value);
				}
			}
		}
		if (type == 7){ // Check in a patient
			addPatient(id);
			printf("------------------------------------------------------------\n");
			printf("%s: Patient ID = %i checking in \n", time, patientList->id);
			printf("------------------------------------------------------------\n");
		}
		if (type == 9){ // ADD a health type for the patient
			Chartptr tempchart = getChart(id);
			if (tempchart){
				if(getHealthType(id, value) == NULL){
					addHealthType(id,value);
				}
			}
		}
		if (type == 8){ //Check patient out of the patient list
			printf("------------------------------------------------------------\n");
			removePatient(id);
			printf("%s: Patient ID = %d checking out\n", time, id);
			printf("------------------------------------------------------------\n");

		}
		if (type == 0){
			printf("------------------------------------------------------------\n");
                        freeChart(id);
                        printf("%s: Patient ID = %d reset data\n", time, id);
                        printf("------------------------------------------------------------\n");
		}
		if (type == 6){ // Print all readings for 1 patient
                        printf("------------------------------------------------------------\n");
                        int q;
			Chartptr chart = getChart(id);
			CBuffptr currbuff = chart  ->  buffer;
                        for (q=0; q <= 4; q++){ // q is the buffer type -1
   	                     	if (q == 0){printf("Temperature:\n");}
                             	if (q == 1){printf("Heart Rate:\n");}
                             	if (q == 2){printf("Systolic Pressure:\n");}
                             	if (q == 3){printf("Diastolic Pressure:\n");}
                             	if (q == 4){printf("Respiration Rate:\n");}
			  	if (getHealthType(id,q+1) && q!=0){ // if the type is that of temperature, divide the value by 10
					currbuff = getHealthType(id, q+1);
					int v;
					for(v = currbuff -> start; v < currbuff -> end; v++){ // iterate from the start to the end
                                        	printf("%s: %d\n",currbuff -> reading[v].timestamp , currbuff -> reading[v].value);
                                        }
                		}
				else if (getHealthType(id,q+1) && q==0){
                                        currbuff = getHealthType(id, q+1);
                                        int v;
                                        for(v = currbuff -> start; v < currbuff -> end; v++){
                                                printf("%s: %.1f\n",currbuff -> reading[v].timestamp , currbuff -> reading[v].value/10.0);
                                        }
                                }
				else {
                                        printf("<none>\n");
                                }
			}
                                       printf("------------------------------------------------------------\n");
		}
	}
printf("\nEnd of Input\n");
}

