#include <math.h>
#include <stdlib.h>
#include <string.h>

#define GCE_LENGHT 64

#include "gceUtiles.h"

//---------------------------------------------------------------------------
//	Determina si una cadena representa un numero decimal
//---------------------------------------------------------------------------
int IsDecimalNumber (char *strNum)
{
  if (strchr(strNum, '.'))
    return 1;
  else
    return 0;
}

//---------------------------------------------------------------------------
//	Trunca un valor decimal con la precicion dada
//---------------------------------------------------------------------------
char *Truncate (double value, int precision)
{
  char *str, *str_i, str_s[GCE_LENGHT] = "-";
  int dec, sig, len;
  bool decimalpoint = false;

  // Se convierte el valor decimal a cadena
  str = _fcvt(value, precision, &dec, &sig);
  str_i = new char [strlen(str)];
  strcpy(str_i, str);
	// Se construye el valor decimal pero en cadena
  if (dec < 0){
    strcpy(str, "0.");
    for (int i = dec; i < 0; i++)
      strcat(str, "0");
    dec = 0;
  }
  else if (dec == 0)
    strcpy(str, "0.");
  else{
	  str[dec] = '.';
	  str[dec + 1] = '\0';
  }

  // Se contatena lo que hay de la cadena despues del punto
  strcat(str, str_i + dec);
  // Se verifica si era un valor negativo
  if (sig != 0){
		strcat(str_s, str);
		strcpy(str, str_s);
	}

  // Elmina los ceros al final de la cadena que estan despues del punto
  len = strlen(str) - 1;
  while ((str[len] == '0' || str[len] == '.') && decimalpoint == false){
    if (str[len] == '.') decimalpoint = true;
    str[len] = '\0';
    len--;
  }

  delete []str_i;
  return str;
}

//---------------------------------------------------------------------------
//	Redondea un numero flotante deternimado por <value> a la presicion especifica 
//	por <digits> 
//---------------------------------------------------------------------------
double Round (double value, unsigned int digits) 
{ 
  signed neg = 1; 
  if(value < 0.0)
  { 
    neg = (-1); 
    value *= (-1); 
  } 

  double inc = 1.0; 
  while(digits)
  {
		inc *= 10.0; 
		--digits; 
  } 

  double dg = value * inc * 10.0;   
  value = floor(floor(dg) / 10.0);

  if ((dg - (value * 10.0)) > 4.9)
	  value += 1.0; 

	return ((value * neg) / inc); 
}

//---------------------------------------------------------------------------
//	Devuelve la potencia de 10 inmediata inferior menor que parametro <value>.
//--------------------------------------------------------------------------- 
int LowerPow (int value, int pow)
{
  while (value % pow != 0) value--;

  return (value > 0) ? value : 1;
}

//---------------------------------------------------------------------------
//	Devuelve la potencia de 10 enmediata superior mayor que parametro <value>.
//---------------------------------------------------------------------------
int UpperPow (int value, int pow)
{
  int p = 0;	
	while (p < value) p += pow;
  
	return p;
}

//---------------------------------------------------------------------------
//	Elimina los espacios que contenga a la izquierda (al principio) la cadena.
//---------------------------------------------------------------------------
void LTrim (char *string)
{
	// Elimino los espacios en el principio de la cadena
	while(string[0] == ' ' || string[0] == '\t' || string[0] == '\r' || string[0] == '\n')
		strcpy(string, string + 1);
}

//---------------------------------------------------------------------------
//	Elimina los espacios que contenga a la dewrecha (al final) la cadena.
//---------------------------------------------------------------------------
void RTrim (char *string)
{
	// Elimino los espacios al final de la cadena
  while(string[strlen(string) - 1] == ' ' || string[strlen(string) - 1] == '\t' ||
			   string[strlen(string) - 1] == '\r' || string[strlen(string) - 1] == '\n') 
		string[strlen(string) - 1] = '\0';
}

//---------------------------------------------------------------------------
//	Elimina los espacios que contenga la cadena a ambos lados.
//---------------------------------------------------------------------------
void Trim (char *string)
{
	if (string)   
  {
	  // Elimino los espacios en el principio de la cadena
		while(string[0] == ' ' || string[0] == '\t' || string[0] == '\r' || string[0] == '\n')
			strcpy(string, string + 1);

    if (*string == '\0') return;

	  // Elimino los espacios al final de la cadena
    while(string[strlen(string) - 1] == ' ' || string[strlen(string) - 1] == '\t' ||
			    string[strlen(string) - 1] == '\r' || string[strlen(string) - 1] == '\n') 
			string[strlen(string) - 1] = '\0';
	}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
char **ExplodeString (char *string, char delimiter, int *count_token)
{
	int i, c, count;
	char **token_list;

	c = count = 0;

	// Determino cuanto token saldran al llamar a strtok 
	for (i = 0; *(string + i) != '\0'; i++) if (*(string + i) == delimiter) count++;
	
	// Lleno la lista de tokens
	token_list = new char *[count + 1];
	token_list[0] = strtok(string, "|");
	c++;

	for(i = 1; i < count + 1; i++)
	{
		token_list[i] = strtok(NULL, "|"); 		
		if (token_list[i] != NULL)	c++;					
	}

	if (count_token) *count_token = c;
	return token_list;
}