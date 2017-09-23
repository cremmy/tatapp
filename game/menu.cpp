/*
 * menu.cpp
 *
 *  Created on: 02-08-2013
 *      Author: crm
 */

#include "menu.h"

#include <sstream>

#include "engine/debug/log.h"

/**** Option ****/
void Menu::Option::initValue(float smin, float smax, float schange)
	{
	min=smin;
	max=smax;
	change=schange;
	val=(min+max)/2;
	}
void Menu::Option::initSubLayer(Layer *s)
	{
	sub=s;
	}

void Menu::Option::increment()
	{
	if(type!=Type::VALUE)
		return;

	val+=change;
	if(val>max)
		val=max;
	}
void Menu::Option::decrement()
	{
	if(type!=Type::VALUE)
		return;

	val-=change;
	if(val<min)
		val=min;
	}

/**** Layer ****/
void Menu::Layer::clear()
	{
	for(std::vector<Option *>::iterator it=opts.begin(); it!=opts.end(); ++it)
		delete *it;
	opts.clear();
	}

void Menu::Layer::addOption(Option *opt)
	{
	opts.push_back(opt);
	}
void Menu::Layer::delOption(int i)
	{
	opts.erase(opts.begin()+i);
	if(i<opt)
		--opt;
	}

void Menu::Layer::setOption(int i)
	{
	opt=i;
	clamp();
	}

void Menu::Layer::prev()
	{
	--opt;
	if(opt<0)
		opt=(int)opts.size()-1;
	}
void Menu::Layer::next()
	{
	++opt;
	if(opt>(int)opts.size()-1)
		opt=0;
	}
void Menu::Layer::first()
	{
	opt=0;
	}
void Menu::Layer::last()
	{
	opt=opts.size()-1;
	}

void Menu::Layer::clamp()
	{
	if(opts.size()==0u)
		return;
	while(opt<0)
		opt+=opts.size();
	while(opt>=(int)opts.size())
		opt-=opts.size();
	}

/**** Menu ****/
void Menu::init(int w, int h, const std::string& font)
	{
	stack.push_back(&root);

	text.init(font, "", w, h, -1, 4);
	text.setAlignCenter();
	text.setAlignMiddle();
	redraw=true;
	}

void Menu::update(float /*dt*/)
	{
	if(!redraw)
		{
		return;
		}

	std::stringstream ss;

	const int size=getLayer()->getOptionCount();

	for(int i=0; i<size; ++i)
		{
		Menu::Option *opt=getLayer()->getOption(i);

		if(i>0)
			ss << '\n';

		const bool cur=getOption()==opt;

		if(cur) ss << '[';
		ss << opt->getName();
		if(opt->getType()==Type::VALUE)
			ss << ": " << opt->getVal();
		if(cur) ss << ']';
		}

	text.setStr(ss.str());
	text.update();

	redraw=false;
	}

void Menu::clear()
	{
	blocked=false;
	exit=false;

	root.clear();

	stack.clear();
	}

Menu::Option* Menu::getOptionByTag(int tag)
	{
	std::vector<Layer *> todo;

	todo.push_back(&root);

	while(todo.size()>0u)
		{
		Layer *l=todo.back();
		todo.pop_back();

		for(int i=0; i<l->getOptionCount(); ++i)
			{
			if(l->getOption(i)->getTag()==tag)
				return l->getOption(i);
			if(l->getOption(i)->getSubLayer())
				todo.push_back(l->getOption(i)->getSubLayer());
			}
		}

	return NULL;
	}

void Menu::unlock()
	{
	blocked=false;
	}

void Menu::up()
	{
	if(isBlocked())
		return;
	if(stack.size()>1)
		stack.pop_back();
	else
		{
		blocked=true;
		exit=true;
		}

	redraw=true;
	}
void Menu::down()
	{
	if(isBlocked())
		return;
	Option *o=getLayer()->getOption();

	switch(o->getType())
		{
		case Type::ACTION:
			blocked=true;
		break;

		case Type::ENTER:
			if(o->getSubLayer())
				{
				Layer *l=o->getSubLayer();
				stack.push_back(l);
				l->setOption(0);

				redraw=true;
				}
		break;

		case Type::RETURN:
			up();
		break;

		case Type::VALUE:
		default:
			//
		break;
		}
	}
void Menu::prev()
	{
	if(isBlocked())
		return;
	stack.back()->prev();

	redraw=true;
	}
void Menu::next()
	{
	if(isBlocked())
		return;
	stack.back()->next();

	redraw=true;
	}
void Menu::first()
	{
	if(isBlocked())
		return;
	stack.back()->first();

	redraw=true;
	}
void Menu::last()
	{
	if(isBlocked())
		return;
	stack.back()->last();

	redraw=true;
	}
void Menu::increment()
	{
	if(isBlocked())
		return;
	stack.back()->getOption()->increment();

	redraw=true;
	}
void Menu::decrement()
	{
	if(isBlocked())
		return;
	stack.back()->getOption()->decrement();

	redraw=true;
	}

/*void Menu::print()
	{
	std::vector<Layer *> list;

	list.push_back(&root);

	while(list.size()>0u)
		{
		Layer *l=list.back();
		list.pop_back();

		for(int i=0; i<l->getOptionCount(); ++i)
			{
			if(l->getOption(i)->getSubLayer())
				list.push_back(l->getOption(i)->getSubLayer());

			bool curr=l->getOption()==l->getOption(i);

			LOG_DEBUG("Menu.print: %d: %s [%s][%s]", i, l->getOption(i)->getName().c_str(), (l->getOption(i)->getSubLayer())?"sub":"", (curr)?"curr":"");
			}
		}
	}*/
