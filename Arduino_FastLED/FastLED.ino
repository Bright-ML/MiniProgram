#include <FastLED.h>
#define LED_PIN  3//我的LED信号线IN接在3引脚
#define NUM_LEDS 177//我的灯带一共级联了12颗LED
//#define LED_TYPE WS2812  // LED灯带型号
//#define COLOR_ORDER GRB // RGB灯珠中红色、绿色、蓝色LED的排列顺序
//uint8_t max_bright = 128; // LED亮度控制变量，可使用数值为0 ~ 255， 数值
CRGB leds[NUM_LEDS];
void FastLEDsetup() {
  //  delay(500); //开机安全延迟
  //  Serial.begin(115200);
  //LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); //初始化光带
  //FastLED.setBrightness(max_bright) ;//设置光带亮度
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);//可以设置全局亮度，调低亮度不刺眼
}
void FastLEDloop(uint8_t command) {
  switch (command) {
    case 0:
      //Serial.println("closelight");
      break;
    case 1:
      BZC();
      break;
    case 2:
      //nightlight();
      break;
    case 3:
      nightlight();
      break;
    case 4:
      colorslight();
      break;
    case 5:
      breathinglight();
      break;
    case 6:
      randomlight();
      break;
  }
}

void openlight() {

  //光圈旋转
  for (uint8_t i = 32; i >= 1; i--) {
    leds[i - 1] = CHSV(230, 236, 180); //设置光带中第一个灯珠颜色为红色
    leds[i + 31] = CHSV(230, 236, 180); //设置光带中第一个灯珠颜色为红色
    if (i % 2) {
      fill_solid(leds + 136, 41, CHSV(230, 236, 230)); //单色填充
      //fill_rainbow(leds+136, 41, 127,250);//渐变色填充
    }
    else {
      fill_solid(leds + 136, 41, CHSV(230, 236, 180));
      //fill_rainbow(leds+136, 41, 130,250);//渐变色填充
    }
    FastLED.show();//更新LED色彩
    delay(100);//等待500毫秒
  }
}
void closelight() {
  Serial.printf("关闭全部");
  fill_solid(leds, NUM_LEDS, CRGB::Black); //注意:第一个参数和第二个参数之和不能大于LED总数量+5
  FastLED.show();
}
void BZC() { //赞币藏 
  static uint8_t hue = 0;
  fill_rainbow(leds + 64, 113, hue++); //渐变色填充
  FastLED.show();
  delay(8);
}
void nightlight() {
  fill_solid(leds, NUM_LEDS, CRGB::White); //单色填充
  FastLED.show();
  delay(8);
}
void colorslight() {
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
}
void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);//使颜色最终逐渐褪色为黑色
  }
}
void breathinglight(){
  static uint8_t hue = 0;
  for (int i = 0; i < 255; i++) {
  fill_solid(leds, NUM_LEDS, CHSV(i, 255, hue++));
  FastLED.show();
  }
}
void randomlight(){
  int i = random(4);
  switch(i){
    case 0:
    colorslight();
    delay(5000);
    break;
    case 1:
    breathinglight();
    delay(5000);
    break;
    case 2:
    openlight();
    delay(5000);
    break;
    case 3:
    nightlight();
    delay(5000);
    break;
    
  }
  
}

//ColorPalette:颜色调色板
//ColorTemperature:渐变
//Fire2012
//Cylon:流光+渐变
/*
      leds[0] = CRGB::Black;//熄灭光带中第一个灯珠(设置颜色为黑色)
      FastLED.show();//更新LED色彩
      delay (500);//等待500毫秒


    CRGB颜色名称列表
    CRGB::AliceBlue
    CRGB::Amethyst
    CRGB::AntiqueWhite
    CRGB::Aqua
    CRGB::Aquamarine
    CRGB::Azure
    CRGB::Beige
    CRGB::Bisque
    CRGB::Black
    CRGB::BlanchedAlmond
    CRGB::Blue
    CRGB::BlueViolet
    CRGB::Brown
    CRGB::BurlyWood
    CRGB::CadetBlue
    CRGB::Chartreuse
    CRGB::Chocolate
    CRGB::Coral
    CRGB::CornflowerBlue
    CRGB::Cornsilk
    CRGB::Crimson
    CRGB::Cyan
    CRGB::DarkBlue
    CRGB::DarkCyan
    CRGB::DarkGoldenrod
    CRGB::DarkGray
    CRGB::DarkGreen
    CRGB::DarkKhaki
    CRGB::DarkMagenta
    CRGB::DarkOliveGreen
    CRGB::DarkOrange
    CRGB::DarkOrchid
    CRGB::DarkRed
    CRGB::DarkSalmon
    CRGB::DarkSeaGreen
    CRGB::DarkSlateBlue
    CRGB::DarkSlateGray
    CRGB::DarkTurquoise
    CRGB::DarkViolet
    CRGB::DeepPink
    CRGB::DeepSkyBlue
    CRGB::DimGray
    CRGB::DodgerBlue
    CRGB::FireBrick
    CRGB::FloralWhite
    CRGB::ForestGreen
    CRGB::Fuchsia
    CRGB::Gainsboro
    CRGB::GhostWhite
    CRGB::Gold
    CRGB::Goldenrod
    CRGB::Gray
    CRGB::Green
    CRGB::GreenYellow
    CRGB::Honeydew
    CRGB::HotPink
    CRGB::IndianRed
    CRGB::Indigo
    CRGB::Ivory
    CRGB::Khaki
    CRGB::Lavender
    CRGB::LavenderBlush
    CRGB::LawnGreen
    CRGB::LemonChiffon
    CRGB::LightBlue
    CRGB::LightCoral
    CRGB::LightCyan
    CRGB::LightGoldenrodYellow
    CRGB::LightGreen
    CRGB::LightGrey
    CRGB::LightPink
    CRGB::LightSalmon
    CRGB::LightSeaGreen
    CRGB::LightSkyBlue
    CRGB::LightSlateGray
    CRGB::LightSteelBlue
    CRGB::LightYellow
    CRGB::Lime
    CRGB::LimeGreen
    CRGB::Linen
    CRGB::Magenta
    CRGB::Maroon
    CRGB::MediumAquamarine
    CRGB::MediumBlue
    CRGB::MediumOrchid
    CRGB::MediumPurple
    CRGB::MediumSeaGreen
    CRGB::MediumSlateBlue
    CRGB::MediumSpringGreen
    CRGB::MediumTurquoise
    CRGB::MediumVioletRed
    CRGB::MidnightBlue
    CRGB::MintCream
    CRGB::MistyRose
    CRGB::Moccasin
    CRGB::NavajoWhite
    CRGB::Navy
    CRGB::OldLace
    CRGB::Olive
    CRGB::OliveDrab
    CRGB::Orange
    CRGB::OrangeRed
    CRGB::Orchid
    CRGB::PaleGoldenrod
    CRGB::PaleGreen
    CRGB::PaleTurquoise
    CRGB::PaleVioletRed
    CRGB::PapayaWhip
    CRGB::PeachPuff
    CRGB::Peru
    CRGB::Pink
    CRGB::Plaid
    CRGB::Plum
    CRGB::PowderBlue
    CRGB::Purple
    CRGB::Red
    CRGB::RosyBrown
    CRGB::RoyalBlue
    CRGB::SaddleBrown
    CRGB::Salmon
    CRGB::SandyBrown
    CRGB::SeaGreen
    CRGB::Seashell
    CRGB::Sienna
    CRGB::Silver
    CRGB::SkyBlue
    CRGB::SlateBlue
    CRGB::SlateGray
    CRGB::Snow
    CRGB::SpringGreen
    CRGB::SteelBlue
    CRGB::Tan
    CRGB::Teal
    CRGB::Thistle
    CRGB::Tomato
    CRGB::Turquoise
    CRGB::Violet
    CRGB::Wheat
    CRGB::White
    CRGB::WhiteSmoke
    CRGB::Yellow
    CRGB::YellowGreen
*/
