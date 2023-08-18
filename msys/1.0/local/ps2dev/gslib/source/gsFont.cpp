/*****************************
*                            *
*      GSLIB by Hiryu        *
*                            *
* gsFont module:             *
* Screen Font print function *
* using gsLib's gsPipe       *
*                            *
*****************************/

#include <tamtypes.h>
#include "gsFont.h"
#include "gsDriver.h"
#include "string.h"


void gsFont::uploadFont(gsFontTex* pFontTex, 
							unsigned int TBbase, int TBwidth, 
							int TBxpos, int TBypos)
{
	// Safety ... dont try to access a null object
	if (m_pFontPipe == NULL)
		return;

	if (pFontTex == NULL)
		return;

	m_TBbase	= TBbase;
	m_TBwidth	= TBwidth;
	m_TBxpos	= TBxpos;
	m_TBypos	= TBypos;

	m_TexWidth =  pFontTex->TexWidth;
	m_TexHeight = pFontTex->TexHeight;
	m_CharGridWidth =  pFontTex->CharGridWidth;
	m_CharGridHeight = pFontTex->CharGridHeight;

	m_NumXChars = pFontTex->NumXChars;
	m_NumYChars = pFontTex->NumYChars;

	m_PSM = pFontTex->PSM;

	memcpy(m_CharWidth, &(pFontTex->CharWidth), 256);

	m_pFontPipe->TextureUpload(m_TBbase, m_TBwidth, m_TBxpos, m_TBypos,
		m_PSM, (const unsigned char*)&(pFontTex->PixelData), m_TexWidth, m_TexHeight);
}

void gsFont::Print(int x, int Xend, int y, int z, unsigned long colour, gsFontAlign alignment, const char* string)
{
	// Safety ... dont try to access a null object
	if (m_pFontPipe == NULL)
		return;

	if (string == NULL)
		return;

	int curr_x;

	char* temp_string = (char*)string;

	int max_length = Xend-x;

	
	m_Bold=0;
	m_Underline=0;

	m_pFontPipe->TextureSet(m_TBbase, m_TBwidth,
		gsDriver::getTexSizeFromInt(m_TexWidth),
		gsDriver::getTexSizeFromInt(m_TexHeight), 
		m_PSM, 0, 0, 0, 0);


	while (temp_string[0] != '\0')	// while there are more lines ...
	{
		int pix_length = 0;		// length of current line in pixels
		int char_length = 0;	// length of current line in characters

		// Calculate width of current line
		GetCurrLineLength(temp_string, max_length, pix_length, char_length);

		// set start Xpos - based on alignment
		switch (alignment)
		{
		case GSFONT_ALIGN_RIGHT:
			curr_x = Xend-pix_length;
			break;

		case GSFONT_ALIGN_CENTRE:
			curr_x = x+((max_length-pix_length)/2);
			break;

		case GSFONT_ALIGN_LEFT:
		default:
			curr_x = x;
			break;
		}

		PrintLine(curr_x, y, z, colour, char_length, temp_string);


		// finished printing the current line, so move onto the next line
		y+=m_CharGridHeight;

		// move the string pointer onto the next line
		temp_string += char_length;
	}
}


void gsFont::GetCurrLineLength(const char* string, int max_length, int& pix_length, int& char_length)
{
	pix_length = 0;
	char_length = 0;

	char* curr_char = (char*)string;	// modifyable string pointer

	// Calculate width of current line
	while (1)
	{

		// Add up width of characters - until newline, or end of string, or max-length exceeded
		switch (*curr_char)
		{
		case 0:
			// If end of string then return
			return;
			break;

		case '\n':
			// since this is a non-printable character, add it so we dont get it on the start of the next line
			char_length++;

			// then return since this is the end of the line
			return;
			break;

		case '\r':	// carriage-return
		case '\b': // Bold on/off - not implemented yet
		case '\a':
			// unprintable or command characters that have no width (regardless of what any width
			// specified in the char width table

			break;

		default:
			int curr_width = m_CharWidth[*curr_char];

			// if adding the current character will push us over the limit, then don't add it
			if ((pix_length+curr_width) > max_length)
				return;

			pix_length+=curr_width;	// add width of current character onto the width

			break;
		}

		char_length++;			// increase the number of characters in the line
		curr_char++;			// move onto the next char

	}
}


void gsFont::PrintLine(int x, int y, int z, unsigned long colour, int length, const char* string)
{
	int row, column, charXpos, charYpos;

	unsigned long col = colour;

	// now print the current line
	for (int char_num=0; char_num<length; char_num++)
	{
		unsigned char curr_char = string[char_num];

		// Add up width of characters - until newline, or end of string, or max-length exceeded
		switch (curr_char)
		{
		case '\n': // have already calc'd length of line based on newline etc, so ignore 
			break;

		case '\r':	// Carriage return
		case '\t':	// tab
			//do nothing
			break;

		case '\b': // Bold on/off
			m_Bold = m_Bold ^ 1;

			if (m_Bold)
				col=colour | 0xFF000000;
			else
				col=colour;

			break;

		case '\a': // Underline on/off
			m_Underline = m_Underline ^ 1;
			break;

		default:
			// print character, and move X position
			// work out row/column position of current character
			row = (curr_char / m_NumXChars);
			column = (curr_char - (row*m_NumXChars));

			// get position of characters in texture
			// must take into account texture offsets within buffer
			charYpos = row    * m_CharGridHeight + m_TBxpos;
			charXpos = column * m_CharGridWidth  + m_TBypos;
			
			int curr_width = m_CharWidth[curr_char];

			// Print the kerned character (assuming that character is in middle of char-grid position
			m_pFontPipe->RectTexture(
				x, y, 
				charXpos, charYpos,
				x+curr_width-1, y+m_CharGridHeight,
				(charXpos+curr_width), (charYpos+m_CharGridHeight),
				z, col);

			if (m_Underline)
			{
				m_pFontPipe->RectFlat(x, y+m_CharGridHeight,
				x+curr_width, y+m_CharGridHeight+1,
				z, colour | 0x7F000000);
			}

			x += curr_width;

			break;
		}
	}
}
