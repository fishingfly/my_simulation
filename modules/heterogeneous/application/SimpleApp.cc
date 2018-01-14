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

#include "SimpleApp.h"

Define_Module(SimpleApp);

void SimpleApp::initialize(int stage) {
	if (stage == 0) {
		toDecisionMaker = findGate("toDecisionMaker");
		fromDecisionMaker = findGate("fromDecisionMaker");
		cModule *tmpMobility = getParentModule()->getSubmodule("veinsmobility");
		mobility = dynamic_cast<Veins::TraCIMobility *>(tmpMobility);
		ASSERT(mobility);
		sumoId = mobility->getExternalId();
		initialPkt = 0;
		tempCHCount = 0;
		nodeState = 0;
		sameForTempUpdate = 0;
		sameForUpdate = 0;
		clusterSize = 0;
		lastTimeBuildConnection = 0;
		timeGWUpload = 0;
		smallestDelayForRoute = 100000;
		hasUpdateGwInfo = false;
		hasSelectedRoute = false;
        Coord coorTemp;
        coorTemp.x = 0;
        coorTemp.y = 0;
        electricMap.insert(std::make_pair("0/0",coorTemp));
        coorTemp.x = 0;
        coorTemp.y = 200;
        electricMap.insert(std::make_pair("0/1",coorTemp));
        coorTemp.x = 0;
        coorTemp.y = 500;
        electricMap.insert(std::make_pair("0/2",coorTemp));
        coorTemp.x = 0;
        coorTemp.y = 700;
        electricMap.insert(std::make_pair("0/3",coorTemp));
        coorTemp.x = 300;
        coorTemp.y = 0;
        electricMap.insert(std::make_pair("1/0",coorTemp));
        coorTemp.x = 300;
        coorTemp.y = 200;
        electricMap.insert(std::make_pair("1/1",coorTemp));
        coorTemp.x = 300;
        coorTemp.y = 500;
        electricMap.insert(std::make_pair("1/2",coorTemp));
        coorTemp.x = 300;
        coorTemp.y = 700;
        electricMap.insert(std::make_pair("1/3",coorTemp));
        coorTemp.x = 600;
        coorTemp.y = 0;
        electricMap.insert(std::make_pair("2/0",coorTemp));
        coorTemp.x = 600;
        coorTemp.y = 200;
        electricMap.insert(std::make_pair("2/1",coorTemp));
        coorTemp.x = 600;
        coorTemp.y = 500;
        electricMap.insert(std::make_pair("2/2",coorTemp));
        coorTemp.x = 600;
        coorTemp.y = 700;
        electricMap.insert(std::make_pair("2/3",coorTemp));
		//record data
		overHead_clustering=0;
		broadcastNum = 0;
		unicastNum = 0;
		this->gatewayNode = false;
		scheduleAt(simTime() + 0.5, new cMessage("Send"));
	}
}

void SimpleApp::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
	    std::cout<<"receive #self# message----------#"<<simTime().dbl()<<std::endl;
	    switch(getVehicleState()) {
        case 0:
            //ini
            if(initialPkt<3) {
                startBroadcast(0);
                addInitialcount();
                scheduleAt(simTime() + 1, new cMessage("Send"));
            }
            else {
                std::cout<<getSumoId();
                std::cerr<<"is tempCH"<<std::endl;
                setVehicleState(2);//set node to be tempCH
                startBroadcast(2);//tempCH broadcast packet
            }
            break;
        case 1:
            // SE not process
            break;
        case 2: {//tempCH
            std::map<std::string,Info> carInfo=getVehicleInfoofTempCluster();
            std::cerr<<simTime()<<"***";
            if (carInfo.size() > 0){
                std::cout<<"temp cluster member:";
                std::map<std::string,Info>::iterator it=carInfo.begin();
                for(;it!=carInfo.end();it++)
                    std::cout<<it->first<<"  ";
                std::cout<<std::endl;
            }else{
                std::cout<<"has no temp cluster members"<<std::endl;
                break;
            }
            if(getTempCHCount()>50) {
                sendToLTE(2);
                setTempCHCount(0);
                setVehicleState(1);
                setInitialcount(0);
                clearTempCluster();
                break;
            }
            else {
                addTempCHCount();
            }
            startBroadcast(2);
            scheduleAt(simTime() + 0.5, new cMessage("Send"));//USED FUL LTE
        }
            break;
        case 3: {//CM
            if (isgatewayNode()) { // CM is a GW node
                std::cout<<getSumoId()<<std::endl;
                startBroadcast(3);
                scheduleAt(simTime() + 3, new cMessage("Send"));//broadcast for other GWs
            }
        }
            break;
        case 4: {//CH
            insertCH_Id(getSumoId());// collect CH Id
            updateCH_Info(getSumoId(),simTime());//collect CH time
            std::map<std::string,Info> carInfo=getVehicleInfoOfCluster();
            std::cerr<<"CH is : "<<getSumoId()<<","<<"cluster member:";
            std::map<std::string,Info>::iterator it=carInfo.begin();
            for(;it!=carInfo.end();it++)
                std::cout<<it->first<<"  ";
            std::cout<<std::endl;
            if(getCHCount() > 20) {
                //package GWInfoToLte
                if ((this->infoGwToLte).size() > 0) {
                    sendToLTE(getVehicleState());
                }
                setCHCount(0);
            }
            else {
                addCHCount();
            }
            startBroadcast(4);
            if (int(simTime().dbl()) - this->timeGWUpload > 6 && this->timeGWUpload != 0) {//GW has no valid, CH need to select a new GW
                //CLEAR
                this->GwForCluster.clear();
                //find
                findTwoGW(vehicleInfoCH);
                //unicast to CM
                std::map<std::string,Info>::iterator it_forName =  GwForCluster.begin();
                startUnicastToGateWay(it_forName->first);
                this->hasFindGw = true;
                this->timeGWUpload = int(simTime().dbl());
            }
            if (clusterSize == vehicleInfoCH.size() && !(this->hasFindGw) && vehicleInfoCH.size() >= 7) {
                findTwoGW(vehicleInfoCH);
                //unicast to CM
                std::map<std::string,Info>::iterator it_forName =  GwForCluster.begin();
                startUnicastToGateWay(it_forName->first);
                this->hasFindGw = true;
                this->timeGWUpload = int(simTime().dbl());
            } else {
                clusterSize = vehicleInfoCH.size();
            }

            //test routing
            if (int(simTime().dbl()) > 139 && !(this->hasSelectedRoute) && this->getCurrentRoadId() == "1/3to1/2") {
                std::vector<Connectivity_Info> onePath;
                Coord destination;
                destination.x = 300;
                destination.y = 100;
                std::vector<std::string> targetIntersectionId = findTwoIntersection(destination);
                findRoutePath( onePath, 0, 0, "1/3to1/2", targetIntersectionId[0]);
                if (this->smallestDelayForRoute == routeDelayIniValue) {
                    findRoutePath( onePath, 0, 0, "1/3to1/2", targetIntersectionId[1]);
                }
                this->routeTable = routeForSelected[this->smallestDelayForRoute];
                this->route_destination = "flow1.3";
                startRouting();
                std::cout<<"OK"<<std::endl;
                this->hasSelectedRoute = true;
            }

            //end test routing
            scheduleAt(simTime() + 0.2, new cMessage("Send"));
        }
            break;
        default:
            break;
	    }
	}
    else {// other message
        HeterogeneousMessage *receiveMessage = dynamic_cast<HeterogeneousMessage *>(msg);
        std::cerr<<"receive #other# message----------#"<<std::endl;
        std::cout<<getVehicleState()<<"receive a msg from"<<receiveMessage->getMsgState()<<std::endl;
        overHead_clustering++;
        switch(getVehicleState()) {
            case 0:
                //ini
                switch(receiveMessage->getMsgState()) {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2: {//ini receive tempCH's packet
                        if(memberOfCluster(receiveMessage->getTempCHRoadID(),getCurrentRoadId())) {
                            std::string carId("node[" +getSumoId()+ "]");
                            if(findFromCHList(receiveMessage->getTempCHInfo(),carId)) {
                                std::cout<<"has included in list"<<std::endl;
                                setVehicleState(1);
                                startUnicast(receiveMessage,1);
                            }
                            else {
                                setVehicleState(1);
                                startUnicast(receiveMessage,1);
                            }
                        }
                        else {
                            scheduleAt(simTime() + 1, new cMessage("Send"));
                        }

                    }
                        break;
                    case 3:
                        break;
                    case 4: {//ini receive CH Message
                        std::string carId("node[" +getSumoId()+ "]");
                        if(findFromCHList(receiveMessage->getCHInfo(),carId)) {
                            setVehicleState(3);//set to be CM
                            startUnicast(receiveMessage,3);
                        }
                        else {
                            if(memberOfCluster(receiveMessage->getCHRoadID(),getCurrentRoadId())) {
                                if(getCurrentRoadId()==receiveMessage->getCurrentRoadId()) {//currentRoad must be same
                                    setVehicleState(1);//set to be SE
                                    startUnicast(receiveMessage,1);
                                    //record CH ID
                                }
                                else {
                                    std::cout<<"ini receive a CH,but it not the right one"<<std::endl;
                                    scheduleAt(simTime() + 1, new cMessage("Send"));
                                }
                            }
                        }
                    }
                    break;
                    case 5:
                        std::cerr<<"current state is ini,receive a Lte msg"<<std::endl;//need to change to be LTE
                        setVehicleState(4);//set to be CH
                        startBroadcast(4);
                        scheduleAt(simTime() + 0.1, new cMessage("Send"));
                        break;
                }
                    break;
            case 1:
                //SE
                switch(receiveMessage->getMsgState()) {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2: {//SE receive tempCH message
                        if(memberOfCluster(receiveMessage->getTempCHRoadID(),getCurrentRoadId())) {
                            std::string carId("node[" +getSumoId()+ "]");
                            if(findFromCHList(receiveMessage->getTempCHInfo(),carId)) {
                                //update
                                setVehicleState(1);
                                std::cout<<"has included in list"<<std::endl;
                                startUnicast(receiveMessage,getVehicleState());
                            }
                            else {
                                //update
                                setVehicleState(1);
                                startUnicast(receiveMessage,getVehicleState());
                            }
                        }
                    }
                        break;
                    case 3:
                        break;
                    case 4: {//SE receive CH msg
                        if(memberOfCluster(receiveMessage->getCHRoadID(),getCurrentRoadId())) {
                            if(getCurrentRoadId() == receiveMessage->getCurrentRoadId()) {//currentRoad must be same
                                setVehicleState(3);//set to be CM
                                setCHId(receiveMessage->getSourceAddress());
                                startUnicast(receiveMessage,getVehicleState());
                            }
                            else {
                                std::cout<<"SE receive a CH,but not the right one"<<std::endl;
                            }
                        }
                    }
                        break;
                    case 5: {//SE receive LTE msg
                        std::cerr<<"SE receive LTE Message"<<endl;
                        setVehicleState(4);//set to be CH
                        startBroadcast(4);
                        scheduleAt(simTime() + 0.01, new cMessage("Send"));
                    }
                        break;
                    default:
                        break;
                }
                break;
            case 2://tempCH
                switch(receiveMessage->getMsgState()) {
                    case 0://tempCH receive ini's packet
                        if(memberOfCluster(getTempClusterQueue(),receiveMessage->getCurrentRoadId())) {
                            std::string carId("node[" +getSumoId()+ "]");
                            if(findFromCHList(getVehicleInfoofTempCluster(),receiveMessage->getSourceAddress())) {
                                //update info
                                std::cout<<"has the node"<<std::endl;
                                Info tempInfo;
                                tempInfo.currentRoadId=receiveMessage->getCurrentRoadId();
                                tempInfo.nextRoadId=receiveMessage->getNodeNextRoadId();
                                tempInfo.pos=receiveMessage->getSenderPos();
                                tempInfo.junctionId=getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                                tempInfo.macAddr = receiveMessage->getMacAddr();
                                std::string temp(receiveMessage->getSourceAddress());
                                std::string destinationTemp("node[" +temp+ "]");
                                pushVehicleInfo(destinationTemp,tempInfo);
                            }
                            else {
                                //add the memeber
                                Info tempInfo;
                                tempInfo.currentRoadId=receiveMessage->getCurrentRoadId();
                                tempInfo.nextRoadId=receiveMessage->getNodeNextRoadId();
                                tempInfo.pos=receiveMessage->getSenderPos();
                                tempInfo.junctionId=getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                                tempInfo.macAddr = receiveMessage->getMacAddr();
                                std::string temp(receiveMessage->getSourceAddress());
                                std::string destinationTemp("node[" +temp+ "]");
                                pushVehicleInfo(destinationTemp,tempInfo);
                            }
                        }
//                        startBroadcast(2);
                        scheduleAt(simTime() + 1, new cMessage("Send"));
                        break;
                    case 1://tempCH receive SE's packet
                        if(memberOfCluster(getTempClusterQueue(),receiveMessage->getCurrentRoadId())) {
                            std::string carId("node[" +getSumoId()+ "]");
                            if(findFromCHList(getVehicleInfoofTempCluster(),receiveMessage->getSourceAddress())) {
                                std::cout<<"has the node"<<std::endl;
                                Info tempInfo;//update
                                tempInfo.currentRoadId=receiveMessage->getCurrentRoadId();
                                tempInfo.nextRoadId=receiveMessage->getNodeNextRoadId();
                                tempInfo.pos=receiveMessage->getSenderPos();
                                tempInfo.junctionId=getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                                tempInfo.macAddr = receiveMessage->getMacAddr();
                                std::string temp(receiveMessage->getSourceAddress());
                                std::string destinationTemp("node[" +temp+ "]");
                                pushVehicleInfo(destinationTemp,tempInfo);
                            }
                            else {
                                //add the memeber
                                Info tempInfo;
                                tempInfo.currentRoadId=receiveMessage->getCurrentRoadId();
                                tempInfo.nextRoadId=receiveMessage->getNodeNextRoadId();
                                tempInfo.pos=receiveMessage->getSenderPos();
                                tempInfo.junctionId=getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                                tempInfo.macAddr = receiveMessage->getMacAddr();
                                std::string temp(receiveMessage->getSourceAddress());
                                std::string destinationTemp("node[" +temp+ "]");
                                pushVehicleInfo(destinationTemp,tempInfo);
                            }
                        }
                        //scheduleAt(simTime() + 1, new cMessage("Send"));new change
                        break;
                    case 2://tempCH receive other tempCH
                        std::cerr<<"receive other tempCH"<<endl;
                        //scheduleAt(simTime() + 1, new cMessage("Send"));new change
                        break;
                    case 3://tempCH receive a CM message
                        break;
                    case 4: {//tempCH receive a CH message
                        if(memberOfCluster(receiveMessage->getTempCHRoadID(),getCurrentRoadId())) {
                            if(getCurrentRoadId()==receiveMessage->getCurrentRoadId()) {
                                setVehicleState(3);//set to be CM
                                setTempCHCount(0);
                                setCHId(receiveMessage->getSourceAddress());
                                clearTempCluster();
                                startUnicast(receiveMessage,getVehicleState());
                            }
                           // else
//                                scheduleAt(simTime() + 0.1, new cMessage("Send"));//new change
                        }
//                        else
//                            scheduleAt(simTime() + 0.1, new cMessage("Send"));new change
                    }
                        break;
                    case 5: {//tempCH receive LTE Message
                        std::cerr<<"tempCH receive LTE Message"<<endl;
                        std::cout<<"current Time:"<<simTime()<<"<->"<<"receive package time"<<"  "<<receiveMessage->getTimestamp()<<std::endl;
                        setVehicleState(4);//set to be CH
                        clearTempCluster();
                        startBroadcast(4);
                        scheduleAt(simTime() + 0.1, new cMessage("Send"));
                    }
                    default:
                        break;
                }
                break;
            case 3://CM
                switch(receiveMessage->getMsgState()) {
                    case 0://CM receive ini
                        break;
                    case 1://CM receive SE
                        break;
                    case 2://CM receive a tempCH
                        std::cout<<"CM receive a tempCH"<<std::endl;
                        break;
                    case 3: {//CM receive a CM
                        if (receiveMessage->getGatewayNode() && isgatewayNode() && receiveMessage->getUsedFor() == BuildConnection) {
                            std::cerr<<"GW receive a GW"<<std::endl;
                            //the GW is neighbor GW?
                            std::string roadIdOfGw(receiveMessage->getCurrentRoadId());
                            if(isNeighborGW( roadIdOfGw, getCurrentRoadId(), receiveMessage->getMsgState())) {// behind GW
                                //process
                                int delayTime = getTimeDelay(receiveMessage);
                                Connectivity_Info tempGwCOnnectivity;
                                tempGwCOnnectivity.connectivityValue = delayTime;
                                tempGwCOnnectivity.macAddrGW = receiveMessage->getMacAddr();
                                if (infoGw[getLastJunctionId(receiveMessage->getCurrentRoadId())].count(receiveMessage->getCurrentRoadId()) > 0) {
                                    infoGw[getLastJunctionId(receiveMessage->getCurrentRoadId())].erase(receiveMessage->getCurrentRoadId());
                                }
                                this->infoGw[getLastJunctionId(receiveMessage->getCurrentRoadId())].insert(std::make_pair(receiveMessage->getCurrentRoadId(), tempGwCOnnectivity));
                                std::cout<<receiveMessage->getSourceAddress()<<"==>"<<getSumoId()<<std::endl;
                            }
                         } else if (receiveMessage->getUsedFor() == BroadcastTopology) {
                             std::cerr<<"# CH receive a msg for topology from another CM /GW #"<<std::endl;
                             if (this->topologySet.count(receiveMessage->getMsgCode()) > 0 ) {
                                 break;
                             } else {
                                 this->topologySet.insert(receiveMessage->getMsgCode());
                                 this->infoGwToLte = receiveMessage->getInfoGWToLte();
                                 if (isgatewayNode()) { // CM is GW
                                     startBroadcastTopologyInfo(receiveMessage->getMsgCode());
                                 }
                             }
                         } else if (receiveMessage->getUsedFor() == RelayRoute) {
                             if (receiveMessage->getRouteTable().size() == 0) {
                                 std::string routeDestination(receiveMessage->getDestinationAddress());
                                 if (routeDestination.find(this->getSumoId()) != std::string::npos) {
                                     std::cerr<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^the routing packet has been received success!!"<<std::endl;
                                 } else {
                                     // broadcast to find
                                     startBroadcastToDestination(receiveMessage);
                                 }
                             } else {
                                 relayRoutingMsg(receiveMessage);
                             }
                         } else if (receiveMessage->getUsedFor() == BroadcastToDestination){
                             std::string tempName(receiveMessage->getDestinationAddress());
                             if (tempName.find(this->getSumoId()) != std::string::npos) {
                                 std::cerr<<"successfully routing"<<std::endl;
                             } else {
                                 std::cout<<"not the destination"<<std::endl;
                             }
                         } else {}

                    }
                        break;
                    case 4: {//CM receive a CH
                        std::string carId("node[" +getSumoId()+ "]");
                        if(carId == "node[flow3.3]" && this->isgatewayNode()) {
                            std::cout<<"aaaa"<<std::endl;
                        }
                        if (receiveMessage->getUsedFor() == BuildConnection && this->isgatewayNode()) {
                            std::string roadIdOfGw(receiveMessage->getCurrentRoadId());
                            std::cout<<receiveMessage->getSourceAddress()<<"==>"<<getSumoId()<<std::endl;
                            std::string carIdForDecide("node[" +getSumoId()+ "]");
//                            if (this->CHId != receiveMessage->getCHId() && (isNeighborGW(roadIdOfGw, getCurrentRoadId(), receiveMessage->getMsgState()))) {
                            if (!findFromCHList(receiveMessage->getCHInfo(),carIdForDecide) && (isNeighborGW(roadIdOfGw, getCurrentRoadId(), receiveMessage->getMsgState()))) {
                                //process
                                std::string nameSou(receiveMessage->getSourceAddress());
                                GWTOGWs.insert(nameSou+"==>"+getSumoId());
                                int delayTime = getTimeDelay(receiveMessage);
                                Connectivity_Info tempGwCOnnectivity;
                                tempGwCOnnectivity.connectivityValue = delayTime;
                                tempGwCOnnectivity.macAddrGW = receiveMessage->getMacAddr();
                                if (this->infoGw[getCurrentJunctionId(receiveMessage->getCurrentRoadId())].count(receiveMessage->getCurrentRoadId()) > 0) {
                                    this->infoGw[getCurrentJunctionId(receiveMessage->getCurrentRoadId())].erase(receiveMessage->getCurrentRoadId());
                                }
                                this->infoGw[getCurrentJunctionId(receiveMessage->getCurrentRoadId())].insert(std::make_pair(receiveMessage->getCurrentRoadId(), tempGwCOnnectivity));
                                std::cout<<receiveMessage->getSourceAddress()<<"==>"<<getSumoId()<<std::endl;
                            }
                         } else if (receiveMessage->getUsedFor() == BroadcastTopology) {
                             std::cerr<<"# CH receive a msg for topology from another CM /GW #"<<std::endl;
                             if ((this->topologySet.count(receiveMessage->getMsgCode()) > 0 )) {

                             } else {
                                 this->infoGwToLte = receiveMessage->getInfoGWToLte();
                                 if (isgatewayNode()) { // CM is GW
                                     this->topologySet.insert(receiveMessage->getMsgCode());
                                     startBroadcastTopologyInfo(receiveMessage->getMsgCode());
                                 }
                             }
                         } else if (receiveMessage->getUsedFor() == RelayRoute) {
                             if (receiveMessage->getRouteTable().size() == 0) {
                                 std::string routeDestination(receiveMessage->getDestinationAddress());
                                 if (routeDestination.find(this->getSumoId()) != std::string::npos) {
                                     std::cerr<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^the routing packet has been received success!!"<<std::endl;
                                 } else {
                                     // broadcast to find
                                     startBroadcastToDestination(receiveMessage);
                                 }
                             } else {
                                 relayRoutingMsg(receiveMessage);
                             }
                         } else if(receiveMessage->getUsedFor() == ToBeGw) {
                             this->setGateWayNode(true);
                             scheduleAt(simTime() + 0.1, new cMessage("Send"));
                         } else if (receiveMessage->getUsedFor() == BroadcastToDestination){
                             std::string tempName(receiveMessage->getDestinationAddress());
                             if (tempName.find(this->getSumoId()) != std::string::npos) {
                                 std::cerr<<"successfully routing"<<std::endl;
                             } else {
                                 std::cout<<"not the destination"<<std::endl;
                             }
                         } else {}

                        if(findFromCHList(receiveMessage->getCHInfo(),carId)) {//whether the car is a member of cluster
                            std::string receiveCHId(receiveMessage->getSourceAddress());
                            if(memberOfCluster(receiveMessage->getCHRoadID(),getCurrentRoadId()) && (receiveCHId == getCHId())) {//whether can be a member of cluster
                                if(receiveMessage->getDeparture()) {
                                    setCHId(receiveMessage->getSourceAddress());
                                    setVehicleState(1);
                                }
                                else {
                                    if(receiveMessage->getCH_Speed() > 0 || ((receiveMessage->getCHInfo())[carId].currentRoadId != getCurrentRoadId())) {
                                        std::string CH_roadId(receiveMessage->getCurrentRoadId());
                                        if(CH_roadId == getCurrentRoadId()) {
                                            if(sameForUpdate == 0 || ((receiveMessage->getCHInfo())[carId].currentRoadId != getCurrentRoadId())) {
                                                //update CM info
                                                setVehicleState(3);
                                                startUnicast(receiveMessage,3);
                                                startUnicast(receiveMessage,3);
                                                setCHId(receiveMessage->getSourceAddress());
                                                recordMacAddr_CH(receiveMessage->getMacAddr());
                                                sameForUpdate++;
                                            }
                                            else {
                                                std::cerr<<"CM don't have to update data,because CH and CM are in same road"<<std::endl;
                                            }
                                        }
                                        else {
                                            //has to update CM info
                                            sameForUpdate = 0;
                                            setVehicleState(3);
                                            startUnicast(receiveMessage,3);
                                            setCHId(receiveMessage->getSourceAddress());
                                            recordMacAddr_CH(receiveMessage->getMacAddr());
                                        }
                                    }
                                    else {
                                        std::cerr<<"CM receive A CH,but CH is static ,so don't update"<<std::endl;
                                    }
                                }
                            }
                            else{
                                //departure from the cluster
                                std::cerr<<getSumoId()<<"start departure"<<std::endl;
                                setVehicleState(6);
                                startUnicast(receiveMessage,6);
                                setVehicleState(0);//state is ini
                                setInitialcount(0);
                                setCHId("");
                                scheduleAt(simTime() + 0.1, new cMessage("Send"));
                            }
                        }
                        else {
                            std::string receiveCHId(receiveMessage->getSourceAddress());
                            if(memberOfCluster(receiveMessage->getCHRoadID(),getCurrentRoadId())&&(receiveCHId==getCHId())) {//whether can be a member of cluster
                                //update CM info
                                setVehicleState(3);
                                startUnicast(receiveMessage,3);
                                startUnicast(receiveMessage,3);
                            }
                            else { //#CM is not in this cluster,but may for GW info
                                std::cout<<"#"<<getSumoId()<<"# CM is not in this cluster"<<std::endl;
                            }
                        }
                    }
                        break;
                    case 5: {//CM receive a LTE
                        /*remove from last CH*/
                        std::cerr<<getSumoId()<<"start departure"<<std::endl;
                        startUnicastDeparture(getMacAddr_CH(),6,getCHId());
                        //set to be CH
                        setVehicleState(4);
                        setCHId("");
                        startBroadcast(4);
                        scheduleAt(simTime() + 0.1, new cMessage("Send"));
                    }
                        break;
                    default:
                        break;
                }
                break;
            case 4://CH
                switch(receiveMessage->getMsgState()) {
                    case 0: {//CH receive a Ini
                        if(memberOfCluster(getClusterQueue(),receiveMessage->getCurrentRoadId())) {
                            if(receiveMessage->getCurrentRoadId() == getCurrentRoadId()){
                                Info tempInfo;
                                tempInfo.currentRoadId = receiveMessage->getCurrentRoadId();
                                tempInfo.nextRoadId = receiveMessage->getNodeNextRoadId();
                                tempInfo.pos = receiveMessage->getSenderPos();
                                tempInfo.junctionId = getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                                tempInfo.macAddr = receiveMessage->getMacAddr();
                                std::string temp(receiveMessage->getSourceAddress());
                                std::string destinationTemp("node[" +temp+ "]");
                                pushVehicleInfoCH(destinationTemp,tempInfo);
                            }
                            else{
                                std::cout<<"CH receiive a ini ,but it's not the right one"<<std::endl;
                            }
                        }
                    }
                        break;
                    case 1://CH receive a SE
                    {
                        if(memberOfCluster(getClusterQueue(),receiveMessage->getCurrentRoadId())) {
                            Info tempInfo;
                            tempInfo.currentRoadId=receiveMessage->getCurrentRoadId();
                            tempInfo.nextRoadId=receiveMessage->getNodeNextRoadId();
                            tempInfo.pos=receiveMessage->getSenderPos();
                            tempInfo.junctionId=getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                            tempInfo.macAddr = receiveMessage->getMacAddr();
                            std::string temp(receiveMessage->getSourceAddress());
                            std::string destinationTemp("node[" +temp+ "]");
                            pushVehicleInfoCH(destinationTemp,tempInfo);
                            //scheduleAt(simTime() + 1, new cMessage("Send"));
                        }
                    }
                        break;
                    case 2://CH receive a tempCH
                    {
                        std::cout<<"CH receive a tempCH"<<std::endl;
                    }
                        break;
                    case 3://CH receive a CM
                    {
                        std::string roadIdOfGw(receiveMessage->getCurrentRoadId());
                        std::string sForDecide(receiveMessage->getSourceAddress());
                        std::string carIdForDecide("node[" +sForDecide+ "]");
                        if (receiveMessage->getGatewayNode() && receiveMessage->getUsedFor() == BuildConnection ){
                            //process
                            std::cout<<receiveMessage->getSourceAddress()<<"==>"<<getSumoId()<<std::endl;
                            if ( !findFromCHList(getVehicleInfoOfCluster(),carIdForDecide) && (isNeighborGW(roadIdOfGw, getCurrentRoadId(), receiveMessage->getMsgState()))) {// GW not a member of cluster
                                std::cout<<receiveMessage->getSourceAddress()<<"==>"<<getSumoId()<<std::endl;
                                std::string nameSou(receiveMessage->getSourceAddress());
                                GWTOGWs.insert(nameSou+"==>"+getSumoId());
                                int delayTime = getTimeDelay(receiveMessage);
                                Connectivity_Info tempGwConnectivity;
                                tempGwConnectivity.connectivityValue = delayTime;
                                tempGwConnectivity.macAddrGW = receiveMessage->getMacAddr();
                                if (tempGwConnectivity.connectivityValue > 0) {
                                    if (this->infoGwToLte[getCurrentRoadId()][getCurrentJunctionId(getCurrentRoadId())].count(receiveMessage->getCurrentRoadId()) > 0) {
                                        this->infoGwToLte[getCurrentRoadId()][getCurrentJunctionId(getCurrentRoadId())].erase(receiveMessage->getCurrentRoadId());
                                    }
                                    this->infoGwToLte[getCurrentRoadId()][getCurrentJunctionId(getCurrentRoadId())].insert(std::make_pair(receiveMessage->getCurrentRoadId(), tempGwConnectivity));
                                    this->hasUpdateGwInfo = true;
                                }
                            } else if (findFromCHList(getVehicleInfoOfCluster(),carIdForDecide)){
                                this->timeGWUpload = int(SimTime().dbl());// for record upload time
                                std::cout<<receiveMessage->getSourceAddress()<<"==>"<<getSumoId()<<std::endl;
                                std::string nameSou(receiveMessage->getSourceAddress());
                                GWTOGWs.insert(nameSou+"==>"+getSumoId());
                                int delayTime = getTimeDelay(receiveMessage);
                                Connectivity_Info tempGwConnectivity;
                                tempGwConnectivity.connectivityValue = delayTime;
                                tempGwConnectivity.macAddrGW = receiveMessage->getMacAddr();
                                std::map<std::string, std::map<std::string, Connectivity_Info>> tempInner = receiveMessage->getInfoGw();
                                if (tempInner.size() > 0 && tempInner[getLastJunctionId(receiveMessage->getCurrentRoadId())].size() > 0) {
                                    if (this->infoGwToLte[getCurrentRoadId()].count(getLastJunctionId(receiveMessage->getCurrentRoadId())) > 0) {
                                        this->infoGwToLte[getCurrentRoadId()].erase(getLastJunctionId(receiveMessage->getCurrentRoadId()));
                                    }
                                    this->infoGwToLte[getCurrentRoadId()].insert(std::make_pair(getLastJunctionId(receiveMessage->getCurrentRoadId()), tempInner[getLastJunctionId(receiveMessage->getCurrentRoadId())]));
                                    this->hasUpdateGwInfo = true;
                                }
                            }
                        } else if (receiveMessage->getUsedFor() == BroadcastTopology) {
                            std::cerr<<"# CH receive a msg for topology from another CM /GW #"<<std::endl;
                            if (this->topologySet.count(receiveMessage->getMsgCode()) > 0) {

                            } else {
                                this->topologySet.insert(receiveMessage->getMsgCode());
                                this->infoGwToLte = receiveMessage->getInfoGWToLte();
                                startBroadcastTopologyInfo(receiveMessage->getMsgCode());
                            }
                        } else if (receiveMessage->getUsedFor() == RelayRoute) {
                            if (receiveMessage->getRouteTable().size() == 0) {
                                std::string routeDestination(receiveMessage->getDestinationAddress());
                                if (routeDestination.find(this->getSumoId()) != std::string::npos) {
                                    std::cerr<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^the routing packet has been received success!!"<<std::endl;
                                } else if (findFromCHList(getVehicleInfoOfCluster(),carIdForDecide)) {
                                    startUnicastToDestination(getVehicleInfoOfCluster()[routeDestination].macAddr, receiveMessage->getDestinationAddress());
                                } else if (findFromCHList(getVehicleInfoOfCluster(),routeDestination)) {
                                    startUnicastToDestination(getVehicleInfoOfCluster()[routeDestination].macAddr, receiveMessage->getDestinationAddress());
                                } else {
                                    // broadcast to find
                                    startBroadcastToDestination(receiveMessage);
                                }
                            } else {
                                relayRoutingMsg(receiveMessage);
                            }
                        } else if (receiveMessage->getUsedFor() == BroadcastToDestination){
                            std::string tempName(receiveMessage->getDestinationAddress());
                            if (tempName.find(this->getSumoId()) != std::string::npos) {
                                std::cerr<<"successfully routing"<<std::endl;
                            } else {
                                std::cout<<"not the destination"<<std::endl;
                            }
                        }
                        std::string s(receiveMessage->getSourceAddress());
                        std::string carId("node[" +s+ "]");
                        std::cerr<<"CH current road id"<<getCurrentRoadId()<<std::endl;
                        if(findFromCHList(getVehicleInfoOfCluster(),carId)) {//whether the car is a member of cluster
                            if(receiveMessage->getDeparture()){
                                //remove the car from CHlist
                                removeVehicleInfoCH(carId);
                            }
                            else{
                                if(memberOfCluster(getClusterQueue(),receiveMessage->getCurrentRoadId())){
                                   //update info
                                   Info tempInfo;
                                   tempInfo.currentRoadId = receiveMessage->getCurrentRoadId();
                                   tempInfo.nextRoadId = receiveMessage->getNodeNextRoadId();
                                   tempInfo.pos = receiveMessage->getSenderPos();
                                   tempInfo.junctionId = getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                                   tempInfo.macAddr = receiveMessage->getMacAddr();
                                   std::string temp(receiveMessage->getSourceAddress());
                                   std::string destinationTemp("node[" +temp+ "]");
                                   pushVehicleInfoCH(destinationTemp,tempInfo);
                                   //scheduleAt(simTime() + 1, new cMessage("Send"));
                                }
                                else{
                                    std::cerr<<"a CM message is not cluster roadID"<<std::endl;
                                }
                            }
                        }
                        else{
                            std::cout<<"CH receive a CM.and CM is not a member of cluster"<<std::endl;
                            std::string temp1=getSumoId();
                            std::string temp2(receiveMessage->getCHId());
                            if(receiveMessage->getCurrentRoadId() == getCurrentRoadId()&& temp1 == temp2){
                               //update info
                               Info tempInfo;
                               tempInfo.currentRoadId=receiveMessage->getCurrentRoadId();
                               tempInfo.nextRoadId=receiveMessage->getNodeNextRoadId();
                               tempInfo.pos=receiveMessage->getSenderPos();
                               tempInfo.junctionId=getCurrentJunctionId(receiveMessage->getCurrentRoadId());
                               tempInfo.macAddr = receiveMessage->getMacAddr();
                               std::string temp(receiveMessage->getSourceAddress());
                               std::string destinationTemp("node[" +temp+ "]");
                               pushVehicleInfoCH(destinationTemp,tempInfo);
                            }
                        }
                    }
                        break;
                    case 4: {//CH receive a CH
                        std::cerr<<"CH receive a CH"<<std::endl;
                        if(getCurrentRoadId() == receiveMessage->getCurrentRoadId()){
                            //merge
//                            if(getVehicleInfoOfCluster().size()<=receiveMessage->getCHInfo().size())
//                            {
//                                //set self to be CM
//                                setVehicleState(3);//set to be CM
//                                setCHId(receiveMessage->getSourceAddress());
//                                std::string temp(receiveMessage->getSourceAddress());
//                                startBroadcastDismiss(4,temp);
//                                startUnicast(receiveMessage,getVehicleState());
//                            }
                            if(getVehicleInfoOfCluster().size()<2){
                                //set self to be CM
                                setVehicleState(3);//set to be CM
                                setCHId(receiveMessage->getSourceAddress());
//                                std::string temp(receiveMessage->getSourceAddress());
//                                startBroadcastDismiss(4,temp);
                                startUnicast(receiveMessage,getVehicleState());
                            }
                        }
                        std::string roadIdOfGw1(receiveMessage->getCurrentRoadId());
                        if (receiveMessage->getGatewayNode() && receiveMessage->getUsedFor() == BuildConnection
                                && (isNeighborGW(roadIdOfGw1, getCurrentRoadId(), receiveMessage->getMsgState()))) {
                            //process
                            std::cout<<receiveMessage->getSourceAddress()<<"==>"<<getSumoId()<<std::endl;
                            std::string nameSou(receiveMessage->getSourceAddress());
                            GWTOGWs.insert(nameSou+"==>"+getSumoId());
                            int delayTime = getTimeDelay(receiveMessage);
                            Connectivity_Info tempGwCOnnectivity;
                            tempGwCOnnectivity.connectivityValue = delayTime;
                            tempGwCOnnectivity.macAddrGW = receiveMessage->getMacAddr();
                            this->infoGwToLte[getCurrentRoadId()][getCurrentJunctionId(receiveMessage->getCurrentRoadId())][receiveMessage->getCurrentRoadId()] = tempGwCOnnectivity;
                            this->hasUpdateGwInfo = true;
                        } else if (receiveMessage->getUsedFor() == BroadcastTopology) {
                            std::cerr<<"# CH receive a msg for topology from another CH #"<<std::endl;
                            if (this->topologySet.count(receiveMessage->getMsgCode()) > 0) {
                                break;
                            } else {
                                this->topologySet.insert(receiveMessage->getMsgCode());
                                this->infoGwToLte = receiveMessage->getInfoGWToLte();
                                startBroadcastTopologyInfo(receiveMessage->getMsgCode());
                            }
                        } else if (receiveMessage->getUsedFor() == RelayRoute) {
                            if (receiveMessage->getRouteTable().size() == 0) {
                                std::string routeDestination(receiveMessage->getDestinationAddress());
                                if (routeDestination.find(this->getSumoId()) != std::string::npos) {
                                    std::cerr<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^the routing packet has been received success!!"<<std::endl;
                                } else {
                                    //broadcast
                                    startBroadcastToDestination(receiveMessage);
                                }
                            } else {
                                relayRoutingMsg(receiveMessage);
                            }
                        } else if (receiveMessage->getUsedFor() == BroadcastToDestination){
                            std::string tempName(receiveMessage->getDestinationAddress());
                            if (tempName.find(this->getSumoId()) != std::string::npos) {
                                std::cerr<<"successfully routing"<<std::endl;
                            } else if (findFromCHList(getVehicleInfoOfCluster(),tempName)) {
                                startUnicastToDestination(getVehicleInfoOfCluster()[tempName].macAddr, tempName);
                            } else {
                                std::cout<<"not the destination"<<std::endl;
                            }
                        }
                }
                        break;
                    case 5: {//CH receive a LTE
                        std::cerr<<"# CH receive a msg for topology from LTE #"<<std::endl;

                        if ( this->topologySet.count(receiveMessage->getMsgCode()) > 0) {
                            break;
                        } else {
                            this->topologySet.insert(receiveMessage->getMsgCode());
                            this->infoGwToLte = receiveMessage->getInfoGWToLte();
                            startBroadcastTopologyInfo(receiveMessage->getMsgCode());
                        }
                    }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
	}
    updateRecordLastTime();
}

//follow codes are added by zy

Coord SimpleApp::getCurrentPos(const simtime_t& t)
{
    return mobility->getPositionAt(t);
}

std::string SimpleApp::getCurrentRoadId()
{
    int temp;
    std::string roadId;
    temp=mobility->getRoadId().find(":",0);
    if(temp!=-1 )
    {
        roadId=lastRoadId;
    }
    else
    {
        roadId=mobility->getRoadId();
        lastRoadId=roadId;
    }
    return roadId;
}

std::list<std::string> SimpleApp::getCurrentRoute()
{
    return mobility->getVehicleCommandInterface()->getPlannedRoadIds();
}

std::string SimpleApp::getCurrentJunctionId(std::string roadId)
{
    std::string tempStr;
    tempStr=roadId.substr(5,7);
    return tempStr;
}

std::string SimpleApp::getLastJunctionId(std::string roadId) {
    std::string tempStr;
    tempStr=roadId.substr(0,3);
    return tempStr;
}

std::string SimpleApp::getNextRoadId(std::list<std::string> route,std::string roadId)
{
    std::string nextRoadId="no next";
    std::list<std::string>::iterator it_l1 = route.begin();
    while(it_l1!=route.end())
    {
        if(*it_l1==roadId)
        {
            if(++it_l1==route.end())
            {
                nextRoadId="no next";
                break;
            }
            else
            {
                nextRoadId=*it_l1;
                break;
            }
        }
        it_l1++;
    }
    return nextRoadId;
}

void SimpleApp::addInitialcount()
{
    initialPkt++;
}
int SimpleApp::getInitialcount()
{
    return initialPkt;
}

void SimpleApp::setInitialcount(int value)
{
    initialPkt=value;
}

void SimpleApp::addTempCHCount()
{
    tempCHCount++;
}
void SimpleApp::setTempCHCount(int value)
{
    tempCHCount=value;
}
int SimpleApp::getTempCHCount()
{
    return tempCHCount;
}

void SimpleApp::addCHCount()
{
    CHCount++;
}

void SimpleApp::setCHCount(int value)
{
    CHCount=value;
}

int SimpleApp::getCHCount()
{
    return CHCount;
}

void SimpleApp::addCMcount()
{
    CMcount++;
}
void SimpleApp::setCMcount(int value)
{
    CMcount=value;
}
int SimpleApp::getCMcount()
{
    return CMcount;
}


void SimpleApp::startBroadcast(int msgState)//orginal
{
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("DSRC Broadcast Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string destination("node[" + it->first + "]");
    sendMessage->setDestinationAddress(destination.c_str());//destination
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setMsgState(msgState);//message state
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    sendMessage->setSendingTime(simTime());
    sendMessage->setIsBroadcast(true);
    if(msgState == 2)//tempCH broadcast
    {
        Info tempInfo;
        tempInfo.currentRoadId = getCurrentRoadId();
        tempInfo.nextRoadId = getNextRoadId(getCurrentRoute(),getCurrentRoadId());
        tempInfo.pos = getCurrentPos(SimTime());
        tempInfo.junctionId = getCurrentJunctionId(getCurrentRoadId());
        std::string destinationTemp("node[" + getSumoId() + "]");
        pushVehicleInfo(destinationTemp,tempInfo);
        sendMessage->setTempCHInfo(getVehicleInfoofTempCluster());
        setTempClusterQueue(getCurrentRoadId());
        sendMessage->setTempCHRoadID(getTempClusterQueue());
        sendMessage->setCH_Speed(getSpeed());
    }
    //CH broadcast
    if(msgState == 4)
    {
        //update CH info
        Info tempInfo;
        tempInfo.currentRoadId = getCurrentRoadId();
        tempInfo.nextRoadId = getNextRoadId(getCurrentRoute(),getCurrentRoadId());
        tempInfo.pos = getCurrentPos(simTime());
        tempInfo.junctionId = getCurrentJunctionId(getCurrentRoadId());
        std::string destinationTemp("node[" + getSumoId() + "]");
        pushVehicleInfoCH(destinationTemp,tempInfo);
        setClusterQueue(getCurrentRoadId());
        sendMessage->setCHRoadID(getClusterQueue());
        sendMessage->setCHInfo(getVehicleInfoOfCluster());
        sendMessage->setCH_Speed(getSpeed());
        if (int(simTime().dbl()) - this->lastTimeBuildConnection > 4) {
            this->lastTimeBuildConnection = int(simTime().dbl());
            sendMessage->setInfoGw(infoGw);
            sendMessage->setGatewayNode(true);
            sendMessage->setUsedFor(BuildConnection);
        }

    }

    if (isgatewayNode()) {// for CM is a GWs
        //update GWs info
        sendMessage->setInfoGw(infoGw);
        sendMessage->setGatewayNode(true);
        sendMessage->setUsedFor(BuildConnection);
    }
    countBroadcastNum();
    send(sendMessage, toDecisionMaker);
    std::cout<<"+++++++++++++++++++++++++++++++++++"<<std::endl;
}

void SimpleApp::startBroadcastDismiss(int msgState,std::string connectCHId)
{
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("DSRC Broadcast Dismiss Message");
    sendMessage->setByteLength(10);
    std::string destination("node[" + connectCHId + "]");
    sendMessage->setDestinationAddress(destination.c_str());//destination
    sendMessage->setSourceAddress(connectCHId.c_str());//source
    sendMessage->setMsgState(msgState);//message state
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    sendMessage->setSendingTime(simTime());
    sendMessage->setIsBroadcast(true);
    //CH broadcast Dismiss
    //updata CH
    Info tempInfo;
    tempInfo.currentRoadId=getCurrentRoadId();
    tempInfo.nextRoadId=getNextRoadId(getCurrentRoute(),getCurrentRoadId());
    tempInfo.pos=getCurrentPos(simTime());
    tempInfo.junctionId=getCurrentJunctionId(getCurrentRoadId());
    std::string destinationTemp("node[" + getSumoId() + "]");
    pushVehicleInfoCH(destinationTemp,tempInfo);

    sendMessage->setDeparture(true);
    setClusterQueue(getCurrentRoadId());
    sendMessage->setCHRoadID(getClusterQueue());
    sendMessage->setCHInfo(getVehicleInfoOfCluster());
    sendMessage->setCH_Speed(getSpeed());
    countBroadcastNum();
    send(sendMessage, toDecisionMaker);
    std::cout<<"+++++++++++++++++++++++++++++++++++"<<std::endl;
}

void SimpleApp::sendToLTE(int msgState)
{
    std::cout << "[" << sumoId << ", " << simTime() <<  "] Sending message to LTE server" << std::endl;
    HeterogeneousMessage* serverMessage = new HeterogeneousMessage();
    serverMessage->setName("Server Message Test");
    serverMessage->setByteLength(10);
    serverMessage->setNetworkType(LTE);
    serverMessage->setDestinationAddress("server");
    serverMessage->setSourceAddress(sumoId.c_str());
    if(msgState == 2)
        serverMessage->setTempCHInfo(getVehicleInfoofTempCluster());
    else {
        serverMessage->setCHInfo(getVehicleInfoOfCluster());
        serverMessage->setInfoGWToLte(this->infoGwToLte);
    }
    serverMessage->setMsgState(msgState);
    send(serverMessage, toDecisionMaker);
}

void SimpleApp::startUnicast(HeterogeneousMessage *receiveMessage,int msgState)
{
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("unicast Test Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string temp(receiveMessage->getSourceAddress());
    std::string destination("node[" +temp+ "]");
    sendMessage->setDestinationAddress(destination.c_str());
    sendMessage->setMsgState(msgState);//message state
    sendMessage->setSendingTime(simTime());
    if(msgState==3)
    {
        sendMessage->setCHId(receiveMessage->getSourceAddress());
    }
    if(msgState==6)//departure msg
    {
        sendMessage->setDeparture(true);
        sendMessage->setMsgState(3);//message state
    }
    sendMessage->setIsBroadcast(false);//unicast
    sendMessage->setMacAddr(receiveMessage->getMacAddr());//set mac address
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    send(sendMessage, toDecisionMaker);
    unicastNum++;
    //std::cout<<"==============================>"<<std::endl;
}

void SimpleApp::startUnicastByGateWay(HeterogeneousMessage *receiveMessage,int msgState) {
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("unicast Test Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string temp(receiveMessage->getSourceAddress());
    std::string destination("node[" +CHId+ "]");
    sendMessage->setDestinationAddress(destination.c_str());
    sendMessage->setMsgState(msgState);//message state
    sendMessage->setSendingTime(simTime());
    if(msgState==3)
    {
        sendMessage->setCHId(receiveMessage->getSourceAddress());
    }
    sendMessage->setIsBroadcast(false);//unicast
    sendMessage->setMacAddr(macAddr_CH);//set mac address
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    send(sendMessage, toDecisionMaker);
    unicastNum++;
    std::cerr<<"==========gateway message unicast====================>"<<std::endl;
}

void SimpleApp::startUnicastToGateWay(std::string GWID) {
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("unicast Test Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string temp = getSumoId();
    std::string destination("node[" +CHId+ "]");
    sendMessage->setDestinationAddress(destination.c_str());
    sendMessage->setMsgState(getVehicleState());//message state
    sendMessage->setIsBroadcast(false);//unicast
    sendMessage->setMacAddr(GwForCluster[GWID].macAddr);//set mac address
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    sendMessage->setUsedFor(ToBeGw);
    send(sendMessage, toDecisionMaker);
    sendMessage->setSendingTime(simTime());
    unicastNum++;
    std::cerr<<"==========CH start to info gateway node====================>"<<std::endl;
}

void SimpleApp::startUnicastToDestination(LAddress::L2Type macAddr, std::string nodeId) {
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("unicast Test Message");
    sendMessage->setByteLength(10);
    sendMessage->setDestinationAddress(nodeId.c_str());
    sendMessage->setMsgState(this->getVehicleState());//message state
    sendMessage->setSendingTime(simTime());
    sendMessage->setIsBroadcast(false);//unicast
    sendMessage->setMacAddr(macAddr);//set mac address
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    unicastNum++;
    send(sendMessage, toDecisionMaker);
}


void SimpleApp::startUnicastDeparture(LAddress::L2Type macAddr,int msgState,std::string Id_CH)
{
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("unicast Test Message");
    sendMessage->setByteLength(10);
    std::string temp(Id_CH);
    std::string destination("node[" +temp+ "]");
    sendMessage->setDestinationAddress(destination.c_str());
    sendMessage->setMsgState(msgState);//message state
    sendMessage->setSendingTime(simTime());
    if(msgState==6)//departure msg
    {
        sendMessage->setDeparture(true);
        sendMessage->setMsgState(3);//message state
    }
    sendMessage->setIsBroadcast(false);//unicast
    sendMessage->setMacAddr(macAddr);//set mac address
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    unicastNum++;
    send(sendMessage, toDecisionMaker);
}


void SimpleApp::setVehicleState(int value)
{
    nodeState=value;
    carState.insert(value);
}

int SimpleApp::getVehicleState()
{
    return nodeState;
}

void SimpleApp::findTwoGW(std::map<std::string,Info> tempInfo) {
    /*find self car*/
    std::string selfCar ("node[" +getSumoId()+ "]");
    Info selfInfo;
    selfInfo.pos.x = 0;
    selfInfo.pos.y = 0;
    if (tempInfo.count(selfCar) > 0) {
        selfInfo = tempInfo[selfCar];
    } else {
        std::cerr<<"can't find the key of CH in vehicleInfoCH,,the key is==="<<selfCar<<std::endl;
    }
    /* filter for same path*/
    std::map<std::string,Info> tempInfoForFind;
    std::map<std::string,Info>::iterator it_filter;
    it_filter = tempInfo.begin();
    while (it_filter != tempInfo.end()) {
        if (it_filter->second.nextRoadId == selfInfo.nextRoadId) {
            tempInfoForFind.insert(std::make_pair(it_filter->first, it_filter->second));
        }
        it_filter++;
    }
    /*find the GWs from the filter set*/
    Coord selfCoorD = selfInfo.pos;
    std::string currentJunctionId = getCurrentJunctionId(getCurrentRoadId());
    Coord currentJunctionCoorD;
    currentJunctionCoorD.x = 0;
    currentJunctionCoorD.y = 0;
    if (electricMap.count(currentJunctionId) > 0) {
        currentJunctionCoorD = electricMap[currentJunctionId];
    } else {
        std::cerr<<"can't find the key of juectionId in electricMap,the key is==="<<currentJunctionId<<std::endl;
    }
    Coord selfToJunction;
    selfToJunction.x = currentJunctionCoorD.x - selfCoorD.x;
    selfToJunction.y = currentJunctionCoorD.y - selfCoorD.y;
    Coord temp;
    std::map<std::string,Info>::iterator it;
    it = tempInfoForFind.begin();
    std::map<int, std::string> result;
    int large = 0;
    int small = 0;
    while(it != tempInfoForFind.end()) {
        if (it->first != selfCar) {
            temp.x = (it->second).pos.x - selfCoorD.x;
            temp.y = (it->second).pos.y - selfCoorD.y;
            int tempResult = temp.x * selfToJunction.x + temp.y * selfToJunction.y;
            if(tempResult > large) {
                large = tempResult;
                result.insert(std::make_pair(tempResult,it->first));
            } else if(tempResult < small) {
                small = tempResult;
                result.insert(std::make_pair(tempResult,it->first));
            }
        }
        it ++;
    }
    /*package GW information*/
//    GwForCluster.insert(std::make_pair(result[large],tempInfo[result[large]]));
    GwForCluster.insert(std::make_pair(result[small],tempInfo[result[small]]));
}

double SimpleApp::getAcceleration() {
    double timeDif = simTime().dbl() - (this->lastUpdateTime).dbl();
    double speedDif = getSpeed() - this->lastSpeed;
    return speedDif/timeDif;
}

double SimpleApp::getEffictiveTime() {
    double acceleration = this->getAcceleration();
    //maybe need to divide into several condition
    //for traffic light
    int32_t  phaseDuration = ((mobility->getCommandInterface())->trafficlight(getCurrentJunctionId(getCurrentRoadId()))).getDataFromTLFroString();
    int32_t  phaseTrafficLight = ((mobility->getCommandInterface())->trafficlight(getCurrentJunctionId(getCurrentRoadId()))).getDataFromTL();
    double distanceToIntersection = getDistanceToIntersection();
    int time;
    if (int(distanceToIntersection - 30) > 0) { // enter intersection range
        if (int(acceleration) == 0) {
            time = int(phaseDuration);
        } else if (int(acceleration) > 0) {
            time =  fabs((this->meanSpeed - getSpeed()) / acceleration)+(distanceToIntersection - fabs((this->meanSpeed - getSpeed()) / (acceleration * 2))) / this->meanSpeed;
        } else {
            time = phaseDuration;
        }
    } else { // not enter intersetion range
        if (int(acceleration) == 0) {
            time = int(phaseDuration);
        } else if (int(acceleration) > 0) {
            time =  fabs((this->meanSpeed - getSpeed()) / acceleration)+(distanceToIntersection - fabs((this->meanSpeed - getSpeed()) / (acceleration * 2))) / this->meanSpeed;
        } else {
            int temp = fabs((this->meanSpeed - getSpeed()) / acceleration)+(distanceToIntersection - fabs((this->meanSpeed - getSpeed()) / (acceleration * 2))) / this->meanSpeed;
            time = temp > phaseDuration ? temp : phaseDuration;
        }
    }
    return time;
}

double SimpleApp::getDistanceToIntersection() {
    Coord IntersectionCoord = electricMap[getCurrentJunctionId(getCurrentRoadId())];
    Coord positionNow = getCurrentPos(simTime());
    return sqrt((positionNow.x - IntersectionCoord.x)*(positionNow.x - IntersectionCoord.x) + (positionNow.y - IntersectionCoord.y)*(positionNow.y - IntersectionCoord.y));
}

void SimpleApp::updateRecordLastTime() {
    this->lastSpeed = getSpeed();
    this->lastUpdateTime = simTime();
}

int SimpleApp::getTimeDelay(HeterogeneousMessage *receiveMessage) {
    if (int(simTime().dbl()*1000000 - (receiveMessage->getSendingTime()).dbl()*1000000) < 0) {
        return 0;
    }
    std::cout<<"delatTime is :"<<int(simTime().dbl()*1000000 - (receiveMessage->getSendingTime()).dbl()*1000000)<<std::endl;
    return int(simTime().dbl()*1000000 - (receiveMessage->getSendingTime()).dbl()*1000000);
}

void SimpleApp::pushVehicleInfo(std::string carId,Info carInfo)
{
    if(vehicleInfo.count(carId)>0)
    {
        vehicleInfo.erase(carId);
    }
    vehicleInfo.insert(std::pair<std::string,Info>(carId,carInfo));
    // sorted by location

}

std::map<std::string,Info> SimpleApp::getVehicleInfoofTempCluster()
{
    return vehicleInfo;
}

std::map<std::string,Info> SimpleApp::getVehicleInfoOfCluster()
{
    return vehicleInfoCH;
}

void SimpleApp::pushVehicleInfoCH(std::string carId,Info carInfo)
{
    if (carId == "node[flow1.2]") {
        std::cout<<"!!!"<<std::endl;
    }
    if(vehicleInfoCH.count(carId)>0)
    {
        vehicleInfoCH.erase(carId);
    }
    vehicleInfoCH.insert(std::pair<std::string,Info>(carId,carInfo));
    //get mean value of speed
    this->meanSpeed = getSpeed();
    //Find the car in front
}

void SimpleApp::removeVehicleInfoCH(std::string carId)
{
    if(vehicleInfoCH.count(carId)>0)
    {
        vehicleInfoCH.erase(carId);
    }
}

std::string SimpleApp::getSumoId()
{
    return sumoId;
}

void SimpleApp::setTempClusterQueue(std::string roadID)
{
    bool needAdd2,needAdd1;
    needAdd2=true;
    needAdd1=true;
    if(tempClusterQueue.empty())
    {
        tempClusterQueue.push(roadID);
    }
    else
    {
        if(tempClusterQueue.size()==2)
        {
            if(tempClusterQueue.front()==roadID||tempClusterQueue.back()==roadID)
            {
                needAdd2=false;
            }
            if(needAdd2)
            {
                tempClusterQueue.pop();
                tempClusterQueue.push(roadID);
            }
        }
        else if(tempClusterQueue.size()==1)
        {
            if(tempClusterQueue.front()==roadID)
            {
                needAdd1=false;
            }
            if(needAdd1)
            {
                tempClusterQueue.push(roadID);
            }
        }
    }
}

std::queue<std::string> SimpleApp::getTempClusterQueue()
{
    return tempClusterQueue;
}

void SimpleApp::clearTempCluster()
{
    vehicleInfo.clear();
    while(!tempClusterQueue.empty())
    {
        tempClusterQueue.pop();
    }
}

void SimpleApp::setClusterQueue(std::string roadID)
{
    bool needAdd2,needAdd1;
    needAdd2=true;
    needAdd1=true;
    int temp;
    temp=roadID.find(":",0);
    if(temp!=-1 )
    {
        std::cerr<<"roadId has some problems!!!"<<roadID<<std::endl;
    }
    else
    {
        if(clusterQueue.empty())
        {
            clusterQueue.push(roadID);
        }
        else
        {
            if(clusterQueue.size()==2)
            {
                if(clusterQueue.front()==roadID||clusterQueue.back()==roadID)
                {
                    needAdd2=false;
                }
                if(needAdd2)
                {
                    clusterQueue.pop();
                    clusterQueue.push(roadID);
                }
            }
            else if(clusterQueue.size()==1)
            {
                if(clusterQueue.front()==roadID)
                {
                    needAdd1=false;
                }
                if(needAdd1)
                {
                    clusterQueue.push(roadID);
                }
            }
          }
    }

}

std::queue<std::string> SimpleApp::getClusterQueue()
{
    return clusterQueue;
}


bool SimpleApp::memberOfCluster(std::queue<std::string> carQueue,std::string roadID)
{
    bool result;
    result=false;
    if(carQueue.size()==0)
    {
        std::cout<<"carID="<<getSumoId()<<std::endl;
        std::cout<<"not a member"<<std::endl;
    }
    else
    {
        if(carQueue.front()==roadID||carQueue.back()==roadID)
        {
            result=true;
        }
    }

    return result;
}

bool SimpleApp::findFromCHList(std::map<std::string,Info> CHInfo,std::string carId)
{
    bool find;
    find = false;
    std::map<std::string,Info>::iterator it = CHInfo.begin();
    for(; it != CHInfo.end(); it++)
        if(it->first == carId)
            find = true;
    return find;
}

void SimpleApp::setCHId(std::string s)
{
    CHId=s;
}
std::string SimpleApp::getCHId()
{
    return CHId;
}

int SimpleApp::getSpeed()
{
    return (int)mobility->getSpeed();
}

bool SimpleApp::isNeighborGW(std::string GWRoadId, std::string selfRoadId, int StateOfNodeReceived) {
    std::string junctionIdBehind_GW = getLastJunctionId(GWRoadId);
    std::string junctionIdFront_GW = getCurrentJunctionId(GWRoadId);
    std::string junctionIdBehind_Self= getLastJunctionId(selfRoadId);
    std::string junctionIdFront_Self = getCurrentJunctionId(selfRoadId);
    bool result = false;
    if (getVehicleState() == 4 && StateOfNodeReceived == 4) {
        if (junctionIdFront_Self == junctionIdFront_GW) {
            result = true;
        }
    } else if (getVehicleState() == 4 && StateOfNodeReceived == 3) {
        if (junctionIdFront_Self == junctionIdBehind_GW) {
            result = true;
        }
    } else if (getVehicleState() == 3 && StateOfNodeReceived == 4) {
        if (junctionIdBehind_Self == junctionIdFront_GW) {
            result = true;
        }
    } else if (getVehicleState() == 3 && StateOfNodeReceived == 3) {
        if (junctionIdBehind_Self == junctionIdBehind_GW) {
            result = true;
        }
    } else {}
    return result;
}

void SimpleApp::insertCH_Id(std::string carId)
{
    CH_Id.insert(carId);
}

std::set<std::string> SimpleApp::getCH_Id(std::string carId)
{
    return CH_Id;
}

void SimpleApp::updateCH_Info(std::string carId,simtime_t time)
{
    if(CH_Info.count(carId)>0)
    {
        CH_Rocord temp;
        temp.now_Time=time;
        temp.start_Time=CH_Info[carId].start_Time;
        CH_Info[carId]=temp;
    }
    else
    {
        CH_Rocord temp;
        temp.now_Time=time;
        temp.start_Time=time;
        CH_Info.insert(std::pair<std::string,CH_Rocord>(carId,temp));
    }
}

std::map<std::string,CH_Rocord> SimpleApp::getCH_Info()
{
    return CH_Info;
}

void SimpleApp::countBroadcastNum()
{
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    broadcastNum += (hosts.size()-1);
}

void SimpleApp::recordMacAddr_CH(LAddress::L2Type temp)
{
    macAddr_CH=temp;
}

LAddress::L2Type SimpleApp::getMacAddr_CH()
{
    return macAddr_CH;
}

bool SimpleApp::junctionIDInRoadId(std::string roadId, std::string junctionId) {
    if (roadId.find(junctionId) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

bool SimpleApp::isgatewayNode() {
    return this->gatewayNode;
}

void SimpleApp::setGateWayNode(bool temp) {
    this->gatewayNode = temp;
}

void SimpleApp::setInfoGw(InfoGW temp) {
    this->infoGw = temp;
}

InfoGW SimpleApp::getInfoGw() {
    return this->infoGw;
}

void SimpleApp::setInfoGwToLte(InfoGWToLte temp) {
    this->infoGwToLte = temp;
}

InfoGWToLte SimpleApp::getInfoGwToLte() {
    return this->infoGwToLte;
}

void SimpleApp::startBroadcastTopologyInfo(int code) {
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("DSRC Broadcast Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string destination("node[" + it->first + "]");
    sendMessage->setDestinationAddress(destination.c_str());//destination
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setMsgState(getVehicleState());//message state
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    sendMessage->setIsBroadcast(true);
    sendMessage->setSendingTime(simTime());
    if (isgatewayNode() || getVehicleState() == 4) {// for CM/CH is a GWs
        //update GWs info
        Info tempInfo;
        tempInfo.currentRoadId=getCurrentRoadId();
        tempInfo.nextRoadId=getNextRoadId(getCurrentRoute(),getCurrentRoadId());
        tempInfo.pos=getCurrentPos(simTime());
        tempInfo.junctionId=getCurrentJunctionId(getCurrentRoadId());
        std::string destinationTemp("node[" + getSumoId() + "]");
        sendMessage->setInfoGWToLte(infoGwToLte);
        sendMessage->setMsgCode(code);
        sendMessage->setUsedFor(BroadcastTopology);
    }
    countBroadcastNum();
    send(sendMessage, toDecisionMaker);
    std::cout<<"broadcast topology msg"<<std::endl;
}

void SimpleApp::startRouting() {
    std::vector<Connectivity_Info>::iterator it_Connectivity_Info = this->routeTable.begin();
    LAddress::L2Type macAddrNextHop = (*it_Connectivity_Info).macAddrGW;
    this->routeTable.erase(it_Connectivity_Info);
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("unicast Test Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string destination("node[" +this->route_destination+ "]");
    sendMessage->setDestinationAddress(destination.c_str());
    sendMessage->setMsgState(getVehicleState());//message state
    sendMessage->setSendingTime(simTime());
    sendMessage->setRoutingStartTime(simTime().dbl());
    sendMessage->setUsedFor(RelayRoute);
    sendMessage->setIsBroadcast(false);//unicast
    sendMessage->setMacAddr(macAddrNextHop);//set mac address
    sendMessage->setSourceAddress(this->getSumoId().c_str());//source
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    sendMessage->setRouteTable(this->routeTable);
    send(sendMessage, toDecisionMaker);
    unicastNum++;
    std::cerr<<"==========start routing message unicast====================>"<<std::endl;
}


void SimpleApp::startBroadcastToDestination(HeterogeneousMessage *receiveMessage) {
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("DSRC Broadcast Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string destination("node[" + it->first + "]");
    sendMessage->setDestinationAddress(destination.c_str());//destination
    sendMessage->setSourceAddress(sumoId.c_str());//source
    sendMessage->setMsgState(this->getVehicleState());//message state
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    sendMessage->setSendingTime(simTime());
    sendMessage->setIsBroadcast(true);
    sendMessage->setUsedFor(BroadcastToDestination);
    sendMessage->setDestinationAddress(receiveMessage->getDestinationAddress());
    countBroadcastNum();
    send(sendMessage, toDecisionMaker);
    std::cout<<"+++++++++++++++++++++++++++++++++++"<<std::endl;
}

void SimpleApp::relayRoutingMsg(HeterogeneousMessage *receiveMessage) {
    this->routeTable = receiveMessage->getRouteTable();
    std::vector<Connectivity_Info>::iterator it_delete = this->routeTable.begin();
    LAddress::L2Type macAddrNextHop = (*it_delete).macAddrGW;
    (this->routeTable).erase(it_delete);
    HeterogeneousMessage *sendMessage = new HeterogeneousMessage();
    sendMessage->setNetworkType(DSRC);
    sendMessage->setName("unicast Test Message");
    sendMessage->setByteLength(10);
    TraCIScenarioManager* manager = TraCIScenarioManagerAccess().get();
    std::map<std::string, cModule*> hosts = manager->getManagedHosts();
    std::map<std::string, cModule*>::iterator it = hosts.begin();
    std::advance(it, intrand(hosts.size()));
    std::string temp(receiveMessage->getSourceAddress());
    sendMessage->setDestinationAddress(receiveMessage->getDestinationAddress());
    sendMessage->setMsgState(getVehicleState());//message state
    sendMessage->setSendingTime(simTime());
    sendMessage->setSendingTime(receiveMessage->getRoutingStartTime());
    sendMessage->setUsedFor(RelayRoute);
    sendMessage->setIsBroadcast(false);//unicast
    sendMessage->setMacAddr(macAddrNextHop);//set mac address
    sendMessage->setSourceAddress(this->sumoId.c_str());//source
    sendMessage->setNodeNextRoadId(getNextRoadId(getCurrentRoute(),getCurrentRoadId()).c_str());//next road id
    sendMessage->setCurrentRoadId(getCurrentRoadId().c_str());//current road id
    sendMessage->setSenderPos(getCurrentPos(simTime()));//current pos
    sendMessage->setRouteTable(this->routeTable);
    send(sendMessage, toDecisionMaker);
    unicastNum++;
    std::cerr<<"==========relay message unicast====================>"<<std::endl;
}

void SimpleApp::findRoutePath( std::vector<Connectivity_Info> onePath, double conenctivity_value,int currentHop, std::string roadIdSourceNode, std::string targetIntersectionId) {
    if (currentHop > 10) return;
    InfoGWToLte tempUse = infoGwToLte;
    InfoGW tempUseInner;
    tempUseInner = tempUse[roadIdSourceNode];
    InfoGW::iterator it = tempUseInner.begin();
    if (tempUseInner.count(targetIntersectionId) > 0) { //find the target intersection
        std::map<std::string, Connectivity_Info> connectionData = tempUseInner[targetIntersectionId];
        std::map<std::string, Connectivity_Info>::iterator it_temp = connectionData.begin();
        while (it_temp != connectionData.end()) {// the road connected to the target intersection ID
            onePath.push_back(it_temp->second);
            conenctivity_value += ((it_temp->second).connectivityValue);
            this->routeForSelected.insert(std::make_pair(conenctivity_value, onePath));
            this->smallestDelayForRoute = this->smallestDelayForRoute < conenctivity_value ? this->smallestDelayForRoute : conenctivity_value;
            //erase
            onePath.pop_back();
            conenctivity_value -= ((it_temp->second).connectivityValue);
            it_temp++;
        }
        return;
    }
    // not the targetIntersection ID
    while ( it != tempUseInner.end() && currentHop < 10) {
        std::map<std::string, Connectivity_Info> connectionData = it->second;
        std::map<std::string, Connectivity_Info>::iterator it_temp = connectionData.begin();
        while (it_temp != connectionData.end()) {// the road connected to the target intersection ID
            onePath.push_back(it_temp->second);
            conenctivity_value += ((it_temp->second).connectivityValue);
            if (junctionIDInRoadId(it_temp->first, targetIntersectionId)) {
                this->routeForSelected.insert(std::make_pair(conenctivity_value, onePath));
                this->smallestDelayForRoute = this->smallestDelayForRoute < conenctivity_value ? this->smallestDelayForRoute : conenctivity_value;
                onePath.pop_back();
                conenctivity_value -= ((it_temp->second).connectivityValue);
            } else {
                currentHop++;
                findRoutePath(onePath, conenctivity_value, currentHop, it_temp->first, targetIntersectionId);
                //erase
                currentHop--;
                conenctivity_value -= ((it_temp->second).connectivityValue);
                onePath.pop_back();
            }
            it_temp++;
        }
        it++;
    }
}

double SimpleApp::computeDis(Coord pos1,Coord pos2)
{
    double dis=sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+((pos1.y-pos2.y))*((pos1.y-pos2.y)));
    return dis;
}

std::vector<std::string> SimpleApp::findTwoIntersection(Coord selfPos) {
    // find the first closest intersection
    std::vector<std::string> result;
    std::vector<std::string> resultReturn;
    std::map<int, std::string> distanceToIntersections;
    std::map<std::string, Coord>::iterator it_map =  electricMap.begin();
    int resultFirst = 20000;
    while(it_map != electricMap.end()) {
        int distance = int(computeDis(selfPos,it_map->second));
        resultFirst = resultFirst < distance ? resultFirst : distance;
        if(distanceToIntersections.count(distance) > 0) {
            result.push_back(it_map->first);
        } else {
            distanceToIntersections.insert(std::make_pair(distance, it_map->first));
        }
        it_map++;
    }
    resultReturn.push_back(distanceToIntersections[resultFirst]);
    distanceToIntersections.erase(resultFirst);
    int resultSecond = 20000;
    std::map<int, std::string>::iterator it_second = distanceToIntersections.begin();
    while(it_second != distanceToIntersections.end()) {
        resultSecond = resultSecond < it_second->first ? resultSecond : it_second->first;
        it_second++;
    }
    resultReturn.push_back(distanceToIntersections[resultSecond]);
    return resultReturn;
}


void SimpleApp::finish(){
    std::set<std::string>::iterator it_forGW = GWTOGWs.begin();
    while(it_forGW != GWTOGWs.end()) {
        std::cout<<*it_forGW<<std::endl;
        it_forGW++;
    }
    std::cout <<getSumoId()<<"received msg count " << overHead_clustering << std::endl;
    std::cout<<"BroadcastNum:"<<broadcastNum<<std::endl;
    std::cout<<"UnicastNum:"<<unicastNum<<std::endl;
    std::set<int>::iterator iter0=carState.begin();
    for(;iter0!=carState.end();iter0++)
    {
       switch(*iter0)
       {
           case 2:
               break;
           case 4:
           {
               std::cout<<"CH count:";
               std::set<std::string>::iterator iter1=CH_Id.begin();
               for(;iter1!=CH_Id.end();iter1++)
               {
                   std::cout<<*iter1<<"===";
               }
               std::cout<<std::endl;
               std::map<std::string,CH_Rocord>::iterator iter2=CH_Info.begin();
               for(;iter2!=CH_Info.end();iter2++)
               {
                   std::cout<<"CH time:"<<iter2->first<<"==="<<(iter2->second).start_Time<<"-----"<<(iter2->second).now_Time<<std::endl;
               }
           }
               break;
           default:
               break;
       }
    }
}
