// CONFIRMED: WORKS ON ESP32 AND OLED SCREEN

// install ibraries
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// definition of OLED display SSD1306 for ESP32 SCA & SDL
#define OLED_CLOCK 20 // SCA pin on Display
#define OLED_DATA 17  // SDL pin on display
#define LED_BUILTIN 2 // GPIO2 of ESP32

// info in U8G2lib.h
// definition of OLED display SSD1306 for ESP32 SPI & I2C, SW
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C(const u8g2_cb_t *rotation, uint8_t clock, uint8_t data, uint8_t reset = U8X8_PIN_NONE)
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, A5, A4, U8X8_PIN_NONE);
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, OLED_CLOCK, OLED_DATA, U8X8_PIN_NONE); // This does not work
// or
// definition of OLED display SSD1306 for ESP32 SPI & I2C, HW
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, A5, A4, U8X8_PIN_NONE);
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C(const u8g2_cb_t *rotation, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE)
// built constructor for the OLED function
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_CLOCK, OLED_DATA, U8X8_PIN_NONE); // This works but according to the function, it shouldn't
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, OLED_CLOCK, OLED_DATA); // This does not work

// version 2 from Dave's Garage https://www.youtube.com/watch?v=W_gwDvxt7zc - lesson 2
int oled_LineH = 0;
// end version 2

// ==================================
// ONE TIME MANDATORY FUNCTION - DO NOT REMOVE
// ==================================
void setup(void)
{
    delay(1000);                  // power-up safety delay
    pinMode(LED_BUILTIN, OUTPUT); // relying on GPIO2 LED to light up on MB

    // version 1 from Aliexpress
    // u8g2.begin();
    // end version 1

    // version 2 from Dave's Garage https://www.youtube.com/watch?v=W_gwDvxt7zc - lesson 2
    // u8g2.begin();
    // u8g2.clear();
    // u8g2.setFont(u8g2_font_profont10_tf);
    // oled_LineH = u8g2.getFontAscent() + u8g2.getFontAscent(); // Descent is a negative number so we add it to the total
    // u8g2.setCursor(0, oled_LineH);
    // u8g2.print("Hello World");
    // u8g2.sendBuffer(); // send it out
    // end version 2

    // version 3 from Dave's Garage https://www.youtube.com/watch?v=qmurt7uGH1Y - lesson 3
    u8g2.begin();
    u8g2.clear();
    u8g2.setFont(u8g2_font_profont10_tf);
    oled_LineH = u8g2.getFontAscent() + u8g2.getFontAscent();
    // end version 3

} // end setup function

// version 3 from Dave's Garage https://www.youtube.com/watch?v=qmurt7uGH1Y - lesson 3
void DrawLinesAndGraphicsFrame(int FramesPerSecond)
{
    u8g2.clearBuffer();
    u8g2.home();

    // Draw some text on the left hand side
    u8g2.setCursor(3, oled_LineH * 2 + 2);
    // u8g2.setCursor(5, 32);
    u8g2.print("Hello");
    u8g2.setCursor(3, oled_LineH * 3 + 2);
    // u8g2.setCursor(5, 42);
    u8g2.print("World");
    u8g2.setCursor(3, oled_LineH * 4 + 2);
    u8g2.printf("%03d \n", FramesPerSecond); // Placeholder for framerate

    u8g2.drawFrame(0, 0, u8g2.getWidth(), u8g2.getHeight()); // Draw a border around the display

    // draw a moire pattern like it's 1984
    // for (int x = 0; x < u8g2.getWidth(); x++) // if I use this then all I see is two triangle atop one another
    for (int x = 0; x < u8g2.getWidth(); x += 4) // this will give me some separation of lines within the two triangles
    {
        u8g2.drawLine(x, 0, u8g2.getWidth() - x, u8g2.getHeight());
    }

    // Draw a reticle on the right hand side
    const int reticleY = u8g2.getHeight() / 2;           // Vertical center of display
    const int reticleR = u8g2.getHeight() / 4 - 2;       // Slightly less than 1/4 screen height
    const int reticleX = u8g2.getWidth() - reticleR - 8; // Right-justified with a small margin

    for (int r = reticleR; r > 0; r -= 3)
    { // draw a series of nested circles
        u8g2.drawCircle(reticleX, reticleY, r);
        u8g2.drawHLine(reticleX - reticleR - 5, reticleY, 2 * reticleR + 10); // H line through reticle center
        u8g2.drawVLine(reticleX, reticleY - reticleR - 5, 2 * reticleR + 10); // V line through reticle center
    }

    u8g2.sendBuffer(); // Send it out
} // end draw lines, graphics function

// FramesPerSecond function
// Tracks a weighted average in order to smooth out the values that is given. Computes the FPS as the simple reciprocal
// of the amount of time taken specified by the caller. so 1/3 of a second is 3 fps. It takes about 10 frames to stabilize.
double FramesPerSecond(double seconds)
{
    static double framesPerSecond;
    framesPerSecond = (framesPerSecond * 0.9) + (1.0 / seconds * 0.1);
    return framesPerSecond;
}
// end version 3

// ==================================
// REPETITIVE MANDATORY FUNCTION - DO NOT REMOVE
// ==================================
void loop(void)
{
    // version 3 from Dave's Garage https://www.youtube.com/watch?v=qmurt7uGH1Y - lesson 3

    // removing delay from LED loop
    static bool bLED = LOW;

    // start our fps tracking
    int fps = 0;

    // for (;;) // infinite loop (aka never ends)
    // {

    // }

    for (;;)
    {
        bLED = !bLED; // toggle LED State
        digitalWrite(LED_BUILTIN, bLED);

        double dStart = millis() / 1000.0; // record the start time
        DrawLinesAndGraphicsFrame(fps);
        double dEnd = millis() / 1000.0; // record the completion time
        fps = FramesPerSecond(dEnd - dStart);
        // DrawLinesAndGraphicsFrame(0);
    }
    // end version 3

    // version 3 - 1
    //  removing delay from LED loop
    //  if left within the loop like this, then the LED would be blinking furiously
    //  static bool bLED = LOW;
    //  bLED = !bLED; // toggle LED State
    //  digitalWrite(LED_BUILTIN, bLED);
    //  end version 3-1

    // version 0
    // digitalWrite(LED_BUILTIN, 0);
    // delay(500);
    // digitalWrite(LED_BUILTIN, 1);
    // delay(500);
    // end version 0

    // version 1 from Aliexpress
    // u8g2.clearBuffer();
    // u8g2.setFont(u8g2_font_ncenB08_tr);
    // // When the cursor is in the very top left corner, any text output goes offscreen.
    // // The cursor MUST be placed at the BOTTOM of the character to be output.
    // u8g2.drawStr(0, 10, "ideaspark 0.96'' OLED");
    // u8g2.drawStr(0, 25, "0.96");
    // u8g2.drawStr(0, 35, "0.96 inch");
    // u8g2.drawStr(0, 45, "0.96 inch OLED");
    // u8g2.drawStr(0, 55, "0.96 inch OLED 128x64");
    // u8g2.sendBuffer();
    // delay(100);
    // end version 1

} // end loop function

// =========================================================
// END OF PROGRAM
// =========================================================