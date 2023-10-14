#include <avr/io.h>
#include <util/delay.h>
#include "project.h"
#include <string.h>

#define BAUD 9600 //defining the BAUD rate globally

const int PUSH_BUTTON = 7; //push button at digital pin 7
const int LED = 8; //LED at digital pin 8
int LED_state = 0b00000001; //beginning state of LED = 1. state = 1 then LED is off, state = 2 then LED is on.

int main(void){
DDRD |= 0b10000000;
DDRB |= 0b00000001;

char On_Command[] = "led_on"; //character arrays containing set commands
char Off_Command[] = "led_off";
char receive_string[8];
int counter = 0;
char received;

uart_init(BAUD); //initialize the BAUDRate setup

int prevButton_state = digitalRead(PUSH_BUTTON); //put the previous state in a variable, used to read the rising edge signal of a button

while(1){ //loop for running system
if((UCSR0A)&(1<<RXC0)){

received = uart_receive(); //user input command for LED

if(received != '\n'){
receive_string[counter++] = received;
}

else{
if( counter == 6 && strncmp(receive_string,On_Command,6)==0 ){ //compare users inputted command with correct command
uart_transmit_array("LED Is On\n",10);
PORTB |= 0b00000001;
}

else if( counter == 7 && strncmp(receive_string,Off_Command,7)==0){ //compare users inputted command with correct command
uart_transmit_array("LED Is Off\n",11);
PORTB &= 0b11111110;
}

else{
uart_transmit_array("ERROR Incorrect Command Inputted\n",33); //error message if command incorrect
}

counter = 0;
}
}

int Button_state = digitalRead(PUSH_BUTTON); //put the state of the button in a variable

if(Button_state == HIGH && Button_state != prevButton_state){ //check for rising edge signal of a button
uart_transmit_array("Button Pressed\n",15);
}

else if (Button_state == LOW && Button_state != prevButton_state){
uart_transmit_array("Button Not Pressed\n",19);
}

prevButton_state  = Button_state; //setting new previous button state for rising edge detection
}

return 0;
}

