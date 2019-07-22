#include 	<stdio.h>
#include	<string.h>
#include	"esp_event_loop.h"	//	for usleep

#include	"neopixel.h"

#define	NEOPIXEL_PORT	18
#define	NR_LED		32
//#define	NR_LED		3
//#define	NEOPIXEL_WS2812
#define	NEOPIXEL_SK6812
#define	NEOPIXEL_RMT_CHANNEL		RMT_CHANNEL_1

static	void
test_neopixel()
{
	pixel_settings_t px;
	uint32_t		pixels[NR_LED];
	int		i;

	neopixel_init(NEOPIXEL_PORT, NEOPIXEL_RMT_CHANNEL);
	usleep(1000*1000);

	for	( i = 0 ; i < NR_LED ; i ++ )	{
		pixels[i] = 0;
	}
	px.pixels = (uint8_t *)pixels;
	px.pixel_count = NR_LED;
	strcpy(px.color_order, "GRB");

	memset(&px.timings, 0, sizeof(px.timings));
	px.timings.mark.level0 = 1;
	px.timings.space.level0 = 1;
	px.timings.mark.duration0 = 12;
#ifdef	NEOPIXEL_WS2812
	px.nbits = 24;
	px.timings.mark.duration1 = 14;
	px.timings.space.duration0 = 7;
	px.timings.space.duration1 = 16;
	px.timings.reset.duration0 = 600;
	px.timings.reset.duration1 = 600;
#endif
#ifdef	NEOPIXEL_SK6812
	px.nbits = 32;
	px.timings.mark.duration1 = 12;
	px.timings.space.duration0 = 6;
	px.timings.space.duration1 = 18;
	px.timings.reset.duration0 = 900;
	px.timings.reset.duration1 = 900;
#endif
	px.brightness = 0x80;
	np_show(&px, RMT_CHANNEL_1);

	int fact = 1;
	while (1) {
		usleep(1000*10);
		for	( int j = 0 ; j < NR_LED ; j += px.nbits / 8 )	{
			np_set_pixel_rgbw(&px, j + 0, i, i, i, i);
			np_set_pixel_rgbw(&px, j + 1, i, i, i, i);
			np_set_pixel_rgbw(&px, j + 2, i, i, i, i);
#ifdef	NEOPIXEL_SK6812
			np_set_pixel_rgbw(&px, j + 3, i, i, i, i);
#endif
		}
		np_show(&px, NEOPIXEL_RMT_CHANNEL);
		if ( fact > 0 ) {
			i += 1;
		} else {
			i -= 1;
		}
		if	( i == 255 )	{
			fact = -1;
		} else
		if	( i == 0 )	{
			fact = 1;
		}
	}

}

extern	void
app_main (void)
{
	test_neopixel();
}

