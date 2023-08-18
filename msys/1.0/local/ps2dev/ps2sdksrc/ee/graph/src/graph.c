/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# (c) 2005 Dan Peori <peori@oopo.net>
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
*/

 #include <tamtypes.h>

 #include <dma.h>
 #include <graph.h>
 #include <stdio.h>
 #include <string.h>
 #include <kernel.h>
 #include <packet.h>
 #include <osd_config.h>

 GRAPH_MODE graph_mode[21] = {
  {  640,  448, 0x02, 1,  286720, GS_SET_DISPLAY(632, 50, 3, 0, 2559,  447) },
  {  640,  512, 0x03, 1,  327680, GS_SET_DISPLAY(652, 72, 3, 0, 2559,  511) },
  {  720,  480, 0x50, 0,  368640, GS_SET_DISPLAY(232, 35, 1, 0, 1439,  479) },
  { 1280,  720, 0x51, 0,  983040, GS_SET_DISPLAY(302, 24, 0, 0, 1279,  719) },
  { 1920, 1080, 0x52, 1, 2088960, GS_SET_DISPLAY(238, 40, 0, 0, 1919, 1079) },
  {  640,  480, 0x1A, 0,  327680, GS_SET_DISPLAY(276, 34, 1, 0, 1279,  479) },
  {  640,  480, 0x1B, 0,  327680, GS_SET_DISPLAY(276, 34, 1, 0, 1279,  479) },
  {  640,  480, 0x1C, 0,  327680, GS_SET_DISPLAY(276, 34, 1, 0, 1279,  479) },
  {  640,  480, 0x1D, 0,  327680, GS_SET_DISPLAY(276, 34, 1, 0, 1279,  479) },
  {  800,  600, 0x2A, 0,  512000, GS_SET_DISPLAY(420, 26, 1, 0, 1599,  599) },
  {  800,  600, 0x2B, 0,  512000, GS_SET_DISPLAY(420, 26, 1, 0, 1599,  599) },
  {  800,  600, 0x2C, 0,  512000, GS_SET_DISPLAY(420, 26, 1, 0, 1599,  599) },
  {  800,  600, 0x2D, 0,  512000, GS_SET_DISPLAY(420, 26, 1, 0, 1599,  599) },
  {  800,  600, 0x2E, 0,  512000, GS_SET_DISPLAY(420, 26, 1, 0, 1599,  599) },
  { 1024,  768, 0x3B, 0,  786432, GS_SET_DISPLAY(580, 34, 1, 0, 2047,  767) },
  { 1024,  768, 0x3C, 0,  786432, GS_SET_DISPLAY(580, 34, 1, 0, 2047,  767) },
  { 1024,  768, 0x3D, 0,  786432, GS_SET_DISPLAY(580, 34, 1, 0, 2047,  767) },
  { 1024,  768, 0x3E, 0,  786432, GS_SET_DISPLAY(580, 34, 1, 0, 2047,  767) },
  { 1280, 1024, 0x4A, 0, 1310720, GS_SET_DISPLAY(348, 40, 0, 0, 1279, 1023) },
  { 1280, 1024, 0x4B, 0, 1310720, GS_SET_DISPLAY(348, 40, 0, 0, 1279, 1023) },
  {  640,  224, 0x02, 0,  143360, GS_SET_DISPLAY(632, 30, 3, 0, 2559,  223) },
 };

 PACKET graph_packet;

 GRAPH_MODE current_graph_mode;

 int current_mode = -1;

 int current_psm = -1;

 int current_displaybuffer = -1;

 int current_drawbuffer = -1;

 int current_drawfield = -1;

 int current_zbuffer = -1;

 int current_zpsm = -1;

 /////////////////////
 // GRAPH FUNCTIONS //
 /////////////////////

 int graph_initialize(void) {

  // Reset the gif.
  ResetEE(0x08);

  // Reset and flush the gs.
  GS_REG_CSR = GS_SET_CSR(0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0);

  // Allocate the packet.
  if (packet_allocate(&graph_packet, 1024) < 0) { return -1; }

  // End function.
  return 0;

 }

 int graph_shutdown(void) {

  // Free the packet.
  if (packet_free(&graph_packet) < 0) { return -1; }

  // End function.
  return 0;

 }
 
 ////////////////////////////
 // GRAPH CONFIG FUNCTIONS //
 ////////////////////////////

 int graph_config_read(char *filename) {
  FILE *infile; char config[512];

  // Open the config file.
  if ((infile = fopen(filename, "r")) < 0) { return -1; }

  // Read the config file contents.
  if (fread(config, 1, sizeof(config), infile) < 0) { return -1; }

  // Close the config file.
  if (fclose(infile) < 0) { return -1; }

  // Set the current mode config information.
  return graph_set_config(config);

 }
 
 int graph_config_write(char *filename) {
  FILE *outfile; char config[512];

  // Get the current mode config information.  
  graph_get_config(config);

  // Open the config file.
  if ((outfile = fopen(filename, "w")) < 0) { return -1; }

  // Write the config file contents.
  if (fwrite(config, 1, strlen(config), outfile) < 0) { return -1; }

  // Close the config file.
  if (fclose(outfile) < 0) { return -1; } 

  // End function.
  return 0;

 }

 /////////////////////////
 // GRAPH GET FUNCTIONS //
 /////////////////////////

 float graph_get_aspect(void) {
  float aspect = 1.00f;

  // Get the tv screen type as defined in the osd configuration.
  if (configGetTvScreenType() == TV_SCREEN_169) { aspect = 1.78f; } else { aspect = 1.33f; }

  // Return the current aspect ratio.
  return (float)((float)aspect * (float)((float)graph_get_height() / (float)graph_get_width()));

 }

 int graph_get_bpp(void) {

  // Return the framebuffer bits-per-pixel.
  if (current_psm == GRAPH_PSM_32)  { return 32; } else
  if (current_psm == GRAPH_PSM_24)  { return 24; } else
  if (current_psm == GRAPH_PSM_16)  { return 16; } else
  if (current_psm == GRAPH_PSM_16S) { return 16; }

  // End function.
  return -1;

 }

 int graph_get_config(char *config) {

  // Save the current mode value.
  switch (current_mode) {
   case GRAPH_MODE_NTSC:	sprintf(config, "GRAPH_MODE_NTSC:");		break;
   case GRAPH_MODE_PAL:		sprintf(config, "GRAPH_MODE_PAL:");		break;
   case GRAPH_MODE_HDTV_480P:	sprintf(config, "GRAPH_MODE_HDTV_480P:");	break;
   case GRAPH_MODE_HDTV_720P:	sprintf(config, "GRAPH_MODE_HDTV_720P:");	break;
   case GRAPH_MODE_HDTV_1080I:	sprintf(config, "GRAPH_MODE_HDTV_1080I:");	break;
   case GRAPH_MODE_VGA_640_60:	sprintf(config, "GRAPH_MODE_VGA_640_60:");	break;
   case GRAPH_MODE_VGA_640_72:	sprintf(config, "GRAPH_MODE_VGA_640_72:");	break;
   case GRAPH_MODE_VGA_640_75:	sprintf(config, "GRAPH_MODE_VGA_640_75:");	break;
   case GRAPH_MODE_VGA_640_85:	sprintf(config, "GRAPH_MODE_VGA_640_85:");	break;
   case GRAPH_MODE_VGA_800_56:	sprintf(config, "GRAPH_MODE_VGA_800_56:");	break;
   case GRAPH_MODE_VGA_800_60:	sprintf(config, "GRAPH_MODE_VGA_800_60:");	break;
   case GRAPH_MODE_VGA_800_72:	sprintf(config, "GRAPH_MODE_VGA_800_72:");	break;
   case GRAPH_MODE_VGA_800_75:	sprintf(config, "GRAPH_MODE_VGA_800_75:");	break;
   case GRAPH_MODE_VGA_800_85:	sprintf(config, "GRAPH_MODE_VGA_800_85:");	break;
   case GRAPH_MODE_VGA_1024_60:	sprintf(config, "GRAPH_MODE_VGA_1024_60:");	break;
   case GRAPH_MODE_VGA_1024_70:	sprintf(config, "GRAPH_MODE_VGA_1024_70:");	break;
   case GRAPH_MODE_VGA_1024_75:	sprintf(config, "GRAPH_MODE_VGA_1024_75:");	break;
   case GRAPH_MODE_VGA_1024_85:	sprintf(config, "GRAPH_MODE_VGA_1024_85:");	break;
   case GRAPH_MODE_VGA_1280_60:	sprintf(config, "GRAPH_MODE_VGA_1280_60:");	break;
   case GRAPH_MODE_VGA_1280_75:	sprintf(config, "GRAPH_MODE_VGA_1280_75:");	break;
   default:			sprintf(config, "GRAPH_MODE_AUTO:");		break;
  }

  // Save the current psm value.
  switch (current_psm) {
   case GRAPH_PSM_32:		sprintf(config, "%sGRAPH_PSM_32:",  config);	break;
   case GRAPH_PSM_24:		sprintf(config, "%sGRAPH_PSM_24:",  config);	break;
   case GRAPH_PSM_16:		sprintf(config, "%sGRAPH_PSM_16:",  config);	break;
   case GRAPH_PSM_16S:		sprintf(config, "%sGRAPH_PSM_16S:", config);	break;
   default:			sprintf(config, "%sGRAPH_PSM_32:",  config);	break;
  }

  // Save the current zpsm value.
  switch (current_zpsm) {
   case GRAPH_PSM_32:		sprintf(config, "%sGRAPH_PSM_32:",  config);	break;
   case GRAPH_PSM_24:		sprintf(config, "%sGRAPH_PSM_24:",  config);	break;
   case GRAPH_PSM_16:		sprintf(config, "%sGRAPH_PSM_16:",  config);	break;
   case GRAPH_PSM_16S:		sprintf(config, "%sGRAPH_PSM_16S:", config);	break;
   default:			sprintf(config, "%sGRAPH_PSM_32:",  config);	break;
  }

  // End function.
  return 0;

 }

 int graph_get_displaybuffer(void) {

  // Return the vram address of the display buffer, in bytes.
  return current_displaybuffer;

 }

 int graph_get_displayfield(void) {

  // If the current mode is interlaced...
  if (graph_get_interlace() == 1) {

   // Return the currently displayed field value.
   if (GS_REG_CSR & (1 << 13)) { return GRAPH_FIELD_ODD; } else { return GRAPH_FIELD_EVEN; }

  }

  // Else, the current mode is non-interlaced...
  return GRAPH_FIELD_BOTH;

 }

 int graph_get_drawbuffer(void) {

  // Return the vram address of the draw buffer, in bytes.
  return current_drawbuffer;

 }

 int graph_get_drawfield(void) {

  // If the current mode is interlaced...
  if (graph_get_interlace() == 1) {

   // Return the current draw field value.
   if (GS_REG_CSR & (1 << 13)) { return GRAPH_FIELD_EVEN; } else { return GRAPH_FIELD_ODD; }

  }

  // Else, the mode is non-interlaced...
  return GRAPH_FIELD_BOTH;

 }

 int graph_get_height(void) {

  // Return the framebuffer height, in pixels.
  return current_graph_mode.height;

 }

 int graph_get_interlace(void) {

  // Return the interlacing value for the current mode.
  return current_graph_mode.interlace;

 }

 int graph_get_psm(void) {

  // Return the the framebuffer pixel storage method.
  return current_psm;

 }

 int graph_get_size(void) {

  // Return the size of the framebuffer, in bytes.
  return current_graph_mode.size * (graph_get_bpp() >> 3);

 }

 int graph_get_width(void) {

  // Return the framebuffer width, in pixels.
  return current_graph_mode.width;

 }

 int graph_get_zbpp(void) {

  // Return the zbuffer bits-per-pixel.
  if (current_zpsm == GRAPH_PSM_32)  { return 32; } else
  if (current_zpsm == GRAPH_PSM_24)  { return 24; } else
  if (current_zpsm == GRAPH_PSM_16)  { return 16; } else
  if (current_zpsm == GRAPH_PSM_16S) { return 16; }

  // End function.
  return -1;

 }

 int graph_get_zbuffer(void) {

  // Return the vram address of the zbuffer, in bytes.
  return current_zbuffer;

 }

 int graph_get_zpsm(void) {

  // Return the zbuffer pixel storage method.
  return current_zpsm;

 }

 int graph_get_zsize(void) {

  // Return the size of the zbuffer, in bytes.
  return current_graph_mode.size * (graph_get_zbpp() >> 3);

 }

 /////////////////////////
 // GRAPH SET FUNCTIONS //
 /////////////////////////

 int graph_set_clearbuffer(int red, int green, int blue) {

  // Reset the packet.
  if (packet_reset(&graph_packet) < 0) { return -1; }

  // Clear the draw framebuffer and zbuffer.
  packet_append_64(&graph_packet, GIF_SET_TAG(6, 1, 0, 0, GIF_TAG_PACKED, 1));
  packet_append_64(&graph_packet, 0x000000000000000E);
  packet_append_64(&graph_packet, GIF_SET_TEST(0, 0, 0, 0, 0, 0, 1, 1));
  packet_append_64(&graph_packet, GIF_REG_TEST_1);
  packet_append_64(&graph_packet, GIF_SET_PRIM(6, 0, 0, 0, 0, 0, 0, 0, 0));
  packet_append_64(&graph_packet, GIF_REG_PRIM);
  packet_append_64(&graph_packet, GIF_SET_RGBAQ(red, green, blue, 0x80, 0x3F800000));
  packet_append_64(&graph_packet, GIF_REG_RGBAQ);
  packet_append_64(&graph_packet, GIF_SET_XYZ(0x0000, 0x0000, 0x0000));
  packet_append_64(&graph_packet, GIF_REG_XYZ2);
  packet_append_64(&graph_packet, GIF_SET_XYZ(0xFFFF, 0xFFFF, 0x0000));
  packet_append_64(&graph_packet, GIF_REG_XYZ2);
  packet_append_64(&graph_packet, GIF_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2));
  packet_append_64(&graph_packet, GIF_REG_TEST_1);

  // Send the packet.
  if (packet_send(&graph_packet, DMA_CHANNEL_GIF, DMA_FLAG_NORMAL) < 0) { return -1; }

  // End function.
  return 0;

 }

 int graph_set_config(char *config) {
  char *temp0, *temp1; int mode, psm, zpsm;

  // Extract the mode config value.
  temp0 = config; temp1 = strtok(temp0, ":"); temp0 += strlen(temp1) + 1;

  // Parse the mode config value.
  if (strcmp(temp1, "GRAPH_MODE_NTSC"        ) == 0) { mode = GRAPH_MODE_NTSC;        } else
  if (strcmp(temp1, "GRAPH_MODE_PAL"         ) == 0) { mode = GRAPH_MODE_PAL;         } else
  if (strcmp(temp1, "GRAPH_MODE_HDTV_480P"   ) == 0) { mode = GRAPH_MODE_HDTV_480P;   } else
  if (strcmp(temp1, "GRAPH_MODE_HDTV_720P"   ) == 0) { mode = GRAPH_MODE_HDTV_720P;   } else
  if (strcmp(temp1, "GRAPH_MODE_HDTV_1080I"  ) == 0) { mode = GRAPH_MODE_HDTV_1080I;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_640"     ) == 0) { mode = GRAPH_MODE_VGA_640;     } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_640_60"  ) == 0) { mode = GRAPH_MODE_VGA_640_60;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_640_72"  ) == 0) { mode = GRAPH_MODE_VGA_640_72;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_640_75"  ) == 0) { mode = GRAPH_MODE_VGA_640_75;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_640_85"  ) == 0) { mode = GRAPH_MODE_VGA_640_85;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_800"     ) == 0) { mode = GRAPH_MODE_VGA_800;     } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_800_56"  ) == 0) { mode = GRAPH_MODE_VGA_800_56;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_800_60"  ) == 0) { mode = GRAPH_MODE_VGA_800_60;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_800_72"  ) == 0) { mode = GRAPH_MODE_VGA_800_72;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_800_75"  ) == 0) { mode = GRAPH_MODE_VGA_800_75;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_800_85"  ) == 0) { mode = GRAPH_MODE_VGA_800_85;  } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1024"    ) == 0) { mode = GRAPH_MODE_VGA_1024;    } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1024_60" ) == 0) { mode = GRAPH_MODE_VGA_1024_60; } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1024_70" ) == 0) { mode = GRAPH_MODE_VGA_1024_70; } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1024_75" ) == 0) { mode = GRAPH_MODE_VGA_1024_75; } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1024_85" ) == 0) { mode = GRAPH_MODE_VGA_1024_85; } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1280"    ) == 0) { mode = GRAPH_MODE_VGA_1280;    } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1280_60" ) == 0) { mode = GRAPH_MODE_VGA_1280_60; } else
  if (strcmp(temp1, "GRAPH_MODE_VGA_1280_75" ) == 0) { mode = GRAPH_MODE_VGA_1280_75; } else { mode = GRAPH_MODE_AUTO; }

  // Read the psm config value.
  temp1 = strtok(temp0, ":"); temp0 += strlen(temp1) + 1;

  // Parse the psm config value.
  if (strcmp(temp1, "GRAPH_PSM_32"  ) == 0) { psm = GRAPH_PSM_32;  } else
  if (strcmp(temp1, "GRAPH_PSM_24"  ) == 0) { psm = GRAPH_PSM_24;  } else
  if (strcmp(temp1, "GRAPH_PSM_16"  ) == 0) { psm = GRAPH_PSM_16;  } else
  if (strcmp(temp1, "GRAPH_PSM_16S" ) == 0) { psm = GRAPH_PSM_16S; } else { psm = GRAPH_PSM_32; }

  // Read the zpsm config value.
  temp1 = strtok(temp0, ":"); temp0 += strlen(temp1) + 1;

  // Parse the zpsm config value.
  if (strcmp(temp1, "GRAPH_PSM_32" ) == 0) { zpsm = GRAPH_PSM_32;  } else
  if (strcmp(temp1, "GRAPH_PSM_24" ) == 0) { zpsm = GRAPH_PSM_24;  } else
  if (strcmp(temp1, "GRAPH_PSM_16" ) == 0) { zpsm = GRAPH_PSM_16;  } else
  if (strcmp(temp1, "GRAPH_PSM_16S") == 0) { zpsm = GRAPH_PSM_16S; } else { zpsm = GRAPH_PSM_32; }

  // Set the video mode with the configured values.
  if (graph_set_mode(mode, psm, zpsm) < 0) { return -1; }

  // End function.
  return 0;

 }

 int graph_set_displaybuffer(int address) {

  // Set up the display buffer.
  GS_REG_DISPFB1 = GS_SET_DISPFB(address >> 13, graph_get_width() >> 6, graph_get_psm(), 0, 0);
  GS_REG_DISPFB2 = GS_SET_DISPFB(address >> 13, graph_get_width() >> 6, graph_get_psm(), 0, 0);

  // Save the display buffer vram address.
  current_displaybuffer = address;

  // End function.
  return 0;

 }

 int graph_set_drawbuffer(int address) {

  // Reset the packet.
  if (packet_reset(&graph_packet) < 0) { return -1; }

  // Set up the draw buffer.
  packet_append_64(&graph_packet, GIF_SET_TAG(5, 1, 0, 0, GIF_TAG_PACKED, 1));
  packet_append_64(&graph_packet, 0x000000000000000E);
  packet_append_64(&graph_packet, GIF_SET_FRAME(address >> 13, graph_get_width() >> 6, graph_get_psm(), 0));
  packet_append_64(&graph_packet, GIF_REG_FRAME_1);
  packet_append_64(&graph_packet, GIF_SET_SCISSOR(0, graph_get_width() - 1, 0, graph_get_height() - 1));
  packet_append_64(&graph_packet, GIF_REG_SCISSOR_1);
  packet_append_64(&graph_packet, GIF_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2));
  packet_append_64(&graph_packet, GIF_REG_TEST_1);
  packet_append_64(&graph_packet, GIF_SET_XYOFFSET((2048 - (graph_get_width() >> 1)) << 4, (2048 - (graph_get_height() >> 1)) << 4));
  packet_append_64(&graph_packet, GIF_REG_XYOFFSET_1);
  packet_append_64(&graph_packet, GIF_SET_PRMODECONT(1));
  packet_append_64(&graph_packet, GIF_REG_PRMODECONT);

  // Send the packet.
  if (packet_send(&graph_packet, DMA_CHANNEL_GIF, DMA_FLAG_NORMAL) < 0) { return -1; }

  // Save the draw buffer vram address.
  current_drawbuffer = address;

  // End function.
  return 0;

 }

 int graph_set_drawfield(int field) {

  // Reset the packet.
  if (packet_reset(&graph_packet) < 0) { return -1; }

  // If the field is odd...
  if (field == GRAPH_FIELD_ODD) {

   // Draw only to odd fields.
   packet_append_64(&graph_packet, GIF_SET_TAG(1, 1, 0, 0, GIF_TAG_PACKED, 1));
   packet_append_64(&graph_packet, 0x000000000000000E);
   packet_append_64(&graph_packet, GIF_SET_SCANMSK(2));
   packet_append_64(&graph_packet, GIF_REG_SCANMSK);

  // Else if the field is even...
  } else if (field == GRAPH_FIELD_EVEN) {

   // Draw only to even fields.
   packet_append_64(&graph_packet, GIF_SET_TAG(1, 1, 0, 0, GIF_TAG_PACKED, 1));
   packet_append_64(&graph_packet, 0x000000000000000E);
   packet_append_64(&graph_packet, GIF_SET_SCANMSK(3));
   packet_append_64(&graph_packet, GIF_REG_SCANMSK);

  // Else, the mode is non-interlaced...
  } else {

   // Draw to all fields.
   packet_append_64(&graph_packet, GIF_SET_TAG(1, 1, 0, 0, GIF_TAG_PACKED, 1));
   packet_append_64(&graph_packet, 0x000000000000000E);
   packet_append_64(&graph_packet, GIF_SET_SCANMSK(0));
   packet_append_64(&graph_packet, GIF_REG_SCANMSK);

  }

  // Send the packet.
  if (packet_send(&graph_packet, DMA_CHANNEL_GIF, DMA_FLAG_NORMAL) < 0) { return -1; }

  // Save the draw field value.
  current_drawfield = field;

  // End function.
  return 0;

 }

 int graph_set_mode(int mode, int psm, int zpsm) {

  // If an automatic mode is requested...
  if (mode == GRAPH_MODE_AUTO) {

   // If the region is SCEE...
   if (*(volatile char *)(0x1FC7FF52) == 'E') {

    // The default mode is PAL.
    mode = GRAPH_MODE_PAL;

   // Else, the default mode is NTSC.
   } else { mode = GRAPH_MODE_NTSC; }

  }

  // Request the mode change.
  SetGsCrt(graph_mode[mode].interlace, graph_mode[mode].mode, 0);

  // Set up the mode.
  GS_REG_PMODE = GS_SET_PMODE(1, 1, 0, 0, 0, 128);
  GS_REG_DISPLAY1 = graph_mode[mode].display;
  GS_REG_DISPLAY2 = graph_mode[mode].display;

  // Save the current graph_mode information.
  current_graph_mode.width     = graph_mode[mode].width;
  current_graph_mode.height    = graph_mode[mode].height;
  current_graph_mode.mode      = graph_mode[mode].mode;
  current_graph_mode.interlace = graph_mode[mode].interlace;
  current_graph_mode.size      = graph_mode[mode].size;
  current_graph_mode.display   = graph_mode[mode].display;

  // Save the current mode.
  current_mode = mode;

  // Save the current psm.
  current_psm = psm;

  // Save the current zpsm.
  current_zpsm = zpsm;

  // End function.
  return 0;

 }

 int graph_set_mode_manual(int width, int height, int mode, int interlace, int size, u64 display, int psm, int zpsm) {

  // Request the mode change.
  SetGsCrt(interlace, mode, 0);

  // Set up the mode.
  GS_REG_PMODE = GS_SET_PMODE(1, 1, 0, 0, 0, 128);
  GS_REG_DISPLAY1 = display;
  GS_REG_DISPLAY2 = display;

  // Save the current mode information.
  current_graph_mode.width     = width;
  current_graph_mode.height    = height;
  current_graph_mode.mode      = mode;
  current_graph_mode.interlace = interlace;
  current_graph_mode.size      = size;
  current_graph_mode.display   = display;

  // Save the current psm.
  current_psm = psm;

  // Save the current zpsm.
  current_zpsm = zpsm;

  // End function.
  return 0;

 }

 int graph_set_texture(int address, int width, int height, int psm) {
  int tw, th;

  // Determine the TW value.
  if (width <=   2) { tw = 1; } else
  if (width <=   4) { tw = 2; } else
  if (width <=   8) { tw = 3; } else
  if (width <=  16) { tw = 4; } else
  if (width <=  32) { tw = 5; } else
  if (width <=  64) { tw = 6; } else
  if (width <= 128) { tw = 7; } else
  if (width <= 256) { tw = 8; } else
  if (width <= 512) { tw = 9; } else { tw = 10; }

  // Determine the TH value.
  if (height <=   2) { th = 1; } else
  if (height <=   4) { th = 2; } else
  if (height <=   8) { th = 3; } else
  if (height <=  16) { th = 4; } else
  if (height <=  32) { th = 5; } else
  if (height <=  64) { th = 6; } else
  if (height <= 128) { th = 7; } else
  if (height <= 256) { th = 8; } else
  if (height <= 512) { th = 9; } else { th = 10; }

  // Reset the packet.
  if (packet_reset(&graph_packet) < 0) { return -1; }

  // Set up the texture.
  packet_append_64(&graph_packet, GIF_SET_TAG(1, 1, 0, 0, GIF_TAG_PACKED, 4));
  packet_append_64(&graph_packet, 0x0000000000000EEEE);
  packet_append_64(&graph_packet, GIF_SET_CLAMP(0, 0, 0, 0, 0, 0));
  packet_append_64(&graph_packet, GIF_REG_CLAMP_1);
  packet_append_64(&graph_packet, GIF_SET_TEXA(0, 1, 255));
  packet_append_64(&graph_packet, GIF_REG_TEXA);
  packet_append_64(&graph_packet, GIF_SET_TEX0(address >> 8, width >> 6, psm, tw, th, 1, 0, 0, 0, 0, 0, 0));
  packet_append_64(&graph_packet, GIF_REG_TEX0_1);
  packet_append_64(&graph_packet, GIF_SET_TEX1(1, 0, 1, 1, 0, 0, 0));
  packet_append_64(&graph_packet, GIF_REG_TEX1_1);

  // Send the packet.
  if (packet_send(&graph_packet, DMA_CHANNEL_GIF, DMA_FLAG_NORMAL) < 0) { return -1; }

  // End function.
  return 0;

 }

 int graph_set_zbuffer(int address) {

  // Reset the packet.
  if (packet_reset(&graph_packet) < 0) { return -1; }

  // Set up the zbuffer.
  packet_append_64(&graph_packet, GIF_SET_TAG(1, 1, 0, 0, GIF_TAG_PACKED, 1));
  packet_append_64(&graph_packet, 0x000000000000000E);
  packet_append_64(&graph_packet, GIF_SET_ZBUF(address >> 13, graph_get_zpsm(), 0));
  packet_append_64(&graph_packet, GIF_REG_ZBUF_1);

  // Send the packet.
  if (packet_send(&graph_packet, DMA_CHANNEL_GIF, DMA_FLAG_NORMAL) < 0) { return -1; }

  // Save the zbuffer vram address.
  current_zbuffer = address;

  // End function.
  return 0;

 }

 //////////////////////////
 // GRAPH VRAM FUNCTIONS //
 //////////////////////////

 #define VIF1_REG_STAT *((vu32 *)(0x10003C00))

 int graph_vram_read(int address, int width, int height, int psm, void *data, int data_size) {

  // Reset the packet.
  if (packet_reset(&graph_packet) < 0) { return -1; }

  // Build the packet.
  packet_append_64(&graph_packet, GIF_SET_TAG(4, 1, 0, 0, GIF_TAG_PACKED, 1));
  packet_append_64(&graph_packet, 0x000000000000000E);
  packet_append_64(&graph_packet, GIF_SET_BITBLTBUF(address >> 8, width >> 6, psm, 0, 0, 0));
  packet_append_64(&graph_packet, GIF_REG_BITBLTBUF);
  packet_append_64(&graph_packet, GIF_SET_TRXPOS(0, 0, 0, 0, 0));
  packet_append_64(&graph_packet, GIF_REG_TRXPOS);
  packet_append_64(&graph_packet, GIF_SET_TRXREG(width, height));
  packet_append_64(&graph_packet, GIF_REG_TRXREG);
  packet_append_64(&graph_packet, GIF_SET_TRXDIR(1));
  packet_append_64(&graph_packet, GIF_REG_TRXDIR);

  // Send the packet.
  if (packet_send(&graph_packet, DMA_CHANNEL_GIF, DMA_FLAG_NORMAL) < 0) { return -1; }

  // Wait for the packet transfer to complete.
  if (dma_channel_wait(DMA_CHANNEL_GIF, -1, DMA_FLAG_NORMAL) < 0) { return -1; }

  // Reverse the bus direction.
  GS_REG_BUSDIR = GS_SET_BUSDIR(1); VIF1_REG_STAT = (1 << 23);

  // Receive the data.
  if (dma_channel_receive(DMA_CHANNEL_VIF1, data, data_size, DMA_FLAG_NORMAL) < 0) { return -1; }

  // Wait for the data transfer to complete.
  if (dma_channel_wait(DMA_CHANNEL_VIF1, -1, DMA_FLAG_NORMAL) < 0) { return -1; }

  // Restore the bus direction.
  GS_REG_BUSDIR = GS_SET_BUSDIR(0); VIF1_REG_STAT = (0 << 23);

  // Flush the cache, just in case.
  FlushCache(0);

  // End function.
  return 0;

 }

 int graph_vram_write(int address, int width, int height, int psm, void *data, int data_size) {

  // Reset the packet.
  if (packet_reset(&graph_packet) < 0) { return -1; }

  // Build the packet.
  packet_append_64(&graph_packet, DMA_SET_TAG(6, 0, DMA_TAG_CNT, 0, 0, 0));
  packet_append_64(&graph_packet, 0x0000000000000000);
  packet_append_64(&graph_packet, GIF_SET_TAG(4, 1, 0, 0, GIF_TAG_PACKED, 1));
  packet_append_64(&graph_packet, 0x000000000000000E);
  packet_append_64(&graph_packet, GIF_SET_BITBLTBUF(0, 0, 0, address >> 8, width >> 6, psm));
  packet_append_64(&graph_packet, GIF_REG_BITBLTBUF);
  packet_append_64(&graph_packet, GIF_SET_TRXPOS(0, 0, 0, 0, 0));
  packet_append_64(&graph_packet, GIF_REG_TRXPOS);
  packet_append_64(&graph_packet, GIF_SET_TRXREG(width, height));
  packet_append_64(&graph_packet, GIF_REG_TRXREG);
  packet_append_64(&graph_packet, GIF_SET_TRXDIR(0));
  packet_append_64(&graph_packet, GIF_REG_TRXDIR);
  packet_append_64(&graph_packet, GIF_SET_TAG(data_size >> 4, 1, 0, 0, GIF_TAG_IMAGE, 1));
  packet_append_64(&graph_packet, 0x0000000000000000);
  packet_append_64(&graph_packet, DMA_SET_TAG(data_size >> 4, 0, DMA_TAG_REF, 0, (u32)data, 0));
  packet_append_64(&graph_packet, 0x0000000000000000);
  packet_append_64(&graph_packet, DMA_SET_TAG(2, 0, DMA_TAG_END, 0, 0, 0));
  packet_append_64(&graph_packet, 0x0000000000000000);
  packet_append_64(&graph_packet, GIF_SET_TAG(1, 1, 0, 0, GIF_TAG_PACKED, 1));
  packet_append_64(&graph_packet, 0x000000000000000E);
  packet_append_64(&graph_packet, 0x0000000000000000);
  packet_append_64(&graph_packet, GIF_REG_TEXFLUSH);

  // Send the packet.
  if (packet_send(&graph_packet, DMA_CHANNEL_GIF, DMA_FLAG_CHAIN) < 0) { return -1; }

  // End function.
  return 0;

 }

 //////////////////////////
 // GRAPH WAIT FUNCTIONS //
 //////////////////////////

 int graph_wait_hsync(void) {

  // Enable the hsync interrupt.
  GS_REG_CSR |= GS_SET_CSR(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  // Wait for the hsync interrupt.
  while (!(GS_REG_CSR & (GS_SET_CSR(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)))) { }

  // Disable the hsync interrupt.
  GS_REG_CSR &= ~GS_SET_CSR(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  // End function.
  return 0;

 }

 int graph_wait_vsync(void) {

  // Enable the vsync interrupt.
  GS_REG_CSR |= GS_SET_CSR(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);

  // Wait for the vsync interrupt.
  while (!(GS_REG_CSR & (GS_SET_CSR(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0)))) { }

  // Disable the vsync interrupt.
  GS_REG_CSR &= ~GS_SET_CSR(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);

  // End function.
  return 0;

 }
