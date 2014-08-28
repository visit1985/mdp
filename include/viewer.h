#if !defined( VIEWER_H )
#define VIEWER_H

#define CP_WHITE  1 // 255
#define CP_BLUE   2 // 123
#define CP_RED    3 // 213
#define CP_YELLOW 4 // 208

static short white_ramp[24] = { 16, 232, 233, 234, 235, 236,
                               237, 238, 239, 240, 241, 242,
                               244, 245, 246, 247, 248, 249,
                               250, 251, 252, 253, 254, 255 };

static short blue_ramp[24]  = { 16,  17,  17,  18,  18,  19,
                                19,  20,  20,  21,  27,  32,
                                33,  38,  39,  44,  45,  45,
                                81,  81,  51,  51, 123, 123 };

static short red_ramp[24]   = { 16,  52,  52,  53,  53,  89,
                                89,  90,  90, 126, 127, 127,
                               163, 163, 164, 164, 200, 200,
                               201, 201, 207, 207, 213, 213 };

#endif // !defined( VIEWER_H )
