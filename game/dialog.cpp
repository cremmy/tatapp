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
#include "level.h"
#include "npc.h"

using namespace Game;

Dialog::Dialog(): mode(Mode::NONE), ready(true), b(0), index(0), waitTimer(0.0f), selectionIndex(0), message(""), logIndex(0), lvl(nullptr), player(nullptr)
	{
	//
	}

Dialog::~Dialog()
	{
	selectionTarget.clear();
	selectionOption.clear();
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


void Dialog::start()
	{
	if(mode!=Mode::NONE)
		return;

	mode=Mode::DIALOG;
	ready=false;
	b=1;
	index=0;
	log.clear();
	}


void Dialog::next()
	{
	if(!ready)
		return;

	if(logIndex>0)
		{
		--logIndex;

		if(logIndex>0 || mode!=Mode::SELECTION)
			{
			message=log[log.size()-1-logIndex];
			LOG_DEBUG("Dialog.next: NEXT: %d -> %d / %d; LOG: \"%s\"", logIndex+1, logIndex, log.size(), message.c_str());
			}
		else
			{
			++selectionIndex;
			selectPrevious();
			}
		}
	else if(mode==Mode::SELECTION)
		{
		ready=false;
		mode=Mode::DIALOG;
		b=selectionTarget[selectionIndex];
		index=0;

		log.back()=message;

		selectionTarget.clear();
		selectionOption.clear();

		message="";
		}
	else
		{
		ready=false;
		--b;
		index=0;

		message="";
		}
	}

void Dialog::previous()
	{
	if(!ready)
		return;

	LOG_DEBUG("PREVIOUS: %d", logIndex);

	++logIndex;

	if(logIndex>=(int)log.size())
		{
		LOG_DEBUG("PREVIOUS: OVERFLOW");
		logIndex=log.size()-1;
		return;
		}

	message=log[log.size()-1-logIndex];
	LOG_DEBUG("Dialog.previous: PREV: %d -> %d / %d; LOG: \"%s\"", logIndex-1, logIndex, log.size(), message.c_str());
	}


void Dialog::selectPrevious()
	{
	if(!ready || mode!=Mode::SELECTION)
		return;

	LOG_DEBUG("SELECT PREVIOUS");

	selectionIndex--;

	if(selectionIndex<0)
		selectionIndex=selectionOption.size()-1;

	message="";

	for(int i=0; i<(int)selectionOption.size(); ++i)
		{
		if(i==selectionIndex)
			{
			message+="[";
			message+=selectionOption[i];
			message+="]\n";
			}
		else
			{
			message+=selectionOption[i];
			message+="\n";
			}
		}
	}

void Dialog::selectNext()
	{
	if(!ready || mode!=Mode::SELECTION)
		return;

	LOG_DEBUG("SELECT NEXT");

	selectionIndex++;

	if(selectionIndex>=(int)selectionOption.size())
		selectionIndex=0;

	message="";

	for(int i=0; i<(int)selectionOption.size(); ++i)
		{
		if(i==selectionIndex)
			{
			message+="[";
			message+=selectionOption[i];
			message+="]\n";
			}
		else
			{
			message+=selectionOption[i];
			message+="\n";
			}
		}
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

		log.clear();
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

/*****************************************************************************/
/* Komendy *******************************************************************/
/*****************************************************************************/
		/** Kontrola **/
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
		else if(cmdpars[0]=="stop")
			{
			ready=true;

			break;
			}
/*****************************************************************************/
		/** Tekst **/
		else if(cmdpars[0]=="str")
			{
			CMD_PARAMS_REQ(1);

			cmdpars.remove(0);
			message+=cmdpars.get();

			continue;
			}
		else if(cmdpars[0]=="strnl")
			{
			CMD_PARAMS_REQ(1);

			cmdpars.remove(0);
			message+=cmdpars.get()+"\n";

			continue;
			}
		else if(cmdpars[0]=="nlstr")
			{
			CMD_PARAMS_REQ(1);

			cmdpars.remove(0);
			message+=(std::string)"\n"+cmdpars.get();

			continue;
			}
		else if(cmdpars[0]=="nl")
			{
			message+="\n";

			continue;
			}
/*****************************************************************************/
		/** Wybory **/
		else if(cmdpars[0]=="wyb")
			{
			CMD_PARAMS_REQ(2);

			if(mode!=Mode::SELECTION)
				{
				mode=Mode::SELECTION;
				}

			selectionTarget.push_back(cmdpars.toInt(1));
			cmdpars.remove(1u);
			cmdpars.remove(0u);
			selectionOption.push_back(cmdpars.get());
			}
/*****************************************************************************/
		/** Ruch NPC **/
		else if(cmdpars[0]=="pos")
			{
			CMD_PARAMS_REQ(4);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setPosition(Engine::Math::AVector(cmdpars.toFloat(2), cmdpars.toFloat(3), cmdpars.toFloat(4)));

			continue;
			}
		else if(cmdpars[0]=="rotz")
			{
			CMD_PARAMS_REQ(3);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setMovement(npc->getOrientation().getRotated(Engine::Math::AVector(0, 0, 1), cmdpars.toFloat(2)), cmdpars.toFloat(3));

			continue;
			}
		}

	ready=true;

	if(mode==Mode::SELECTION)
		{
		selectionIndex=1;
		selectPrevious();
		log.push_back("<<temp>>");
		}
	else
		{
		log.push_back(message);
		LOG_DEBUG("Dialog.update: [%s]", message.c_str());
		}

#undef CMD_PARAMS_REQ
	}
