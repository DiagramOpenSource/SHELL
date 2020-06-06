/*

    Mu-law to linear conversion

*/

extern const unsigned short u2s[];
extern const unsigned char s2u[];

#define audio_u2s(x)  ((short) (u2s[(unsigned  char) (x)]))
#define audio_s2u(x)  (s2u[((unsigned short) (x)) >> 3])

/* Note that 8 bit PCM samples are unsigned, with 0x80
   representing the zero crossing and 0x0 and 0xFF the
   extrema. */

#define audio_u2c(x)   (0x80 + ((unsigned char) (u2s[(unsigned  char) (x)] >> 8)))
#define audio_c2u(x)  (s2u[(((short) ((((char) (x)) - 0x80) & 0xFF)) << 5) + 0xF])
