AC_INIT([abrt-netbook], [0.0.1])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_ARG_ENABLE(debug,
              [AC_HELP_STRING([--enable-debug],
                             [Enable debug information])],
              [CXXFLAGS="$CXXFLAGS -DDEBUG -ggdb -g"])

AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_PROG_CXX

AC_SYS_LARGEFILE
CXXFLAGS="$CXXFLAGS -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64"

PKG_CHECK_MODULES([GTK], [gtk+-2.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.21])
PKG_CHECK_MODULES(CAPPLET, mx-1.0)
PKG_CHECK_MODULES(CPA, mx-gtk-1.0)
PKG_CHECK_MODULES(CC, gmodule-export-2.0 libgnome-control-center-extension)
CC_DIR=$($PKG_CONFIG libgnome-control-center-extension --variable=extensiondir)
AC_SUBST(CC_DIR)

# Set ALL_ALL_LINGUAS based on the .po files present. Optional argument is the
# name of the po directory. $podir/LINGUAS.ignore can be used to ignore a
# subset of the po files.
AC_DEFUN([AS_ALL_LINGUAS],
[
 AC_MSG_CHECKING([for linguas])
 podir="m4_default([$1],[$srcdir/po])"
 linguas=`cd $podir && ls *.po 2>/dev/null | awk 'BEGIN { FS="."; ORS=" " } { print $[]1 }'`
 if test -f "$podir/LINGUAS.ignore"; then
   ALL_LINGUAS="";
   ignore_linguas=`sed -n -e 's/^\s\+\|\s\+$//g' -e '/^#/b' -e '/\S/!b' \
                       -e 's/\s\+/\n/g' -e p "$podir/LINGUAS.ignore"`;
   for lang in $linguas; do
     if ! echo "$ignore_linguas" | grep -q "^${lang}$"; then
       ALL_LINGUAS="$ALL_LINGUAS $lang";
     fi;
   done;
 else
   ALL_LINGUAS="$linguas";
 fi;
 AC_SUBST([ALL_LINGUAS])
 AC_MSG_RESULT($ALL_LINGUAS)
])

AS_ALL_LINGUAS
IT_PROG_INTLTOOL([0.35.0])

AM_GLIB_GNU_GETTEXT

GETTEXT_PACKAGE=abrt-netbook
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],
                   ["$GETTEXT_PACKAGE"],
                   [Name of default gettext domain])

CONF_DIR='${sysconfdir}/${PACKAGE_NAME}'
VAR_RUN='${localstatedir}/run'
PLUGINS_CONF_DIR='${sysconfdir}/${PACKAGE_NAME}/plugins'
PLUGINS_LIB_DIR='${libdir}/${PACKAGE_NAME}'
ENABLE_SOCKET_OR_DBUS='-DENABLE_DBUS=1'

DEBUG_DUMPS_DIR='${localstatedir}/cache/${PACKAGE_NAME}'
DEBUG_INFO_DIR='${localstatedir}/cache/${PACKAGE_NAME}-di'

AC_SUBST(CONF_DIR)
AC_SUBST(VAR_RUN)
AC_SUBST(DL_LIBS, -ldl)

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/ControlPanelApplet/Makefile
	po/Makefile.in
])

AC_OUTPUT
