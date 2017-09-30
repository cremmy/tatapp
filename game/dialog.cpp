/*
 * dialog.cpp
 *
 *  Created on: 8 wrz 2017
 *      Author: crm
 */

#include "dialog.h"

#include <sstream>

#include "engine/debug/log.h"
#include "engine/render/render.h"
#include "engine/sound/soundplayer.h"
#include "engine/utils/stringparser.h"
#include "level.h"
#include "npc.h"
#include "player.h"

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

	/*int comCount=0;*/

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

		/*comCount++;*/

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
		else if(cmdpars[0]=="w8")
			{
			CMD_PARAMS_REQ(1);

			ready=false;
			waitTimer=cmdpars.toFloat(1);
			++index;

			return;
			}
/*****************************************************************************/
		/** Tekst **/
		else if(cmdpars[0]=="str")
			{
			CMD_PARAMS_REQ(1);

			//cmdpars.remove(0);
			message+=cmdpars.get().substr(4);

			continue;
			}
		else if(cmdpars[0]=="strnl")
			{
			CMD_PARAMS_REQ(1);

			//cmdpars.remove(0);
			message+=cmdpars.get().substr(6)+"\n";

			continue;
			}
		else if(cmdpars[0]=="nlstr")
			{
			CMD_PARAMS_REQ(1);

			//cmdpars.remove(0);
			message+=(std::string)"\n"+cmdpars.get().substr(6);

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
		else if(cmdpars[0]=="roty")
			{
			CMD_PARAMS_REQ(3);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setMovement(npc->getOrientation().getRotated(Engine::Math::AVector(0, 1, 0), cmdpars.toFloat(2)), cmdpars.toFloat(3));

			continue;
			}
		else if(cmdpars[0]=="rotx")
			{
			CMD_PARAMS_REQ(3);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setMovement(npc->getOrientation().getRotated(Engine::Math::AVector(1, 0, 0), cmdpars.toFloat(2)), cmdpars.toFloat(3));

			continue;
			}
		else if(cmdpars[0]=="rotate")
			{
			CMD_PARAMS_REQ(3);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setMovement(Engine::Math::Orientation(
				npc->getOrientation().getPosition(),
				Engine::Math::AMatrixRotZ(cmdpars.toFloat(2))*Engine::Math::AVector(1, 0, 0),
				Engine::Math::AVector(0, 0, 1)), cmdpars.toFloat(3));

			continue;
			}
		else if(cmdpars[0]=="move")
			{
			CMD_PARAMS_REQ(5);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			// move testator.ini  -2 -2 1  4.0
			if(cmdpars.count()==6)
				{
				npc->setMovement(Engine::Math::Orientation(Engine::Math::AVector(cmdpars.toFloat(2), cmdpars.toFloat(3), cmdpars.toFloat(4)), npc->getOrientation()), cmdpars.toFloat(5));
				}
			// move testator.ini  -2 -2 1  0 0 1  90.0 4.0
			else if(cmdpars.count()==11)
				{
				// Oś i kąt obrotu
				Engine::Math::Orientation target=npc->getOrientation();
				target.rotate(Engine::Math::AVectorNormalize(Engine::Math::AVector(cmdpars.toFloat(5), cmdpars.toFloat(6), cmdpars.toFloat(7))), cmdpars.toFloat(8));
				target.setPosition(Engine::Math::AVector(cmdpars.toFloat(2), cmdpars.toFloat(3), cmdpars.toFloat(4)));
				if(cmdpars.count()>9)
					{
					target.setScale(cmdpars.toFloat(10));
					}
				npc->setMovement(target, cmdpars.toFloat(9));

				}
			else
				{
				// Orientacja
				npc->setMovement(Engine::Math::Orientation(
					Engine::Math::AVector(cmdpars.toFloat(2), cmdpars.toFloat(3), cmdpars.toFloat(4)),
					Engine::Math::AVector(cmdpars.toFloat(5), cmdpars.toFloat(6), cmdpars.toFloat(7)),
					Engine::Math::AVector(cmdpars.toFloat(8), cmdpars.toFloat(9), cmdpars.toFloat(10)),
					(cmdpars.count()>12)?cmdpars.toFloat(12):1.0f
					), cmdpars.toFloat(11));
				}

			continue;
			}
		else if(cmdpars[0]=="orientation")
			{
			// move testator.ini  -2 -2 1  1 0 0  0 0 1
			CMD_PARAMS_REQ(10);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setOrientation(Engine::Math::Orientation(
				Engine::Math::AVector(cmdpars.toFloat(2), cmdpars.toFloat(3), cmdpars.toFloat(4)),
				Engine::Math::AVector(cmdpars.toFloat(5), cmdpars.toFloat(6), cmdpars.toFloat(7)),
				Engine::Math::AVector(cmdpars.toFloat(8), cmdpars.toFloat(9), cmdpars.toFloat(10))
				));

			continue;
			}
/*****************************************************************************/
		/** Ruch gracza **/
		else if(cmdpars[0]=="pmove")
			{
			// pmove -2 -2 1  0 0 1  4.0
			CMD_PARAMS_REQ(7);

			/*if(cmdpars.count()==5)
				{
				// Orientacja
				Engine::Math::Orientation target;

				target.lookAt(
						Engine::Math::AVector(cmdpars.toFloat(1), cmdpars.toFloat(2), cmdpars.toFloat(3)),
						player->getOrientation().getPosition());

				player->setMovement(target, cmdpars.toFloat(4));
				}
			else */if(cmdpars.count()==8)
				{
				// Orientacja
				Engine::Math::Orientation target;

				target.lookAt(
						Engine::Math::AVector(cmdpars.toFloat(1), cmdpars.toFloat(2), cmdpars.toFloat(3)),
						Engine::Math::AVector(cmdpars.toFloat(4), cmdpars.toFloat(5), cmdpars.toFloat(6)));

				player->setMovement(target, cmdpars.toFloat(7));
				}
			else if(cmdpars.count()==11)
				{
				// Oś i kąt obrotu
				Engine::Math::Orientation target=player->getOrientation();
				target.rotate(Engine::Math::AVectorNormalize(Engine::Math::AVector(cmdpars.toFloat(5), cmdpars.toFloat(6), cmdpars.toFloat(7))), cmdpars.toFloat(8));
				target.setPosition(Engine::Math::AVector(cmdpars.toFloat(2), cmdpars.toFloat(3), cmdpars.toFloat(4)));
				if(cmdpars.count()>9)
					{
					target.setScale(cmdpars.toFloat(10));
					}
				player->setMovement(target, cmdpars.toFloat(9));
				}
			else
				{
				LOG_ERROR("Dialog.update: Za malo argumentow do \"lookat\", otrzymano %d", cmdpars.count());
				}

			continue;
			}
		else if(cmdpars[0]=="lookat")
			{
			// lookat -2 -2 1  4.0
			CMD_PARAMS_REQ(3);

			player->setLookAt(
				Engine::Math::AVector(cmdpars.toFloat(1), cmdpars.toFloat(2), cmdpars.toFloat(3)),
				(cmdpars.count()>4)?cmdpars.toFloat(4):0.0f);

			continue;
			}
/*****************************************************************************/
		/** Widoczność **/
		else if(cmdpars[0]=="show")
			{
			CMD_PARAMS_REQ(1);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setVisibility(true);
			}
		else if(cmdpars[0]=="hide")
			{
			CMD_PARAMS_REQ(1);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setVisibility(false);
			}
/*****************************************************************************/
		/** Kolizje **/
		else if(cmdpars[0]=="collide")
			{
			CMD_PARAMS_REQ(1);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setCollisionEnabled(true);
			}
		else if(cmdpars[0]=="uncollide")
			{
			CMD_PARAMS_REQ(1);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setCollisionEnabled(false);
			}
/*****************************************************************************/
		/** Skrypty **/
		else if(cmdpars[0]=="enable")
			{
			CMD_PARAMS_REQ(1);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setScriptEnabled(true);

			if(cmdpars.count()>2)
				{
				LOG_INFO("Dialog:update: Ustawianie skryptu npc \"%s\" na \"%s\"", cmdpars[1].c_str(), cmdpars[2].c_str());
				npc->setScript(cmdpars[2]);
				}
			}
		else if(cmdpars[0]=="disable")
			{
			CMD_PARAMS_REQ(1);

			NPC* npc=lvl->findNPCByName(cmdpars[1]);

			if(!npc)
				{
				LOG_ERROR("Dialog.update: Nie znaleziono NPC \"%s\"", cmdpars[1].c_str());
				continue;
				}

			npc->setScriptEnabled(false);
			}
/*****************************************************************************/
		/** Fade **/
		else if(cmdpars[0]=="fade")
			{
			if(cmdpars.count()==2)
				{
				player->getFade().set(1, cmdpars.toFloat(1));
				}
			else if(cmdpars.count()==3)
				{
				player->getFade().set(1, cmdpars.toFloat(1), cmdpars.toFloat(2));
				}
			else if(cmdpars.count()==4)
				{
				player->getFade().set(1, cmdpars[1], cmdpars.toFloat(2), cmdpars.toFloat(3));
				}
			else if(cmdpars.count()==5)
				{
				player->getFade().set(cmdpars.toInt(1), cmdpars[2], cmdpars.toFloat(3), cmdpars.toFloat(4));
				}
			else
				{
				CMD_PARAMS_REQ(2);
				}
			}
		else if(cmdpars[0]=="unfade")
			{
			player->getFade().set(1, nullptr, 0.0f, 0.0f);
			}
/*****************************************************************************/
		/** Zmienne i baza danych **/
		else if(cmdpars[0]=="save")
			{
			CMD_PARAMS_REQ(2);

			LOG_DEBUG("Dialog.update: \"set\": ustawianie \"%s\" na %d", cmdpars[1].c_str(), cmdpars.toInt(2));
			player->getDatabase().setVal(cmdpars[1], cmdpars.toInt(2));
			}
		else if(cmdpars[0]=="add")
			{
			CMD_PARAMS_REQ(2);

			LOG_DEBUG("Dialog.update: \"add\": zwiekszanie \"%s\" o %d", cmdpars[1].c_str(), cmdpars.toInt(2));
			player->getDatabase().setVal(cmdpars[1], player->getDatabase().getVal(cmdpars[1])+cmdpars.toInt(2));
			}
		else if(cmdpars[0]=="if")
			{
			CMD_PARAMS_REQ(4);

			int dbv=player->getDatabase().getVal(cmdpars[1]);

			if(cmdpars.count()==5)
				{
				if(dbv==cmdpars.toInt(2))
					{
					LOG_DEBUG("Dialog.update: \"if\": Porownywanie z \"%s\": %s==%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					b=cmdpars.toInt(3);
					index=-1;
					continue;
					}
				else
					{
					LOG_DEBUG("Dialog.update: \"if\": Porownywanie z \"%s\": %s!=%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					b=cmdpars.toInt(4);
					index=-1;
					continue;
					}
				}
			else
				{
				int pv=cmdpars.toInt(2);

				if(dbv<pv)
					{
					LOG_DEBUG("Dialog.update: \"if\": Porownywanie z \"%s\": %s<%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					b=cmdpars.toInt(3);
					index=-1;
					continue;
					}
				else if(dbv==pv)
					{
					LOG_DEBUG("Dialog.update: \"if\": Porownywanie z \"%s\": %s==%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					b=cmdpars.toInt(4);
					index=-1;
					continue;
					}
				else
					{
					LOG_DEBUG("Dialog.update: \"if\": Porownywanie z \"%s\": %s>%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					b=cmdpars.toInt(5);
					index=-1;
					continue;
					}
				}
			}
		else if(cmdpars[0]=="ifi")
			{
			if(cmdpars.count()<5)
				{
				LOG_ERROR("Dialog.update: Za malo argumentow do \"ifi\", oczekiwano 5/6, otrzymano %d", cmdpars.count());
				continue;
				}

			int dbv=player->getDatabase().getVal(cmdpars[1]);

			if(cmdpars.count()==5)
				{
				if(dbv==cmdpars.toInt(2))
					{
					LOG_DEBUG("Dialog.update: \"ifi\": Porownywanie z \"%s\": %s==%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					index=cmdpars.toInt(3)-1;
					continue;
					}
				else
					{
					LOG_DEBUG("Dialog.update: \"ifi\": Porownywanie z \"%s\": %s!=%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					index=cmdpars.toInt(4)-1;
					continue;
					}
				}
			else
				{
				int pv=cmdpars.toInt(2);

				if(dbv<pv)
					{
					LOG_DEBUG("Dialog.update: \"ifi\": Porownywanie z \"%s\": %s<%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					index=cmdpars.toInt(3)-1;
					continue;
					}
				else if(dbv==pv)
					{
					LOG_DEBUG("Dialog.update: \"ifi\": Porownywanie z \"%s\": %s==%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					index=cmdpars.toInt(4)-1;
					continue;
					}
				else
					{
					LOG_DEBUG("Dialog.update: \"ifi\": Porownywanie z \"%s\": %s>%d", cmdpars[1].c_str(), cmdpars[2].c_str(), dbv);
					index=cmdpars.toInt(5)-1;
					continue;
					}
				}
			}
		else if(cmdpars[0]=="switch")
			{
			if(cmdpars.count()<3)
				{
				LOG_ERROR("Dialog.update: Za malo argumentow do \"switch\", oczekiwano 5/6, otrzymano %d", cmdpars.count());
				continue;
				}

			int dbv=player->getDatabase().getVal(cmdpars[1]);

			if(dbv<0 || dbv>(int)cmdpars.count()-2)
				{
				LOG_DEBUG("Dialog.update: \"switch\": Brak akcji dla wartosci %d (lub jest ona ujemna)", dbv);
				continue;
				}

			LOG_DEBUG("Dialog.update: \"switch\": Akcja %d -> %d", dbv, cmdpars.toInt(dbv+2));

			b=cmdpars.toInt(dbv+2);
			index=index-1;
			continue;
			}
		else if(cmdpars[0]=="switchi")
			{
			if(cmdpars.count()<5)
				{
				LOG_ERROR("Dialog.update: Za malo argumentow do \"switchi\", oczekiwano 6/7, otrzymano %d", cmdpars.count());
				continue;
				}

			int dbv=player->getDatabase().getVal(cmdpars[1]);

			if(dbv<0 || dbv>(int)cmdpars.count()-2)
				{
				LOG_DEBUG("Dialog.update: \"switchi\": Brak akcji dla wartosci %d (lub jest ona ujemna)", dbv);
				continue;
				}

			LOG_DEBUG("Dialog.update: \"switchi\": Akcja %d -> %d", dbv, cmdpars.toInt(dbv)-2);

			index=cmdpars.toInt(dbv-2)-1;
			continue;
			}
/*****************************************************************************/
		/** Dzwieki **/
		else if(cmdpars[0]=="sound")
			{
			CMD_PARAMS_REQ(2);

			float volume=cmdpars.toFloat(2);

			if(volume>1.0f)
				volume=1.0f;
			else if(volume<0.0f)
				volume=0.0f;

			Engine::Sound::getInstance().play(cmdpars[1], volume*128);

			//player->setPosition(Engine::Math::AVector());
			}
/*****************************************************************************/
		/** Inne **/
		/*else if(cmdpars[0]=="lvl")
			{
			CMD_PARAMS_REQ(1);

			if(!lvl->init(cmdpars[1]))
				{
				LOG_ERROR("Dialog.update: Ups, chyba zepsulem gre. Tehe~");
				ready=true;
				break;
				}

			//player->setPosition(Engine::Math::AVector());
			}*/
		else if(cmdpars[0]=="light")
			{
			CMD_PARAMS_REQ(6);

			Engine::Render::getInstance().setLight(
				Engine::Math::AVector(cmdpars.toFloat(1), cmdpars.toFloat(2), cmdpars.toFloat(3)),
				Engine::Math::AVector(cmdpars.toFloat(4), cmdpars.toFloat(5), cmdpars.toFloat(6)));
			}
/*****************************************************************************/
		/** Inne **/
		else
			{
			/*--comCount;*/
			LOG_ERROR("Dialog.update: Nieznana komenda \"%s\"", cmdpars[0].c_str());
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
		//LOG_DEBUG("Dialog.update: [%s]", message.c_str());
		}

	/*if(comCount<=0)
		{
		LOG_ERROR("Dialog.update: Nie znaleziono ani jednej poprawnej komendy [b %d]", b);

		b=0;
		ready=false;
		}*/

#undef CMD_PARAMS_REQ
	}
