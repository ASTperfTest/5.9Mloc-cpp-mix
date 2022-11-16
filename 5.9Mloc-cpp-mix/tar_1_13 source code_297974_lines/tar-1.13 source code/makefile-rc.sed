s,$[({]top_builddir[)}]/intl/libintl.a,-lintl,
s/^tar_OBJECTS.*=/& tar-rc.o/
/^.cc*.o:/i\
# Rule to make compiled resource (Windows)\
%-rc.o: %.rc\
	windres -i $< -o $@
s,/cygdrive/\([^/]*\)/,\1:/,g
