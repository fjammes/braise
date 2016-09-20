/*! \file multiprotocolShield.h
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


/***********************************************************************
 * Includes
 ***********************************************************************/
#include "Arduino.h"

// This library if for managing the pin expansor
#include "../MCP23008/MCP23008.h"

#ifndef inttypes_h
	#include <inttypes.h>
#endif 

/***********************************************************************
 * Class
 ***********************************************************************/

#ifndef MULTIPROTOCOLSHIELD_H
#define MULTIPROTOCOLSHIELD_H

/***********************************************************************
 * Main Class
 ***********************************************************************/
class multiprotocolShield
{

//**********************************************************************	
// Public functions. 
//**********************************************************************	

	public:
		
		
		void begin();
	
		void ON();
		void OFF();
		
		void setMUX(void);
		void disableMUX(void);
		
		void setCS(void);
		void unsetCS(void);

//**********************************************************************	
// Private functions. 
//**********************************************************************	

		private:

};


#endif

#ifndef SOCKET1CLASS_H
#define SOCKET1CLASS_H

/***********************************************************************
 * This class derivate from multiprotocolShield
 * This class manages the SOCKET1 of the shield
 ***********************************************************************/
class socket1Class : public multiprotocolShield
{
	public:

		socket1Class();
		
		void ON();
		void OFF();
		void setMUX(void);
		void setCS(void);
		void unsetCS(void);
		
		uint8_t socket_state;
		
};

extern socket1Class socket1;

#endif



#ifndef SOCKET0CLASS_H
#define SOCKET0CLASS_H

/***********************************************************************
 * This class derivate from multiprotocolShield
 * This class manages the SOCKET0 of the shield
 ***********************************************************************/
class socket0Class : public multiprotocolShield
{
	public:
		
		socket0Class();
		
		void ON();
		void OFF();		
		void setMUX(void);		
		void setCS(void);
		void unsetCS(void);
		
		uint8_t socket_state;
		
};

extern socket0Class socket0;

#endif

