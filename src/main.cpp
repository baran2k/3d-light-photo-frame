#include <Adafruit_NeoPixel.h>

#define ledPin 6 // пин ленты
#define numpixels 15 // кол-во светодиодов
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numpixels, ledPin, NEO_GRB + NEO_KHZ800);

#include <Bounce2.h>
#define BUTTON_PIN 2 // пин кнопки
Bounce button = Bounce();

#define colorCount 8 // количество цветов + 1 для радуги

int numColor = colorCount - 1; // цвет с которого начинается (не забываем про 0); (colorCount - 1) - это радуга.

uint32_t color[colorCount - 1] =
    {
        strip.Color(255, 255, 255),
        strip.Color(255, 0, 255),
        strip.Color(0, 255, 0),
        strip.Color(0, 0, 255),
        strip.Color(255, 255, 0),
        strip.Color(255,91,0),
        strip.Color(255, 0, 0)};

int bright = 127; // начальная яркость

byte *Wheel(byte WheelPos)
{
  static byte c[3];
  if (WheelPos < 85)
  {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  }
  else if (WheelPos < 170)
  {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  }
  else
  {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }
  return c;
}

void rainbowCycle(int SpeedDelay)
{
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  {
    button.update();
    if (button.fell())
    {
      numColor++;
      break;
    }
    for (i = 0; i < numpixels; i++)
    {
      button.update();
      if (button.fell())
      {
        numColor++;
        break;
      }
      c = Wheel(((i * 256 / numpixels) + j) & 255);
      strip.setPixelColor(i, strip.Color(*c, *(c + 1), *(c + 2)));
    }
    strip.show();
    delay(SpeedDelay);
  }
}

void setup()
{
  button.attach(BUTTON_PIN, INPUT);
  button.interval(5);

  strip.begin();
  strip.setBrightness(bright);
  strip.show();
}

void loop()
{
  button.update();

  if (button.read() and button.currentDuration() > 1200)
  {
    bright++;
    if (bright > 255)
      bright = 0;
  }

  if (button.fell())
    numColor++;

  if (numColor == colorCount)
    numColor = 0;

  if (numColor == colorCount - 1)
    rainbowCycle(20);

  for (int i = 0; i < numpixels; i++)
  {
    strip.setPixelColor(i, color[numColor]);
    strip.setBrightness(bright);
    strip.show();
  }
}