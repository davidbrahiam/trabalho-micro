/***************************************************************
*  Name    : standard.h                                        *
*  Author  : Owen O' Hehir                                     *
*  Notice  : Copyright (c) 2007                                *
*          : All Rights Reserved                               *
*  Date    : 1/1/2006                                          *
*  Version : 1.0                                               *
*Description: Standard Header file							   *
****************************************************************/

#ifndef standard
#define standard
	
	// Standard definitions. Note 'u' means unsigned
	#define TRUE 1u
	#define FALSE 0u
	#define SUCCESS 1u
	#define FAIL 0u
	#define ODD 1u
	#define EVEN 0u
	#define NULL 0			// ALREADY DEFINIED IN stddeh.h
	#define DONE 0u
	#define NOT_DONE 1u
	#define ZERO 0u
	#define LOW 0u
	#define HIGH 1u	
		
	/////////////////////////////////////////////////////////////////////////////////
	//                                                                             //
	//                                                                             //
	#define FOSC 2						// Oscillator Freq 20Mhz 				   //
	//                                                                             //
	//                                                                             //
	/////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	//                                                                             //
	//                                                                             //
	#define MPLAB_SIM FALSE								// USING MPLAB SIM??   	   //
	//                                                 	// USE TO DISABLE LCD ETC..//
	//                                                                             //
	/////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////
	//                                                                             //
	//                                                                             //
	#define DEBUG 0	                                    	// CHANGE WHEN FINISHED//
	//									                      					   //
	//                                                                             //
	/////////////////////////////////////////////////////////////////////////////////
	
	/* Debug levels:
		0	All off
		1
	*/	
#endif
