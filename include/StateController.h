#ifndef _STATE_CONTROLLER_H
#define _STATE_CONTROLLER_H

#include "Configuration.h"
#include "BaseManager.h"

typedef enum {
    S_HOME_SCREEN = 0,
    S_INVOKING_CONFIG,
    S_CONFIG,
} state_t;

class CStateController: public CBaseManager {
    
public:
	CStateController(CDevice *d);
    void loop();

#ifdef KEYPAD
    virtual void keyEvent(key_status_t key);
#endif

private:
    unsigned long tMillis;
    unsigned long tMillisConfig;
    
    std::vector<CBaseManager*> managers;
    CBaseManager *configManager;
    CDevice *device;

    state_t state;

    void drawStateHome();
    void drawStateInvokeConfig();
    void drawStateConfig();
    
};

#endif