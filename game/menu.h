/*
 * menu.h
 *
 *  Created on: 02-08-2013
 *      Author: crm
 */

#pragma once

#include <string>
#include <vector>

#include "engine/graphics/spriteptr.h"
#include "engine/graphics/ui/text.h"

class Menu
	{
	public:
		enum class Type
			{
			ACTION,
			ENTER,
			RETURN,
			VALUE,
			};

		class Option;
		class Layer;

		class Option
			{
			private:
				int tag;

				Type type;

				float val;
				float min, max, change;

				std::string name;

				Layer *sub;

			public:
				 Option(int tag, Type type, const std::string& name=""): tag(tag), type(type), val(0.0f), min(0.0f), max(0.0f), change(0.0f), name(name), sub(NULL) {}
				~Option() {if(sub) delete sub;}

				void initValue(float smin, float smax, float schange);
				void initSubLayer(Layer *s);

				void increment();
				void decrement();

				void setVal(float s) {val=s; if(s<min) s=min; if(s>max) s=max;}
				void setName(const std::string& s) {name=s;}

				int getTag() const {return tag;}
				const std::string& getName() const {return name;}
				Type getType() const {return type;}
				float getVal() const {return val;}
				Layer* getSubLayer() const {return sub;}
			};

		class Layer
			{
			private:
				std::vector<Option *> opts;

				int opt;

				void clamp();

			public:
				 Layer(): opt(0) {opts.reserve(4);}
				~Layer() {clear();}

				void clear();

				void addOption(Option *opt);
				void delOption(int i);

				void setOption(int i);

				void prev();
				void next();
				void first();
				void last();

				int getOptionCount() const {return opts.size();}
				int getOptionId() const {return opt;}
				Option* getOption() const {return opts[opt];}
				Option* getOption(int i) const {return opts[i%opts.size()];}	// Modulo, dla 0 opcji i tak by sie wysypalo~
				Option* getLastOption() const {return opts[getOptionCount()-1];}
				Option* getFirstOption() const {return opts[0];}
			};

		bool blocked;	// Wybrano opcje typu ACTION - oczekiwanie na reakcje... reszty kodu?
		bool exit;		// true jesli probowano wyjsc z pierwszej (najnizszej) warstwy

		bool redraw;

		Layer root;

		std::vector<Layer *> stack;

		Engine::Graphics::UI::Text text;

	public:
		 Menu(): blocked(false), exit(false), redraw(true), root(), stack() {}
		virtual ~Menu() {clear();}

		void init(int w, int h, const std::string& font);
		void update(float dt);
		void clear();

		Layer* getRootLayer() {return &root;}
		Layer* getLayer() {return stack.back();}
		Option* getOption() {return getLayer()->getOption();}
		const Engine::Graphics::UI::Text& getText() const {return text;}

		Option* getOptionByTag(int tag);

		bool isBlocked() const {return blocked;}
		bool isExited() const {return exit;}

		void unlock();

		void    up();
		void  down();
		void  prev();
		void  next();
		void first();
		void  last();
		void increment();
		void decrement();
	};
