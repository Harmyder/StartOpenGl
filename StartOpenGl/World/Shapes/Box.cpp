#include "stdafx.h"

#include "World\Shapes\Box.h"

namespace World
{
    void Box::Accept(IViewportVisitor& vv)
    {
        vv.DrawBox(*this);
    }
}
