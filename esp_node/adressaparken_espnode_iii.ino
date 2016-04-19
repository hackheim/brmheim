#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#define PIN 2

#define ASCII_NUM_START 48
#define ASCII_NUM_END 57
#define ASCII_L_ALPHA_START 97
#define ASCII_L_ALPHA_END 122
#define ASCII_U_ALPHA_START 65
#define ASCII_U_ALPHA_END 90

#define ASCII_U_ALPHA_HEX_START 65
#define ASCII_U_ALPHA_HEX_END 70

char serial_incmd[1024];
byte serial_incmd_idx;
byte serial_inbyte;
byte debug_mode = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_GRB + NEO_KHZ800);

ESP8266WiFiMulti WiFiMulti;

byte adc_idx = 0;
unsigned long adc_avg = 0;

unsigned long MIC_CACHE = 120;
unsigned int MIC_PCACHE = 120;
unsigned int MIC_PIDX = 0;
unsigned int MIC_RIDX = 0;
unsigned long MIC_TIME = 5;
unsigned long MIC_TIME_TMP = 0;

unsigned int RAW_COLOR = strip.Color(255,0,0);
unsigned int PEAK_COLOR = strip.Color(255,255,255);

const uint16_t port = 8888;
const char * host = "192.168.177.239";

WiFiClient client;

void setup() {
  strip.begin();
  strip.show();

  Serial.begin(115200);
  delay(10);
  pinMode(A0,INPUT);
  WiFiMulti.addAP("ITsjefen OpenZone");

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

    clrStrip();
    strip.setPixelColor(5,strip.Color(0,0,64));
    strip.show();

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    clrStrip();
    strip.setPixelColor(5,strip.Color(128,128,0));
    strip.show();

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

    goonline();

  //setLevel(255,0,0,100);
}


void goonline(){
    clrStrip();
    strip.setPixelColor(5,strip.Color(128,128,0));
    strip.show();
    Serial.print("connecting to ");
    Serial.println(host);

    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        clrStrip();
        strip.setPixelColor(5,strip.Color(255,0,0));
        strip.show();
        return;
    }else{
      Serial.println("connected");
        clrStrip();
        strip.setPixelColor(5,strip.Color(0,128,0));
        strip.show();
    }

}

void loop() {
  if(client.connected()){
    serialCore();
    unsigned int tmp_val = analogRead(A0);
    MIC_CACHE = MIC_CACHE + tmp_val;
    if(MIC_RIDX == 300){
      MIC_RIDX = 0;
      client.print(MIC_CACHE/300);
      //Serial.println(MIC_CACHE/200);
      MIC_CACHE = 0;
    }
    MIC_RIDX++;
  }else{
    goonline();
  }
}


void ipLevel(byte in_r, byte in_g, byte in_b, unsigned int in_l){
  unsigned int tmp_color = strip.Color(in_r,in_g,in_b);
  if(in_l <= strip.numPixels()){
    for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, 0);
    }
    for(uint16_t i=0; i<in_l; i++) {
          strip.setPixelColor(i, tmp_color);
    }
    strip.show();
  }
}

void ipRange(byte in_r, byte in_g, byte in_b, unsigned int in_f, unsigned int in_l){
  unsigned int tmp_color = strip.Color(in_r,in_g,in_b);
  for(unsigned int i=in_f; i<in_l; i++) {
    strip.setPixelColor(i, tmp_color);
  }
}

void clrStrip(){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
}

void setLevel(unsigned int in_peak_color, unsigned int in_raw_color, unsigned int in_peak, unsigned int in_raw){
  if(in_raw <= strip.numPixels() and in_peak <= strip.numPixels()){
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        if(i < in_raw){
          strip.setPixelColor(i, in_raw_color);
        }else{
          strip.setPixelColor(i, 0);
        }
    }
    for(uint16_t i=0; i<in_peak; i++) {
        if(i < in_raw){
          strip.setPixelColor(i, in_raw_color);
        }else{
          strip.setPixelColor(i, in_peak_color);
        }
    }
    strip.show();
  }
}

void parsestr(String s, int& r, int& g, int& b, int& l){
  //placeholder variables, I think we can concatenate these into the line parsing the strings if neccesary
  String w = "";
  String x = "";
  String y = "";
  String z = "";

  int firstComma = s.indexOf(",");
  w = s.substring(0, firstComma);
  int secondComma = s.indexOf(",", firstComma + 1);
  x = s.substring(firstComma +1, secondComma);
  int thirdComma = s.indexOf(",", secondComma + 1);
  y = s.substring(secondComma +1, thirdComma);
  int fourthComma = s.indexOf(",", thirdComma + 1);
  z = s.substring(thirdComma +1, fourthComma);

  r = w.toInt();
  g = x.toInt();
  b = y.toInt();
  l = z.toInt();
}
