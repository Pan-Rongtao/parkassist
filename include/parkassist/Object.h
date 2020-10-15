#pragma once
#include "parkassist/Def.h"

namespace nb {

class NB_API Object
{
public:
	virtual ~Object() = default;

protected:
	Object() = default;
};

using ObjectPtr = std::shared_ptr<Object>;

}