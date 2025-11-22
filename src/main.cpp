//---------------------------------------------------------------------------------------//
//                                                                                       //
//  Temperature Logger For Aquarium                                                      //
//  -------------------------------                                                      //
//  Collects temperature from DS18B20 sensor every 15 minutes and appends it to SD card  //
//                                                                                       //
//  Mason Erman - 11/19/2025                                                             //
//                                                                                       //
//---------------------------------------------------------------------------------------//


#include <SD.h>                                                // Install Library SD
#include <OneWire.h>                                           // Install Library OneWire
#include <DallasTemperature.h>                                 // Install Library DallasTemperature


#define ONE_WIRE_BUS 2                                         // Pin 2 will be used for temperature data line
const int chipSelect = 10;                                     // Pin 10 will be used to select the sd card for data transfer


File myFile;                                                   // Setup a file to write to
OneWire oneWire(ONE_WIRE_BUS);                                 // Setup a oneWire instance to communicate with OneWire sensor 
DallasTemperature sensors(&oneWire);                           // Pass our oneWire reference to Dallas Temperature.


int timeElapsed = 0;                                           // Start the elapsed time


void setup(void)
{

  Serial.begin(9600);                                          // Sets up the serial monitor on a computer for debugging/oversight
  // while (!Serial);                                          // Uncomment this line to pause program until serial monitor opens
  Serial.println("Arduino Aquarium Temperature Logger");

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed.");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is the wiring fully seated?");
    Serial.println("3. is the sd card proper format (fat32)?");
    Serial.println("Press reset after fixing the issue");
    while (true);
  }

  Serial.println(" Success.");

  myFile = SD.open("temperatures.csv", FILE_WRITE);            // Opens the temperatures.csv file with write permissions under myFile

  
  if (myFile) {                                                // If the file opened, proceed
    
    myFile.println("Time Elapsed (m),Temperature (f)");        // Appends Temperature data in fahrenheit to file
    Serial.println("Title Appended.");                         // Confirms file opened and printed on serial monitor
    
    myFile.close();                                            // Close the file

  } else {
    Serial.println("Error opening temperatures.csv");          // If the file didn't open, print an error
  }

  sensors.begin();                                             // Start the sensor

}

void loop(void)
{ 
  
  sensors.requestTemperatures();                              // Sends the command to get temperatures
  Serial.print(sensors.getTempCByIndex(0)*1.8+32);            // Prints the temperature in Fahrenheit to the serial monitor

  myFile = SD.open("temperature.csv", FILE_WRITE);            // Opens the temperature.csv file with write permissions under myFile

  
  if (myFile) {                                               // If the file opened, proceed
    
    myFile.print(timeElapsed);                                // Appends elapsed time to file
    myFile.print(",");                                        // Appends a comma (comma seperated file)
    myFile.println(sensors.getTempCByIndex(0)*1.8+32);        // Appends temperature data in fahrenheit to file, and newline

    Serial.println(" Appended.");                             // Confirms file opened and printed on serial monitor
    
    myFile.close();                                           // Close the file

  } else {
    Serial.println(" not appended: error");                   // If the file didn't open, print an error
  }
  
  delay(900000);                                              // Delay 15 minutes between logs
  timeElapsed += 15;                                          // Add 15 minutes to the elapsed time

}
