#include <FastLED.h>

#define LED_PIN  5

#define COLOR_ORDER GRB
#define CHIPSET     WS2811

#define BRIGHTNESS 5 //5/60


// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;


const bool    kMatrixVertical = false;

//##### Globale Konstanten #####

//kosntate der Lead-Field-Matrix
const double K    = 1;   //1
//Dipol
const double xQ1  = 5;   //5
const double yQ1  = 4;    //0
const double xQ2  = -5;  //-5
const double yQ2  = -3;    //0

double        I1  = -29;
double        I2  = -10;



#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];
void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin(9600);
  Serial.println("end setup");
}



/*byte LedWerte[NUM_LEDS]{
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 
  0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00000000, 
  0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 
  0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 
  0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 
  0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 
  0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 
  0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff
};

static const long test_bitmap[] = {
  0xff0000, 0xff0008, 0xff0029, 0x42004a, 0x19004a, 0x00004a, 0x00184a, 0x00394a, 0x004931, 0x004510, 0x004900, 0x214500, 0x4a4500, 0x4a2400, 0x4a0400, 0x4a0000, 
  0x7b0000, 0x7b0019, 0x7b004a, 0x6b007b, 0x3a007b, 0x00007b, 0x002d7b, 0x00657b, 0x00795a, 0x007921, 0x087900, 0x427900, 0x7b7900, 0x7b4100, 0x7b0800, 0x7b0000, 
  0xa50000, 0x9c0021, 0x9c006b, 0x94009c, 0x4a009c, 0x0000a5, 0x003d9c, 0x00869c, 0x00a273, 0x00a229, 0x10a200, 0x5aa200, 0xa59e00, 0x9c5500, 0xa50c00, 0x9c0000, 
  0xb50000, 0xb50029, 0xb5007b, 0xa500b5, 0x5200b5, 0x0800b5, 0x0049b5, 0x009ab5, 0x00b684, 0x00b631, 0x19ba00, 0x6bb600, 0xb5b600, 0xb56500, 0xb51000, 0xb50000, 
  0xce0000, 0xce0029, 0xce0084, 0xbd00ce, 0x6300ce, 0x0800ce, 0x0051ce, 0x00aace, 0x00ce94, 0x00ce3a, 0x19ce00, 0x73ce00, 0xceca00, 0xce7100, 0xce1400, 0xce0000, 
  0xde0000, 0xde0031, 0xde0094, 0xce00de, 0x6b00de, 0x0800de, 0x0059de, 0x00bade, 0x00dfa5, 0x00df42, 0x21df00, 0x84df00, 0xdedb00, 0xde7900, 0xde1800, 0xde0000, 
  0xf70000, 0xf70031, 0xf7009c, 0xde00f7, 0x7300f7, 0x0800f7, 0x0061f7, 0x00caf7, 0x00f3b5, 0x00f34a, 0x21f300, 0x8cf300, 0xf7ef00, 0xf78600, 0xf71c00, 0xf70000, 
  0xff0000, 0xff003a, 0xff00a5, 0xe600ff, 0x7b00ff, 0x0800ff, 0x0065ff, 0x00d7ff, 0x00ffbd, 0x00ff4a, 0x21ff00, 0x94ff00, 0xfffb00, 0xff8e00, 0xff1c00, 0xff0000, 
  0xff0000, 0xff003a, 0xff00a5, 0xe600ff, 0x7b00ff, 0x0800ff, 0x0065ff, 0x00d7ff, 0x00ffbd, 0x00ff4a, 0x21ff00, 0x94ff00, 0xfffb00, 0xff8a00, 0xff1c00, 0xff0000, 
  0xff454a, 0xff455a, 0xff45ad, 0xe645ff, 0x8445ff, 0x4a45ff, 0x4279ff, 0x42dbff, 0x4affbd, 0x42ff63, 0x4aff42, 0x9cff42, 0xfffb42, 0xff9642, 0xff4d4a, 0xff4542, 
  0xff7573, 0xff717b, 0xff75bd, 0xef75ff, 0x9c75ff, 0x7375ff, 0x7392ff, 0x73dfff, 0x73ffc5, 0x73ff84, 0x7bff73, 0xadff73, 0xfffb73, 0xffa673, 0xff7573, 0xff7173, 
  0xff9294, 0xff9294, 0xff92c5, 0xef92ff, 0xad92ff, 0x9492ff, 0x94a6ff, 0x94e3ff, 0x94ffce, 0x94ff9c, 0x94ff94, 0xbdff94, 0xfffb94, 0xffb694, 0xff9694, 0xff9294, 
  0xffaaad, 0xffaaad, 0xffaace, 0xefaaff, 0xbdaaff, 0xadaaff, 0xadbaff, 0xade7ff, 0xadffd6, 0xadffb5, 0xadffad, 0xc5ffad, 0xffffad, 0xffc6ad, 0xffaaad, 0xffaaad, 
  0xffbebd, 0xffbec5, 0xffbede, 0xf7beff, 0xcebeff, 0xbdbeff, 0xbdcaff, 0xbdefff, 0xbdffde, 0xbdffc5, 0xbdffbd, 0xd6ffbd, 0xffffbd, 0xffd2bd, 0xffbebd, 0xffbebd, 
  0xffcece, 0xffced6, 0xffcee6, 0xf7ceff, 0xdeceff, 0xceceff, 0xced7ff, 0xcef3ff, 0xceffe6, 0xceffd6, 0xceffce, 0xdeffce, 0xffffce, 0xffdfce, 0xffd2ce, 0xffd2ce, 
  0xffdfde, 0xffdfde, 0xffdfef, 0xf7dfff, 0xe6dfff, 0xdedfff, 0xdee7ff, 0xdef7ff, 0xdeffef, 0xdeffe6, 0xdeffde, 0xe6ffde, 0xffffde, 0xffebde, 0xffdfde, 0xffdfde
};*/

// Demo that USES "XY" follows code below
/*
void loop()
{
    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / kMatrixHeight));
    DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    if( ms < 5000 ) {
      FastLED.setBrightness( scale8( BRIGHTNESS, (ms * 256) / 5000));
    } else {
      FastLED.setBrightness(BRIGHTNESS);
    }
    FastLED.show();
}
void loop()
{
  for (int x = 0; x < 255; x++)
  {
    SetFrame(x);
    FastLED.show();
  } 
  while(1);
}
*/
byte NewHue = 0;

uint16_t XY( uint8_t x, uint8_t y){
  uint16_t i;
    if (kMatrixVertical == false) {
      if( y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * kMatrixWidth) + x;
      }
    } else { // vertical positioning
      if ( x & 0x01) {
        i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
      } else {
        i = kMatrixHeight * (kMatrixWidth - x) - (y+1);
      }
    }
  return i;
}

void ClearMatrix(){
  for (int x = 0; x < NUM_LEDS; x++)
  {
    leds[x];
  }
  leds[0];
}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8){
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}

void Bitmap2frame(){
  
}

void SetFrame(byte Hue){
  for( byte y = 0; y < kMatrixHeight; y++) {     
    for( byte x = 0; x < kMatrixWidth; x++) {
      leds[ XY(x, y)]  = CHSV( Hue, 255, 255);
    }
  }
}

float Potential(byte m){ //returns Potential value of Output:leds[m]
  return (L(m,1)* I1 + L(m,2)* I2);
}
float L(byte m, byte n){ //koeffizient Lead-Field-Matrix (m-ter Messpunkt[0-255] n-ter Quellstrom[0,1])
  return (1/(K*4*PI*sqrt(sq(Xm(m)-Xn(n))+sq(Ym(m)-Yn(n)))));
}
double Xm(byte m){ //returns X value of Output:leds[m]
  byte row = (m / kMatrixWidth);
  if(row % 2){  //row is odd
    return ((((((1+row)*kMatrixWidth)- m)-1)*2)-15);
  }
  else{         //row is even
    return (((m - (row*kMatrixWidth))*2)-15);
  }
}
double Ym(byte m){ //returns Y value of Output:leds[m]
  if(0 <= m <= NUM_LEDS-1){
    return (((m / kMatrixWidth)*2)-15);
  }
}
byte Mxy(double x, double y){ //returns leds[m] value of coordinates
  bool reverserow = false;
  byte row    = (y/2)+8;
  byte collum = (x/2)+8;
  if(row % 2){  //row is odd and needs to be reversed
    collum = kMatrixWidth - collum -1;
  }
  return (row * kMatrixWidth + collum);
}

double Xn(byte n){ //returns X value of Dipollocation
  if(n == 1){
    return xQ1;
  }
  if(n == 2){
    return xQ2;
  }
}
double Yn(byte n){ //returns Y value of Dipollocation
  if(n == 1){
    return yQ1;
  }
  if(n == 2){
    return yQ2;
  }
}

byte Value2Hue(float Value, double MaxValue){
  float NormValue = abs(Value)/MaxValue;
  if (NormValue > 1){
    NormValue = 1;
  }
  if(Value >= 0){
    return (80 -(NormValue*80));
  }
  else{
    return (80 +(NormValue*80));
  }
}

byte Value2HSValue(float Value, double MaxValue){
  float NormValue = abs(Value)/MaxValue;
    if (NormValue > 1){
      NormValue = 1;
    }
    return (NormValue*255);
}
  //Bresenham's_line_algorithm
  
void plotLine(double x0, double y0, double x1, double y1){
  double dx = x1 - x0;
  double dy = y1 - y0;
  double  D = 2*dy - dx;
  double  y = y0;

  for(double x = x0 ; x <= x1; x++){
        //plot(x, y)
    leds[Mxy(x, y)] = 0xffffff;
    if(D > 0){
      y = y + 1;
      D = D - 2*dx;
    }
    D = D + 2*dy;
  }
}
    

void loop(){
  bool returning = true;
  bool first = false;
  for(double y = -15 ; y <= 15; y++){
    double x=y;
    Serial.print("Mxy(");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(") = ");
    Serial.println(Mxy(x,y));
  }
  while (1) {
    I2 = I1*-1;
    
    for (int x = 0; x < NUM_LEDS; x++){
      //leds[x] = CHSV( (Potential(x)*1000), 255, 255);
      leds[x] = CHSV(Value2Hue(Potential(x),0.2), 255, Value2HSValue(Potential(x),0.1));
      if(first){
        Serial.print(x);
        Serial.print("Potential");
        Serial.println(Potential(x));
      }
    }
    plotLine(xQ2,yQ2,xQ1,yQ1);
    FastLED.show();
    //delay(500);
    if (returning){
      I1 -= 1;
      if (I1 <= -30) {
        first = false;
        returning = false;
      }
    }
    else{
      I1 += 1;
      if (I1 >= 30) {
        returning = true;
      }
    }
  }
}
  /*
   * Serial.print(x);
    Serial.print("Potential");
    Serial.print(Potential(x));
    Serial.print("Xm:");
    Serial.print(Xm(x));
    Serial.print("Ym:");
    Serial.println(Ym(x));
  //leds[0] = 0xffffff;
  //leds[16] = 0xffffff;
  //leds[255] = 0xffffff;
  Serial.println(Ym(255));
  Serial.println(Ym(215));
  Serial.println(Ym(148));
  Serial.println(Ym(128));
  Serial.println(Ym(127));
  Serial.println(Ym(27));
  Serial.print("255:");
  Serial.println(Xm(255));
  Serial.print("236:");
  Serial.println(Xm(236));
  Serial.print("211:");
  Serial.println(Xm(211));
  Serial.print("169:");
  Serial.println(Xm(169));
  Serial.print("150:");
  Serial.println(Xm(150));
  Serial.print("107:");
  Serial.println(Xm(107));
  Serial.print("3%2:");
  Serial.println(3%2);
  Serial.print("1%2:");
  Serial.println(1%2);
  Serial.print("0%2:");
  Serial.println(0%2);


  
  */
