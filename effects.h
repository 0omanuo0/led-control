#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> 
#endif
#define PIN       13 
#define NUMPIXELS 240 //1m=60
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int DELAYVAL = 30;
int DELAYFADE = 5;
#define ANCHO 7

bool FLAGOFF = false;

int r[] = {255, 255, 255, 127, 0,   0,   0,   0,   0,   127, 255, 255};
int g[] = {0,   127, 255, 255, 255, 255, 255, 127, 0,   0,   0,   0};
int b[] = {0,   0,   0,   0,   0,   127, 255, 255, 255, 255, 255, 127};



void init_leds(){
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif
    pixels.begin(); 
}



void fade(){
  for(int i=0; i<255; i++) {
    if(FLAGOFF){Serial.println("e");goto enda;}
    for(int ii=0; ii<NUMPIXELS; ii++){
      pixels.setPixelColor(ii, pixels.Color(i, 255-i, 0));
    }
    delay(DELAYFADE);
    pixels.show();
  }

  // fade from red to blue
  for(int i=0; i<255; i++) {
    if(FLAGOFF){goto enda;}
    for(int ii=0; ii<NUMPIXELS; ii++){
      pixels.setPixelColor(ii, pixels.Color(255-i, 0, i));
    }
    pixels.show();
    delay(DELAYFADE);
  }

  // fade from blue to green
  for(int i=0; i<255; i++) {
    if(FLAGOFF){goto enda;}
    for(int ii=0; ii<NUMPIXELS; ii++){
      pixels.setPixelColor(ii, pixels.Color(0, i, 255-i));
    }
    pixels.show();
    delay(DELAYFADE);
  }
enda:
delay(100);
FLAGOFF = false;
}




void breath(int color[3]){
  float gamma = 0.14; // affects the width of peak (more or less darkness)
  float beta = 0.5;
  for (int ii=0;ii<255;ii++){
    if(FLAGOFF){goto endd;}
    float param = exp(-1.0*pow(((float(ii)/float(255))-beta)/gamma,2.0)/2.0);
    float r1 = float(color[0])*param;
    float g1 = float(color[1])*param;
    float b1 = float(color[2])*param;
    delay(DELAYFADE);
    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(int(r1), int(g1), int(b1)));
    }
    pixels.show();
  }
  endd:
  FLAGOFF = false;
}


void rainbowbreath(){
  for(int i=0; i<12;i++){
    int col[] = {r[i],g[i],b[i]};
    breath(col);
  }
}


void slide1color(int color1[3]){

  int s2[] = {color1[0]/4,  color1[1]/4,  color1[2]/4 };
  int s3[] = {color1[0]/8,  color1[1]/8,  color1[2]/8 };
  int s4[] = {color1[0]/16, color1[1]/16, color1[2]/16};
  int s5[] = {color1[0]/32, color1[1]/32, color1[2]/32};


  for(int i=0; i<NUMPIXELS+ANCHO; i++) { // For each pixel...
    if(FLAGOFF){goto endb;}
    pixels.setPixelColor(i, pixels.Color(color1[0],color1[1],color1[2]));
    pixels.setPixelColor(i+1, pixels.Color(s2[0], s2[1], s2[2]));
    pixels.setPixelColor(i+2, pixels.Color(s3[0], s3[1], s3[2]));
    pixels.setPixelColor(i+3, pixels.Color(s4[0], s4[1], s4[2]));    
    pixels.setPixelColor(i-1, pixels.Color(s2[0], s2[1], s2[2]));
    pixels.setPixelColor(i-2, pixels.Color(s3[0], s3[1], s3[2]));
    pixels.setPixelColor(i-3, pixels.Color(s4[0], s4[1], s4[2]));
    pixels.setPixelColor(i-4, pixels.Color(s5[0], s5[1], s5[2]));


    pixels.show(); 

    delay(DELAYVAL);
  }
  endb:
  FLAGOFF = false;
}


void rainbowslide(){
  for(int i=0; i<12;i++){
    int col[] = {r[i],g[i],b[i]};
    slide1color(col);
  }
}


void rebote1color(int color1[3]){

  int s2[] = {color1[0]/2,  color1[1]/2,  color1[2]/2 };
  int s3[] = {color1[0]/4,  color1[1]/4,  color1[2]/4 };
  int s4[] = {color1[0]/8,  color1[1]/8,  color1[2]/8 };
  int s5[] = {color1[0]/16, color1[1]/16, color1[2]/16};
  int s6[] = {color1[0]/32, color1[1]/32, color1[2]/32};

  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    if(FLAGOFF){goto endc;FLAGOFF = false;}
    pixels.setPixelColor(i, pixels.Color(color1[0],color1[1],color1[2]));
    pixels.setPixelColor(i-ANCHO+6, pixels.Color(s2[0], s2[1], s2[2]));
    pixels.setPixelColor(i-ANCHO+5, pixels.Color(s3[0], s3[1], s3[2]));
    pixels.setPixelColor(i-ANCHO+4, pixels.Color(s4[0], s4[1], s4[2]));
    pixels.setPixelColor(i-ANCHO+3, pixels.Color(s5[0], s5[1], s5[2]));
    pixels.setPixelColor(i-ANCHO+2, pixels.Color(s6[0], s6[1], s6[2]));
    pixels.setPixelColor(i-ANCHO,   pixels.Color(0,0,0));

    pixels.show(); 

    delay(DELAYVAL);
  }
  for(int i=NUMPIXELS; i>0; i--) { // For each pixel...
    if(FLAGOFF){goto endc;}
    pixels.setPixelColor(i, pixels.Color(0,255,0));
    pixels.setPixelColor(i, pixels.Color(color1[0],color1[1],color1[2]));
    pixels.setPixelColor(i+ANCHO-+6, pixels.Color(s2[0], s2[1], s2[2]));
    pixels.setPixelColor(i+ANCHO-5, pixels.Color(s3[0], s3[1], s3[2]));
    pixels.setPixelColor(i+ANCHO-4, pixels.Color(s4[0], s4[1], s4[2]));
    pixels.setPixelColor(i+ANCHO-3, pixels.Color(s5[0], s5[1], s5[2]));
    pixels.setPixelColor(i+ANCHO-2, pixels.Color(s6[0], s6[1], s6[2]));
    pixels.setPixelColor(i+ANCHO,   pixels.Color(0,0,0));

    pixels.show(); 

    delay(DELAYVAL);
  }
  endc:
  FLAGOFF = false;
}



void rebote2colores(int color1[3], int color2[3]){

  int s2[] = {color1[0]    + color2[0]/3, color1[1]    + color2[1]/3,  color1[2]    + color2[1]/3};
  int s3[] = {color1[0]/2  + color2[0]/4, color1[1]/2  + color2[1]/4,  color1[2]/2  + color2[1]/4};
  int s4[] = {color1[0]/6  + color2[0]/2, color1[1]/6  + color2[1]/2,  color1[2]/6  + color2[1]/2};
  int s5[] = {color1[0]/10 + color2[0]/6, color1[1]/10 + color2[1]/6,  color1[2]/10 + color2[1]/6};
  int s6[] = {color1[0]/16 + color2[0]/8, color1[1]/16 + color2[1]/8,  color1[2]/16 + color2[1]/8};
  int s7[] = {color2[0]/25, color2[1]/25, color2[2]/25};

  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    if(FLAGOFF){goto endf;FLAGOFF = false;}
    pixels.setPixelColor(i, pixels.Color(color1[0],color1[1],color1[2]));
    pixels.setPixelColor(i-ANCHO+6, pixels.Color(s2[0], s2[1], s2[2]));
    pixels.setPixelColor(i-ANCHO+5, pixels.Color(s3[0], s3[1], s3[2]));
    pixels.setPixelColor(i-ANCHO+4, pixels.Color(s4[0], s4[1], s4[2]));
    pixels.setPixelColor(i-ANCHO+3, pixels.Color(s5[0], s5[1], s5[2]));
    pixels.setPixelColor(i-ANCHO+2, pixels.Color(s6[0], s6[1], s6[2]));
    pixels.setPixelColor(i-ANCHO+1, pixels.Color(s7[0], s7[1], s7[2]));
    pixels.setPixelColor(i-ANCHO,   pixels.Color(0,0,0));

    pixels.show(); 

    delay(DELAYVAL);
  }
  for(int i=NUMPIXELS; i>0; i--) { // For each pixel...
    if(FLAGOFF){goto endf;}
    pixels.setPixelColor(i, pixels.Color(0,255,0));
    pixels.setPixelColor(i, pixels.Color(color1[0],color1[1],color1[2]));
    pixels.setPixelColor(i+ANCHO-+6, pixels.Color(s2[0], s2[1], s2[2]));
    pixels.setPixelColor(i+ANCHO-5, pixels.Color(s3[0], s3[1], s3[2]));
    pixels.setPixelColor(i+ANCHO-4, pixels.Color(s4[0], s4[1], s4[2]));
    pixels.setPixelColor(i+ANCHO-3, pixels.Color(s5[0], s5[1], s5[2]));
    pixels.setPixelColor(i+ANCHO-2, pixels.Color(s6[0], s6[1], s6[2]));
    pixels.setPixelColor(i+ANCHO,   pixels.Color(0,0,0));

    pixels.show(); 

    delay(DELAYVAL);
  }
  endf:
  FLAGOFF = false;
}
