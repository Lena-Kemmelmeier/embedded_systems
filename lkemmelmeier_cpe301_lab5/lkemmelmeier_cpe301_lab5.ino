/*
 * Name: Lena Kemmelmeier
 * Lab Partner: Emma Cornia
 * Assignment: Lab 5
 * Class: CPE 301.1104
 * Date: 3/6/2024
*/

//
// Program to echo serial input characters from
// Putty keyboard back to Putty display.
// This version uses proper pointers.
//
//Written by D. Egbert, Version 1.5, 04/03/2017
//              Shawn Ray, Version 1.6 6/19/2022
//

//
// Program to echo serial input characters from
// Putty keyboard back to Putty display.
// This version uses proper pointers.
//
//Written by D. Egbert, Version 1.5, 04/03/2017
//              Shawn Ray, Version 1.6 6/19/2022
//

 #define RDA 0x80
 #define TBE 0x20  
 volatile unsigned char *myUCSR0A = (unsigned char *)0x00C0;
 volatile unsigned char *myUCSR0B = (unsigned char *)0x00C1;
 volatile unsigned char *myUCSR0C = (unsigned char *)0x00C2;
 volatile unsigned int  *myUBRR0  = (unsigned int *) 0x00C4;
 volatile unsigned char *myUDR0   = (unsigned char *)0x00C6;
//
// main()
//
void setup()
{
  // initialize the serial port on USART0:
  U0init(9600);
}
void loop()
{
  unsigned char cs1;
  while (U0kbhit()==0){}; // wait for RDA = true
  cs1 = U0getchar();    // read character

  char firstDigit = ((cs1 & 0xF0) >> 4) + 0x30;
  char secondDigit = (cs1 & 0x0F) + 0x30;

  if(cs1 != '\n') {
    U0putchar('0');
    U0putchar('x');
    U0putchar(firstDigit);
    U0putchar(secondDigit);
    U0putchar('\n');
  }
}
//
// function to initialize USART0 to "int" Baud, 8 data bits,
// no parity, and one stop bit. Assume FCPU = 16MHz.
//
void U0init(unsigned long U0baud)
{
//  Students are responsible for understanding
//  this initialization code for the ATmega2560 USART0
//  and will be expected to be able to intialize
//  the USART in differrent modes.
//
  unsigned long FCPU = 16000000;
  unsigned int tbaud;
  tbaud = (FCPU / 16 / U0baud - 1);
  // Same as (FCPU / (16 * U0baud)) - 1;
  *myUCSR0A = 0x20;
  *myUCSR0B = 0x18;
  *myUCSR0C = 0x06;
  *myUBRR0  = tbaud;
}
//
// Read USART0 RDA status bit and return non-zero true if set
//
unsigned char U0kbhit() {
  return (*myUCSR0A & RDA);
}
//
// Read input character from USART0 input buffer
//
unsigned char U0getchar() {
  unsigned char c;
  while(!(*myUCSR0A & (1 << 7)));
  c = *myUDR0;
  return c;
}
//
// Wait for USART0 (myUCSR0A) TBE to be set then write character to
// transmit buffer
//
void U0putchar(unsigned char U0pdata) {
  while(!(*myUCSR0A & TBE));
  *myUDR0 = U0pdata;
}