//
// Generated file, do not edit! Created by nedtool 4.6 from veins/modules/messages/WaveShortMessage.msg.
//

#ifndef _WAVESHORTMESSAGE_M_H_
#define _WAVESHORTMESSAGE_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "veins/base/utils/Coord.h"
#include "veins/base/utils/SimpleAddress.h"
// }}

// cplusplus {{
#include <queue>
typedef std::queue<std::string> clusterQueue;
// }}

// cplusplus {{
#include <map>
#include <queue>
struct Info
{
    std::string nextRoadId;
    std::string currentRoadId;
    std::string junctionId;
    Coord pos;
};

struct Connectivity_Info {
	double connectivityValue;
	LAddress::L2Type macAddrGW;
};

typedef std::map<std::string, Info> infoDSRC;
typedef std::map<std::string, std::map<std::string, Connectivity_Info>> InfoGW;
typedef std::map<std::string, std::map<std::string, std::map<std::string, Connectivity_Info>>> InfoGWToLte;
// }}

/**
 * Class generated from <tt>veins/modules/messages/WaveShortMessage.msg:65</tt> by nedtool.
 * <pre>
 * packet WaveShortMessage
 * {
 *     //Version of the Wave Short Message
 *     int wsmVersion = 0;
 *     //Determine which security mechanism was used
 *     int securityType = 0;
 *     //Channel Number on which this packet was sent
 *     int channelNumber;
 *     //Data rate with which this packet was sent
 *     int dataRate = 1;
 *     //Power Level with which this packet was sent
 *     int priority = 3;
 *     //Unique number to identify the service
 *     int psid = 0;
 *     //Provider Service Context
 *     string psc = "Service with some Data";
 *     //Length of Wave Short Message
 *     int wsmLength;
 *     //Data of Wave Short Message
 *     string wsmData = "Some Data";
 * 
 *     int senderAddress = 0;
 *     int recipientAddress = -1;
 *     int serial = 0;
 *     Coord senderPos;
 *     simtime_t timestamp = 0;
 *     //zy
 *     LAddress::L2Type macAddr;
 *     bool isBroadcast;
 *     int msgState;
 *     string nodeNextRoadId;
 *     string currentRoadId;
 *     infoDSRC CHInfo;
 *     infoDSRC tempCHInfo;
 *     clusterQueue tempCHRoadID;
 *     clusterQueue CHRoadID;
 *     bool departure;
 *     string CHId;
 *     int CH_Speed;
 *     bool gatewayNode;
 *     InfoGW infoGw;
 *     InfoGWToLte infoGWToLte;
 *     int msgCode;
 *     int usedFor;
 * }
 * </pre>
 */
class WaveShortMessage : public ::cPacket
{
  protected:
    int wsmVersion_var;
    int securityType_var;
    int channelNumber_var;
    int dataRate_var;
    int priority_var;
    int psid_var;
    opp_string psc_var;
    int wsmLength_var;
    opp_string wsmData_var;
    int senderAddress_var;
    int recipientAddress_var;
    int serial_var;
    Coord senderPos_var;
    simtime_t timestamp_var;
    LAddress::L2Type macAddr_var;
    bool isBroadcast_var;
    int msgState_var;
    opp_string nodeNextRoadId_var;
    opp_string currentRoadId_var;
    infoDSRC CHInfo_var;
    infoDSRC tempCHInfo_var;
    clusterQueue tempCHRoadID_var;
    clusterQueue CHRoadID_var;
    bool departure_var;
    opp_string CHId_var;
    int CH_Speed_var;
    bool gatewayNode_var;
    InfoGW infoGw_var;
    InfoGWToLte infoGWToLte_var;
    int msgCode_var;
    int usedFor_var;

  private:
    void copy(const WaveShortMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const WaveShortMessage&);

  public:
    WaveShortMessage(const char *name=NULL, int kind=0);
    WaveShortMessage(const WaveShortMessage& other);
    virtual ~WaveShortMessage();
    WaveShortMessage& operator=(const WaveShortMessage& other);
    virtual WaveShortMessage *dup() const {return new WaveShortMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getWsmVersion() const;
    virtual void setWsmVersion(int wsmVersion);
    virtual int getSecurityType() const;
    virtual void setSecurityType(int securityType);
    virtual int getChannelNumber() const;
    virtual void setChannelNumber(int channelNumber);
    virtual int getDataRate() const;
    virtual void setDataRate(int dataRate);
    virtual int getPriority() const;
    virtual void setPriority(int priority);
    virtual int getPsid() const;
    virtual void setPsid(int psid);
    virtual const char * getPsc() const;
    virtual void setPsc(const char * psc);
    virtual int getWsmLength() const;
    virtual void setWsmLength(int wsmLength);
    virtual const char * getWsmData() const;
    virtual void setWsmData(const char * wsmData);
    virtual int getSenderAddress() const;
    virtual void setSenderAddress(int senderAddress);
    virtual int getRecipientAddress() const;
    virtual void setRecipientAddress(int recipientAddress);
    virtual int getSerial() const;
    virtual void setSerial(int serial);
    virtual Coord& getSenderPos();
    virtual const Coord& getSenderPos() const {return const_cast<WaveShortMessage*>(this)->getSenderPos();}
    virtual void setSenderPos(const Coord& senderPos);
    virtual simtime_t getTimestamp() const;
    virtual void setTimestamp(simtime_t timestamp);
    virtual LAddress::L2Type& getMacAddr();
    virtual const LAddress::L2Type& getMacAddr() const {return const_cast<WaveShortMessage*>(this)->getMacAddr();}
    virtual void setMacAddr(const LAddress::L2Type& macAddr);
    virtual bool getIsBroadcast() const;
    virtual void setIsBroadcast(bool isBroadcast);
    virtual int getMsgState() const;
    virtual void setMsgState(int msgState);
    virtual const char * getNodeNextRoadId() const;
    virtual void setNodeNextRoadId(const char * nodeNextRoadId);
    virtual const char * getCurrentRoadId() const;
    virtual void setCurrentRoadId(const char * currentRoadId);
    virtual infoDSRC& getCHInfo();
    virtual const infoDSRC& getCHInfo() const {return const_cast<WaveShortMessage*>(this)->getCHInfo();}
    virtual void setCHInfo(const infoDSRC& CHInfo);
    virtual infoDSRC& getTempCHInfo();
    virtual const infoDSRC& getTempCHInfo() const {return const_cast<WaveShortMessage*>(this)->getTempCHInfo();}
    virtual void setTempCHInfo(const infoDSRC& tempCHInfo);
    virtual clusterQueue& getTempCHRoadID();
    virtual const clusterQueue& getTempCHRoadID() const {return const_cast<WaveShortMessage*>(this)->getTempCHRoadID();}
    virtual void setTempCHRoadID(const clusterQueue& tempCHRoadID);
    virtual clusterQueue& getCHRoadID();
    virtual const clusterQueue& getCHRoadID() const {return const_cast<WaveShortMessage*>(this)->getCHRoadID();}
    virtual void setCHRoadID(const clusterQueue& CHRoadID);
    virtual bool getDeparture() const;
    virtual void setDeparture(bool departure);
    virtual const char * getCHId() const;
    virtual void setCHId(const char * CHId);
    virtual int getCH_Speed() const;
    virtual void setCH_Speed(int CH_Speed);
    virtual bool getGatewayNode() const;
    virtual void setGatewayNode(bool gatewayNode);
    virtual InfoGW& getInfoGw();
    virtual const InfoGW& getInfoGw() const {return const_cast<WaveShortMessage*>(this)->getInfoGw();}
    virtual void setInfoGw(const InfoGW& infoGw);
    virtual InfoGWToLte& getInfoGWToLte();
    virtual const InfoGWToLte& getInfoGWToLte() const {return const_cast<WaveShortMessage*>(this)->getInfoGWToLte();}
    virtual void setInfoGWToLte(const InfoGWToLte& infoGWToLte);
    virtual int getMsgCode() const;
    virtual void setMsgCode(int msgCode);
    virtual int getUsedFor() const;
    virtual void setUsedFor(int usedFor);
};

inline void doPacking(cCommBuffer *b, WaveShortMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, WaveShortMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef _WAVESHORTMESSAGE_M_H_

