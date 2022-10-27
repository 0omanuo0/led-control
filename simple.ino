#include "effects.h"

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//eyeyeyey

void setup() {
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.begin(9600);
  init_leds();
  xTaskCreatePinnedToCore(mainLoop, "Task1", 10000, NULL, 1, NULL,  0); 
  xTaskCreatePinnedToCore(effects, "Task2", 10000, NULL, 1, NULL,  1); 
}


String data = "";
String splitted_data[6];

int c1[] = {255,0,255};
int c2[] = {0,255,255};

int effect = 0;

void loop(){}

void mainLoop(void * unused) {
  while(true){
    if (SerialBT.available()){
      data = SerialBT.readString();
      Serial.println(data);
      strsplit(data,',');
      if(splitted_data[0] == "c1"){
        
        c1[0] = splitted_data[1].toInt();
        c1[1] = splitted_data[2].toInt();
        c1[2] = splitted_data[3].toInt();
      }
      else if(splitted_data[0] == "c2"){
        c1[0] = splitted_data[1].toInt();
        c1[1] = splitted_data[2].toInt();
        c1[2] = splitted_data[3].toInt();
        c2[0] = splitted_data[4].toInt();
        c2[1] = splitted_data[5].toInt();
        c2[2] = splitted_data[6].toInt();
      }
      else{
        Serial.println("asdasdasd");
        detect_effect();
      }
      FLAGOFF = true;
    }
    delay(100); 
  }
}




void strsplit(String dat, char separ){
  int pos = 0;
  for (int i = 0; i < 6;i++){splitted_data[i] = "";}
  for (int i = 0; i < data.length();i++){
    if (data[i] == ','){pos++;}
    else{splitted_data[pos] += data[i];}
  }
}


void effects(void * unused){
  while(true){
    switch(effect){
      case 0:
        fade();
        break;
      case 1:
        rainbowbreath();
        break;
      case 2:
        rainbowslide();
        break;
      case 3:
        breath(c1);
        break;
      case 4:
        slide1color(c1);
        break;
      case 5:
        rebote1color(c1);
        break;
      case 6:
        rebote2colores(c1,c2);
        break;
      default:
        break;
    }
  }
}


void detect_effect(){
  data = splitted_data[0];
  if(data == "Fade"){     effect = 0;}
  else if(data == "rainbowbreath"){effect = 1;}
  else if(data == "rainbowslide"){effect = 2;}
  else if(data == "breath"){effect = 3;}
  else if(data == "slide1color"){effect = 4;}
  else if(data == "rebote1color"){effect = 5;}
  else if(data == "rebote2color"){effect = 6;}
  else {effect = 0;}
}