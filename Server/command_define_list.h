#define Logger      0x10
#define Verifier    0x11
#define Server      0x12
#define WebUI       0x13

#define Char 0xa0
#define Uchar 0xa1
#define Int 0xb0
#define Uint 0xb1

#define PUBKEY_SND      0x01
#define PUBKEY_RES      0xf1
//#define PRIKEY_SND      0x01
//#define PRIKEY_RES      0xf1
#define VIDEO_DATA_SND  0x02
#define VIDEO_DATA_RES  0xf2
#define HASH_REQ        0x03
#define HASH_SND        0xf3
#define NEW_HASH_SND    0x04
#define NEW_HASH_RES    0xf4
#define CID_REQ         0x05
#define CID_SND         0xf5
#define VER_REQ         0x06
#define VER_RES         0xf6
#define VER_RSLT_SND    0x07
#define VER_RSLT_RES    0xf7
#define CONFIG_REQ      0x08
#define CONFIG_RES      0xf8
#define HI_I_M          0x09
#define NICE_2_MEET_U   0xf9

#define STILL_ALIVE     0xee
#define TEST_CMD        0xff