

/* Simple types.  */
FUNDAMENTAL (ADDR, Addr, LIBELFBITS);
FUNDAMENTAL (OFF, Off, LIBELFBITS);
FUNDAMENTAL (HALF, Half, LIBELFBITS);
FUNDAMENTAL (WORD, Word, LIBELFBITS);
FUNDAMENTAL (SWORD, Sword, LIBELFBITS);
FUNDAMENTAL (XWORD, Xword, LIBELFBITS);
FUNDAMENTAL (SXWORD, Sxword, LIBELFBITS);

/* The structured types.  */
TYPE (Ehdr, LIBELFBITS)
TYPE (Phdr, LIBELFBITS)
TYPE (Shdr, LIBELFBITS)
TYPE (Sym, LIBELFBITS)
TYPE (Rel, LIBELFBITS)
TYPE (Rela, LIBELFBITS)
TYPE (Note, LIBELFBITS)
TYPE (Dyn, LIBELFBITS)
TYPE (Syminfo, LIBELFBITS)
TYPE (Move, LIBELFBITS)
TYPE (Lib, LIBELFBITS)


/* Prepare for the next round.  */
#undef LIBELFBITS
