
#include "universalid.hpp"

#include <ostream>
#include <stdexcept>
#include <sstream>

namespace
{
    struct TypeData
    {
            CSMWorld::UniversalId::Class mClass;
            CSMWorld::UniversalId::Type mType;
            const char *mName;
    };

    static const TypeData sNoArg[] =
    {
        { CSMWorld::UniversalId::Class_None, CSMWorld::UniversalId::Type_None, "empty" },
        { CSMWorld::UniversalId::Class_RecordList, CSMWorld::UniversalId::Type_Globals, "Global Variables" },

        { CSMWorld::UniversalId::Class_None, CSMWorld::UniversalId::Type_None, 0 } // end marker
    };

    static const TypeData sIdArg[] =
    {

        { CSMWorld::UniversalId::Class_None, CSMWorld::UniversalId::Type_None, 0 } // end marker
    };

    static const TypeData sIndexArg[] =
    {

        { CSMWorld::UniversalId::Class_None, CSMWorld::UniversalId::Type_None, 0 } // end marker
    };
}

CSMWorld::UniversalId::UniversalId (Type type) : mArgumentType (ArgumentType_None), mType (type), mIndex (0)
{
    for (int i=0; sNoArg[i].mName; ++i)
        if (type==sNoArg[i].mType)
        {
            mClass = sNoArg[i].mClass;
            return;
        }

    throw std::logic_error ("invalid argument-less UniversalId type");
}

CSMWorld::UniversalId::UniversalId (Type type, const std::string& id)
: mArgumentType (ArgumentType_Id), mType (type), mId (id), mIndex (0)
{
    for (int i=0; sIdArg[i].mName; ++i)
        if (type==sIdArg[i].mType)
        {
            mClass = sIdArg[i].mClass;
            return;
        }

    throw std::logic_error ("invalid ID argument UniversalId type");
}

CSMWorld::UniversalId::UniversalId (Type type, int index)
: mArgumentType (ArgumentType_Index), mType (type), mIndex (index)
{
    for (int i=0; sIndexArg[i].mName; ++i)
        if (type==sIndexArg[i].mType)
        {
            mClass = sIndexArg[i].mClass;
            return;
        }

    throw std::logic_error ("invalid index argument UniversalId type");
}

CSMWorld::UniversalId::Class CSMWorld::UniversalId::getClass() const
{
    return mClass;
}

CSMWorld::UniversalId::ArgumentType CSMWorld::UniversalId::getArgumentType() const
{
    return mArgumentType;
}

CSMWorld::UniversalId::Type CSMWorld::UniversalId::getType() const
{
    return mType;
}

const std::string& CSMWorld::UniversalId::getId() const
{
    if (mArgumentType!=ArgumentType_Id)
        throw std::logic_error ("invalid access to ID of non-ID UniversalId");

    return mId;
}

int CSMWorld::UniversalId::getIndex() const
{
    if (mArgumentType!=ArgumentType_Index)
        throw std::logic_error ("invalid access to index of non-index UniversalId");

    return mIndex;
}

bool CSMWorld::UniversalId::isEqual (const UniversalId& universalId) const
{
    if (mClass!=universalId.mClass || mArgumentType!=universalId.mArgumentType || mType!=universalId.mType)
            return false;

    switch (mArgumentType)
    {
        case ArgumentType_Id: return mId==universalId.mId;
        case ArgumentType_Index: return mIndex==universalId.mIndex;

        default: return true;
    }
}

bool CSMWorld::UniversalId::isLess (const UniversalId& universalId) const
{
    if (mType<universalId.mType)
        return true;

    if (mType>universalId.mType)
        return false;

    switch (mArgumentType)
    {
        case ArgumentType_Id: return mId<universalId.mId;
        case ArgumentType_Index: return mIndex<universalId.mIndex;

        default: return false;
    }
}

std::string CSMWorld::UniversalId::getTypeName() const
{
    const TypeData *typeData = mArgumentType==ArgumentType_None ? sNoArg :
        (mArgumentType==ArgumentType_Id ? sIdArg : sIndexArg);

    for (int i=0; typeData[i].mName; ++i)
        if (typeData[i].mType==mType)
            return typeData[i].mName;

    throw std::logic_error ("failed to retrieve UniversalId type name");
}

std::string CSMWorld::UniversalId::toString() const
{
    std::ostringstream stream;

    stream << getTypeName();

    switch (mArgumentType)
    {
        case ArgumentType_None: break;
        case ArgumentType_Id: stream << " " << mId;
        case ArgumentType_Index: stream << " " << mIndex;
    }

    return stream.str();
}

bool CSMWorld::operator== (const CSMWorld::UniversalId& left, const CSMWorld::UniversalId& right)
{
    return left.isEqual (right);
}

bool CSMWorld::operator!= (const CSMWorld::UniversalId& left, const CSMWorld::UniversalId& right)
{
    return !left.isEqual (right);
}

bool CSMWorld::operator< (const UniversalId& left, const UniversalId& right)
{
    return left.isLess (right);
}

std::ostream& CSMWorld::operator< (std::ostream& stream, const CSMWorld::UniversalId& universalId)
{
    return stream << universalId.toString();
}