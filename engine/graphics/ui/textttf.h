/*
 * textttf.h
 *
 *  Created on: 2 mar 2017
 *      Author: crm
 */

#pragma once

#include <string>
#include <vector>

namespace Engine
	{
	namespace Graphics
		{
		namespace UI
			{
			class TextTTF
				{
				public:
					enum Options
						{
					// Wyrownanie w poziomie
						ALIGN_LEFT=		0x1<<0,
						ALIGN_CENTER=	0x1<<1,
						ALIGN_RIGHT=	0x1<<2,
					// Wyrownanie w pionie
						ALIGN_TOP=		0x1<<3,
						ALIGN_MIDDLE=	0x1<<4,
						ALIGN_BOTTOM=	0x1<<5,

					// Zawijanie
						WRAP=			0x1<<6,					// Zawijanie tekstu

						CLIP=			0x1<<7,					// Przyciecie do max wymiarow
						OVERFLOWH=		0x1<<8,					// Moze wyleciec w poziomie
						OVERFLOWV=		0x1<<9,					// Moze wyleciec w pionie
						OVERFLOW=		OVERFLOWH|OVERFLOWV,	// Moze wyleciec w dowolnym wymiarze

						DEFAULT=ALIGN_LEFT|ALIGN_TOP|WRAP|OVERFLOWV,		// Domyslne wartosci
						};

				protected:
					/*struct LineInfo
						{
						int start, length, pixs;			// poczatek, liczba znakow, dlugosc w pikselach
						};

					bool update;

					int w, h;								// Docelowe
					int rw, rh;								// Rzeczywiste

					int line, space, tab;					// Wymiary linii (wysokosc), spacji i taba (rozstaw kolumn, co ile pikseli)
					float alpha;

					std::string str;

					std::vector<LineInfo> lines;

					Options opts;*/

				public:
					TextTTF();
					~TextTTF();

					/*static bool init();
					static bool clear();

					bool init(const std::string& font);*/
				};

			} /* namespace UI */
		} /* namespace Graphics */
	} /* namespace Engine */
