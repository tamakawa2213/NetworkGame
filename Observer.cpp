#include "Observer.h"

namespace
{
    bool Winner = false;
}

namespace Observer
{
    void SetWinner(bool result)
    {
        Winner = result;
    }

    bool GetWinner()
    {
        return Winner;
    }

}
