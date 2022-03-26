#include <Arduino.h>
#include <Adafruit_SSD1306.h> //  Pour fonctionner l'ecran OLED 
#include <Adafruit_BME280.h> // Pour fonctionner le BME280
#include "RTClib.h" // Pour fonctionner le RTC 

// Declaration Variables 
unsigned long  TempsActuel, TempsAvant,TempsActuelTmp, TempsAvantTmp;
const unsigned long DELAY_TIME = 6000, DELAY_CHANGE = 12000, DELAY_TEMP =2000;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
float Temperature, Humidity, Pressure; 
String TimeServer; 
bool heure; 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Parametrer l'ecran OLED 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS3231 rtc;      // Declration object rtc
Adafruit_BME280 bme; // Declaration objet bme 

#define SEALEVELPRESSURE_HPA (1013.25)


void displayValues();  // Declaration fonction pour affichage les valeur sur l'ecran OLED
void displayTime();    // Declaration Fonction pour affichage du temps sur l'ecran OLED
void displayTmpHum();


// Fonction Affichage de la temperature et humidity 
void displayTmpHum() {
  
  TempsActuel = millis();
  

  if (TempsActuel - TempsAvant  >= DELAY_CHANGE) {

       TempsAvant = TempsActuel;
       heure= false; 

       // Boucle while pour afficher la temperature et humidity  pendant 6 secondes 
       while (TempsActuel - TempsAvant  <= DELAY_TIME )
       {
         displayValues();   // Appeler la fonction pour afficher la temperature sur l'ecran OLED 
         TempsActuel = millis();
       }
       
    } 
}


// Implementation de la fonction d'affichage de valeur 
void displayValues() {
  
  TempsActuelTmp = millis();

    // Affichage de la temperature et humidity chaque 2 secondes  
  if (TempsActuelTmp -  TempsAvantTmp  >= DELAY_TEMP) {
      
      display.clearDisplay();  // effacer l'ecran 

      display.clearDisplay(); 

    
      // display temperature
      display.setTextSize(1);
      display.setCursor(0,5);
      display.print("Temperature: ");
      Temperature = bme.readTemperature(); 
      display.print(String(Temperature));  // Recupurer la donnee de temperature et l'afficher 
      display.print(" ");
      display.cp437(true);
      display.write(167);
      display.print("C");
      
      // display humidity
      display.setTextSize(1);
      display.setCursor(0,22);
      display.print("Humidity: ");
      Humidity = bme.readHumidity();
      display.print(String(Humidity)); // Recupurer la donnee de l'humidity et l'afficher 
      display.print(" %"); 
      display.display();

      
      // display Pressure 
      display.setTextSize(1);
      display.setCursor(0,40);
      display.print("Pressure :");
      Pressure = bme.readPressure(); 
      display.print(String(Pressure)); // Recupurer la donnee de l'humidity et l'afficher 
      display.print(" hPa"); 
      display.display();
      
      TempsAvantTmp = TempsActuelTmp;
  
    } 
  
}

// Implemetation de la fonction d'affichage du temps 
void displayTime() {

   heure = true; 

   DateTime now = rtc.now(); // Declaration object Now qui recois les donnees actuelle de la part de l'object rtc 

    display.clearDisplay(); 

    // display time 
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Time : ");
    display.setTextSize(2);
    display.setCursor(0,10);
    display.print(now.hour(), DEC);   // recuperer et afficher l'heure actuelle 
    display.print(':');
    display.print(now.minute(), DEC);   // recuperer et afficher les minutes 
    display.print(':');
    display.print(now.second(), DEC);  // recuperer et afficher les secondes

    // display Date
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Date : ");
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print(now.year(), DEC);  // recuperer et afficher l'annee actuelle
    display.print('/');
    display.print(now.month(), DEC); // recuperer et afficher le mois actuel 
    display.print('/');
    display.print(now.day(), DEC);  // recuperer et afficher le jour actuel 
    display.print(" (");
    display.print(daysOfTheWeek[now.dayOfTheWeek()]);  // afficher le nom du jour 
    display.print(") ");

    TimeServer = String(now.hour(), DEC) + ':' + String(now.minute(),DEC) + ':' + String(now.second(), DEC) + ' ' + String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(),DEC) + " (" + String (daysOfTheWeek[now.dayOfTheWeek()])+ ") ";
 
    display.display();  // appeler la fonction pour afficher 
    
}


String ReadTemperature() {
    return String(Temperature);
  }

String ReadHumidity() {
    return String(Humidity);
  }

String ReadPressure() {
    return String(Pressure);
  }

String TimeRead(){
  
  if (heure)
  {
    return TimeServer;
    
  }else
  { 
    DateTime now2 = rtc.now(); // Declaration object now2 qui recois les donnees actuelle de la part de l'object rtc 
    String TimeServer1 = String(now2.hour(), DEC) + ':' + String(now2.minute(),DEC) + ':' + String(now2.second(), DEC) + ' ' + String(now2.year(), DEC) + '/' + String(now2.month(), DEC) + '/' + String(now2.day(),DEC) + " (" + String (daysOfTheWeek[now2.dayOfTheWeek()])+ ") ";
    return TimeServer1;
  }
  
}

// Replaces placeholder with DHT values
String processor(const String& var){
  
  if(var == "TEMPERATURE"){
    return ReadTemperature();
  }
  else if(var == "HUMIDITY"){
    return ReadHumidity();
  }
  else if(var == "PRESSURE"){
    return ReadPressure();
  }
  else if(var == "TIME"){
    return TimeRead();
  }
  
  return String();
}


