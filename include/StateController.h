#ifndef _STATE_CONTROLLER_H
#define _STATE_CONTROLLER_H

#include "Configuration.h"
#include "BaseManager.h"

class CStateController {
    
public:
	CStateController(CDevice *d);
    void loop();

private:
    unsigned long tMillis;

    std::vector<CBaseManager*> managers;

    CDevice *device;
};

#endif