//
// Generated file, do not edit! Created by nedtool 4.6 from veins/modules/messages/WaveShortMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "WaveShortMessage_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

Register_Class(WaveShortMessage);

WaveShortMessage::WaveShortMessage(const char *name, int kind) : ::cPacket(name,kind)
{
    this->wsmVersion_var = 0;
    this->securityType_var = 0;
    this->channelNumber_var = 0;
    this->dataRate_var = 1;
    this->priority_var = 3;
    this->psid_var = 0;
    this->psc_var = "Service with some Data";
    this->wsmLength_var = 0;
    this->wsmData_var = "Some Data";
    this->senderAddress_var = 0;
    this->recipientAddress_var = -1;
    this->serial_var = 0;
    this->timestamp_var = 0;
    this->isBroadcast_var = 0;
    this->msgState_var = 0;
    this->nodeNextRoadId_var = 0;
    this->currentRoadId_var = 0;
    this->departure_var = 0;
    this->CHId_var = 0;
    this->CH_Speed_var = 0;
    this->gatewayNode_var = 0;
    this->msgCode_var = 0;
    this->usedFor_var = 0;
}

WaveShortMessage::WaveShortMessage(const WaveShortMessage& other) : ::cPacket(other)
{
    copy(other);
}

WaveShortMessage::~WaveShortMessage()
{
}

WaveShortMessage& WaveShortMessage::operator=(const WaveShortMessage& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void WaveShortMessage::copy(const WaveShortMessage& other)
{
    this->wsmVersion_var = other.wsmVersion_var;
    this->securityType_var = other.securityType_var;
    this->channelNumber_var = other.channelNumber_var;
    this->dataRate_var = other.dataRate_var;
    this->priority_var = other.priority_var;
    this->psid_var = other.psid_var;
    this->psc_var = other.psc_var;
    this->wsmLength_var = other.wsmLength_var;
    this->wsmData_var = other.wsmData_var;
    this->senderAddress_var = other.senderAddress_var;
    this->recipientAddress_var = other.recipientAddress_var;
    this->serial_var = other.serial_var;
    this->senderPos_var = other.senderPos_var;
    this->timestamp_var = other.timestamp_var;
    this->macAddr_var = other.macAddr_var;
    this->isBroadcast_var = other.isBroadcast_var;
    this->msgState_var = other.msgState_var;
    this->nodeNextRoadId_var = other.nodeNextRoadId_var;
    this->currentRoadId_var = other.currentRoadId_var;
    this->CHInfo_var = other.CHInfo_var;
    this->tempCHInfo_var = other.tempCHInfo_var;
    this->tempCHRoadID_var = other.tempCHRoadID_var;
    this->CHRoadID_var = other.CHRoadID_var;
    this->departure_var = other.departure_var;
    this->CHId_var = other.CHId_var;
    this->CH_Speed_var = other.CH_Speed_var;
    this->gatewayNode_var = other.gatewayNode_var;
    this->infoGw_var = other.infoGw_var;
    this->infoGWToLte_var = other.infoGWToLte_var;
    this->msgCode_var = other.msgCode_var;
    this->usedFor_var = other.usedFor_var;
}

void WaveShortMessage::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->wsmVersion_var);
    doPacking(b,this->securityType_var);
    doPacking(b,this->channelNumber_var);
    doPacking(b,this->dataRate_var);
    doPacking(b,this->priority_var);
    doPacking(b,this->psid_var);
    doPacking(b,this->psc_var);
    doPacking(b,this->wsmLength_var);
    doPacking(b,this->wsmData_var);
    doPacking(b,this->senderAddress_var);
    doPacking(b,this->recipientAddress_var);
    doPacking(b,this->serial_var);
    doPacking(b,this->senderPos_var);
    doPacking(b,this->timestamp_var);
    doPacking(b,this->macAddr_var);
    doPacking(b,this->isBroadcast_var);
    doPacking(b,this->msgState_var);
    doPacking(b,this->nodeNextRoadId_var);
    doPacking(b,this->currentRoadId_var);
    doPacking(b,this->CHInfo_var);
    doPacking(b,this->tempCHInfo_var);
    doPacking(b,this->tempCHRoadID_var);
    doPacking(b,this->CHRoadID_var);
    doPacking(b,this->departure_var);
    doPacking(b,this->CHId_var);
    doPacking(b,this->CH_Speed_var);
    doPacking(b,this->gatewayNode_var);
    doPacking(b,this->infoGw_var);
    doPacking(b,this->infoGWToLte_var);
    doPacking(b,this->msgCode_var);
    doPacking(b,this->usedFor_var);
}

void WaveShortMessage::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->wsmVersion_var);
    doUnpacking(b,this->securityType_var);
    doUnpacking(b,this->channelNumber_var);
    doUnpacking(b,this->dataRate_var);
    doUnpacking(b,this->priority_var);
    doUnpacking(b,this->psid_var);
    doUnpacking(b,this->psc_var);
    doUnpacking(b,this->wsmLength_var);
    doUnpacking(b,this->wsmData_var);
    doUnpacking(b,this->senderAddress_var);
    doUnpacking(b,this->recipientAddress_var);
    doUnpacking(b,this->serial_var);
    doUnpacking(b,this->senderPos_var);
    doUnpacking(b,this->timestamp_var);
    doUnpacking(b,this->macAddr_var);
    doUnpacking(b,this->isBroadcast_var);
    doUnpacking(b,this->msgState_var);
    doUnpacking(b,this->nodeNextRoadId_var);
    doUnpacking(b,this->currentRoadId_var);
    doUnpacking(b,this->CHInfo_var);
    doUnpacking(b,this->tempCHInfo_var);
    doUnpacking(b,this->tempCHRoadID_var);
    doUnpacking(b,this->CHRoadID_var);
    doUnpacking(b,this->departure_var);
    doUnpacking(b,this->CHId_var);
    doUnpacking(b,this->CH_Speed_var);
    doUnpacking(b,this->gatewayNode_var);
    doUnpacking(b,this->infoGw_var);
    doUnpacking(b,this->infoGWToLte_var);
    doUnpacking(b,this->msgCode_var);
    doUnpacking(b,this->usedFor_var);
}

int WaveShortMessage::getWsmVersion() const
{
    return wsmVersion_var;
}

void WaveShortMessage::setWsmVersion(int wsmVersion)
{
    this->wsmVersion_var = wsmVersion;
}

int WaveShortMessage::getSecurityType() const
{
    return securityType_var;
}

void WaveShortMessage::setSecurityType(int securityType)
{
    this->securityType_var = securityType;
}

int WaveShortMessage::getChannelNumber() const
{
    return channelNumber_var;
}

void WaveShortMessage::setChannelNumber(int channelNumber)
{
    this->channelNumber_var = channelNumber;
}

int WaveShortMessage::getDataRate() const
{
    return dataRate_var;
}

void WaveShortMessage::setDataRate(int dataRate)
{
    this->dataRate_var = dataRate;
}

int WaveShortMessage::getPriority() const
{
    return priority_var;
}

void WaveShortMessage::setPriority(int priority)
{
    this->priority_var = priority;
}

int WaveShortMessage::getPsid() const
{
    return psid_var;
}

void WaveShortMessage::setPsid(int psid)
{
    this->psid_var = psid;
}

const char * WaveShortMessage::getPsc() const
{
    return psc_var.c_str();
}

void WaveShortMessage::setPsc(const char * psc)
{
    this->psc_var = psc;
}

int WaveShortMessage::getWsmLength() const
{
    return wsmLength_var;
}

void WaveShortMessage::setWsmLength(int wsmLength)
{
    this->wsmLength_var = wsmLength;
}

const char * WaveShortMessage::getWsmData() const
{
    return wsmData_var.c_str();
}

void WaveShortMessage::setWsmData(const char * wsmData)
{
    this->wsmData_var = wsmData;
}

int WaveShortMessage::getSenderAddress() const
{
    return senderAddress_var;
}

void WaveShortMessage::setSenderAddress(int senderAddress)
{
    this->senderAddress_var = senderAddress;
}

int WaveShortMessage::getRecipientAddress() const
{
    return recipientAddress_var;
}

void WaveShortMessage::setRecipientAddress(int recipientAddress)
{
    this->recipientAddress_var = recipientAddress;
}

int WaveShortMessage::getSerial() const
{
    return serial_var;
}

void WaveShortMessage::setSerial(int serial)
{
    this->serial_var = serial;
}

Coord& WaveShortMessage::getSenderPos()
{
    return senderPos_var;
}

void WaveShortMessage::setSenderPos(const Coord& senderPos)
{
    this->senderPos_var = senderPos;
}

simtime_t WaveShortMessage::getTimestamp() const
{
    return timestamp_var;
}

void WaveShortMessage::setTimestamp(simtime_t timestamp)
{
    this->timestamp_var = timestamp;
}

LAddress::L2Type& WaveShortMessage::getMacAddr()
{
    return macAddr_var;
}

void WaveShortMessage::setMacAddr(const LAddress::L2Type& macAddr)
{
    this->macAddr_var = macAddr;
}

bool WaveShortMessage::getIsBroadcast() const
{
    return isBroadcast_var;
}

void WaveShortMessage::setIsBroadcast(bool isBroadcast)
{
    this->isBroadcast_var = isBroadcast;
}

int WaveShortMessage::getMsgState() const
{
    return msgState_var;
}

void WaveShortMessage::setMsgState(int msgState)
{
    this->msgState_var = msgState;
}

const char * WaveShortMessage::getNodeNextRoadId() const
{
    return nodeNextRoadId_var.c_str();
}

void WaveShortMessage::setNodeNextRoadId(const char * nodeNextRoadId)
{
    this->nodeNextRoadId_var = nodeNextRoadId;
}

const char * WaveShortMessage::getCurrentRoadId() const
{
    return currentRoadId_var.c_str();
}

void WaveShortMessage::setCurrentRoadId(const char * currentRoadId)
{
    this->currentRoadId_var = currentRoadId;
}

infoDSRC& WaveShortMessage::getCHInfo()
{
    return CHInfo_var;
}

void WaveShortMessage::setCHInfo(const infoDSRC& CHInfo)
{
    this->CHInfo_var = CHInfo;
}

infoDSRC& WaveShortMessage::getTempCHInfo()
{
    return tempCHInfo_var;
}

void WaveShortMessage::setTempCHInfo(const infoDSRC& tempCHInfo)
{
    this->tempCHInfo_var = tempCHInfo;
}

clusterQueue& WaveShortMessage::getTempCHRoadID()
{
    return tempCHRoadID_var;
}

void WaveShortMessage::setTempCHRoadID(const clusterQueue& tempCHRoadID)
{
    this->tempCHRoadID_var = tempCHRoadID;
}

clusterQueue& WaveShortMessage::getCHRoadID()
{
    return CHRoadID_var;
}

void WaveShortMessage::setCHRoadID(const clusterQueue& CHRoadID)
{
    this->CHRoadID_var = CHRoadID;
}

bool WaveShortMessage::getDeparture() const
{
    return departure_var;
}

void WaveShortMessage::setDeparture(bool departure)
{
    this->departure_var = departure;
}

const char * WaveShortMessage::getCHId() const
{
    return CHId_var.c_str();
}

void WaveShortMessage::setCHId(const char * CHId)
{
    this->CHId_var = CHId;
}

int WaveShortMessage::getCH_Speed() const
{
    return CH_Speed_var;
}

void WaveShortMessage::setCH_Speed(int CH_Speed)
{
    this->CH_Speed_var = CH_Speed;
}

bool WaveShortMessage::getGatewayNode() const
{
    return gatewayNode_var;
}

void WaveShortMessage::setGatewayNode(bool gatewayNode)
{
    this->gatewayNode_var = gatewayNode;
}

InfoGW& WaveShortMessage::getInfoGw()
{
    return infoGw_var;
}

void WaveShortMessage::setInfoGw(const InfoGW& infoGw)
{
    this->infoGw_var = infoGw;
}

InfoGWToLte& WaveShortMessage::getInfoGWToLte()
{
    return infoGWToLte_var;
}

void WaveShortMessage::setInfoGWToLte(const InfoGWToLte& infoGWToLte)
{
    this->infoGWToLte_var = infoGWToLte;
}

int WaveShortMessage::getMsgCode() const
{
    return msgCode_var;
}

void WaveShortMessage::setMsgCode(int msgCode)
{
    this->msgCode_var = msgCode;
}

int WaveShortMessage::getUsedFor() const
{
    return usedFor_var;
}

void WaveShortMessage::setUsedFor(int usedFor)
{
    this->usedFor_var = usedFor;
}

class WaveShortMessageDescriptor : public cClassDescriptor
{
  public:
    WaveShortMessageDescriptor();
    virtual ~WaveShortMessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(WaveShortMessageDescriptor);

WaveShortMessageDescriptor::WaveShortMessageDescriptor() : cClassDescriptor("WaveShortMessage", "cPacket")
{
}

WaveShortMessageDescriptor::~WaveShortMessageDescriptor()
{
}

bool WaveShortMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<WaveShortMessage *>(obj)!=NULL;
}

const char *WaveShortMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int WaveShortMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 31+basedesc->getFieldCount(object) : 31;
}

unsigned int WaveShortMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<31) ? fieldTypeFlags[field] : 0;
}

const char *WaveShortMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "wsmVersion",
        "securityType",
        "channelNumber",
        "dataRate",
        "priority",
        "psid",
        "psc",
        "wsmLength",
        "wsmData",
        "senderAddress",
        "recipientAddress",
        "serial",
        "senderPos",
        "timestamp",
        "macAddr",
        "isBroadcast",
        "msgState",
        "nodeNextRoadId",
        "currentRoadId",
        "CHInfo",
        "tempCHInfo",
        "tempCHRoadID",
        "CHRoadID",
        "departure",
        "CHId",
        "CH_Speed",
        "gatewayNode",
        "infoGw",
        "infoGWToLte",
        "msgCode",
        "usedFor",
    };
    return (field>=0 && field<31) ? fieldNames[field] : NULL;
}

int WaveShortMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='w' && strcmp(fieldName, "wsmVersion")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "securityType")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "channelNumber")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataRate")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "priority")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "psid")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "psc")==0) return base+6;
    if (fieldName[0]=='w' && strcmp(fieldName, "wsmLength")==0) return base+7;
    if (fieldName[0]=='w' && strcmp(fieldName, "wsmData")==0) return base+8;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderAddress")==0) return base+9;
    if (fieldName[0]=='r' && strcmp(fieldName, "recipientAddress")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "serial")==0) return base+11;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderPos")==0) return base+12;
    if (fieldName[0]=='t' && strcmp(fieldName, "timestamp")==0) return base+13;
    if (fieldName[0]=='m' && strcmp(fieldName, "macAddr")==0) return base+14;
    if (fieldName[0]=='i' && strcmp(fieldName, "isBroadcast")==0) return base+15;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgState")==0) return base+16;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeNextRoadId")==0) return base+17;
    if (fieldName[0]=='c' && strcmp(fieldName, "currentRoadId")==0) return base+18;
    if (fieldName[0]=='C' && strcmp(fieldName, "CHInfo")==0) return base+19;
    if (fieldName[0]=='t' && strcmp(fieldName, "tempCHInfo")==0) return base+20;
    if (fieldName[0]=='t' && strcmp(fieldName, "tempCHRoadID")==0) return base+21;
    if (fieldName[0]=='C' && strcmp(fieldName, "CHRoadID")==0) return base+22;
    if (fieldName[0]=='d' && strcmp(fieldName, "departure")==0) return base+23;
    if (fieldName[0]=='C' && strcmp(fieldName, "CHId")==0) return base+24;
    if (fieldName[0]=='C' && strcmp(fieldName, "CH_Speed")==0) return base+25;
    if (fieldName[0]=='g' && strcmp(fieldName, "gatewayNode")==0) return base+26;
    if (fieldName[0]=='i' && strcmp(fieldName, "infoGw")==0) return base+27;
    if (fieldName[0]=='i' && strcmp(fieldName, "infoGWToLte")==0) return base+28;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgCode")==0) return base+29;
    if (fieldName[0]=='u' && strcmp(fieldName, "usedFor")==0) return base+30;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *WaveShortMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "string",
        "int",
        "string",
        "int",
        "int",
        "int",
        "Coord",
        "simtime_t",
        "LAddress::L2Type",
        "bool",
        "int",
        "string",
        "string",
        "infoDSRC",
        "infoDSRC",
        "clusterQueue",
        "clusterQueue",
        "bool",
        "string",
        "int",
        "bool",
        "InfoGW",
        "InfoGWToLte",
        "int",
        "int",
    };
    return (field>=0 && field<31) ? fieldTypeStrings[field] : NULL;
}

const char *WaveShortMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int WaveShortMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string WaveShortMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getWsmVersion());
        case 1: return long2string(pp->getSecurityType());
        case 2: return long2string(pp->getChannelNumber());
        case 3: return long2string(pp->getDataRate());
        case 4: return long2string(pp->getPriority());
        case 5: return long2string(pp->getPsid());
        case 6: return oppstring2string(pp->getPsc());
        case 7: return long2string(pp->getWsmLength());
        case 8: return oppstring2string(pp->getWsmData());
        case 9: return long2string(pp->getSenderAddress());
        case 10: return long2string(pp->getRecipientAddress());
        case 11: return long2string(pp->getSerial());
        case 12: {std::stringstream out; out << pp->getSenderPos(); return out.str();}
        case 13: return double2string(pp->getTimestamp());
        case 14: {std::stringstream out; out << pp->getMacAddr(); return out.str();}
        case 15: return bool2string(pp->getIsBroadcast());
        case 16: return long2string(pp->getMsgState());
        case 17: return oppstring2string(pp->getNodeNextRoadId());
        case 18: return oppstring2string(pp->getCurrentRoadId());
        case 19: {std::stringstream out; out << pp->getCHInfo(); return out.str();}
        case 20: {std::stringstream out; out << pp->getTempCHInfo(); return out.str();}
        case 21: {std::stringstream out; out << pp->getTempCHRoadID(); return out.str();}
        case 22: {std::stringstream out; out << pp->getCHRoadID(); return out.str();}
        case 23: return bool2string(pp->getDeparture());
        case 24: return oppstring2string(pp->getCHId());
        case 25: return long2string(pp->getCH_Speed());
        case 26: return bool2string(pp->getGatewayNode());
        case 27: {std::stringstream out; out << pp->getInfoGw(); return out.str();}
        case 28: {std::stringstream out; out << pp->getInfoGWToLte(); return out.str();}
        case 29: return long2string(pp->getMsgCode());
        case 30: return long2string(pp->getUsedFor());
        default: return "";
    }
}

bool WaveShortMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setWsmVersion(string2long(value)); return true;
        case 1: pp->setSecurityType(string2long(value)); return true;
        case 2: pp->setChannelNumber(string2long(value)); return true;
        case 3: pp->setDataRate(string2long(value)); return true;
        case 4: pp->setPriority(string2long(value)); return true;
        case 5: pp->setPsid(string2long(value)); return true;
        case 6: pp->setPsc((value)); return true;
        case 7: pp->setWsmLength(string2long(value)); return true;
        case 8: pp->setWsmData((value)); return true;
        case 9: pp->setSenderAddress(string2long(value)); return true;
        case 10: pp->setRecipientAddress(string2long(value)); return true;
        case 11: pp->setSerial(string2long(value)); return true;
        case 13: pp->setTimestamp(string2double(value)); return true;
        case 15: pp->setIsBroadcast(string2bool(value)); return true;
        case 16: pp->setMsgState(string2long(value)); return true;
        case 17: pp->setNodeNextRoadId((value)); return true;
        case 18: pp->setCurrentRoadId((value)); return true;
        case 23: pp->setDeparture(string2bool(value)); return true;
        case 24: pp->setCHId((value)); return true;
        case 25: pp->setCH_Speed(string2long(value)); return true;
        case 26: pp->setGatewayNode(string2bool(value)); return true;
        case 29: pp->setMsgCode(string2long(value)); return true;
        case 30: pp->setUsedFor(string2long(value)); return true;
        default: return false;
    }
}

const char *WaveShortMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 12: return opp_typename(typeid(Coord));
        case 14: return opp_typename(typeid(LAddress::L2Type));
        case 19: return opp_typename(typeid(infoDSRC));
        case 20: return opp_typename(typeid(infoDSRC));
        case 21: return opp_typename(typeid(clusterQueue));
        case 22: return opp_typename(typeid(clusterQueue));
        case 27: return opp_typename(typeid(InfoGW));
        case 28: return opp_typename(typeid(InfoGWToLte));
        default: return NULL;
    };
}

void *WaveShortMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    WaveShortMessage *pp = (WaveShortMessage *)object; (void)pp;
    switch (field) {
        case 12: return (void *)(&pp->getSenderPos()); break;
        case 14: return (void *)(&pp->getMacAddr()); break;
        case 19: return (void *)(&pp->getCHInfo()); break;
        case 20: return (void *)(&pp->getTempCHInfo()); break;
        case 21: return (void *)(&pp->getTempCHRoadID()); break;
        case 22: return (void *)(&pp->getCHRoadID()); break;
        case 27: return (void *)(&pp->getInfoGw()); break;
        case 28: return (void *)(&pp->getInfoGWToLte()); break;
        default: return NULL;
    }
}


