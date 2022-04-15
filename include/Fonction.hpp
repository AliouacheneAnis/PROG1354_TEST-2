#include <Arduino.h>
#include <Adafruit_SSD1306.h> //  Pour fonctionner l'ecran OLED 
#include <Adafruit_BME280.h> // Pour fonctionner le BME280
#include "RTClib.h" // Pour fonctionner le RTC 

void displayValues();     // Declaration fonction pour affichage les valeur sur l'ecran OLED
void displayTime();       // Declaration Fonction pour affichage du temps sur l'ecran OLED
void displayTmpHum();     // Declaration Fonction pour affichage les donnees temperature, humidite et pression 
void getTime();           // Declaration Fonction pour affichage les donnees temperature, humidite et pression 
void getData ();          // Declaration Fonction pour affichage les donnees temperature, humidite et pression 

// Declaration Fonctions pour transformer les donnees en string 
String ReadTemperature(); 
String ReadHumidity();
String ReadPressure();
String TimeRead();


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels



// Parametrer l'ecran OLED 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS3231 rtc;      // Declration object rtc
Adafruit_BME280 bme; // Declaration objet bme 

// Declaration Variables 
unsigned long  TempsActuel, TempsAvant,TempsActuelTmp, TempsAvantTmp;
const unsigned long DELAY_TIME = 6000, DELAY_CHANGE = 12000, DELAY_TEMP =1000;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 
float Temperature, Humidity, Pressure; 
String TimeServer, Seconde, Minute, Heure, Jour, Mois, Annee, Jrn;  

#define SEALEVELPRESSURE_HPA (1013.25)

void getTime(){
 
   DateTime now = rtc.now(); // Declaration object Now qui recois les donnees actuelle de la part de l'object rtc 
   
   // recupuration du temps actuel et Changement type de donnees en string
   Heure = String(now.hour(), DEC); 
   Minute = String(now.minute(),DEC); 
   Seconde = String(now.second(), DEC); 
   Annee  = String(now.year(), DEC); 
   Mois = String(now.month(), DEC);
   Jour = String(now.day(),DEC); 
   Jrn = String (daysOfTheWeek[now.dayOfTheWeek()]) ;

   // creation chaine de caracter pour l'envoi sur page web  
   TimeServer = Heure + ':' + Minute + ':' + Seconde + ' ' + Annee + '/' + Mois + '/' + Jour + " (" + Jrn + ") ";
}

// Fonction pour demander de capter les donnees 
void getData (){
  Temperature = bme.readTemperature(); 
  Humidity = bme.readHumidity();
  Pressure = bme.readPressure(); 
}



// Fonction Affichage de la temperature et humidity 
void displayTmpHum() {
  
  TempsActuel = millis();
  

  if (TempsActuel - TempsAvant  >= DELAY_CHANGE) {

       TempsAvant = TempsActuel;

       // Boucle while pour afficher la temperature et humidity  pendant 6 secondes 
       while (TempsActuel - TempsAvant  <= DELAY_TIME )
       {
         getTime();
         getData ();
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
      display.print(String(Temperature));  // Recupurer la donnee de temperature et l'afficher 
      display.print(" ");
      display.cp437(true);
      display.write(167);
      display.print("C");
      
      // display humidity
      display.setTextSize(1);
      display.setCursor(0,22);
      display.print("Humidity: ");
      display.print(String(Humidity)); // Recupurer la donnee de l'humidity et l'afficher 
      display.print(" %"); 
      display.display();

      
      // display Pressure 
      display.setTextSize(1);
      display.setCursor(0,40);
      display.print("Pressure :");
      display.print(String(Pressure)); // Recupurer la donnee de l'humidity et l'afficher 
      display.print(" hPa"); 
      display.display();
      
      TempsAvantTmp = TempsActuelTmp;
  
    } 
  
}

// Implemetation de la fonction d'affichage du temps 
void displayTime() {


    display.clearDisplay(); 

    // display time 
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Time : ");
    display.setTextSize(2);
    display.setCursor(0,10);
    display.print(Heure);   // recuperer et afficher l'heure actuelle 
    display.print(':');
    display.print(Minute);   // recuperer et afficher les minutes 
    display.print(':');
    display.print(Seconde);  // recuperer et afficher les secondes

    // display Date
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Date : ");
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print(Annee);  // recuperer et afficher l'annee actuelle
    display.print('/');
    display.print(Mois); // recuperer et afficher le mois actuel 
    display.print('/');
    display.print(Jour);  // recuperer et afficher le jour actuel 
    display.print(" (");
    display.print(Jrn);  // afficher le nom du jour 
    display.print(") ");

    display.display();  // appeler la fonction pour afficher 
    
}


// Fonctions qui change le type en string pour l'envoi avec ajax 
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
    return TimeServer;
  }








