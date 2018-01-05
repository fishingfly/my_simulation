//
// Copyright (C) 2006-2014 Florian Hagenauer <hagenauer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef __VEINS_SIMPLEAPP_H_
#define __VEINS_SIMPLEAPP_H_

#include <omnetpp.h>
#include <queue>
#include "veins/modules/heterogeneous/messages/HeterogeneousMessage_m.h"
#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

using Veins::TraCIScenarioManager;
using Veins::TraCIScenarioManagerAccess;

/**
 * @brief
 * A simple application that sends messages via the DONTCARE channel to other cars and sometimes a
 * message to the server via LTE.
 *
 * @author Florian Hagenauer
 */

enum NodeState
{
    Ini,//0
    SE,//1
    tempCH,//2
    CM,//3
    CH,//4
    lte,//5
    departure,//6
    dismiss//7
};

struct CH_Rocord
{
    simtime_t start_Time;
    simtime_t now_Time;
};

class SimpleApp: public cSimpleModule {

protected:
	int toDecisionMaker;
	int fromDecisionMaker;
	std::string sumoId;
	std::string lastRoadId;
	std::string CHId;
	LAddress::L2Type macAddr_CH;
	int nodeState;
	int initialPkt;
	int tempCHCount;
	int CHCount;
	int CMcount;
	int sameForTempUpdate;
	int sameForUpdate;
	std::map<std::string,Info> vehicleInfo;
	std::map<std::string,Info> vehicleInfoCH;
	std::queue<std::string> clusterQueue;
	std::queue<std::string> tempClusterQueue;
	//record DATA
	int overHead_clustering;
	std::map<std::string,CH_Rocord> CH_Info;
	std::set<std::string> CH_Id;
	std::set<int> carState;
	int broadcastNum;
	int unicastNum;


protected:
	virtual void initialize(int stage);
	virtual void handleMessage(cMessage *msg);
	void finish();
	//zy
	Veins::TraCIMobility* mobility;
    Coord getCurrentPos(const simtime_t& t);
    std::string getCurrentRoadId();
    std::string getCurrentJunctionId(std::string roadId);
    std::string getNextRoadId(std::list<std::string> route,std::string roadId);
    std::list<std::string> getCurrentRoute();
    std::string getSumoId();
	void addInitialcount();
	int getInitialcount();
	void setInitialcount(int value);
	void addTempCHCount();
	void setTempCHCount(int value);
	int getTempCHCount();
    void addCHCount();
    void setCHCount(int value);
    int getCHCount();
    void addCMcount();
    void setCMcount(int value);
    int getCMcount();
	void startBroadcast(int msgState);
	void startBroadcastDismiss(int msgState,std::string connectCHId);
	void sendToLTE(int msgState);
	void startUnicast(HeterogeneousMessage *receiveMessage,int msgState);
	void startUnicastDeparture(LAddress::L2Type macAddr,int msgState,std::string Id_CH);
	void setVehicleState(int value);
	int getVehicleState();

	void pushVehicleInfo(std::string carId,Info carInfo);
	std::map<std::string,Info> getVehicleInfoofTempCluster();

	void pushVehicleInfoCH(std::string carId,Info carInfo);
	std::map<std::string,Info> getVehicleInfoOfCluster();
	void removeVehicleInfoCH(std::string carId);

	void setTempClusterQueue(std::string roadID);
	std::queue<std::string> getTempClusterQueue();
	void clearTempCluster();

	void setClusterQueue(std::string roadID);
    std::queue<std::string> getClusterQueue();

	bool memberOfCluster(std::queue<std::string> carQueue,std::string roadID);
	bool findFromCHList(std::map<std::string,Info> CHInfo,std::string carId);

	void setCHId(std::string s);
	std::string getCHId();

	int getSpeed();

	void recordMacAddr_CH(LAddress::L2Type temp);
	LAddress::L2Type getMacAddr_CH();
	//record date
	void insertCH_Id(std::string carId);
	std::set<std::string> getCH_Id(std::string carId);
	void updateCH_Info(std::string carId,simtime_t time);
	std::map<std::string,CH_Rocord> getCH_Info();
	void countBroadcastNum();
};

#endif