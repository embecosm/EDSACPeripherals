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

// Size of bitmap

static const int PIXEL_WIDTH  = 384;
static const int ROW_DEPTH    = HOLE_DIAM;
static const int BITMAP_BYTES = (PIXEL_WIDTH + 7) / 8;

// Size of hole images

static const int IMAGE_BYTES = (ROW_DEPTH * ROW_DEPTH + 7) / 8;

// Image maps for holes and sprocket holes. These are generated on the host
// computer, using the genmap.c program

static const uint8_t hole_map[IMAGE_BYTES] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0xff,
  0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xe0,
  0x00, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xff,
  0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0xff,
  0xff, 0x80, 0x01, 0xff, 0xff, 0xff, 0xff, 0xf0,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x3f,
  0xff, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xff,
  0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff,
  0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x81,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff,
  0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03,
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff,
  0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xf8,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x1f,
  0xff, 0xff, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff,
  0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff,
  0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xc0, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f,
  0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xe0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00,
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
  0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x03, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xff,
  0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0xf0,
  0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x3f,
  0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff,
  0xff, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xc0,
  0x00, 0x03, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff,
  0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 0xff, 0xff,
  0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f,
  0xff, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 0xff,
  0xff, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfc,
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x7f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff,
  0xff, 0xff, 0xf0, 0x00, 0x07, 0xff, 0xff, 0xff,
  0xfc, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x07,
  0xff, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xff, 0xff,
  0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf8,
  0x00, 0x00, 0x07, 0xff, 0xff, 0xfc, 0x00, 0x00,
  0x01, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff,
  0x80, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00,
  0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00
};

//! Standard setup code.

void setup ()
{
  // Standard serial port, so we can communicate with a console.
  Serial.begin(9600);

  // NOTE: SOME PRINTERS NEED 9600 BAUD instead of 19200, check test page.
  mySerial.begin(19200);  // Initialize SoftwareSerial
  printer.begin();        // Init printer (same regardless of serial type)

}

//! Print a 5 hole row.

//! Holes are circles centered at x-offset HOLE_START and then every HOLE_OFF
//! bits.

//! Data holes are HOLE_DIAM bits in diameter, sprocket holes are SPROCKET_DIAM
//! bits in diameter. Rows have a blank separation of HOLE_DIAM bits.

//! We have to print one bit row at a time because of the limited memory on
//! the Uno.

//! @param[in] val  Data to print

static void
print6Holes (uint8_t val)
{
  // Break out the data
  bool data[5];

  for (int i = 0; i < 5; i++)
    data[i] = (val >> i & 1) == 1;


  // Mark up the separator in the bitmap (4th column) and print it as the
  // blank row.

  int socket_off = HOLE_START + HOLE_OFF * 3 - ROW_DEPTH / 2;

  for (int y = 0; y < ROW_DEPTH; y++)
    {
      uint8_t row[BITMAP_BYTES];	// Single line
      memset (row, 0, BITMAP_BYTES);    // Clear all the bits

      for (int x = 0; x < ROW_DEPTH; x++)
	{
	  // Bit position for the complete row

	  int rowbit    = socket_off + x;
	  int rowbyte   = rowbit / 8;
	  int rowbitoff = 7 - rowbit % 8;

	  // Bit postion for the image map

	  int imgbit    = y * ROW_DEPTH + x;
	  int imgbyte   = imgbit / 8;
	  int imgbitoff = 7 - imgbit % 8;

	  uint8_t imgval = (sprocket_map[imgbyte] >> imgbitoff) & 1;
	  row[rowbyte] |= imgval << rowbitoff;
	}

      // Print the line
      printer.printBitmap (PIXEL_WIDTH, 1, row, false);
    }

  // Set up the data and print it out.

  for (int y = 0; y < ROW_DEPTH; y++)
    {
      uint8_t row[BITMAP_BYTES];	// Single line
      memset (row, 0, BITMAP_BYTES);    // Clear all the bits

      for(int i = 0; i < 6; i++)
	{
	  // Allow for the sprocket hole in the 4th position

	  int  data_no = (i > 3) ? i - 1 : i;

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

	      if (3 == i)
		{
		  // Sprocket hole

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
// End:
