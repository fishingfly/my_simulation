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

#include "SimpleServerApp.h"

Define_Module(SimpleServerApp);

SimpleServerApp::SimpleServerApp() {}

SimpleServerApp::~SimpleServerApp() {}

void SimpleServerApp::initialize(int stage){
	ApplicationBase::initialize(stage);
	if(stage == 0){
		debug = par("debug").boolValue();
		receivedMessages = 0;
		uniqueCode = 1;
		manager = TraCIScenarioManagerAccess().get();
        eNodeBMobility = dynamic_cast<MobilityBase*>(
                    getModuleByPath("scenario.eNodeB1")->getSubmodule("mobility")
                    );
		ASSERT(manager);
	}
}

void SimpleServerApp::finish(){
	std::cout << "[SimpleServerApp] Received " << receivedMessages << " messages via LTE."
		<< std::endl;
}

void SimpleServerApp::handleMessageWhenUp(cMessage *msg){
	HeterogeneousMessage* receiveMessage = dynamic_cast<HeterogeneousMessage*>(msg);
	if(receiveMessage)
	{
		std::cerr<<"# LTE has received a message from a CH or tempCH ! #"<<receiveMessage->getSourceAddress()<<std::endl;
	    receivedMessages++;
		tempCHMsgToDB(receiveMessage);//UPdate lte DB
		if(receiveMessage->getMsgState() == 2)
		{
	        std::string sourceAddress = gatherFromDB();
	        if(sourceAddress != "")
	        {
	            std::cerr << "[SimpleServerApp, " << simTime() << "] Received Heterogeneous Message from " << sourceAddress << std::endl;
	            /*reply a message to vehicle*/
	            HeterogeneousMessage *reply = new HeterogeneousMessage("Server Reply");
	            IPv4Address address = manager->getIPAddressForID(sourceAddress);
	            reply->setSourceAddress("server");
	            reply->setMsgState(5);
	            std::cerr << "[LTE_Server_App, " << simTime() << "] Sending Message back to " << address << std::endl;
	            socket.sendTo(reply, address, 4242);
	        }
		} else if (receiveMessage->getMsgState() == 4) {
		    //process routing message
		    pushInfoGWToLte(receiveMessage->getInfoGWToLte());
		    //time to multi-cast to several CHs
            selectCHsAndUnicast();
		    //multicast by condition
		    if ((!hasBroadcast && CHInfo.size() == this->lastCHInfoSize) || (this->CHHasChangedRoad) ) {
		       if ((this->infoGWToLte).size() > 0) {
		           if ((this->infoGWToLte).size() > 2) {
		               std::cout<<"sad"<<std::endl;
		           }
		           std::cout<<(this->infoGWToLte).size()<<std::endl;
                   startMulticastToCHs(getUniqueCode());
                   this->hasBroadcast = true;
		       }
		    }
            //CLEAR
		    this->lastCHInfoSize = CHInfo.size();
		    this->CHHasChangedRoad = false;
		    this->tempCHInfo.clear();
		    choosedCHs.clear();

		}
	}
	delete msg;
}

bool SimpleServerApp::handleNodeStart(IDoneCallback *doneCallback){
	socket.setOutputGate(gate("udpOut"));
	int localPort = par("localPort");
	socket.bind(localPort);
	return true;
}

bool SimpleServerApp::handleNodeShutdown(IDoneCallback *doneCallback){
	return true;
}

void SimpleServerApp::handleNodeCrash(){}

void SimpleServerApp::tempCHMsgToDB(HeterogeneousMessage* heterogeneousMessage)
{
    std::map<std::string,Info> tempInfo;
    // update lteDB
    if ((heterogeneousMessage->getTempCHInfo()).size() != 0 && (heterogeneousMessage->getCHInfo()).size() == 0) {
        tempInfo = heterogeneousMessage->getTempCHInfo();
        this->tempCHInfo = tempInfo;
    } else if ((heterogeneousMessage->getCHInfo()).size() != 0){
        tempInfo = heterogeneousMessage->getCHInfo();
        // store CH info for routing
        std::string tempTest(heterogeneousMessage->getSourceAddress());
        std::string nodeName("node[" +tempTest+ "]");
        CHInfo.insert(std::make_pair(tempTest,tempInfo[nodeName]));
        //need to update CHInfo.some CH has become invalid

    }

    std::map<std::string,Info>::iterator it_temp = tempInfo.begin();
    //set request
    RequestRecord tempRecord;
    tempRecord.requestJuctionId = it_temp->second.junctionId;
    tempRecord.requestCurrentRoadId = it_temp->second.currentRoadId;
    setRequestInfo(tempRecord);//set request over
    for(;it_temp != tempInfo.end();it_temp++)
    {
        if(lteDB.count(it_temp->first)>0)
        {
            if (lteDB[it_temp->first].currentRoadId != (it_temp->second).currentRoadId) {
                this->CHHasChangedRoad = true;
            }
            lteDB.erase(it_temp->first);
        } else {
            this->CHHasChangedRoad = true;
        }
        lteDB.insert(std::pair<std::string,Info>(it_temp->first,it_temp->second));
    }
}

void SimpleServerApp::analyzeCHMsg(HeterogeneousMessage* heterogeneousMessage)
{

}

std::string SimpleServerApp::gatherFromDB()
{
    RequestRecord rr = getRequestInfo();
    std::map<std::string,Info>::iterator it_findMax = lteDB.begin();
    std::map<std::string,int> numOfNextRoad;
    for(;it_findMax != lteDB.end();it_findMax++)
    {
        if(it_findMax->second.junctionId == rr.requestJuctionId)
        {
            if(numOfNextRoad.count(it_findMax->second.nextRoadId)>0)
            {
                ++numOfNextRoad[it_findMax->second.nextRoadId];
            }
            else
                numOfNextRoad.insert(std::pair<std::string,int>(it_findMax->second.nextRoadId,1));
        }
    }
    //find the maxnumber of nextRoadId
    std::map<std::string,int>::iterator it_numOfNextRoad = numOfNextRoad.begin();
    std::string tempMaxNextRoadId;
    int tempNum = it_numOfNextRoad->second;
    for(;it_numOfNextRoad != numOfNextRoad.end();it_numOfNextRoad++)
    {
        if(it_numOfNextRoad->second >= tempNum)
        {
            tempNum = it_numOfNextRoad->second;
            tempMaxNextRoadId = it_numOfNextRoad->first;
        }
    }
    std::map<std::string,Info>::iterator it_distinguish = this->tempCHInfo.begin();
    std::map<std::string,Info> candidateOfCH;//the candidate of CH
    std::map<std::string,Info> otherCar;// other of current Road
    for(;it_distinguish != this->tempCHInfo.end();it_distinguish++)
    {
        if(strcmp(rr.requestCurrentRoadId.c_str(),it_distinguish->second.currentRoadId.c_str())==0)//request road
        {
            if(tempMaxNextRoadId == it_distinguish->second.nextRoadId)
            {
                Info tempCH;
                tempCH.nextRoadId = it_distinguish->second.nextRoadId;
                tempCH.pos = it_distinguish->second.pos;
                tempCH.currentRoadId = it_distinguish->second.currentRoadId;
                tempCH.junctionId = it_distinguish->second.junctionId;
                candidateOfCH.insert(std::pair<std::string,Info>(it_distinguish->first,tempCH));
            }
            else
            {
                Info tempCH;
                tempCH.nextRoadId = it_distinguish->second.nextRoadId;
                tempCH.pos = it_distinguish->second.pos;
                tempCH.currentRoadId = it_distinguish->second.currentRoadId;
                tempCH.junctionId = it_distinguish->second.junctionId;
                otherCar.insert(std::pair<std::string,Info>(it_distinguish->first,tempCH));
            }
        }
    }
    double weight1 = 0.5;
    double weight2 = 0.25;
    double weight3 = 0.25;
    double disInCluster = 0;
    double disOfOther = 0;
    double maxDisOfCH = 0;
    double maxDisOfOther = 0;
    double disToLte = 0;
    double totalWeight = 0;
    std::map<std::string,Info>::iterator it_CH;
    std::map<std::string,Info>::iterator ir_Other;
    std::map<std::string,InfoAndWeigth> newCHCandidate;//has weight
    if(candidateOfCH.size() == 1)
    {
        it_CH=candidateOfCH.begin();
        std::string onlyChooseCHId = it_CH->first;
        int start = onlyChooseCHId.find("[");
        int end = onlyChooseCHId.find("]");
        onlyChooseCHId = onlyChooseCHId.substr(start+1,end-start-1);
        std::cerr<<"chooseCHId="<<onlyChooseCHId<<std::endl;
        return onlyChooseCHId;
    }
    for(it_CH = candidateOfCH.begin();it_CH != candidateOfCH.end();it_CH++)
    {
        maxDisOfCH = getMaxDis(candidateOfCH,it_CH->first,it_CH->second);
        maxDisOfOther = getMaxDis(otherCar,it_CH->first,it_CH->second);
        disInCluster = getMeanDis(candidateOfCH,it_CH->first,it_CH->second);
        disOfOther = getMeanDis(otherCar,it_CH->first,it_CH->second);
        disToLte = computeDis(it_CH->second.pos,geteNodeBPos());
        if(maxDisOfOther == 0)
        {
            totalWeight = weight1*(disInCluster/maxDisOfCH) + weight3*(disToLte/500);
            //totalWeight=(weight1*disInCluster+weight3*disToLte)/(maxDisOfCH+1000);
        }
        else if(maxDisOfCH == 0)
        {
            totalWeight = weight2*(disOfOther/maxDisOfOther) + weight3*(disToLte/500);
//            totalWeight=(weight2*disOfOther+weight3*disToLte)/(maxDisOfOther+1000);
        }
        else if(maxDisOfOther == 0 && maxDisOfCH == 0)
        {
            totalWeight = weight3*(disToLte/500);
        }
        else
            totalWeight = weight1*(disInCluster/maxDisOfCH)+weight2*(disOfOther/maxDisOfOther)+weight3*(disToLte/500);
//            totalWeight=(weight1*disInCluster+weight2*disOfOther+weight3*disToLte)/(maxDisOfCH+maxDisOfOther+1000);
        InfoAndWeigth tempData;
        tempData.currentRoadId = it_CH->second.currentRoadId;
        tempData.junctionId = it_CH->second.junctionId;
        tempData.nextRoadId = it_CH->second.nextRoadId;
        tempData.pos = it_CH->second.pos;
        tempData.weight = totalWeight;
        newCHCandidate.insert(std::pair<std::string,InfoAndWeigth>(it_CH->first,tempData));
    }
    // find the min weight as CH
    std::map<std::string,InfoAndWeigth>::iterator it_end=newCHCandidate.begin();
    std::string choosedCHId;
    double w=1;
    for(;it_end != newCHCandidate.end();it_end++)
    {
        if(it_end->second.weight<w)
        {
            choosedCHId = it_end->first;
            w = it_end->second.weight;
        }
    }
    int start = choosedCHId.find("[");
    int end = choosedCHId.find("]");
    choosedCHId = choosedCHId.substr(start+1,end-start-1);
    std::cerr<<"chooseCHId="<<choosedCHId<<std::endl;
    return choosedCHId;
}

double SimpleServerApp::getMeanDis(std::map<std::string,Info> tempDB,std::string carId,Info carInfo)
{
    std::map<std::string,Info>::iterator it=tempDB.begin();
    double meanDis=0;
    int count=0;
    for(;it!=tempDB.end();it++)
    {
        if(carId!=it->first)
        {
            count++;
            meanDis+=computeDis(carInfo.pos,it->second.pos);
        }
    }
    if(count==0)
    {
        return 0;
    }
    else
        return (meanDis/count);
}

double SimpleServerApp::getMaxDis(std::map<std::string,Info> tempDB,std::string carId,Info carInfo)
{
    std::map<std::string,Info>::iterator it=tempDB.begin();
    double maxDis=0;
    double dis=0;
    for(;it!=tempDB.end();it++)
    {
        if(carId!=it->first)
        {
            dis=computeDis(carInfo.pos,it->second.pos);
            if(maxDis<dis)
                maxDis=dis;
        }
    }
    return maxDis;
}

double SimpleServerApp::computeDis(Coord pos1,Coord pos2)
{
    double dis=sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+((pos1.y-pos2.y))*((pos1.y-pos2.y)));
    return dis;
}

Coord SimpleServerApp::geteNodeBPos()
{
    return eNodeBMobility->getCurrentPosition();
}

void SimpleServerApp::setRequestInfo(RequestRecord temp)
{
    reqRecord.requestJuctionId=temp.requestJuctionId;
    reqRecord.requestCurrentRoadId=temp.requestCurrentRoadId;
}

RequestRecord SimpleServerApp::getRequestInfo()
{
    return reqRecord;
}

void SimpleServerApp::pushInfoGWToLte(InfoGWToLte tempInfo) {
    InfoGWToLte::iterator it;
    it = tempInfo.begin();
    while (it != tempInfo.end()) {
          if (infoGWToLte.count(it->first) > 0) {
            infoGWToLte.erase(it->first);
        }
        if ((it->second).size() > 0) {
            infoGWToLte.insert(std::make_pair(it->first,it->second));
        }
        it++;
    }
}

double SimpleServerApp::getTriangleArea(Coord p0, Coord p1, Coord p2) {
    Coord ab, bc;
    ab.x = p1.x - p0.x;
    ab.y = p1.y - p0.y;
    bc.x = p2.x - p1.x;
    bc.y = p2.y - p1.y;
    return fabs((ab.x * bc.y - ab.y * bc.x) / 2.0);
}

bool SimpleServerApp::isInTriangle(Coord a, Coord b, Coord c, Coord d) {
    double sabc, sadb, sbdc, sadc;
    sabc = getTriangleArea(a, b, c);
    sadb = getTriangleArea(a, d, b);
    sbdc = getTriangleArea(b, d, c);
    sadc = getTriangleArea(a, d, c);
    double sumSuqar = sadb + sbdc + sadc;
    if (-0.0001 < (sabc - sumSuqar) && (sabc - sumSuqar) < 0.0001) {
        return true;
    } else {
        return false;
    }
}

bool SimpleServerApp::isTriangle(Coord p0, Coord p1, Coord p2) {
    Coord ab, bc;
    ab.x = p1.x - p0.x;
    ab.y = p1.y - p0.y;
    bc.x = p2.x - p1.x;
    bc.y = p2.y - p1.y;
    double multipleResult = fabs((ab.x * bc.y - ab.y * bc.x) / 2.0);
    double result = sqrt(ab.x * ab.x + ab.y * ab.y) + sqrt(bc.x * bc.x + bc.y * bc.y);
    int cos = abs(int(multipleResult / result) * 10);
    if (cos > 9) return true;
    else return false;
}

void SimpleServerApp::selectCHsAndUnicast() {
    std::map<std::string,Info>::iterator it = CHInfo.begin();
    if (CHInfo.size() < 3) {
        while( it != CHInfo.end()) {
            choosedCHs.push_back(it->first);
            it++;
        }
    } else {
        std::vector<std::string> nodeName;
        it = CHInfo.begin();
        while( it != CHInfo.end()) {
            nodeName.push_back(it->first);
            it++;
        }
        for (int i = 0; i < nodeName.size(); i++) {
            for (int j = 0; j < nodeName.size(); j++) {
                if (j == i) continue;
                for (int n = 0; n < nodeName.size(); n++) {
                    if (n == i || n == j) continue;
                    if (!isTriangle(CHInfo[nodeName[i]].pos, CHInfo[nodeName[j]].pos, CHInfo[nodeName[n]].pos)) {
                        continue;
                    } else {
                        if (isInTriangle(CHInfo[nodeName[i]].pos, CHInfo[nodeName[j]].pos, CHInfo[nodeName[n]].pos, geteNodeBPos())) {
                            choosedCHs.push_back(nodeName[i]);
                            choosedCHs.push_back(nodeName[j]);
                            choosedCHs.push_back(nodeName[n]);
                            return;
                        }
                    }
                }
            }
        }
    }
    if (choosedCHs.size() == 0) {
        std::cerr<<"# something is wrong with choosedCHs for topology info broadcast! and select the CHs with no condition"<<std::endl;
        it = CHInfo.begin();
        while( it != CHInfo.end()) {
            choosedCHs.push_back(it->first);
            it++;
        }
    }
}
void SimpleServerApp::startMulticastToCHs( int number ) {
    for (int i = 0; i < choosedCHs.size(); i++) {
        /*reply a message to vehicle*/
        HeterogeneousMessage *reply = new HeterogeneousMessage("Server Reply");
        IPv4Address address = manager->getIPAddressForID(choosedCHs[i]);
        reply->setSourceAddress("server");
        reply->setMsgState(5);
        reply->setInfoGWToLte(infoGWToLte);
        reply->setMsgCode(number);
        std::cerr << "[LTE_Server_App, " << simTime() << "] Sending routing Msg back to " << address << std::endl;
        socket.sendTo(reply, address, 4242);
    }
}

int SimpleServerApp::getUniqueCode() {
    uniqueCode++;
    return uniqueCode;
}

void SimpleServerApp::clearExpiredDataForGW() {

}
