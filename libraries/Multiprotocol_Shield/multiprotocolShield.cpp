/*! \file multiprotocolShield.cpp
    \brief Library for managing the Multiprotocol Radio Shield v.2.0
    
    Copyright (C) 2015 Libelium Comunicaciones Distribuidas S.L.
    http://www.libelium.com
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
  
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
    Version:		1.0
    Implementation:	Ahmad Saad

 */

#include <multiprotocolShield.h>

//**********************************************************************
// Instances of the classes
//**********************************************************************
socket1Class socket1;
socket0Class socket0;


//!*************************************************************
//!	Name: begin()												
//!	Description: Initializes the I/O pins of theshield
//!	Param : void									
//!	Returns: void
//!*************************************************************
void multiprotocolShield::begin()
{
	mcp.begin();
	delay(10);
	
	// I/O configuration
	mcp.pinMode(0, OUTPUT);
	mcp.pinMode(1, OUTPUT);
	mcp.pinMode(2, OUTPUT);
	mcp.pinMode(3, OUTPUT);
	mcp.pinMode(4, OUTPUT);
	mcp.pinMode(5, OUTPUT);
	mcp.pinMode(6, INPUT);
	mcp.pinMode(7, OUTPUT);
	delay(100);
	
	// Initial State
	mcp.digitalWrite(0, LOW);
	mcp.digitalWrite(1, LOW);
	mcp.digitalWrite(2, LOW);
	mcp.digitalWrite(3, HIGH);
	mcp.digitalWrite(4, HIGH);
	mcp.digitalWrite(5, LOW);
	//mcp.digitalWrite(6, HIGH);
	mcp.digitalWrite(7, HIGH);
	delay(100);
}

//!*************************************************************
//!	Name: disableMUX()											
//!	Description: Disables the multiplexor 
//!	Param : void										
//!	Returns: void
//!*************************************************************
void multiprotocolShield::disableMUX(void)
{
	mcp.digitalWrite(4, HIGH);
}


//!*************************************************************
//!	Name: socket1Class()											
//!	Description: Class constructor 
//!	Param : void									
//!	Returns: void
//!*************************************************************
socket1Class::socket1Class() {}


//!*************************************************************
//!	Name: ON()												
//!	Description: Switches ON the power suplly of the SOCKET1
//!	Param : void										
//!	Returns: void
//!*************************************************************
void socket1Class::ON()
{
	
	if (socket0.socket_state != 1)
		begin();
	
	delay(5);
	mcp.digitalWrite(0, HIGH);
	
	socket1.socket_state = 1;
	
}


//!*************************************************************
//!	Name: OFF()										
//!	Description: Switches OFF the power suplly of the SOCKET1 
//!	Param : void								
//!	Returns: void
//!*************************************************************
void socket1Class::OFF()
{
	mcp.digitalWrite(0, LOW);
	socket1.socket_state = 0;
}
	
	
//!*************************************************************
//!	Name: setMUX()												
//!	Description: Configures the multiplexor in the SOCKET1
//!	Param : void										
//!	Returns: void
//!*************************************************************	
void socket1Class::setMUX(void)
{
	mcp.digitalWrite(4, LOW);
	mcp.digitalWrite(5, HIGH);
}


//!*************************************************************
//!	Name: setCS()												
//!	Description: Enables the SPI of the SOCKET1 (5V Level)
//!	Param : void										
//!	Returns: void
//!*************************************************************
void socket1Class::setCS(void)
{	
	mcp.digitalWrite(2, LOW);
	delay(1);
}


//!*************************************************************
//!	Name: unsetCS()												
//!	Description: Disables the SPI of the SOCKET1
//!	Param: void										
//!	Returns: void
//!*************************************************************
void socket1Class::unsetCS(void)
{
	mcp.digitalWrite(2, HIGH);
	delay(1);
}


//!*************************************************************
//!	Name: socket0Class()										
//!	Description: Class contructor
//!	Param : void									
//!	Returns: void
//!*************************************************************
socket0Class::socket0Class(){}


//!*************************************************************
//!	Name: ON()												
//!	Description: Switches ON the power supply of the SOCKET0
//!	Param : void										
//!	Returns: void
//!*************************************************************
void socket0Class::ON()
{
	if (socket1.socket_state != 1)
		begin();
	
	delay(5);
	mcp.digitalWrite(1, HIGH);
	
	socket0.socket_state = 1;
}


//!*************************************************************
//!	Name: OFF()												
//!	Description: Switches OFF the power supply of the SOCKET0
//!	Param : void										
//!	Returns: void
//!*************************************************************
void socket0Class::OFF()
{
	mcp.digitalWrite(1, LOW);
	socket0.socket_state = 0;
}


//!*************************************************************
//!	Name: setMUX()												
//!	Description: Configures the multiplexor in the SOCKET0
//!	Param : void										
//!	Returns: void
//!*************************************************************
void socket0Class::setMUX(void)
{
	mcp.digitalWrite(4, LOW);
	mcp.digitalWrite(5, LOW);
}


//!*************************************************************
//!	Name: setCS()												
//!	Description: Enables the SPI of the SOCKET0 (3V3 Level)
//!	Param : void										
//!	Returns: void
//!*************************************************************
void socket0Class::setCS(void)
{
	mcp.digitalWrite(3, LOW);
	delay(3);
}


//!*************************************************************
//!	Name: setCS()												
//!	Description: Disables the SPI of the SOCKET0 (3V3 Level)
//!	Param : void										
//!	Returns: void 
//!*************************************************************
void socket0Class::unsetCS(void)
{	
	mcp.digitalWrite(3, HIGH);
	delay(2);
}


