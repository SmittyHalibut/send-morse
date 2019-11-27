/*
 * A quick code snippit to send morse code on an Arduino or similar microcontroller.
 *
 * THIS IS NOT COMPLETE CODE.  This is not a Library in the Arduino sense.
 * You need to modify this code to fit into your application.  The good news is,
 * it's pretty simple to do.
 * 
 * As written, it blinks an LED connected to LED_PIN.  You can change the 
 * digitalWrite() lines to whatever you want to do to generate morse code.
 */

#define LED_PIN 13

// Morse Code constants
unsigned char const morse[28] PROGMEM = {
   0x05,   // A  .-     00000101
   0x18,   // B  -...   00011000
   0x1A,   // C  -.-.   00011010
   0x0C,   // D  -..    00001100
   0x02,   // E  .      00000010
   0x12,   // F  ..-.   00010010
   0x0E,   // G  --.    00001110
   0x10,   // H  ....   00010000
   0x04,   // I  ..     00000100
   0x17,   // J  .---   00010111
   0x0D,   // K  -.-    00001101
   0x14,   // L  .-..   00010100
   0x07,   // M  --     00000111
   0x06,   // N  -.     00000110
   0x0F,   // O  ---    00001111
   0x16,   // P  .--.   00010110
   0x1D,   // Q  --.-   00011101
   0x0A,   // R  .-.    00001010
   0x08,   // S  ...    00001000
   0x03,   // T  -      00000011
   0x09,   // U  ..-    00001001
   0x11,   // V  ...-   00010001
   0x0B,   // W  .--    00001011
   0x19,   // X  -..-   00011001
   0x1B,   // Y  -.--   00011011
   0x1C,   // Z  --..   00011100
   0x01,   // space     00000001
   0x5A,   // @  .--.-. 01011010
};

// The length of one morse code symbol (one "dit", and 
// the spaces between dits and dashes.)
void MorsePause(int ndx) {
  delay(ndx*250);
}

// Blink a character in morse code.
void sendMorse(uint8_t chr) {
  uint8_t bit;

  if (chr == ' ')
    chr = pgm_read_byte(morse + 26);
  else if (chr == '@')
    chr = pgm_read_byte(morse + 27);
  else if (chr >= 'A' && chr <= 'Z')
    chr = pgm_read_byte(morse + (chr-'A'));
  else {
    // Invalid character. Indicate by long blink. Mostly for debugging.
    digitalWrite(LED_PIN, HIGH);
    MorsePause(15);
    digitalWrite(LED_PIN, LOW);
    return;
  }

  // Get rid of padding, looking for the start bit.
  for (bit = 0; bit < 8 && (chr & 0x80) == 0; bit++, chr<<=1)
    ;

  // Pass the start bit, then send the rest of the char in morse
  for (bit++, chr<<=1; bit < 8; bit++, chr<<=1) {
    digitalWrite(LED_PIN, HIGH);
    if (chr & 0x80) {
      MorsePause(3);
    } else {
      MorsePause(1);
    }
    digitalWrite(LED_PIN, LOW);
    MorsePause(1);
  }
  MorsePause(4);  // Inter character pause
}

// Used when sending F("Strings") stored in FLASH
void sendMorseStr(const __FlashStringHelper *str) {
  PGM_P p = reinterpret_cast<PGM_P>(str);
  while (1) {
    unsigned char c = pgm_read_byte(p++);
    if(c==0) break;
    sendMorse(c);
  }
}

// Used when sending standard char* strings.
void sendMorseStr(const char *msg) {
  for (; *msg != '\0'; msg++) {
    sendMorse(*msg);
  }
}


