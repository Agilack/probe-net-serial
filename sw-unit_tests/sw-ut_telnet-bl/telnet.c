#include "hardware.h"
#include "main.h"
#include "uart.h"
#include "ip_tcp.h"

#define OPT_NAWS     31 /* Window size option (RFC1073) */
#define OPT_LINEMODE 34 /* (RFC1116) */

#define DRAW_COLOR_RED  0x31
#define DRAW_COLOR_BLUE 0x34
#define DRAW_COLOR_CYAN 0x36

enum telnet_state { NOCONN, CONFIG, CONFIG_WAIT, DRAW, IDLE };

static tcp_conn *telnet_conn;
static enum telnet_state state;
static int cols, rows;

int draw_color_back(u8 *buffer, int color);
int draw_color_front(u8 *buffer, int color);
int draw_erase_screen(u8 *buffer);
int draw_goto(u8 *buffer, int x, int y);
int draw_box (u8 *buffer, int x, int y, int w, int h);

static u8 bckg[] = {
    /* Set Color (back blue, front white) */
    0x1B,'[','4','4','m',  0x1B,'[','3','7','m',
    /* Erase screen */
    0x1B,'[','2','J',  0x1B,'[','H',
    /* Goto 4:20 */
    0x1B,'[','4',';','2','0','H',
    /* Set Special Character Mode */
    0x1B,'(','0',
    /* Show title line */
    0x6c,0x71,0x71,0x75, 0x1B,'(','B', ' ',
    'T','e','l','n','e','t',' ','U','n','i','t','-','t','e','s','t',' ',
    0x1B,'(','0', 0x74,0x71,0x71,0x71,0x71,0x71,0x71,0x6B,
    /* Show mid line */
    0x1B,'[','5',';','2','0','H',
    0x78,
    0x1B,'(','B', ' ','H','e','l','l','o',' ','W','o','r','l','d',' ','!',
    ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
    0x1B,'(','0', 0x78,
    /* Show bottom line */
    0x1B,'[','6',';','2','0','H',
    0x6D,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,
    0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,
    0x71,0x6A,
    0x1B,'(','B'
    };

void telnet_accept(tcp_conn *conn)
{
	uart_puts("TELNET: connection established\r\n");
	telnet_conn = conn;
	/* Set default terminal config */
	cols = 80;
	rows = 25;
	state = CONFIG;
}

void telnet_closed(void)
{
	uart_puts("TELNET: connection terminated\r\n");
}

void telnet_recv(u8 *d, int len)
{
	int i;
	int cmd;
	u8  *buffer, *txb;
	int len_max;
	
	uart_puts("TELNET: DATA len="); uart_puthex16(len); uart_crlf();
	for (i = 0; i < len; i++)
	{
		uart_puthex8(d[i]);
		uart_putc(' ');
	}
	uart_crlf();
	
	txb = tcp_buffer_tx(telnet_conn, &len_max);
	buffer = txb;
	len = 0;
	
	/* Special charater */
	if (d[0] == 0x1B)
	{
		if ( (d[1] == 0x5B) && (d[2] == 0x41) )
			uart_puts("Mouse UP\r\n");
		if ( (d[1] == 0x5B) && (d[2] == 0x42) )
			uart_puts("Mouse DOWN\r\n");
		if ( (d[1] == 0x5B) && (d[2] == 0x43) )
			uart_puts("Mouse RIGHT\r\n");
		if ( (d[1] == 0x5B) && (d[2] == 0x44) )
			uart_puts("Mouse LEFT\r\n");
	}
	/* Ctrl-C */
	if (d[0] == 0x03)
	{
		tcp_send(telnet_conn, (u8 *)"\x1B(B\x1B[39m\x1B[49m", 13);
		tcp_close(telnet_conn);
		return;
	}
	/* Ctrl-D */
	if (d[0] == 0x04) { }
	
	// IAC
	if (d[0] == 0xFF)
	{
		// SB: Subnegociation of option
		if (d[1] == 0xFA)
		{
			if (d[2] == OPT_LINEMODE)
			{
			}
			if (d[2] == OPT_NAWS)
			{
				cols = (d[3] << 8) | d[4];
				rows = (d[5] << 8) | d[6];
				state = DRAW;
			}
		}
	}
	
	cmd = d[0] - '0';
	
	if (state == CONFIG)
	{
		// IAC DO NAWS
		memcpy(buffer, "\xFF\xFD\x1F", 3);
		buffer += 3;
		
		// IAC WILL ECHO
		memcpy(buffer, "\xFF\xFB\x01", 3);
		buffer += 3;
		
		// IAC DO LINEMODE
		memcpy(buffer, "\xFF\xFD\x22", 3);
		buffer += 3;
		
		tcp_tx(telnet_conn, 0, buffer - txb);
		state = CONFIG_WAIT;
	}
	else if (state == DRAW)
	{
		u8 msg[] = "Hello World !";
		int x, y;
		int l;
		
		l = sizeof(msg);
		
		buffer += draw_color_front(buffer, DRAW_COLOR_CYAN);
		buffer += draw_color_back(buffer, DRAW_COLOR_BLUE);
		buffer += draw_erase_screen(buffer);
		
		x = (cols - l + 4) / 2;
		y = (rows - 3) / 2;
		buffer += draw_box(buffer, x, y, l + 4, 3);
		
		buffer += draw_goto(buffer, x + 2, y + 1);
		memcpy(buffer, msg, sizeof(msg));
		buffer += sizeof(msg);
		
		uart_puts("DRAW buffer len="); uart_puthex(buffer - txb);
		uart_crlf();
		tcp_tx(telnet_conn, 0, buffer - txb);
		state = NOCONN;
	}
	/* after reconfig */
	else if (state == IDLE)
	{
		tcp_send(telnet_conn, bckg, sizeof(bckg) );
	}
	else if (state == IDLE)
	{
		if (cmd == 0)
		{
			u8 buf[] = "\x71\x71\x75\x1B(B Telnet \x1B(0\x74\x71\x71\x71\x1B(B";
			tcp_send(telnet_conn, buf, 24);
		}
		else if (cmd == 1)
			// Erase Screen
			tcp_send(telnet_conn, (u8 *)"\x1B[2J\x1B[H", 7);
		else if (cmd == 2)
		//	u8 color[] = "\x1B[31m"; // 5
			tcp_send(telnet_conn, (u8 *)"\x1B[10;33H", 8);
		else if (cmd == 3)
			tcp_send(telnet_conn, (u8 *)"\x1B[1K", 4);
		else if (cmd == 4)
			// Special graphic character mode
			tcp_send(telnet_conn, (u8 *)"\x1B(0", 3);
		else if (cmd == 5)
			tcp_send(telnet_conn, (u8*)"\x6C\x71\x71\x71\x6B", 5);
		else if (cmd == 6)
			tcp_send(telnet_conn, (u8 *)"\x1B(B", 3);
		else if (cmd == 7)
			tcp_send(telnet_conn, (u8*)"\x1B[11;34HHello", 13);
	}
}

int draw_color_back(u8 *buffer, int color)
{
	u8 cmd[] = "\x1B[49m";
	memcpy(buffer, cmd, 5);
	buffer[3] = color;
	return(5);
}

int draw_color_front(u8 *buffer, int color)
{
	u8 cmd[] = "\x1B[39m";
	memcpy(buffer, cmd, 5);
	buffer[3] = color;
	return(5);
}

int draw_erase_screen(u8 *buffer)
{
	u8 cmd[] = "\x1B[2J\x1B[H";
	memcpy(buffer, cmd, 7);
	return(7);
}

int draw_goto(u8 *buffer, int x, int y)
{
	u8 cmd_goto[] = "\x1B[01;01H";

	cmd_goto[2] = '0' + (y / 10);
	cmd_goto[3] = '0' + (y % 10);
	cmd_goto[5] = '0' + (x / 10);
	cmd_goto[6] = '0' + (x % 10);
	memcpy(buffer, cmd_goto, 8);
	
	return(8);
}

int draw_box(u8 *buffer, int x, int y, int w, int h)
{
	u8  *b;
	int i, y1;
	u8 cmd_mg[]   = "\x1B(0";
	u8 cmd_mn[]   = "\x1B(B";
	
	b = buffer;
	
	/* Switch to Graphic mode */
	memcpy(b, cmd_mg, 3);
	b += 3;
	
	/* Draw the first (upper) line */
	b += draw_goto(b, x, y);
	b[0] = 0x6C; /* Upper left corner */
	for (i = 1; i < (w - 1); i++)
		b[i] = 0x71; /* Horizontal line */
	b[w - 1] = 0x6B; /* Upper right corner */
	b += w;
	
	/* Draw the left and right borders */
	y1 = y + 1;
	for (i = 1; i < (h - 1); i++)
	{
		b += draw_goto(b, x, y1);
		*b++ = 0x78;
		b += draw_goto(b, x + w - 1, y1);
		*b++ = 0x78;
		y1++;
	}
	
	/* Draw the bottom line */
	b += draw_goto(b, x, y1);
	b[0] = 0x6D;
	for (i = 1; i < (w - 1); i++)
		b[i] = 0x71;
	b[w - 1] = 0x6A;
	b += w;
	
	/* Switch to normal (text) mode */
	memcpy(b, cmd_mn, 3);
	b += 3;
	
	return (b - buffer);
}
/* EOF */
