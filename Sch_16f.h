/*
 * 
 * scheduler for pic16f877a using timer1
 * scheduler can be adapted to a given tick time using TICK_TIME in Sch_16f.h
 * must determine the frequency of oscillator in F_OSC in Sch_16f.h
 * change how many task can the scheduler handle using ( SCH_MAX_TASKS ) 
 * must be changed carefully and not to add tasks more than ( SCH_MAX_TASKS )
 * if there is no tasks to be executed at a given time the micro will go to sleep 
 * to use the scheduler : 
 * 1 - call SCH_Init() in the  main before while 
 * 2 - call SCH_Add_Task() to add the tasks
 * 3 - call SCH_Start() in the main to start scheduler
 * 4 - call SCH_Dispatch_Tasks() in the while loop
 * 
 */

#ifndef SCH_16F_H
#define	SCH_16F_H

#include <xc.h>
#include "temp_sensor.h"
#include "temp_control.h"
#include "SSD.h"
#include "switchs.h"
#include "eeprom.h"


// system defines 
#define ERROR_SCH_TOO_MANY_TASKS (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK (3)
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER (3)
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START (4)
#define ERROR_SCH_LOST_SLAVE (5)
#define ERROR_SCH_CAN_BUS_ERROR (6)
#define RETURN_NORMAL  0
#define RETURN_ERROR  1


// configurations 
#define SCH_MAX_TASKS (7) // determine the max number of tasks
#define TICK_TIME (1) // determine the tick time in ms 
#define F_OSC (8000000) 
#define Error_port PORTD
//uncomment to enable SCH_Report_Status()
//#define SCH_REPORT_ERRORS 


//types 
typedef unsigned char tByte ;
typedef unsigned int tWord ;

// Total memory per task is 7 bytes
typedef struct
{
    // Pointer to the task (must be a 'void (void)' function)
    void (*pTask)(void);
    // Delay (ticks) until the function will (next) be run
    // - see SCH_Add_Task() for further details
    tWord Delay;
    // Interval (ticks) between subsequent runs.
    // - see SCH_Add_Task() for further details
    tWord Period;
    // Incremented (by scheduler) when task is due to execute
    tByte RunMe;
} sTask;

// ------ Public function prototypes ------------------------------- //
// Core scheduler functions
tByte SCH_Delete_Task(const tByte);
void SCH_Init(void);
void SCH_Dispatch_Tasks(void);
tByte SCH_Add_Task(void (*) (void), const tWord, const tWord);
void SCH_Report_Status(void);
void SCH_Start(void);
void SCH_Stop(void);
void __interrupt ()  SCH_Update (void);

#endif	/* SCH_16F_H */

