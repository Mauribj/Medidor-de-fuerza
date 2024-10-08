#include <Arduino.h>
#include "HX711.h"
#define LED 22
float m[100];
float weightA128, aux;
int p;
int a = 0;
int tecla, tempset;
int h = 0;
int x = 1;
int c = 0, r;
int usuario;
float medicion[20];
int mes[20];
int fecha[20];
int temporizador = 0;

// Pin de datos y de reloj
byte pinData = 15;
byte pinClk = 14;

HX711 bascula;
 
// Parámetro para calibrar el peso y el sensor
float factor_calibracion = 40000; //Este valor del factor de calibración funciona para mi. El tuyo probablemente será diferente.


void setup()
{
  Serial.begin(9600);
  // wait for serial1 port to connect. Needed for native USB port only
  while (!Serial)
  {
    delay(10);
  }
  pinMode(LED, OUTPUT);
    // Iniciar sensor
  bascula.begin(pinData, pinClk);
 
  // Aplicar la calibración
  bascula.set_scale();
  // Iniciar la tara
  // No tiene que haber nada sobre el peso
  bascula.tare();
 
  // Obtener una lectura de referencia
  long zero_factor = bascula.read_average();
  // Mostrar la primera desviación
  a = 1;
}
void loop()
{
  switch (a)
  {
    // Comprueba que el Puerto Serial Funciona
    case 1:
      Serial.println("Inicio");
      delay(1000);
      // Eleccion de Nueva Medición o ver los Registros
      Serial.println("*************************");
      Serial.println("**   Menu Principal    **");
      Serial.println("*************************");
      Serial.println("-Iniciar medicion- s");
      Serial.println("-Registros- d");
      a = 2;
      break;
    case 2:
      tecla = Serial.read();
      if (tecla == 100)
      { // D Enter
        usuario = 1;
        r = 0;
        Serial.println("*************************");
        Serial.println("Registros:");
        Serial.println("s/w subir/bajar | a volver");
        Serial.print("Usuario ");
        Serial.println(usuario);
        Serial.print("Fecha de Revision: ");
        Serial.print(fecha[r]);
        Serial.print("/");
        Serial.println(mes[r]);
        Serial.print("Fuerza pico: ");
        Serial.print(medicion[r]);
        Serial.println(" Kgf");
        a = 10;
        x = 0;
      }
      if (tecla == 115)
      { // S Abajo
        Serial.println("*************************");
        Serial.println("Iniciar Nueva Medicion");
        Serial.println("d continuar | a volver");
        a = 3;
        x = 0;
      }
      break;
    // Se registra al usuario
    case 3:
      tecla = Serial.read();
      if (tecla == 100)
      { // enter
        // Pide Ingresar un Fecha de tratamiento
        Serial.println("*************************");
        Serial.println("Ingrese la fecha de tratamiento:");
        Serial.println("d continuar | a volver");
        Serial.println("+/- suma/resta 1 y w/s suma/resta 5");
        Serial.println(fecha[c]);
        a = 4;
        x = 0;
      }
      if (tecla == 97)
      {
        Serial.println("*************************");
        Serial.println("**   Menu Principal    **");
        Serial.println("*************************");
        Serial.println("-Iniciar medicion- s");
        Serial.println("-Registros- d");
        a = 2;
        x = 0;
      }
      break;
    case 4:
      tecla = Serial.read();
      if (tecla == 43)
      {
        if (fecha[c] < 31)
        { // tecla + suma de 1
          fecha[c] = fecha[c] + 1;
          Serial.println(fecha[c]);
        }
        else
        {
          fecha[c] = 31;
          Serial.println(fecha[c]);
        }
      }
      if (tecla == 45)
      {
        if (fecha[c] > 0)
        {
          fecha[c] = fecha[c] - 1;
          Serial.println(fecha[c]); // con el signo - resta de a 1
        }
        else
        {
          fecha[c] = 0;
          Serial.println(fecha[c]);
        }
      }
      if (tecla == 100)
      { // enter
        // Pide elegir el tiempo en el que se  va a ser la prueba
        Serial.println("*************************");
        Serial.println("Ingrese el mes de tratamiento:");
        Serial.println("d continuar | a volver");
        Serial.println("+/- suma/resta 1 y w/s suma/resta 5");
        Serial.println(mes[c]);
        a = 11;
        x = 0;
      }
      if (tecla == 119)
      {
        if (fecha[c] < 26)
        { // tecla W suma de 5
          fecha[c] = fecha[c] + 5;
          Serial.println(fecha[c]);
        }
        else
        {
          fecha[c] = 31;
          Serial.println(fecha[c]);
        }
      }
      if (tecla == 115)
      {
        if (fecha[c] > 0)
        {
          fecha[c] = fecha[c] - 5; // tecla S resta de 5
          Serial.println(fecha[c]);
        }
        else
        {
          fecha[c] = 0;
          Serial.println(fecha[c]);
        }
      }
      if (tecla == 97)
      {
        Serial.println("*************************");
        Serial.println("Iniciar Nueva Medicion");
        Serial.println("d continuar | a volver");
        a = 3;
        x = 0;
      }
      delay(1000);
      break;
    case 5:
      tecla = Serial.read();
      if (tecla == 43)
      {
        temporizador = temporizador + 1;
        Serial.println(temporizador); // con el signo + suma de a 1
      }
      if (tecla == 45)
      {
        if (temporizador > 0)
        {
          temporizador = temporizador - 1;
          Serial.println(temporizador); // con el signo - resta de a 1
        }
        else
        {
          temporizador = 0;
          Serial.println(temporizador);
        }
      }
      if (tecla == 119)
      { // arriba
        temporizador = temporizador + 5; // suma de a 5
        Serial.println(temporizador);
      }
      if (tecla == 115)
      { // abajo
        if (temporizador > 0)
        { // resta de a 5
          temporizador = temporizador - 5;
          Serial.println(temporizador);
        }
        else
        {
          temporizador = 0;
          Serial.println(temporizador);
        }
      }
      if (tecla == 100)
      {
        Serial.println("*************************");
        Serial.println("¿Comenzar Medicion? d - Si | a - No");
        a = 6;
        x = 0;
      }
      if (tecla == 97)
      {
        Serial.println("*************************");
        Serial.println("Ingrese el mes de tratamiento:");
        Serial.println("d continuar | a volver");
        Serial.println("+/- suma/resta 1 y w/s suma/resta 5");
        Serial.println(fecha[c]);
        a = 4;
        x = 0;
      }
      delay(1000);
      break;
    
    case 6:
      tecla = Serial.read();

      if (tecla == 100)
      {
        a = 7;
        x = 0;
      }
      if (tecla == 97)
      {
        temporizador = 0;
        Serial.println("*************************");
        Serial.println("Seleccione tiempo de Medicion");
        Serial.println("d continuar | a volver");
        Serial.println("+/-suma/resta 1 y w/s suma/resta 5");
        Serial.println(temporizador);
        a = 5;
        x = 0;
      }
      break;


    case 7:
      Serial.println("*************************");
      Serial.println("Tire del Agarre en:");
      h = 3;
      Serial.println(h);
      delay(1000);
      h = 2;
      Serial.println(h);
      delay(1000);
      h = 1;
      Serial.println(h);
      delay(1000);
      a = 8;
      break;

    // Serial.print(medicion);
    // Tiempo elegido
    case 8:
    // Aplicar calibración
  bascula.set_scale(factor_calibracion);
      tempset = temporizador;
      p = 0;
      digitalWrite(LED, HIGH); // enciende el led testigo
      Serial.println("Temporizador:");
      for (int e = 0; e < temporizador; e++)
      {
        Serial.println(tempset);
        for (int u = 0; u < 4; u++) {
          //lee la medicion en el canal A y lo guarda en un vector
          m[p] = bascula.get_units(), 3;
          p++;
          delay(250);
        }
        tempset--; //resta una unidad en el temporizador
      }
      digitalWrite(LED, LOW);// apaga el led testigo
      Serial.println("*************************");
      Serial.println("Grafica de fuerza");
      Serial.println("*************************");
      for (int u = 0; u < p; u++) {
        for (int y = 0; y < m[u]; y++) {
          Serial.print("-"); // imprime un guion segun el valor de la medicion
        }
        Serial.print("| ");
        Serial.println(m[u]);
      }
      delay(1000);
      // ordeno el vector de mayor a menor
      for (int i = 0; i < p; i++)
      {
        for (int j = i + 1; j < p; j++)
        {
          if (m[i] < m[j])
          {
            aux = m[i];
            m[i] = m[j];
            m[j] = aux;
          }
        }
      }
      medicion[c] = m[0];
      Serial.println("*************************");
      Serial.print("Usuario ");
      usuario = c + 1;
      Serial.println(usuario);
      Serial.print("Fecha de Revision: ");
      Serial.print(fecha[c]);
      Serial.print("/");
      Serial.println(mes[c]);
      Serial.print("Fuerza pico: ");
      Serial.print(medicion[c]);
      Serial.println(" Kgf");
      Serial.println("Presione 'd' para volver al menu");
      c++;
      a = 9;
      break;

    //
    case 9:
      tecla = Serial.read();

      if (tecla == 100)
      {
        Serial.println("*************************");
        Serial.println("**   Menu Principal    **");
        Serial.println("*************************");
        Serial.println("-Iniciar medicion- s");
        Serial.println("-Registros- d");
        a = 2;
        x = 0;
      }
      delay(1000);
      break;

    //
    case 10:
      tecla = Serial.read();
      if (tecla == 97)
      {
        Serial.println("*************************");
        Serial.println("**   Menu Principal    **");
        Serial.println("*************************");
        Serial.println("-Iniciar medicion- s");
        Serial.println("-Registros- d");
        a = 2;
        x = 0;
      }
      if (tecla == 119)
      {
        r = r + 1;
        usuario = usuario + 1;
        Serial.println("*************************");
        Serial.println("Registros:");
        Serial.println("s/w subir/bajar | a volver");
        Serial.print("Usuario ");
        Serial.println(usuario);
        Serial.print("Fecha de Revision: ");
        Serial.print(fecha[r]);
        Serial.print("/");
        Serial.println(mes[r]);
        Serial.print("Fuerza pico: ");
        Serial.print(medicion[r]);
        Serial.println(" Kgf");
      }
      if (tecla == 115)
      {
        if (r > 0)
        {
          r = r - 1;
          usuario = usuario - 1;
          Serial.println("*************************");
          Serial.println("Registros:");
          Serial.println("s/w subir/bajar | a volver");
          Serial.print("Usuario ");
          Serial.println(usuario);
          Serial.print("Fecha de Revision: ");
          Serial.print(fecha[r]);
          Serial.print("/");
          Serial.println(mes[r]);
          Serial.print("Fuerza pico: ");
          Serial.print(medicion[r]);
          Serial.println(" Kgf");
        }
        else
        {
          usuario = 1;
          r = 0;
          Serial.println("*************************");
          Serial.println("Registros:");
          Serial.println("s/w subir/bajar | a volver");
          Serial.print("Usuario ");
          Serial.println(usuario);
          Serial.print("Fecha de Revision: ");
          Serial.print(fecha[r]);
          Serial.print("/");
          Serial.println(mes[r]);
          Serial.print("Fuerza pico: ");
          Serial.print(medicion[r]);
          Serial.println(" Kgf");
          a = 10;
          x = 0;
        }
      }
      break;
    case 11:
      tecla = Serial.read();
      if (tecla == 43)
      {
        if (mes[c] < 12)
        { // tecla + suma de 1
          mes[c] = mes[c] + 5;
          Serial.println(mes[c]);
        }
        else
        {
          mes[c] = 12;
          Serial.println(mes[c]);
        }
      }
      if (tecla == 45)
      {
        if (mes[c] > 0)
        {
          mes[c] = mes[c] - 1;
          Serial.println(mes[c]); // con el signo - resta de a 1
        }
        else
        {
          mes[c] = 0;
          Serial.println(mes[c]);
        }
      }
      if (tecla == 100)
      { // enter
        // Pide elegir el tiempo en el que se  va a ser la prueba
        temporizador = 0;
        Serial.println("*************************");
        Serial.println("Seleccione tiempo de Medicion");
        Serial.println("d continuar | a volver");
        Serial.println("+/-suma/resta 1 y w/s suma/resta 5");
        Serial.println(temporizador);
        a = 5;
        x = 0;
      }
      if (tecla == 119)
      {
        if (mes[c] < 7)
        { // tecla W suma de 5
          mes[c] = mes[c] + 5;
          Serial.println(mes[c]);
        }
        else
        {
          mes[c] = 12;
          Serial.println(mes[c]);
        }
      }
      if (tecla == 115)
      {
        if (mes[c] > 0)
        {
          mes[c] = mes[c] - 5; // tecla S resta de 5
          Serial.println(mes[c]);
        }
        else
        {
          mes[c] = 0;
          Serial.println(mes[c]);
        }
      }
      if (tecla == 97)
      {
        Serial.println("*************************");
        Serial.println("Ingrese la fecha de tratamiento:");
        Serial.println("d continuar | a volver");
        Serial.println("+/- suma/resta 1 y w/s suma/resta 5");
        Serial.println(fecha[c]);
        a = 4;
        x = 0;
      }
  }
}