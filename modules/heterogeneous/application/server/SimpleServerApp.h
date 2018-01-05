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

#ifndef SIMPLESERVER_H_
#define SIMPLESERVER_H_

#include <omnetpp.h>
#include "ApplicationBase.h"
#include "INETDefs.h"
#include "UDPSocket.h"
#include "IPv4Address.h"
#include "veins/modules/heterogeneous/messages/HeterogeneousMessage_m.h"
#include "veins/modules/mobility/traci/TraCIScenarioManager.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "MobilityBase.h"

/*
 * @brief
 * A simple server that just prints the received heterogeneous messages.
 *
 * @author Florian Hagenauer
 */

struct RequestRecord
{
    std::string requestJuctionId;
    std::string requestCurrentRoadId;
};

struct SimpleInfo
{
    std::string vehilceId;
    std::string nextRoadId;
    Coord pos;
};

struct InfoAndWeigth
{
    std::string nextRoadId;
    std::string currentRoadId;
    std::string junctionId;
    Coord pos;
    double weight;
};

using Veins::TraCIScenarioManager;
using Veins::TraCIScenarioManagerAccess;

class SimpleServerApp: public ApplicationBase {
	protected:
		UDPSocket socket;
		TraCIScenarioManager* manager;
		MobilityBase* eNodeBMobility;
		long receivedMessages;
		bool debug;
		std::map<std::string,Info> lteDB;
		RequestRecord reqRecord;
		std::string maxNextRoadId;
	    InfoGWToLte infoGWToLte_var;


	public:
		SimpleServerApp();
		virtual ~SimpleServerApp();

		virtual int numInitStages() const {
			return 4;
		}
		virtual void initialize(int stage);
		virtual void finish();
		virtual void handleMessageWhenUp(cMessage *msg);

		virtual bool handleNodeStart(IDoneCallback *doneCallback);
		virtual bool handleNodeShutdown(IDoneCallback *doneCallback);
		virtual void handleNodeCrash();

		//zy
		void tempCHMsgToDB(HeterogeneousMessage* heterogeneousMessage);
		void analyzeCHMsg(HeterogeneousMessage* heterogeneousMessage);
		std::string gatherFromDB();
		Coord geteNodeBPos();
		void setRequestInfo(RequestRecord temp);
		RequestRecord getRequestInfo();
		double getMeanDis(std::map<std::string,Info> tempDB,std::string carId,Info carInfo);
		double computeDis(Coord pos1,Coord pos2);
		double getMaxDis(std::map<std::string,Info> tempDB,std::string carId,Info carInfo);

};

#endif /* SIMPLESERVER_H_ */
