#ifndef _ETH_APP_H_
#define _ETH_APP_H_

#include "psock.h"
#include "pt.h"

struct app_state {
	unsigned char timer;
	struct psock sin;
	struct psock sout;
	struct pt outputpt;
	struct pt scriptpt;
	char state;
	char inputbuf[50];
};

typedef struct app_state uip_tcp_appstate_t;

void net_app_callback(void);
#define UIP_APPCALL     net_app_callback

#endif
