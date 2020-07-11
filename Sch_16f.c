
#include "Sch_16f.h"

// ------ Private function prototypes ------------------------------
static void SCH_Go_To_Sleep(void);

/* ------ Private variables ----------------------------------------*/
static tWord Error_tick_count_G; // Keeps track of time since last error was recorded
static tByte Last_error_code_G; // The code of the last error (reset after ~1 minute)
//
tWord timer_value ; //holds the value to be loaded to the timer 

// ------ Public variable definitions ------------------------------
sTask SCH_tasks_G[SCH_MAX_TASKS]; // The array of tasks
tByte Error_code_G = 0;           // Used to display the error code

//
extern MODE_STATE_t mode ; // variable will hold the system state 
extern POWER_MODES_t power_mode ; // variable will hold the power state 

/*
 * The ISR handle two interrupts :
 *   - > the timer 1 interrupt :
 *        * The timer will overflow every TICK_TIME ms.
 *        * Timer 1 is the main scheduler handling all the tasks in a cooperative style.
 *
 *   - > the external interrupt :
 *        * handle the power button.
 *        * check on the current power state of the system.
 *        * if the system is on and the ISR is fired will power off the system.
 *        * if the system is off and the ISR is fired will power on the system.
 *        * if the system is off : SSD , led cooler , heater are off + the
 *        * timer will be off. 
 */
void __interrupt ()  SCH_Update (void){
    if (PIR1 & (1 << 0)){
        //reload the timer
        TMR1L = timer_value ; 
        TMR1H = ( timer_value >> 8 ) ;
        PIR1 &= ~ (1 << 0) ; //clear flag
        tByte Index;
        // NOTE: calculations are in *TICKS* (not milliseconds)
        for (Index = 0; Index < SCH_MAX_TASKS; Index++)
        {
            // Check if there is a task at this location
            if (SCH_tasks_G[Index].pTask)
            {
                if (SCH_tasks_G[Index].Delay == 0)
                {
                    // The task is due to run
                    SCH_tasks_G[Index].RunMe += 1; // Inc. the 'RunMe' flag
                    if (SCH_tasks_G[Index].Period)
                    {
                        // Schedule periodic tasks to run again
                        SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                    }
                }
                else
                {
                    // Not yet ready to run: just decrement the delay
                    SCH_tasks_G[Index].Delay -= TICK_TIME;
                }
            }
        }
    }
    // external interrupt on b0
    if (INTCON & (1 << 1)){
        CLEAR_BIT(INTCON , 1); // clear the external interrupt flag 
        switch (power_mode){
            // if we were in the on state and the power button was pressed
            case ON_STATE :
                power_mode = OFF_STATE ;
                ssd_turn_off();
                temp_control_off();
                SCH_Stop();
                break;
             // if we were in the off state and the power button was pressed    
            case OFF_STATE :
                power_mode = ON_STATE ;
                SCH_Start();
                break;
        }
    }
}

/******************************************************************************
* Description : Prepares scheduler data structures and sets up timer interrupts
 *              at required rate. Must call this function before using 
 *              the scheduler. 
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void SCH_Init(void)
{
    tByte i;
    for (i = 0; i < SCH_MAX_TASKS; i++)
    {
        SCH_Delete_Task(i);
    }
    // Reset the global error variable
    // - SCH_Delete_Task() will generate an error code,
    // (because the task array is empty)
    Error_code_G = 0;

    // timer settings 
    T1CON = 0X30; // load Timer 2 control register
    PIR1 &= ~ (1 << 0) ; //clear flag
    PIE1 |= (1 << 0); // enable interrupt
    INTCON |= (1 << 7); //Enable global interrupt
    INTCON |= (1 << 6); //Enable global interrupt
    // calculate the value to be loaded to the timer
    timer_value = 65536 - ( (TICK_TIME * F_OSC ) / (1000 * 4 * 8 ) );
    TMR1L = timer_value ; 
    TMR1H = ( timer_value >> 8 ) ;
}

/******************************************************************************
* Description : start the scheduling  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void SCH_Start(void)
{
    SET_BIT(T1CON , 0);
}
/******************************************************************************
* Description : stop the scheduling  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void SCH_Stop(void)
{
    CLEAR_BIT(T1CON , 0);
    int i = 0 ;
    for (i = 0 ; i < SCH_MAX_TASKS ; i++){
        SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
    }
}
/******************************************************************************
* Description : add tasks to the scheduler   
* Parameters  : function name  void (* pFunction)()
 *              delay till execution const tWord DELAY
 *              period of the task const tWord PERIOD
* Return type : tByte                                                                           
*******************************************************************************/
tByte SCH_Add_Task(void (* pFunction)(),
    const tWord DELAY,
    const tWord PERIOD)
    {
    tByte Index = 0;
    // First find a gap in the array (if there is one)
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }
    // Have we reached the end of the list?
    if (Index == SCH_MAX_TASKS)
    {
        // Task list is full
        //
        // Set the global error variable
        //Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        // Also return an error code
        return SCH_MAX_TASKS;
    }
    // If we're here, there is a space in the task array
    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Delay = DELAY;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;
    return Index; // return position of task (to allow later deletion)
}

/******************************************************************************
* Description : put the micro into sleep  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void SCH_Go_To_Sleep(void)
{
   asm( " SLEEP " );
}

/******************************************************************************
* Description : This is the 'dispatcher' function. When a task (function)
                is due to run, SCH_Dispatch_Tasks() will run it.
                This function must be called (repeatedly) from the main loop. 
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void SCH_Dispatch_Tasks(void)
{
    tByte Index;
    // Dispatches (runs) the next task (if one is ready)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if (SCH_tasks_G[Index].RunMe > 0)
        {
            (*SCH_tasks_G[Index].pTask)(); // Run the task
            SCH_tasks_G[Index].RunMe -= 1; // Reset / reduce RunMe flag
            // Periodic tasks will automatically run again
            // - if this is a 'one shot' task, remove it from the array
            if (SCH_tasks_G[Index].Period == 0)
            {
                SCH_Delete_Task(Index);
            }
        }
    }
    // Report system status
    SCH_Report_Status();
    // The scheduler enters idle mode at this point
    SCH_Go_To_Sleep();
}


/******************************************************************************
* Description : delete a given task  
* Parameters  : the task index ( const tByte TASK_INDEX )
* Return type : tByte                                                                           
*******************************************************************************/
tByte SCH_Delete_Task(const tByte TASK_INDEX)
{
    tByte Return_code;
    if (SCH_tasks_G[TASK_INDEX].pTask == 0)
    {
        // No task at this location...
        //
        // Set the global error variable
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        Return_code = RETURN_ERROR;
    }
    else
    {
        Return_code = RETURN_NORMAL;
    }
    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;
    return Return_code; // return status
}

/******************************************************************************
* Description : report status must enable SCH_REPORT_ERRORS in Sch_16f.h  
* Parameters  : none 
* Return type : void                                                                           
*******************************************************************************/
void SCH_Report_Status(void)
{
    #ifdef SCH_REPORT_ERRORS 
    // ONLY APPLIES IF WE ARE REPORTING ERRORS
    // Check for a new error code
    if (Error_code_G != Last_error_code_G)
    {
        // Negative logic on LEDs assumed
        Error_port = 255 - Error_code_G;
        Last_error_code_G = Error_code_G;
        if (Error_code_G != 0)
        {
            Error_tick_count_G = 60000;
        }
        else
        {
            Error_tick_count_G = 0;
        }
    }
    else
    {
        if (Error_tick_count_G != 0)
        {
            if (--Error_tick_count_G == 0)
            {
                Error_code_G = 0; // Reset error code
            }
        }
    }
    #endif
}



