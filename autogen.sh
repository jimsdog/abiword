#!/bin/sh

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

olddir=`pwd`
cd $srcdir

# find plugins Makefile templates
find plugins -name Makefile.am | sed  's|.am$||g' > plugin-makefiles.m4

# create plugin list
(cd plugins && find . -maxdepth 1 -type d | grep -v '^\.$' | grep -v '\./\.' | sed 's|\./||g' | xargs echo) > plugin-list.m4

# create plugin configuration
find plugins -name plugin.m4 | xargs cat > plugin-configure.m4

# find extra m4 files provided by plugins and symlink them
for f in ` find ./plugins -name '*.m4' | grep -v 'plugin\.m4'`; do
    ln -sf $f
done

autoreconf --force --install --symlink

cd $olddir

conf_flags="--enable-maintainer-mode"

if test x$NOCONFIGURE = x; then
  echo Running $srcdir/configure $conf_flags "$@" ...
  $srcdir/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile. || exit 1
else
  echo Skipping configure process.
fi

