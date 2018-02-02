// Thermal binary printer

// Copyright (C) 2017 Embecosm Limited <www.embecosm.com>

// Contributor: Peter Bennett <peter.bennett@embecosm.com>
// Contributor: Dan Gorringe <dan.gorringe@embecosm.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "Adafruit_Thermal.h"

// Here's the new syntax when using SoftwareSerial (e.g. Arduino Uno) ----
// If using hardware serial instead, comment out or remove these lines:

#include "SoftwareSerial.h"

// Printer connection buttons

#define TX_PIN 6 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial (RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

// Constants for holes

static const int HOLE_START    = 44;	// Center of first hole
static const int HOLE_OFF      = 61;	// Offset to next hole
static const int HOLE_DIAM     = 50;
static const int SPROCKET_DIAM = 35;

// Which is the sprocket hole

static const int SPROCKET_HOLE = 3;

// Size of bitmap

static const int PIXEL_WIDTH  = 384;
static const int ROW_DEPTH    = HOLE_DIAM;
//static const int SPACER_DEPTH = ROW_DEPTH / 2;
static const int SPACER_DEPTH = ROW_DEPTH;
static const int BITMAP_BYTES = (PIXEL_WIDTH + 7) / 8;

// Size of hole images

static const int IMAGE_BYTES = (ROW_DEPTH * ROW_DEPTH + 7) / 8;

// Image maps for holes and sprocket holes. These are generated on the host
// computer, using the genmap.c program

static const uint8_t hole_map[IMAGE_BYTES] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x05, 0xb6, 0xc0, 0x00, 0x00, 0x00, 0x06, 0xdb,
  0x6c, 0x00, 0x00, 0x00, 0x0b, 0x6d, 0xb6, 0xc0,
  0x00, 0x00, 0x0d, 0xb6, 0xdb, 0x6c, 0x00, 0x00,
  0x06, 0xdb, 0x6d, 0xb6, 0xc0, 0x00, 0x03, 0x6d,
  0xb6, 0xdb, 0x6c, 0x00, 0x01, 0xb6, 0xdb, 0x6d,
  0xb6, 0x80, 0x00, 0xdb, 0x6d, 0xb6, 0xdb, 0x60,
  0x00, 0x6d, 0xb6, 0xdb, 0x6d, 0xb6, 0x00, 0x36,
  0xdb, 0x6d, 0xb6, 0xdb, 0x00, 0x1b, 0x6d, 0xb6,
  0xdb, 0x6d, 0xb0, 0x0d, 0xb6, 0xdb, 0x6d, 0xb6,
  0xda, 0x02, 0xdb, 0x6d, 0xb6, 0xdb, 0x6d, 0x81,
  0x6d, 0xb6, 0xdb, 0x6d, 0xb6, 0xd0, 0x36, 0xdb,
  0x6d, 0xb6, 0xdb, 0x6c, 0x1b, 0x6d, 0xb6, 0xdb,
  0x6d, 0xb6, 0x8d, 0xb6, 0xdb, 0x6d, 0xb6, 0xdb,
  0x62, 0xdb, 0x6d, 0xb6, 0xdb, 0x6d, 0xb1, 0x6d,
  0xb6, 0xdb, 0x6d, 0xb6, 0xdb, 0x36, 0xdb, 0x6d,
  0xb6, 0xdb, 0x6d, 0x9b, 0x6d, 0xb6, 0xdb, 0x6d,
  0xb6, 0xd5, 0xb6, 0xdb, 0x6d, 0xb6, 0xdb, 0x6c,
  0xdb, 0x6d, 0xb6, 0xdb, 0x6d, 0xb6, 0x6d, 0xb6,
  0xdb, 0x6d, 0xb6, 0xdb, 0x56, 0xdb, 0x6d, 0xb6,
  0xdb, 0x6d, 0xb3, 0x6d, 0xb6, 0xdb, 0x6d, 0xb6,
  0xd9, 0xb6, 0xdb, 0x6d, 0xb6, 0xdb, 0x6d, 0x5b,
  0x6d, 0xb6, 0xdb, 0x6d, 0xb6, 0xcd, 0xb6, 0xdb,
  0x6d, 0xb6, 0xdb, 0x66, 0xdb, 0x6d, 0xb6, 0xdb,
  0x6d, 0xb5, 0x6d, 0xb6, 0xdb, 0x6d, 0xb6, 0xdb,
  0x36, 0xdb, 0x6d, 0xb6, 0xdb, 0x6d, 0x8b, 0x6d,
  0xb6, 0xdb, 0x6d, 0xb6, 0xc1, 0xb6, 0xdb, 0x6d,
  0xb6, 0xdb, 0x68, 0x5b, 0x6d, 0xb6, 0xdb, 0x6d,
  0xb4, 0x0d, 0xb6, 0xdb, 0x6d, 0xb6, 0xdb, 0x02,
  0xdb, 0x6d, 0xb6, 0xdb, 0x6d, 0x80, 0x6d, 0xb6,
  0xdb, 0x6d, 0xb6, 0xc0, 0x16, 0xdb, 0x6d, 0xb6,
  0xdb, 0x60, 0x03, 0x6d, 0xb6, 0xdb, 0x6d, 0xb0,
  0x00, 0xb6, 0xdb, 0x6d, 0xb6, 0xda, 0x00, 0x1b,
  0x6d, 0xb6, 0xdb, 0x6d, 0x00, 0x01, 0xb6, 0xdb,
  0x6d, 0xb6, 0x80, 0x00, 0x5b, 0x6d, 0xb6, 0xdb,
  0x40, 0x00, 0x05, 0xb6, 0xdb, 0x6d, 0x80, 0x00,
  0x00, 0xdb, 0x6d, 0xb6, 0xc0, 0x00, 0x00, 0x0d,
  0xb6, 0xdb, 0x60, 0x00, 0x00, 0x00, 0xdb, 0x6d,
  0xa0, 0x00, 0x00, 0x00, 0x05, 0xb6, 0xc0, 0x00,
  0x00
};

static const uint8_t sprocket_map[IMAGE_BYTES] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x02, 0xdb, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x6d, 0xb0, 0x00, 0x00, 0x00, 0x01, 0xb6,
  0xdb, 0x00, 0x00, 0x00, 0x00, 0xdb, 0x6d, 0xb0,
  0x00, 0x00, 0x01, 0x6d, 0xb6, 0xdb, 0x00, 0x00,
  0x00, 0x36, 0xdb, 0x6d, 0x80, 0x00, 0x00, 0x1b,
  0x6d, 0xb6, 0xd8, 0x00, 0x00, 0x0d, 0xb6, 0xdb,
  0x6d, 0x00, 0x00, 0x06, 0xdb, 0x6d, 0xb6, 0xc0,
  0x00, 0x03, 0x6d, 0xb6, 0xdb, 0x68, 0x00, 0x00,
  0xb6, 0xdb, 0x6d, 0xb6, 0x00, 0x00, 0x5b, 0x6d,
  0xb6, 0xdb, 0x40, 0x00, 0x0d, 0xb6, 0xdb, 0x6d,
  0xb0, 0x00, 0x06, 0xdb, 0x6d, 0xb6, 0xd8, 0x00,
  0x01, 0x6d, 0xb6, 0xdb, 0x6d, 0x00, 0x00, 0x36,
  0xdb, 0x6d, 0xb6, 0xc0, 0x00, 0x1b, 0x6d, 0xb6,
  0xdb, 0x60, 0x00, 0x05, 0xb6, 0xdb, 0x6d, 0xb4,
  0x00, 0x00, 0xdb, 0x6d, 0xb6, 0xdb, 0x00, 0x00,
  0x6d, 0xb6, 0xdb, 0x6d, 0x80, 0x00, 0x16, 0xdb,
  0x6d, 0xb6, 0xd0, 0x00, 0x03, 0x6d, 0xb6, 0xdb,
  0x6c, 0x00, 0x00, 0xb6, 0xdb, 0x6d, 0xb6, 0x00,
  0x00, 0x1b, 0x6d, 0xb6, 0xdb, 0x00, 0x00, 0x05,
  0xb6, 0xdb, 0x6d, 0x80, 0x00, 0x00, 0xdb, 0x6d,
  0xb6, 0xd0, 0x00, 0x00, 0x2d, 0xb6, 0xdb, 0x68,
  0x00, 0x00, 0x06, 0xdb, 0x6d, 0xb4, 0x00, 0x00,
  0x01, 0x6d, 0xb6, 0xdb, 0x00, 0x00, 0x00, 0x16,
  0xdb, 0x6d, 0x00, 0x00, 0x00, 0x03, 0x6d, 0xb6,
  0x80, 0x00, 0x00, 0x00, 0x36, 0xdb, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x6d, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00
};


//! Standard setup code.

//! @Note Due to a bug in Wiring, GNU standard syntax will not work for this
//!       function. The type has to be on the same line as the function name.

void setup ()
{
  // Standard serial port, so we can communicate with a console.

  Serial.begin(9600);

  // NOTE: SOME PRINTERS NEED 9600 BAUD instead of 19200, check test page.

  mySerial.begin(19200);  // Initialize SoftwareSerial
  printer.begin();        // Init printer (same regardless of serial type)

  // A header to help the user

  printer.justify('C');
  printer.println("start"); // To indicate where the first order is
  printer.justify('L');

  // Blank paper to feed into the reader

  for (int x = 0; x < 24; x++)
    {
      printer.println(); //an empty line
    }

}

//! Print a 5 hole row.

//! Holes are circles centered at x-offset HOLE_START and then every HOLE_OFF
//! bits.

//! Data holes are HOLE_DIAM bits in diameter, sprocket holes are SPROCKET_DIAM
//! bits in diameter. Rows have a blank separation of HOLE_DIAM bits.

//! We have to print one bit row at a time because of the limited memory on
//! the Uno.

//! The encoding of bits within a byte, is that the leftmost bit on the page
//! is the most significant bit in the byte.

//! @param[in] val  Data to print

static void
print6Holes (uint8_t val)
{
  // Break out the data

  bool data[5];

  for (int i = 0; i < 5; i++)
    data[i] = ((val >> i) & 1) == 1;

  // A general vector for a row of bits.

  uint8_t row[BITMAP_BYTES];

  // Print some blank lines before each data row

  memset (row, 0, BITMAP_BYTES);

  for (int i = 0; i < SPACER_DEPTH; i++)
    printer.printBitmap (PIXEL_WIDTH, 1, row, false);

  // Set up the data and print it out.

  for (int y = 0; y < ROW_DEPTH; y++)
    {
      uint8_t row[BITMAP_BYTES];	// Single line
      memset (row, 0, BITMAP_BYTES);    // Clear all the bits

      for(int i = 0; i < 6; i++)
	{
	  // Allow for the sprocket hole in the 2nd position

	  int  data_no = (i > SPROCKET_HOLE) ? i - 1 : i;

	  int x_off = HOLE_START + HOLE_OFF * i - ROW_DEPTH / 2;

	  for (int x = 0; x < ROW_DEPTH; x++)
	    {
	      // Bit position for the complete row

	      int rowbit    = x_off + x;
	      int rowbyte   = rowbit / 8;
	      int rowbitoff = 7- rowbit % 8;

	      // Bit postion for the image map

	      int imgbit    = y * ROW_DEPTH + x;
	      int imgbyte   = imgbit / 8;
	      int imgbitoff = 7 - imgbit % 8;

	      if (SPROCKET_HOLE == i)
		{
		  // Sprocket hole, in line

		  uint8_t imgval = (sprocket_map[imgbyte] >> imgbitoff) & 1;
		  row[rowbyte] |= imgval << rowbitoff;
		}
	      else if (data[data_no])
		{
		  // Mark the hole

		  uint8_t imgval = (hole_map[imgbyte] >> imgbitoff) & 1;
		  row[rowbyte] |= imgval << rowbitoff;
		}
	      else
		{
		  // Don't mark the hole

		  uint8_t imgval = (hole_map[imgbyte] >> imgbitoff) & 1;
		  row[rowbyte] &= ~(imgval << rowbitoff);
		}
	    }
	}

      // Print the data line

      printer.printBitmap (PIXEL_WIDTH, 1, row, false);
    }

}       // print6Holes ();


//! Receive one 5-bit number over serial and print it.

//! We accept ASCII digits, and print when we receive a non-digit.

void loop() {

  int   intVal = 0;
  bool  doPrint = false;

  while (!doPrint)
    {
      if (Serial.available () > 0)
	{
	  char ch = Serial.read ();

	  // Print back for debugging

	  Serial.print("IncomingByte is: ");
	  Serial.println(ch);

	  // If an ascii number increment the integer value

	  if (('0' <= ch) && (ch <= '9'))
	    {
	      intVal = (intVal * 10) + (ch - '0');
	      Serial.print ("Incrementing integer value to 0x");
	      Serial.println(intVal, HEX);
	    }
	  else
	    {
	      // Non-digit triggers printing
	      doPrint = true;
	    }
	}
    }

  // Print out the value

  Serial.print("Printing: 0x");
  Serial.print(intVal, HEX);

  if (intVal > 31)
    Serial.println (" (bottom 5 bits only)");
  else
    Serial.println ();

  print6Holes(intVal);

}	// loop ()


// Local Variables:
// mode: C++
// c-file-style: "gnu"
// indent-tabs-mode: nil
// End:
