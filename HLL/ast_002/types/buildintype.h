#ifndef BUILDINTYPE_H
#define BUILDINTYPE_H

#include "buildin/maptype.h"
#include "buildin/stacktype.h"
#include "buildin/vectortype.h"


namespace qyvlik {
namespace typer {

typedef BaseTypeMetaData< qint8>     Int8TypeMetaData;
typedef BaseTypeMetaData<qint16>    Int16TypeMetaData;
typedef BaseTypeMetaData<qint32>    Int32TypeMetaData;
typedef BaseTypeMetaData<qint64>    Int64TypeMetaData;
typedef BaseTypeMetaData<double>    DoubleTypeMetaData;
typedef BaseTypeMetaData<QString>   StringTypeMetaData;

}
}

#endif // BUILDINTYPE_H

