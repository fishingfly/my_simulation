//
// Generated file, do not edit! Created by nedtool 4.6 from veins/modules/heterogeneous/messages/HeterogeneousMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "HeterogeneousMessage_m.h"

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

Register_Class(HeterogeneousMessage);

HeterogeneousMessage::HeterogeneousMessage(const char *name, int kind) : ::WaveShortMessage(name,kind)
{
    this->sourceAddress_var = 0;
    this->destinationAddress_var = 0;
    this->networkType_var = 0;
    this->sendingTime_var = 0;
}

HeterogeneousMessage::HeterogeneousMessage(const HeterogeneousMessage& other) : ::WaveShortMessage(other)
{
    copy(other);
}

HeterogeneousMessage::~HeterogeneousMessage()
{
}

HeterogeneousMessage& HeterogeneousMessage::operator=(const HeterogeneousMessage& other)
{
    if (this==&other) return *this;
    ::WaveShortMessage::operator=(other);
    copy(other);
    return *this;
}

void HeterogeneousMessage::copy(const HeterogeneousMessage& other)
{
    this->sourceAddress_var = other.sourceAddress_var;
    this->destinationAddress_var = other.destinationAddress_var;
    this->networkType_var = other.networkType_var;
    this->sendingTime_var = other.sendingTime_var;
}

void HeterogeneousMessage::parsimPack(cCommBuffer *b)
{
    ::WaveShortMessage::parsimPack(b);
    doPacking(b,this->sourceAddress_var);
    doPacking(b,this->destinationAddress_var);
    doPacking(b,this->networkType_var);
    doPacking(b,this->sendingTime_var);
}

void HeterogeneousMessage::parsimUnpack(cCommBuffer *b)
{
    ::WaveShortMessage::parsimUnpack(b);
    doUnpacking(b,this->sourceAddress_var);
    doUnpacking(b,this->destinationAddress_var);
    doUnpacking(b,this->networkType_var);
    doUnpacking(b,this->sendingTime_var);
}

const char * HeterogeneousMessage::getSourceAddress() const
{
    return sourceAddress_var.c_str();
}

void HeterogeneousMessage::setSourceAddress(const char * sourceAddress)
{
    this->sourceAddress_var = sourceAddress;
}

const char * HeterogeneousMessage::getDestinationAddress() const
{
    return destinationAddress_var.c_str();
}

void HeterogeneousMessage::setDestinationAddress(const char * destinationAddress)
{
    this->destinationAddress_var = destinationAddress;
}

int HeterogeneousMessage::getNetworkType() const
{
    return networkType_var;
}

void HeterogeneousMessage::setNetworkType(int networkType)
{
    this->networkType_var = networkType;
}

simtime_t HeterogeneousMessage::getSendingTime() const
{
    return sendingTime_var;
}

void HeterogeneousMessage::setSendingTime(simtime_t sendingTime)
{
    this->sendingTime_var = sendingTime;
}

class HeterogeneousMessageDescriptor : public cClassDescriptor
{
  public:
    HeterogeneousMessageDescriptor();
    virtual ~HeterogeneousMessageDescriptor();

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

Register_ClassDescriptor(HeterogeneousMessageDescriptor);

HeterogeneousMessageDescriptor::HeterogeneousMessageDescriptor() : cClassDescriptor("HeterogeneousMessage", "WaveShortMessage")
{
}

HeterogeneousMessageDescriptor::~HeterogeneousMessageDescriptor()
{
}

bool HeterogeneousMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<HeterogeneousMessage *>(obj)!=NULL;
}

const char *HeterogeneousMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int HeterogeneousMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int HeterogeneousMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *HeterogeneousMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sourceAddress",
        "destinationAddress",
        "networkType",
        "sendingTime",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int HeterogeneousMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sourceAddress")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationAddress")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "networkType")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "sendingTime")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *HeterogeneousMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "int",
        "simtime_t",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *HeterogeneousMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int HeterogeneousMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    HeterogeneousMessage *pp = (HeterogeneousMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string HeterogeneousMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    HeterogeneousMessage *pp = (HeterogeneousMessage *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getSourceAddress());
        case 1: return oppstring2string(pp->getDestinationAddress());
        case 2: return long2string(pp->getNetworkType());
        case 3: return double2string(pp->getSendingTime());
        default: return "";
    }
}

bool HeterogeneousMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    HeterogeneousMessage *pp = (HeterogeneousMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setSourceAddress((value)); return true;
        case 1: pp->setDestinationAddress((value)); return true;
        case 2: pp->setNetworkType(string2long(value)); return true;
        case 3: pp->setSendingTime(string2double(value)); return true;
        default: return false;
    }
}

const char *HeterogeneousMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *HeterogeneousMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    HeterogeneousMessage *pp = (HeterogeneousMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


