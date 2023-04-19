/*
 * UserUtils.c
 *
 *  Created on: Apr 18, 2023
 *      Author: Wring8
 */

#include <UserUtils.h>

/*
 * Converts a uint8_t with 0 as full forward, 127 as inactive, and 255 as full reverse to a uint16_t with 1100 as full backward, 1500 as inactive, and 1900 as full forward
 */
PUBLIC pwm_t MapInputToPWM(uint8_t input, const double scale, pwm_t minValue){
	//SerialPrintln("\tMapInputToPWM: %d",THRUSTER_SAFE_MIN_VALUE + (pwm_t)(PWM_SCALE * input));
	return minValue + (pwm_t)(scale * input);
}



//These ones may be useful in the future but have not been properly tested yet



/*
 * Converts a uint8_t into 2 chars '0'-'F' stored in a uint16_t
 */
//PUBLIC uint16_t int8toHexInt16(uint8_t num){
//	char hex[2];
//	uint8_t a = (num & 0xF0)>>4;
//	hex[0] = a < 10 ? a + '0' : a-10 + 'A';
//	uint8_t b = (num & 0x0F);
//	hex[1] = b < 10 ? b + '0' : b-10 + 'A';
//	return *((uint16_t*)hex);
//}

/*
 * Converts a 32 bit float into 8 chars '0'-'F'
 */
//PUBLIC const char * floatToConstChar(float flt){
//	uint8_t* floatAs4uint8s = ((uint8_t*)(&flt));		//store 32 bit float in 4 uint8_t
//	uint16_t floatAs4uint16s[4];							//declare 4 uint16_t. each will store 2 ascii characters '0'-'F'. 8 characters representing 4 bits each to form the float
//
//	floatAs4uint16s[0] = int8toHexInt16( floatAs4uint8s[0] );		//assign the 4 uint16_t's
//	floatAs4uint16s[1] = int8toHexInt16( floatAs4uint8s[1] );
//	floatAs4uint16s[2] = int8toHexInt16( floatAs4uint8s[2] );
//	floatAs4uint16s[3] = int8toHexInt16( floatAs4uint8s[3] );
//
//	const char * floatAs8chars = (char*)floatAs4uint16s;		//cast from uint16_t to const char
//	return floatAs8chars;
//}
