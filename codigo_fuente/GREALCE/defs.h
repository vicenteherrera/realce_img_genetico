//#define CBuilder
#define CVIP_tools

#ifdef CBuilder
#define GRF_EXT ".bmp"
#define Color_White 0x00FFFFFF
#else
#define GRF_EXT ".jpg"
#define GRF_FORMAT JPG
#define BANDS 1
#define Color_White 255
#endif