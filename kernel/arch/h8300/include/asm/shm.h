
#ifndef _H8300_SHM_H
#define _H8300_SHM_H


/* on the m68k both bits 0 and 1 must be zero */

#ifndef CONFIG_SUN3
#define SHM_ID_SHIFT	9
#else
#define SHM_ID_SHIFT	7
#endif
#define _SHM_ID_BITS	7
#define SHM_ID_MASK	((1<<_SHM_ID_BITS)-1)

#define SHM_IDX_SHIFT	(SHM_ID_SHIFT+_SHM_ID_BITS)
#define _SHM_IDX_BITS	15
#define SHM_IDX_MASK	((1<<_SHM_IDX_BITS)-1)

#endif /* _H8300_SHM_H */
