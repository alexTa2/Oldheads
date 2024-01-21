//******************************************************************************
//
//  Description: This file contains the function prototypes used in all files
//
//  Alex Tayyeb
//  Sept 2022
//  Built with IAR Embedded Workbench Version:(7.20.1)
//******************************************************************************
// Functions

// Initialization
//----------------------------
void Init_Conditions(void); //Init_Conditions prototype
void Display_Process(void); // Display_Process prototype
void Init_LEDs(void); //Init_LEDS prototype
void Init_ADC(void); //Init_ADC Prototpye
void Init_Clocks(void); // Init Clocks prototpype
void Init_LEDs(void); //Init LEDs prototype
void Init_Serial_UCA1(void); //Init serial port for UCA1 prototype
void Init_Serial_UCA0(void); //Init Serial port of UCA0 prototype
void Init_IOT(void); //Init IOT prototype
//----------------------------

//Line Detection
//----------------------------
void detectWhite(void); //white line detection function prototype
void detectBlack(void); //black line detection function prototype
void lineFollowerPID(void); //line follower PID function prototype
void lineTurn(void); //EV lining up function prototype
//----------------------------

// Interrupts
void enable_interrupts(void); //function to enable interrupts function prototype
__interrupt void Timer0_B0_ISR(void); // Timer B0 ISR function prototype
__interrupt void switch_interrupt(void); //switch interrupt function prototype


//Command Structure
//----------------------------
void commandsReceived(void); //Commands received function prototype
void endOfCommand(char); //endOfCommandChar function prototype
void clearCommand(void); //clearCommand function prototype
void runCommandInstant(char); //runCommandInstant function prototype
void IOT_Setup(void); //IOT_Setup function prototype
void nextCommand(void); //next command function prototype
void pingServer(void); //ping the server function prototype
//----------------------------


////Display ADC Functions
//----------------------------
void hex2bcd(unsigned int); //hex value to binary coded decimal function prototype
void displayADC(char, char); //display ADC values function prototype
//----------------------------


//Serial Functions
//----------------------------
void USCI_A1_transmit(void); //enable transmit to UCA1 function prototype
void USCI_A0_transmit(void); //enable transmit to UCA0 function prototype
void IOT_Process(void); //enable the IOT function prototype
//----------------------------


// LCD
//----------------------------
void Display_Process(void); //Display process function prototype
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4); //display update function prototype
void Init_LCD(void); //initialize LCD function prototype
void lcd_BIG_mid(void); //cahange lcd to 3 lines function prototype
void lcd_4line(void); //change lcd to 4 lines function prototype
//----------------------------



// Menu
//----------------------------
void Menu_Process(void); //Menu Process function prototype
//----------------------------

// Ports----------------------
void Init_Ports(void); //Init ports function prototype
void Init_Port1(void); //Init port1 function prototype
void Init_Port2(void);//Init port2 function prototype
void Init_Port3(char smclk); //Init port3 function prototype
void Init_Port4(void);//Init port4 function prototype
void Init_Port5(void);//Init port5 function prototype
void Init_Port6(void);//Init port6 function prototype
//----------------------------
// Timers
//----------------------------
void Init_Timers(void); //Init Timers function prototype
void Init_Timer_B0(void); //Init timer B0 prototype
void Init_Timer_B1(void); //Init timer B1 prototype
void Init_Timer_B2(void); //Init timer B2 prototype
void Init_Timer_B3(void); //Init timer B3 prototype
//----------------------------






