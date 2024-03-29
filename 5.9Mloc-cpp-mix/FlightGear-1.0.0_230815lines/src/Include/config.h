
/* Special single config.h for MSVC6 build - Geoff McLane - 23 July, 2003  */

/* Define to enable plib joystick support */
#ifndef  ENABLE_PLIB_JOYSTICK
#define ENABLE_PLIB_JOYSTICK
#endif   // #ifndef  ENABLE_PLIB_JOYSTICK

/* Define to enable threaded tile paging */
#undef ENABLE_THREADS

/* Define so that JSBSim compiles in 'library' mode */
#define FGFS 1

/* Define to build with jpeg screen shot server */
#undef FG_JPEG_SERVER

/* Define for no logging output */
#undef FG_NDEBUG

/* Define to build with Oliver's networking */
#undef FG_NETWORK_OLK

/* Define to enable plib joystick name support */
#define FG_PLIB_JOYSTICK_GETNAME

/* Define to build with Christian Mayer's weather code */
#undef FG_WEATHERCM

/* Define if the X Window System is missing or not being used.  */
#define X_DISPLAY_MISSING 1

/* Define if the CONSOLE output has no ANSI driver (eg no "esc [ ..." seqs.) */
#define  NO_ANSI_DRIVER

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define to package name - not used? */
#define PACKAGE "FlightGear"

/* Define to package version - use in main.cxx */
#define FLIGHTGEAR_VERSION "MSVC6-WIN32-1.0.0"

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
/* #undef TIME_WITH_SYS_TIME */

/* Define if your <sys/time.h> declares struct tm.  */
#define TM_IN_SYS_TIME 1

/* Define to version number */
#define VERSION "1.0.0"

/* Define if compiling on a Winbloze (95, NT, etc.) platform */
#define WIN32 1

/* Define if you have the GetLocalTime function.  */
#define HAVE_GETLOCALTIME 1

/* Define if you have the ftime function.  */
#define HAVE_FTIME 1

/* Define if you have the getitimer function.  */
/* #define HAVE_GETITIMER 1 */

/* Define if you have the getrusage function.  */
/* #define HAVE_GETRUSAGE 1 */

/* Define if you have the gettimeofday function.  */
/* #define HAVE_GETTIMEOFDAY 1 */

/* Define if you have the mktime function.  */
#define HAVE_MKTIME 1

/* Define if you have the rand function.  */
#define HAVE_RAND 1

/* Define if you have the random function.  */
/* #define HAVE_RANDOM 1   */

/* Define if you have the rint function.  */
/* #define HAVE_RINT 1 */

/* Define if you have the setitimer function.  */
/* #define HAVE_SETITIMER 1 */

/* Define if you have the signal function.  */
#define HAVE_SIGNAL 1

/* Define if you have the strstr function.  */
#define HAVE_STRSTR 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <getopt.h> header file.  */
#define HAVE_GETOPT_H 1

/* Define if you have the <malloc.h> header file.  */
#define HAVE_MALLOC_H 1

/* Define if you have the <stdlib.h> header file.  */
#define HAVE_STDLIB_H 1

/* Define if you have the <sys/time.h> header file.  */
/* #define HAVE_SYS_TIME_H 1 */

/* Define if you have the <sys/timeb.h> header file.  */
#define HAVE_SYS_TIMEB_H 1

/* Define if you have the <unistd.h> header file.  */
/* #define HAVE_UNISTD_H 1 */

/* Define if you have the <values.h> header file.  */
/* #define HAVE_VALUES_H 1 */

/* Define if you have the <winbase.h> header file.  */
#define HAVE_WINBASE_H 1

/* Define if you have the <windows.h> header file.  */
#define HAVE_WINDOWS_H 1

/* Define if you have the GL library (-lGL).  */
#define HAVE_LIBGL 1

/* Define if you have the GLU library (-lGLU).  */
#define HAVE_LIBGLU 1

/* Define if you have the GLcore library (-lGLcore).  */
/* #undef HAVE_LIBGLCORE */

/* Define if you have the ICE library (-lICE).  */
/* #define HAVE_LIBICE 1 */

/* Define if you have the MesaGL library (-lMesaGL).  */
/* #undef HAVE_LIBMESAGL */

/* Define if you have the MesaGLU library (-lMesaGLU).  */
/* #undef HAVE_LIBMESAGLU */

/* Define if you have the SM library (-lSM).  */
/* #define HAVE_LIBSM 1 */

/* Define if you have the X11 library (-lX11).  */
/* #define HAVE_LIBX11 1 */

/* Define if you have the Xext library (-lXext).  */
/* #define HAVE_LIBXEXT 1 */

/* Define if you have the Xi library (-lXi).  */
/* #define HAVE_LIBXI 1 */

/* Define if you have the Xmu library (-lXmu).  */
/* #define HAVE_LIBXMU 1 */

/* Define if you have the Xt library (-lXt).  */
/* #define HAVE_LIBXT 1 */

/* Define if you have the glut library (-lglut).  */
#define HAVE_LIBGLUT 1

/* Define if you have the m library (-lm).  */
#define HAVE_LIBM 1

/* Define if you have the socket library (-lsocket).  */
/* #undef HAVE_LIBSOCKET */

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H 1

#define HAVE_MEMCPY 1

#define NOMINMAX 1

#ifndef ENABLE_AUDIO_SUPPORT
#define  ENABLE_AUDIO_SUPPORT
#endif

#ifndef  FG_GLUT_H
#define  FG_GLUT_H   <GL/glut.h>
#endif   // #ifndef FG_GLUT_H

// eof - config.h-msvc6 - single flightgear config.h file
