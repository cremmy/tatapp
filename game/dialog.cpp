/*
 * dialog.cpp
 *
 *  Created on: 8 wrz 2017
 *      Author: crm
 */

#include "dialog.h"

#include <sstream>

#include "engine/debug/log.h"
#include "engine/utils/stringparser.h"

using namespace Game;

Dialog::Dialog(): mode(Mode::NONE), ready(true), b(0), index(0), waitTimer(0.0f), selectionIndex(0), message(""), logIndex(0)
	{
	//
	}

Dialog::~Dialog()
	{
	selection.clear();
	log.clear();
	}


bool Dialog::init(const std::string& path)
	{
	if(!script.init(path, "dialog"))
		{
		return false;
		}

	return true;
	}

void Dialog::update(float dt)
	{
	if(mode==Mode::NONE)
		{
		return;
		}

	if(b<=0)
		{
		LOG_DEBUG("Dialog.update: Konczenie dialogu (b<=0)");
		mode=Mode::NONE;
		return;
		}

	if(ready)
		{
		return;
		}

	if(waitTimer>0.0f)
		{
		waitTimer-=dt;
		return;
		}

	//LOG_DEBUG("Dialog.update: Parsowanie...");

	Engine::Utils::StringParser cmdpars("", " ");

#define CMD_PARAMS_REQ(n) if(cmdpars.count()<n+1)\
	{\
	LOG_ERROR("Dialog.update: [%4d:%2d] Za malo argumentow komendy \"%s\", oczekiwano %d, otrzymano %d", b, index, cmdpars[0].c_str(), n, cmdpars.count()-1);\
	continue;\
	}

	for(/**/; index<100; ++index)
		{
		std::string cmd;
		std::stringstream ss;

		if(index==0)
			{
			ss << b;
			cmd=script.get(ss.str());

			if(cmd.length()<1)
				{
				cmd=script.get(ss.str()+" 0");
				}
			}
		else
			{
			ss << b << " " << index;
			cmd=script.get(ss.str());
			}

		if(cmd.length()<1)
			{
			continue;
			}

		cmdpars=cmd;

		if(cmdpars.count()<1)
			{
			continue;
			}

		//LOG_DEBUG("Dialog.update: [%5d:%2d][%s]", b, index, cmd.c_str());

		if(cmdpars[0]=="setb")
			{
			CMD_PARAMS_REQ(1);

			b=cmdpars.toInt(1u);
			index=-1;
			message="";

			if(b<0)
				{
				//mode=Mode::NONE;
				break;
				}

			continue;
			}
		else if(cmdpars[0]=="seti")
			{
			CMD_PARAMS_REQ(1);

			index=cmdpars.toInt(1u)-1;

			if(index<0)
				{
				index=0;
				}

			continue;
			}
		else if(cmdpars[0]=="str")
			{
			//CMD_PARAMS_REQ(1);

			cmdpars.remove(0);
			message+=cmdpars.get();

			continue;
			}
		}

	ready=true;

	LOG_DEBUG("Dialog.update: [%s]", message.c_str());

#undef CMD_PARAMS_REQ
	}


void Dialog::start()
	{
	if(mode!=Mode::NONE)
		return;

	mode=Mode::DIALOG;
	ready=false;
	b=1;
	index=0;
	}


void Dialog::next()
	{
	if(!ready)
		return;

	if(logIndex>0)
		{
		--logIndex;

		if(logIndex>0)
			{
			message=log[log.size()-1-logIndex];
			}
		}
	else
		{
		ready=false;
		--b;
		index=0;

		log.push_back(message);
		message="";
		}
	}

void Dialog::previous()
	{
	if(!ready)
		return;

	++logIndex;

	if(logIndex>=(int)log.size())
		{
		logIndex=log.size()-1;
		}

	message=log[log.size()-1-logIndex];
	}


void Dialog::selectPrevious()
	{
	if(!ready || mode!=Mode::SELECTION)
		return;

	ready=false;

	selectionIndex--;

	if(selectionIndex<0)
		selectionIndex=selection.size()-1;
	}

void Dialog::selectNext()
	{
	if(!ready || mode!=Mode::SELECTION)
		return;

	ready=false;

	selectionIndex++;

	if(selectionIndex>=(int)selection.size())
		selectionIndex=0;
	}

