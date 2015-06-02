
#include "api.h"

#define API_BASE 0x08000100
#define API_SPI  (API_BASE + 0x10)
#define API_ETH  (API_SPI  + 0x18)
#define API_IP   (API_ETH  + 0x1C)
#define API_DHCP (API_IP   + 0x18)

void api_init(void)
{
	eth_init         = (int  (*)(void)   )*(u32 *)(API_ETH + 0x00);
	eth_periodic     = (void (*)(void)   )*(u32 *)(API_ETH + 0x04);
	eth_status       = (int  (*)(void)   )*(u32 *)(API_ETH + 0x08);
	eth_set_rx       = (void (*)(u8 *)   )*(u32 *)(API_ETH + 0x0C);
	eth_set_tx       = (void (*)(u8 *)   )*(u32 *)(API_ETH + 0x10);
	eth_set_callback = (void (*)(u32)    )*(u32 *)(API_ETH + 0x14);
	eth_stack_init   = (void (*)(void)   )*(u32 *)(API_ETH + 0x18);
	
	ip_tx             = (void (*)(int)    )*(u32 *)(API_IP  + 0x00);
	ip_prepare        = (void (*)(ip_hdr*))*(u32 *)(API_IP  + 0x04);
	ip_proto_register = (void (*)(u8,u32) )*(u32 *)(API_IP  + 0x08);
	ip_cksum = (u16 (*)(u32,const u8*,u16))*(u32 *)(API_IP  + 0x0C);
	htons             = (u16  (*)(u16)    )*(u32 *)(API_IP  + 0x10);
	htonl             = (u32  (*)(u32)    )*(u32 *)(API_IP  + 0x14);
	
	dhcp_periodic     = (void (*)(void)   )*(u32 *)(API_DHCP + 0x00);
}
/* EOF */
