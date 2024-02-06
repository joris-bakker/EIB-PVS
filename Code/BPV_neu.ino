/*      display biopotential distribution on 16x16 rgb led matrix 
        Projekt Medizinische Elektronik WS2023 HTWK Leipzig Prof. Laukner
        Joris Bakker, Julius Zeng, Luis Klaus 20EIB
        (c) Julius Zeng 04.12.2023
*/      
#include <avr/pgmspace.h>     //"write" and read from progmem
#include <FastLED.h>          // official FastLed 3.6.0 release doesnt work (missing uno r4 minima support)
                              // #1523 from facchinm (bit buggy but works) (04.12.2023)--> https://github.com/FastLED/FastLED/tree/0398b9a99901d00044de821ed86e8537995f561b 
#define LED_PIN       5       //5
#define BUTTON_PIN    2       //2
#define COLOR_ORDER   GRB     //GRB     breaks if changed
#define CHIPSET       WS2812B //WS2812B breaks if changed
#define BRIGHTNESS    60      //60 normal, 5 testing, 255 maximum (too bright)
#define MATRIX_WIDTH  16      //16      breaks if changed
#define MATRIX_HEIGHT 16      //16      breaks if changed
#define NUM_LEDS      (MATRIX_WIDTH * MATRIX_HEIGHT)

//##### global constants #####

const int RefreshTime   = 50; //50 ms per frame --> 20FPS (Stable)  [mode 0(potential) --> ~47ms/frame] [mode 1(vector) --> ~11ms/frame]

const double K          = 1;  //1 Lead-Field-Matrix constant
const int rQ            = 5;  //5 radius of Q
const int OffsetxQ      = 3;  //0 center
const int OffsetyQ      = 3;  //0 center
const int fI            = 40; //40 faktor of I

const int TracerLength  = 100;//30 100
const int ModeDefault   = 0;  //unused
const unsigned long TimeDefault = 300000; //Time to change mode (to default) 300000 -> 5min, 60000 -> 1min 

//EKG Data radius,betrag
const float Rad[350]    PROGMEM = {-0.1989096379, -0.1599311478, -0.1099585464, -0.04269711614, 0.05239506231, 0.1891771762, 0.3737521874, 0.5786085734, 0.7419007946, 0.823790367, 0.8326362674, 0.7972301336, 0.7427494824, 0.6838217857, 0.6256208843, 0.5662991542, 0.4975146476, 0.400800397, 0.2350551793, -0.08949824416, -0.6472523364, -1.16166403, -1.424373868, -1.533945144, -1.571001402, -1.572359622, -1.555712178, -1.530571817, -1.502457149, -1.474632801, -1.448938034, -1.426241222, -1.406715994, -1.390018499, -1.375409957, -1.36186314, -1.348193504, -1.333249254, -1.31616267, -1.296603027, -1.274911708, -1.252010989, -1.229092588, -1.207231594, -1.187105631, -1.168904202, -1.152392563, -1.137041582, -1.12215105, -1.106932052, -1.090540971, -1.072065211, -1.050450444, -1.024325029, -0.991580796, -0.9482442735, -0.8847661969, -0.7690693605, -0.3980156239, 1.420297627, 1.939144148, 2.075849095, 2.147822603, 2.196257825, 2.231925645, 2.258806306, 2.278919167, 2.293710578, 2.304463653, 2.312330092, 2.318199906, 2.322532592, 2.325243857, 2.325714338, 2.32294461, 2.315830554, 2.303493871, 2.285591459, 2.262543806, 2.23564844, 2.207060745, 2.179627493, 2.156560765, 2.14095904, 2.135229097, 2.140533653, 2.156456095, 2.18106914, 2.211452678, 2.244468766, 2.27745248, 2.30855882, 2.336732508, 2.361437733, 2.382313054, 2.39886251, 2.410242107, 2.415188304, 2.412154565, 2.399737767, 2.377425952, 2.346513145, 2.310734106, 2.27604887, 2.249420632, 2.237183031, 2.243833464, 2.27151877, 2.319874826, 2.385924275, 2.464245279, 2.547925235, 2.630397519, 2.707513276, 2.779303389, 2.852789225, 2.955825197, -2.98797756, -0.8443209114, -0.5026134727, -0.4215746239, -0.3836128885, -0.360168345, -0.3427737887, -0.3278278529, -0.3134319022, -0.2984113319, -0.2819147073, -0.2631973738, -0.2414569848, -0.2156499568, -0.1842116819, -0.1445344345, -0.09184361553, -0.01643397718, 0.1042313965, 0.3319173777, 0.8441835315, 1.690062784, 2.212866325, 2.441762454, 2.558415715, 2.62619056, 2.667639127, 2.691810389, 2.702305177, 2.699952681, 2.683728803, 2.651021675, 2.597848955, 2.520205645, 2.420405265, 2.33049337, 2.387302854, 2.917885578, -2.823464085, -2.68753061, -2.682031787, -2.709031468, -2.744258025, -2.781710815, -2.820054503, -2.858677607, -2.89619036, -2.929724936, -2.954846227, -2.966235149, -2.959073059, -2.930550496, -2.880711961, -2.812241135, -2.729470982, -2.637323973, -2.540779251, -2.444957989, -2.355374951, -2.277671791, -2.216466704, -2.173756674, -2.147949466, -2.134335386, -2.126763941, -2.119481182, -2.108235325, -2.09050481, -2.065226471, -2.032427521, -1.992948409, -1.94824387, -1.900148818, -1.85049991, -1.800611869, -1.750777206, -1.700065806, -1.646622418, -1.588415859, -1.524158811, -1.454041237, -1.380000806, -1.305419515, -1.234351547, -1.170586025, -1.116889983, -1.074613589, -1.043620253, -1.02240613, -1.008321357, -0.9979011664, -0.9873562635, -0.9732181898, -0.9530226529, -0.9258265262, -0.8923625625, -0.8547504556, -0.8158557928, -0.7785296506, -0.7449819979, -0.7164341813, -0.6930525156, -0.6740842905, -0.6581152138, -0.6433951443, -0.6281910058, -0.6111144534, -0.5913600875, -0.5688026788, -0.543942064, -0.5177318413, -0.4913570026, -0.4660242163, -0.4428037557, -0.4225315658, -0.40575877, -0.392729845, -0.383377051, -0.3773306263, -0.3739539017, -0.3724133848, -0.371783783, -0.3711717631, -0.3698305464, -0.3672389828, -0.3631329828, -0.357494832, -0.350517036, -0.3425583536, -0.3341034274, -0.3257290229, -0.3180732057, -0.3118006479, -0.307558127, -0.3059188546, -0.3073213206, -0.3120153405, -0.3200314703, -0.3311871514, -0.3451339611, -0.3614390603, -0.3796861754, -0.3995816608, -0.4210604122, -0.444404262, -0.4704172132, -0.5007766884, -0.5389233452, -0.5928957486, -0.6879214884, -0.9738820506, -2.8928313, 2.801674769, 2.659410461, 2.576720171, 2.51462337, 2.466097908, 2.43071966, 2.409007718, 2.400561688, 2.403696084, 2.41577521, 2.433736725, 2.454526817, 2.475369168, 2.493896325, 2.508200151, 2.516847922, 2.518891287, 2.513879366, 2.501877063, 2.483483895, 2.459846209, 2.432656472, 2.404137503, 2.377014809, 2.354480308, 2.340134469, 2.337847583, 2.351396855, 2.383646845, 2.43510169, 2.502182586, 2.576580539, 2.647206512, 2.704106877, 2.741406947, 2.757479468, 2.753267375, 2.730396764, 2.68978512, 2.630628044, 2.549344532, 2.437991718, 2.281841984, 2.057786487, 1.744729811, 1.367540961, 1.020438647, 0.7735952943, 0.6274630903, 0.5616312813, 0.5617528202, 0.6178565438, 0.7126854318, 0.8131987744, 0.8817675701, 0.8987988032, 0.8688024331, 0.8101238711, 0.7434537545, 0.6850882356, 0.6451576953, 0.6289304329, 0.6387379346, 0.6750572784, 0.7364349829, 0.8186657325, 0.9142213027, 1.013102571, 1.105263653, 1.183116056, 1.242485165, 1.281926609, 1.301404306, 1.30117335, 1.28116839, 1.240929752, 1.180103227, 1.099579832, 1.003093701, 0.898393799, 0.796572172};
const float Bet[350]    PROGMEM = {0.0145930007, 0.01304520295, 0.01149339729, 0.009946086597, 0.008467302719, 0.007194251975, 0.006331034913, 0.006074319987, 0.006477998153, 0.007438539539, 0.008824076518, 0.01052230899, 0.01238975461, 0.01420962087, 0.01569844289, 0.01654925193, 0.01649329942, 0.0153785063, 0.01330451108, 0.0109866431, 0.01054321415, 0.01427183374, 0.02141956267, 0.03058580149, 0.04104573172, 0.05236773744, 0.06418833464, 0.07613097247, 0.08778418817, 0.09871525544, 0.1085091388, 0.1168219179, 0.1234357427, 0.1283023913, 0.1315650253, 0.1335520525, 0.1347420783, 0.1357036434, 0.1370169314, 0.1391867217, 0.1425573371, 0.1472420149, 0.1530800623, 0.1596326555, 0.1662211215, 0.172002678, 0.1760717801, 0.1775719592, 0.1758025951, 0.1703064679, 0.1609268211, 0.1478269481, 0.1314706252, 0.1125674302, 0.09199234775, 0.07069375878, 0.04961010806, 0.02964403623, 0.01212418747, 0.009105966423, 0.02181052054, 0.03357806214, 0.04333485193, 0.05111667476, 0.05711806196, 0.06158283789, 0.06476668733, 0.06691812882, 0.06826740948, 0.06901957589, 0.06935000761, 0.06940148228, 0.06928232692, 0.06906597377, 0.06879314758, 0.06847815016, 0.06811961695, 0.06771408402, 0.0672690509, 0.06681217047, 0.06639487394, 0.06609112622, 0.06599358053, 0.0662089658, 0.06685199522, 0.06803397865, 0.06984164864, 0.07230604918, 0.07536962909, 0.07886596738, 0.08252462071, 0.08600391336, 0.08894400901, 0.09102712844, 0.09203175166, 0.09187069012, 0.09060721115, 0.0884480903, 0.08571659818, 0.08281045708, 0.08014868414, 0.07810888075, 0.07695907745, 0.0768003951, 0.07754958429, 0.078983116, 0.08083238365, 0.08288727506, 0.08505486919, 0.08733033151, 0.08966442192, 0.09175862324, 0.09286700629, 0.09169016204, 0.0863983856, 0.07477309863, 0.05449256087, 0.02444984939, 0.027842668, 0.08654827615, 0.1627034257, 0.2542390862, 0.3588416193, 0.4727197101, 0.5905622532, 0.7057628956, 0.81084737, 0.898066275, 0.9600986583, 0.9907941453, 0.9858719058, 0.9434973735, 0.8646758899, 0.75344428, 0.6169372584, 0.4656964748, 0.3158401109, 0.2007350626, 0.1887450886, 0.2667602069, 0.3546111277, 0.4219027725, 0.460240088, 0.4685673839, 0.4496731977, 0.4088010465, 0.3526509855, 0.2884655731, 0.2231841704, 0.162711088, 0.1113369702, 0.07132505917, 0.04272393868, 0.02404202792, 0.01641265682, 0.02337542785, 0.03685644992, 0.05120454349, 0.0640171118, 0.07386815463, 0.08007579741, 0.08264408969, 0.08211454215, 0.07934432221, 0.07526692259, 0.07069317044, 0.0661963282, 0.06209358911, 0.05849736123, 0.05538964431, 0.0526868382, 0.0502905657, 0.04813119125, 0.04620085555, 0.04456334473, 0.04333427683, 0.04264150815, 0.0425857399, 0.04321414196, 0.04450363575, 0.04634655142, 0.04854472282, 0.05082744491, 0.05289835923, 0.05449776126, 0.05545819042, 0.05573565535, 0.05540940353, 0.05465363697, 0.05369222859, 0.05275086505, 0.05201932612, 0.05163078991, 0.05165848149, 0.05212596257, 0.05302571828, 0.05433880976, 0.05604630168, 0.05812514066, 0.06052995287, 0.0631734698, 0.06592242898, 0.06861765615, 0.07111230301, 0.07331184213, 0.07519922503, 0.07683662941, 0.07834607922, 0.0798796376, 0.08159179785, 0.08362110301, 0.08607906372, 0.08903972218, 0.0925271237, 0.09650651642, 0.1008891709, 0.1055553432, 0.110389336, 0.1153133191, 0.1203071004, 0.1254077052, 0.1306908025, 0.1362416927, 0.1421249623, 0.1483597473, 0.1549043006, 0.1616514909, 0.168436279, 0.1750556003, 0.181299025, 0.1869852763, 0.1919968014, 0.1963037339, 0.1999703543, 0.20314102, 0.2060073659, 0.2087631521, 0.2115563455, 0.2144490253, 0.217394062, 0.2202336013, 0.2227193132, 0.2245496645, 0.22541632, 0.2250506034, 0.223261635, 0.2199598688, 0.2151628095, 0.2089831277, 0.2016026011, 0.1932376672, 0.1841034485, 0.1743828107, 0.1642055764, 0.1536409253, 0.1427037028, 0.131373098, 0.1196200592, 0.1074380962, 0.09487116667, 0.08203257665, 0.0691104202, 0.05635775292, 0.04406885766, 0.03254600321, 0.02206410413, 0.01284828985, 0.005165717664, 0.002866985348, 0.007664476758, 0.01199353673, 0.01566099262, 0.01890980612, 0.02199412471, 0.02511939933, 0.02842061909, 0.03196063266, 0.03573962296, 0.03970802963, 0.04377882289, 0.04783835895, 0.05175645016, 0.05539628208, 0.05862434525, 0.06132006008, 0.06338434317, 0.06474607412, 0.06536545524, 0.06523377464, 0.06437005046, 0.06281614105, 0.06063264756, 0.05789784981, 0.0547108531, 0.05119835431, 0.04752243706, 0.04388480377, 0.04052062989, 0.03767319866, 0.03554271754, 0.03421762568, 0.03362266038, 0.03352400174, 0.03359326283, 0.03348979997, 0.03292409625, 0.03169398085, 0.02970199277, 0.0269622532, 0.02360077971, 0.01985116498, 0.01604859152, 0.01262402595, 0.01007040674, 0.008747608687, 0.00849515961, 0.008685356044, 0.00878148546, 0.008574640725, 0.008110836257, 0.007580203358, 0.007213647089, 0.007175738673, 0.007501529705, 0.008138102868, 0.009028524264, 0.01013763382, 0.01142290432, 0.01280743009, 0.01418525203, 0.01545157355, 0.01653789495, 0.01743716793, 0.0182103684, 0.01896921854, 0.01983522705, 0.02088757829, 0.02212496362, 0.02346197888, 0.02475795246, 0.02585869096, 0.02663368675, 0.02700217034, 0.02694842498, 0.02652797526, 0.02586422291, 0.02513200552, 0.0245229827, 0.02419304849, 0.02420801942};

//##### global variables #####

int   Mode              = 0; // 0 = Potential, 1 = Vector
bool  ModeChanged       = true;
bool  NewRefresh        = true;
unsigned long ModeTime  = 0;
unsigned long Time      = 0;

int   Frame     = 0; //current frame
float RadFrame  = 0;
float BetFrame  = 0;

float xQ1       = 0;
float yQ1       = 0;
double I1       = 0;
float xQ2       = 0;
float yQ2       = 0;
double I2       = 0;

int TracerNow   = 0;
int LedLine;

//##### global arrays #####
CRGB leds[NUM_LEDS];
int TracerMemory[TracerLength]; //memoryarray of vector tracer

//##### conversion functions #####
//(coordinates)
double Xn(byte n){ //returns X value of Dipollocation
  if(n == 1){
    return (xQ1 + OffsetxQ);
  }
  if(n == 2){
    return (xQ2 + OffsetxQ);
  }
}

double Yn(byte n){ //returns Y value of Dipollocation
  if(n == 1){
    return (yQ1 + OffsetyQ);
  }
  if(n == 2){
    return (yQ2 + OffsetyQ);
  }
}

double Xm(byte m){ //returns X value of Output:leds[m]
  byte row = (m / MATRIX_WIDTH);
  if(row % 2){  //row is odd
    return ((((((1+row)*MATRIX_WIDTH)- m)-1)*2)-15);
  }
  else{         //row is even
    return (((m - (row*MATRIX_WIDTH))*2)-15);
  }
}

double Ym(byte m){ //returns Y value of Output:leds[m]
  if(0 <= m <= NUM_LEDS-1){
    return (((m / MATRIX_WIDTH)*2)-15);
  }
}

byte Mxy(double x, double y){ //returns leds[m] value of coordinates (reverse function of Xm/Ym)
  bool reverserow = false;
  byte row    = (y/2)+8;
  byte collum = (x/2)+8;
  if(row % 2){  //row is odd and needs to be reversed
    collum = MATRIX_WIDTH - collum -1;
  }
  return (row * MATRIX_WIDTH + collum);
}

int full2cut(int input){ //converts full coordinates to cut coordinates (-15,-14,-13,-12,... --> -8,-7,-7,-6,...)
  if (input % 2){ //input is odd
    if(input > 0){
      input++;
    }
    else{
      input--;
    }
  }
  return input / 2;
}

int cut2odd(int input){ //converts cut coordinates to odd coordinates (-8,-7,-6,... --> -15,-13,-11,...)
  if(input > 0){
    input = (input*2) - 1;
  }
  if(input < 0){
    input = (input*2) + 1;
  }
  return input; 
}
//(pixel values) Hue:0red 96green 160blue | Sat:  0white 255color | Val(brighness: 0off 255full)
byte Value2Hue(float Value, double MaxValue){ //converts potential value to pixel hue
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

byte Value2HSValue(float Value, double MaxValue){ //converts potential value to pixel HSValue
  float NormValue = abs(Value)/MaxValue;
    if (NormValue > 1){
      NormValue = 1;
    }
    return (NormValue*255);
}

//##### calculation functions #####

float L(byte m, byte n){ //koeffizient Lead-Field-Matrix (m-ter Messpunkt[0-255] n-ter Quellstrom[0,1])
  return (1/(K*4*PI*sqrt(sq(Xm(m)-Xn(n))+sq(Ym(m)-Yn(n)))));
}

float Potential(byte m){ //returns Potential value of Output:leds[m]
  return (L(m,1)* I1 + L(m,2)* I2);
}

//##### visualisation functions #####

void plotLine(int x0, int y0, int x1, int y1, byte hue){ //plots line between 2 coordinates + hue
  int  dx = full2cut(x1) - full2cut(x0); //int
  int  dy = full2cut(y1) - full2cut(y0); //int
  byte  NPixel;   //number of all Pixels per Line
  byte  NSteps;   //number of steps per Line
  bool    PlotX;
  bool    UpNPixel = false;
  bool    UpNSteps = false;

  if(abs(dx) >= abs(dy)){  //x>y
    //Serial.println("x>y Horizontal");
    NPixel = abs(dx)+1; //+1
    NSteps = abs(dy)+1; //+1
    PlotX  = true;
    if(dx > 0){UpNPixel = true;}
    if(dy > 0){UpNSteps = true;}
  }
  else{                   //y>x
    //Serial.println("y>x Vertikal");
    NPixel = abs(dy)+1; //+1
    NSteps = abs(dx)+1; //+1
    PlotX  = false;
    if(dx > 0){UpNSteps = true;}
    if(dy > 0){UpNPixel = true;}
  }
  int  XNow = full2cut(x0); //int
  int  YNow = full2cut(y0); //int
  byte    PixelNow  = 1;
  byte    StepNow   = 1;
  double  RatioStart  = NPixel/NSteps;
  double  RatioNow;
  byte    StepNrm;  //normal lengh of step
  byte    StepOdd;  //odd lengh of step
  byte    StepLengh;
  byte    NStepsNrm;//number of normal steps per Line
  byte    NStepsOdd;//number of odd steps per Line
  if(NPixel % NSteps){  //--> odd Steps required
    StepNrm = NPixel/NSteps +1;
    StepOdd = StepNrm -1;
  }
  else{                 //--> no odd steps
    StepNrm = NPixel/NSteps;
  }
  for(byte Step = 1; Step <= NSteps; Step++){
    if(PixelNow <= NPixel){  //redundant glaube
      RatioNow = ((NPixel+1) - PixelNow)/(NSteps+1 - Step);
      if((RatioNow/RatioStart)>=1){StepLengh = StepNrm;}
      else{StepLengh = StepOdd;}
      for(byte StepPixel = 1 ; StepPixel <= StepLengh; StepPixel++){
        LedLine = Mxy(cut2odd(XNow), cut2odd(YNow));
        leds[LedLine] = CHSV(hue, 255, 255);
        if(PlotX){
          if(UpNPixel){XNow++;}
          else{XNow--;}
        }
        else{
          if(UpNPixel){YNow++;}
          else{YNow--;}
        }
        PixelNow++;
      }
    }
    if(PlotX){
      if(UpNSteps){YNow++;}
      else{YNow--;}
    }
    else{
      if(UpNSteps){XNow++;}
      else{XNow--;}
    }
    StepNow++;
    RatioNow = ((NPixel-PixelNow) / (NSteps-StepNow));
  }
}

void vectorrad(float betrag, float radwinkel){ //plots vector of given value(betrag 0-1) and angle(rad) //rad = deg * (3.14159265359/180); 
  double r = 100*betrag;
  if(r >= 15){r=15;}
  double xvec = r*cos(radwinkel);
  double yvec = r*sin(radwinkel);
  int xout = xvec;
  int yout = yvec;

  int xstart = 1;
  int ystart = 1;
  if(xvec < 0){
    xstart = -1;
  }
  if(yvec < 0){
    ystart = -1;
  }
  plotLine(xstart,ystart,xout,yout,Value2Hue(betrag,0.7));
}

void tracer(int Led, int hue, int maxval){ //plots fading tracer of leds[m]  maxval is the maxbrightness of tracerled TracerMemory 
  TracerMemory[TracerNow] = Led;
  int TracerMemNow = TracerNow;
  if(maxval <=0 || maxval > 255){
    maxval = 255;
  }
  for(int n = TracerLength; n > 0; n--){
    TracerMemNow--;
    if(TracerMemNow < 0){
      TracerMemNow = TracerLength - 1;
    }
    double multiplyer = n; //double/float needed
    int val = ((maxval - 50) * (multiplyer/TracerLength)) + 50; //-+ offset 
    leds[TracerMemory[TracerMemNow]]  = CHSV( hue, 255, val);
  }
  TracerNow++;
  if(TracerNow >= TracerLength){
    TracerNow = 0;
  }
}

void TracerReset(){ //sets TracerMemory to 0,0
  for(int n = 0; n < TracerLength; n++){
    TracerMemory[n] = Mxy(0,0);
  }
  LedLine = Mxy(0,0);
}

void ShowVector(bool ShowTracer){ //displays vector (with tracer--> true)
  RadFrame = pgm_read_float(&(Rad[Frame]));
  BetFrame = pgm_read_float(&(Bet[Frame]));

  ClearMatrix();
  if(ShowTracer){
    tracer(LedLine,160,100);
  }
  vectorrad(BetFrame, RadFrame);
  FastLED.show();
}

void ShowPotential(){ //displays potential
  RadFrame = pgm_read_float(&(Rad[Frame]));
  BetFrame = pgm_read_float(&(Bet[Frame]));

  xQ1 = rQ * sin(RadFrame);
  xQ2 = xQ1 * -1;
  yQ1 = rQ * cos(RadFrame);
  yQ2 = yQ1 * -1;
  I1  = fI * BetFrame;
  I2  = I1 * -1;
  float PotFrame;
    
  for (int x = 0; x < NUM_LEDS; x++){
    PotFrame = Potential(x);
    leds[x] = CHSV(Value2Hue(PotFrame,0.15), 255, Value2HSValue(PotFrame,0.05));
  } 
  //vectorrad(BetFrame, RadFrame);
  FastLED.show();
}

void NextFrame(int add){ //1   advances to the next frame (input is added to current frame number) 
  Frame = Frame +add;
  if(Frame >= 350){
    Frame = 0;
  }
}

void ClearMatrix(){ //sets all matrix values to 0
  for (int x = 0; x < NUM_LEDS; x++)
  {
    leds[x]= 0x000000;;
  }
}

void ChangeMode(){ //changes current mode if button is pressed or timed
  if(digitalRead(2) == 0){
    ModeChanged = true;
    Mode++;
    if(Mode >= 2){
      Mode = 0;
    }
    delay(500);
  }
  /* automatic mode change after time
  if(millis() - ModeTime >= TimeDefault){
    ModeChanged = true;
    Mode++;
    if(Mode >= 2){
      Mode = 0;
    }
    delay(500);
  }
  */
}

void setup() {
  pinMode(2, INPUT_PULLUP);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  //Serial.begin(9600);
  //Serial.println("end setup");
}

void loop(){
  while(Mode == 0){ //potential
    if(ModeChanged){
      ModeTime = millis();
      ModeChanged = false;
    }
    if(NewRefresh){
      Time = millis();

      ShowPotential();
      NextFrame(1);

      //Serial.println(millis() - Time); // Time to process and show Frame
      NewRefresh = false;
    }
    if(millis() - Time >= RefreshTime){
      NewRefresh = true;
    }
    ChangeMode();
  }
  while(Mode == 1){ //vector
    if(ModeChanged){
      TracerReset();
      ModeTime = millis();
      ModeChanged = false;
    }
    if(NewRefresh){
      Time = millis();

      ShowVector(true);
      NextFrame(1);

      //Serial.println(millis() - Time); // Time to process and show Frame
      NewRefresh = false;
    }
    if(millis() - Time >= RefreshTime){
      NewRefresh = true;
    }
    ChangeMode();
  }
}