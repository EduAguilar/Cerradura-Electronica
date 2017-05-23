//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// Cerradura Electronica con Alarma sonora
// 
// Made by Edu Aguilar
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/4958982-cerradura-electronica-con-alarma-sonora

// Se incluyen Librerias a utilizar:

//Libreria para el teclado
#include <Keypad.h>
//Libreria para el LCD
#include <LiquidCrystal.h>


/*
   Conexión del LCD al arduino mediante sus pins analógicos:
	* Pin A0 del Arduino con RS(Register Select) del LCD
	* Pin A1 del Arduino con E(Enable) del LCD
	* Pin A2 del Arduino con DB4 del LCD
	* Pin A3 del Arduino con DB5 del LCD
	* Pin A4 del Arduino con DB6 del LCD
	* Pin A5 del Arduino con DB7 del LCD    

   Conexión del KeyPad al Arduino mediante sus pines digitales:
    * Fila 1 del KeyPad con pin D9 del Arduino
    * Fila 2 del KeyPad con pin D8 del Arduino
    * Fila 3 del KeyPad con pin D7 del Arduino
    * Fila 4 del KeyPad con pin D6 del Arduino

    * Columna 1 del KeyPad con pin D5 del Ardui no
    * Columna 2 del KeyPad con pin D4 del Ardui no
    * Columna 3 del KeyPad con pin D3 del Arduino
    * Columna 4 del KeyPad con pin D2 del Arduino
*/

LiquidCrystal lcd (A0, A1, A2, A3, A4, A5);//Definimos los pines para la pantalla

//Matriz del KeyPad 4x4
const byte Filas = 4;  //Cuatro filas
const byte Cols = 4;   //Cuatro columnas
byte Pins_Filas[] = {9,8,7,6};//Pines de Arduino a las filasbyte 
byte Pins_Cols[] = {5,4,3,2}; // Pines de Arduino a las columnas.
//no se utilizan los pines 1 y 0 para no interferir en Rx y Tx


//Definimos el teclado
char Teclas [ Filas ][ Cols ] =
 {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
 };

const int CantDigClave = 4; //Tamaño de la clave
char codigoSecreto[CantDigClave] = {'2','2','5','5'}; // Aqui va el codigo secreto


int posicion=0;    // necesaria para la clave
int cursor=5;      // posicion inicial de la clave en el LCD
int clave=0;       // Valor para indicar que la clave es correcta, 0 = incorrecta, 1 = Correcta
int ledVerde=12;   // pin para el LED verde
int ledRojo=11;    // pin para el LED rojo
int buzzer=10;     // pin altavoz
int cont=0;		   // Contador de errores



  // Create the Keypad

  Keypad Teclado = Keypad(makeKeymap(Teclas), Pins_Filas, Pins_Cols, Filas, Cols);

  void setup() 

  {
      Serial.begin(9600) ;
      lcd.begin(16,2);       // inicializamos el LCD.
      pinMode (ledVerde,OUTPUT);
      pinMode (ledRojo, OUTPUT);
      pinMode (buzzer, OUTPUT);
      digitalWrite(ledRojo,HIGH); // encendemos el LED rojo
      digitalWrite(ledVerde, LOW); // apagamos el verde
      lcd.setCursor(0,0);     // situamos el cursor el la posición 0 de la linea 0.
      lcd.print("Introduzca clave"); // escribimos en LCD
      lcd.setCursor(cursor,1); // cursor en la posicion de la variable, linea 1
      lcd.print("__a_"); // borramos de la pantalla los numeros  
	  lcd.setCursor(cursor,1);    
  }

  void loop() 

  {
      char pulsacion = Teclado.getKey() ; // leemos pulsacion
      //lcd.setCursor(0, 1);
      if (pulsacion != 0) //Si el valor es 0 es que no se ha pulsado ninguna tecla
        { // descartamos almohadilla y asterisco
          if (pulsacion != '#' && pulsacion != '*' && clave==0)
           { lcd.print(pulsacion); // imprimimos pulsacion
             cursor++;             // incrementamos el cursor
             tone(buzzer,800);     // tono de pulsacion
             delay(10);
             noTone(buzzer);

      //--- Condicionales para comprobar la clave introducida -----------
      // comparamos entrada con cada uno de los digitos, uno a uno
      if (pulsacion == codigoSecreto[posicion])
          posicion ++; // aumentamos posicion si es correcto el digito

      if (posicion == CantDigClave)
       { // comprobamos que se han introducido los 4 digitos correctamente
         lcd.setCursor(0,0);      // situamos el cursor el la pos 0 de la linea 0.
         lcd.print(" Clave correcta ");// escribimos en LCD     
         delay(200);
         // tono de clave correcta
         tone(buzzer,500);
         delay(100);
         noTone(buzzer);
         tone(buzzer,600);
         delay(100);
         noTone(buzzer);
         tone(buzzer,800);
         delay(200);
         noTone(buzzer);

         lcd.setCursor(0,1); // cursor en la posicion 0, linea 1
      	 lcd.print("  tiene 10 seg   ");            
         clave=1; // indicamos que la clave es correcta
         digitalWrite(ledRojo,LOW); // apagamos el LED rojo
         digitalWrite(ledVerde, HIGH); // encendemos el verde
         
         delay(6000);//seteamos que esperara solo 6 segundos para volver a cerrar el sistema
         clave=0; // indicamos que se ha borrado la clave
         cursor=5;
         posicion=0;
         digitalWrite(ledVerde, LOW); // Apagamos el verde         
         digitalWrite(ledRojo,HIGH); // Encendemos el LED rojo
      	 lcd.setCursor(0,0);     // situamos el cursor el la posición 0 de la linea 0.
      	 lcd.print("Introduzca Clave"); // escribimos en LCD
         lcd.setCursor(0,1); // cursor en la posicion 0, linea 1
      	 lcd.print("      ____       "); // borramos de la pantalla los numeros
         lcd.setCursor(5,1);
         cont=0;//reseteamos el contador de errores         

     }
            
     //--- En el caso de que este incompleta o no hayamos acertado ----------
     if(cursor>8)        // comprobamos que no pase de la cuarta posicion
       {   cursor=5;     // lo volvemos a colocar al inicio
           posicion=0;           // borramos clave introducida
           lcd.setCursor(5,1);
           lcd.print("____");  // borramos la clave de la pantalla	        
           lcd.setCursor(5,1);      
           if(clave==0)         // comprobamos que no hemos acertado
              { tone(buzzer,70,500); // para generar tono de error
                delay(250); 
                noTone(buzzer);
                cont++;//Aumentamos contador de errores
                lcd.setCursor(0,0);     // situamos el cursor el la posición 0 de la linea 0.
      	 		lcd.print("Clave Incorrecta"); // escribimos en LCD
                delay(3000);//tiempo de espera de 3 segundos
         		lcd.setCursor(5,1); // cursor en la posicion 5, linea 1
                lcd.setCursor(0,0);     // situamos el cursor el la posición 0 de la linea 0.
      	 		lcd.print("Introduzca clave"); // escribimos en LCD
                lcd.setCursor(5,1); // cursor en la posicion 0, linea 1
              }
          
        Serial.println(cont);
        //Si acaso la cantidad de errores es igual a 3 se activará la alarma sonora
          if(cont==3)
        	{
             tone(buzzer,800,500);//Tono de Alarma
             cont=0;//reseteamos el contador de errores
        	}        
        }
     }
   } 

 //--- Condicionales para resetear clave introducida -------------
 if (pulsacion == '*')
     { // se debe presionar asterisco para borra los digitos ingresados
       posicion = 0;
       cursor = 5;
       clave=0;
       posicion=0;
       lcd.setCursor(0,0); // situamos el cursor el la posición 2 de la linea 0.
       lcd.print("Introduzca clave"); // escribimos en LCD
       lcd.setCursor(5,1);
       lcd.print("____"); // borramos de la pantalla los numeros
       lcd.setCursor(5,1);
    
       digitalWrite(ledRojo,HIGH); // encendemos el LED rojo
       digitalWrite(ledVerde, LOW); // apagamos el verde
    }
 }
