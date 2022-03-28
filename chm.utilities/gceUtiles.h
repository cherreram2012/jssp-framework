//==========================================================================//
//		        Funciones de Utileria del GCE-CAD/CAM. UDG										//
//      	    Ultima revision: 15 de enero del 2011		 											//
//==========================================================================//
#pragma once
#ifndef gceUtiles_H
#define gceUtiles_H

// Determina si la cadena <value> representa un numero decimal
int IsDecimalNumber ( char value );

// Trunca un numero flotante deternimado por <value> a la <presicion> deseada.                                    
char *Truncate      ( double value, int precision );

// Redondea un numero flotante deternimado por <value> a la presicion especifica         
// por <digits>                                 
double Round        ( double value, unsigned int digits = 1 );

// Devuelve la potencia <pow> inmediata inferior menor que <value>		
int LowerPow        ( int value, int pow);

// Devuelve la potencia <pow> inmediata superior mayor que <value>	
int UpperPow       ( int value, int pow);

// Elimina del principio de la cadena los caracteres del argumento 'pattern'							
void LTrim 					( char *string );

// Elimina del final de la cadena los caracteres del argumento 'pattern'											
void RTrim 					( char *string );

// Elimina del principio y el final de la cadena los caracteres del argumento 'pattern'				
void Trim  					( char *string );

// Divide
char **ExplodeString ( char *string, char delimiter, int *count_token );
#endif


