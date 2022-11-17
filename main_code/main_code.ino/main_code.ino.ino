#include "distance_gp2.h"
#include "ir_sensor.h"
#include "line_follower.h"
#include "ultrasonic_sensor.h"
#include "motor_control.h"

// Front Line Follower Sensor Pin Out
#define FLFS_R_pin 23
#define FLFS_M_pin 25
#define FLSF_L_pin 27

// Back Line Follower Sensor Pin Out
#define BLFS_R_pin 43
#define BLFS_M_pin 41
#define BLFS_L_pin 39

// Stepper Motors Pin Out
//Back Left
#define BL_Wheel_Enable 47
#define BL_Wheel_Direction 3
//Back Right
#define BR_Wheel_Enable 51
#define BR_Wheel_Direction 5
//Front Left
#define FL_Wheel_Enable 49
#define FL_Wheel_Direction 4
//Front Right
#define FR_Wheel_Enable 53
#define FR_Wheel_Direction 6

// Front Right IR Sensor
#define FR_Turn_Sensor 36
// Back Left IR Sensor
#define BL_Turn_sensor 34

// Back GP2 Distance Sensor
#define GP2_Sensor A0

// Ultrasonic Sensor
#define ultrasonic_trig 2
#define ultrasonic_echo A7

// WDT
#define WDT_KEY (0xA5)


void watchdogSetup(void) {
  /*** watchdogDisable (); ***/
}

void WDT_Handler(void)
{
  /* Clear status bit to acknowledge interrupt by dummy read. */
  WDT->WDT_SR; // Clear status register

  printf("help! in WDT\n");
}

void setup() {

  // Enable watchdog.
  WDT->WDT_MR = WDT_MR_WDD(0xFFF) |
                WDT_MR_WDFIEN |  //  Triggers an interrupt or WDT_MR_WDRSTEN to trigger a Reset
                WDT_MR_WDV(256 * 5); // Watchdog triggers a reset or an interrupt after 5 seconds if underflow
  // 2 seconds equal 84000000 * 5 = 420000000 clock cycles
  /* Slow clock is running at 32.768 kHz
    watchdog frequency is therefore 32768 / 128 = 256 Hz
    WDV holds the period in 256th of seconds  */
  NVIC_EnableIRQ(WDT_IRQn);

  Serial.begin(115200);
}

void loop()
{

  //Restart watchdog
  WDT->WDT_CR = WDT_CR_KEY(WDT_KEY)
                | WDT_CR_WDRSTT;

}
