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

#define BuildConnection 1
#define BroadcastTopology 2
#define RelayRoute 3
#define ToBeGw 4

struct CH_Rocord
{
    simtime_t start_Time;
    simtime_t now_Time;
};

struct RouteInfo {
    std::string nodeId;
    LAddress::L2Type macAddr;
    double connectivityValue;
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
	double meanSpeed;
	int clusterSize;
	bool hasFindGw;
	bool isBehindGw;
	simtime_t lastUpdateTime;
	double lastSpeed;
	Coord lastCoorD;
	std::map<std::string,Info> vehicleInfo;
	std::map<std::string,Info> vehicleInfoCH;
	std::queue<std::string> clusterQueue;
	std::queue<std::string> tempClusterQueue;


	InfoGW infoGw;
	InfoGWToLte infoGwToLte;
	std::map<std::string, Coord> electricMap;
	std::map<std::string,Info> GwForCluster;
	std::set<int> topologySet;

	std::map<double, std::vector<Connectivity_Info>> routeForSelected;
	std::vector<Connectivity_Info> routeTable;

	//record DATA
	int overHead_clustering;
	std::map<std::string,CH_Rocord> CH_Info;
	std::set<std::string> CH_Id;
	std::set<int> carState;
	int broadcastNum;
	int unicastNum;
	bool gatewayNode;
	std::set<std::string> GWTOGWs;


protected:
	virtual void initialize(int stage);
	virtual void handleMessage(cMessage *msg);
	void finish();
	//zy
	Veins::TraCIMobility* mobility;
    Coord getCurrentPos(const simtime_t& t);
    std::string getCurrentRoadId();
    std::string getCurrentJunctionId(std::string roadId);
    std::string getLastJunctionId(std::string roadId);
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
	void startUnicastByGateWay(HeterogeneousMessage *receiveMessage,int msgState);
	void startUnicastToGateWay(std::string GWID);
	void startUnicastDeparture(LAddress::L2Type macAddr,int msgState,std::string Id_CH);
	void setVehicleState(int value);
	int getVehicleState();
	void findTwoGW(std::map<std::string,Info> tempInfo);
	double getAcceleration();
	double getEffictiveTime();
	double getDistanceToIntersection();
	void updateRecordLastTime();
	double getTimeDelay(HeterogeneousMessage *receiveMessage);

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
	bool isNeighborGW(std::string GWRoadId, std::string selfRoadId, int StateOfNodeReceived);

	void recordMacAddr_CH(LAddress::L2Type temp);

	LAddress::L2Type getMacAddr_CH();

    bool isgatewayNode();
    void setGateWayNode(bool temp);
    void setInfoGw(InfoGW temp);
    InfoGW getInfoGw();
    void setInfoGwToLte(InfoGWToLte temp);
    InfoGWToLte getInfoGwToLte();

    void startBroadcastTopologyInfo(int code);
    void relayRoutingMsg(HeterogeneousMessage *receiveMessage);
    void findRoutePath( std::vector<Connectivity_Info> onePath, double conenctivity_value,int currentHop, std::string roadIdSourceNode, std::string targetIntersectionId);


	//record date
	void insertCH_Id(std::string carId);
	std::set<std::string> getCH_Id(std::string carId);
	void updateCH_Info(std::string carId,simtime_t time);
	std::map<std::string,CH_Rocord> getCH_Info();
	void countBroadcastNum();
};

#endif
