#include "stdafx.h"

#include "World\Shapes\Box.h"

using namespace std;

namespace World
{
    void Box::Accept(IViewportVisitor& vv)
    {
        vv.DrawBox(*this);
    }
}
