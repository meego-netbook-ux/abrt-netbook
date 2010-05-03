AM_CPPFLAGS = 						\
	-I.. $(CAPPLET_CFLAGS) 				\
	-DLIBEXECDIR=\"@libexecdir@\" 			\
	-DPKGLIBDIR=\"$(pkglibdir)\" 			\
	-DLOCALEDIR=\"$(localedir)\" 			\
	-DPLUGIN_LOCALEDIR=\"$(localedir)\" 		\
	-DPANELSDIR=\"$(ABRT_PANELS_DIR)\" 	\
	-DTHEMEDIR=\"$(ABRT_THEME_DIR)\"	\
	-Wall -Wmissing-declarations

AM_LDFLAGS = \
	$(CAPPLET_LIBS)

ccmodulesdir = $(CC_DIR)
ccmodules_LTLIBRARIES = libabrt.la

libabrt_la_SOURCES = \
	abrt-module.c \
	abrt-cc-panel.h \
	abrt-cc-panel.c
libabrt_la_CPPFLAGS = $(AM_CPPFLAGS) $(CC_CFLAGS) $(CPA_CFLAGS)
libabrt_la_LIBADD = $(CC_LIBS) $(CPA_LIBS)
libabrt_la_LDFLAGS = $(AM_LDFLAGS) -module -avoid-version

@INTLTOOL_DESKTOP_RULE@

desktopfiles_in_files=abrt-properties.desktop.in
desktopfiles_files=$(desktopfiles_in_files:.desktop.in=.desktop)

%.desktop.in: %.desktop.in.in Makefile
	$(QUITE_GEN)sed -e "s|\@libexecdir\@|$(libexecdir)|" $< > $@

EXTRA_DIST = abrt-properties.desktop.in.in

desktopfilesdir = $(datadir)/applications
desktopfiles_DATA = $(desktopfiles_files)

