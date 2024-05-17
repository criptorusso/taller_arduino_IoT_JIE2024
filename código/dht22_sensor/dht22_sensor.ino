#include <DHT22.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <floatToString.h>
//define pin data

DHT22 dht22(7); // definir aqui el pin de datos utilizado
float max_temp = 30.5;

// datos de configuración el display oled
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int led_alarma = 8; // pin utilizado para el led indicador alarma
 
void setup() {
  Serial.begin(9600); //conexion serial para datos del sensor
  //Serial1.begin(9600); // NO Disponible para Arduino UNO. Conexion serial al ESP01
  pinMode(led_alarma, OUTPUT);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void loop() {
  //Serial.println(dht22.debug()); //optionnal

  float t = dht22.getTemperature();
  float h = dht22.getHumidity();

  if (dht22.getLastError() != dht22.OK) {
    //Serial.print("error :");
    //Serial.println(dht22.getLastError());
  }

  //Serial.print("h");Serial.print(h,1); //prefijo h para humedad y t para temperatura
  //Serial.print("t");Serial.println(t,1);//prefijo t para temperatura
   
  delay(2000); //retardo en milisegundos
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Temp: " + String(t));
  display.println("Hum : " + String(h));    
  display.display();   

  if (t>=max_temp){
    digitalWrite(led_alarma, HIGH);
    display.println(F("Temperatura Alta"));
    display.display();
    char Dato[15];
    floatToString(t, Dato, sizeof(Dato), 2); // libreria disponible en el repo de Arduino para convertir float a string
    if (Serial.availableForWrite()){
      Serial.write(Dato);
      Serial.flush();
    }
      delay(5000);
  }
  if (t<max_temp){
    digitalWrite(led_alarma, LOW);
  }
}
