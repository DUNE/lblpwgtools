# What follows here is a modified version of macro taken from
#
# Version 1.0
# GNU Autoconf Macros for SWIG 
#  <http://www.tekrat.com/swigm4.php>
# Tekrat Labs <http://www.tekrat.com>
# Brian Shire <shire@tekrat.com>
#
# The code in this file is licensed under the....
# The MIT License
#
# Copyright (c) 2003, Tekrat Labs <http://www.tekrat.com>, 
#  Brian Shire <shire@tekrat.com>
#
# Permission is hereby granted, free of charge, to any person obtaining 
# a copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense,  
# and/or sell copies of the Software, and to permit persons to whom the  
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included 
# in all copies or substantial portions of the Software.
#
# THE   SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
# NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAAGES OR OTHER LIABILITY, 
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.
#

# Perl...
# AC_SWIG_PERL(<if-found>, <if-not-found>)
# Substitutes: @perl@ @perl_includes@ @perl_installdir@
#-----------------------------------------------------------------------------
AC_DEFUN([AC_SWIG_PERL], [
	AC_REQUIRE([AC_PROG_LIBTOOL])dnl
	build_perl="no"
        # --enable-perl
	perl_req="no";
        AC_ARG_ENABLE(perl,
                [  --enable-perl             Enable/Require Perl support], [perl_req="$enableval"])

        if test x$perl_req != xno; then
	build_perl="yes"
                # --with-perl=PATH
                AC_ARG_WITH(perl,
                        [  --with-perl=PATH       Specify path to Perl interpreter],
                        [perl="$withval"],[perl="perl"])
		
                AC_PATH_PROGS(perl,[$perl],"no")
               
        
                # Test perl     
                if [ test $perl = "no" ]; then
 			AC_MSG_WARN(Could not find perl.  Please use --with-perl option.)                        
          		build_perl="no"
	        else

                        # Check for Includes
                        AC_MSG_CHECKING(for Perl includes)
                        perl_includes="`$perl -MConfig -e 'print $Config{archlibexp}'`/CORE"
			AC_MSG_RESULT($perl_includes)
	dnl check CFLAGS 
	dnl thanks to a mail posted by <jelenz@students.wisc.edu> 
			AC_MSG_CHECKING(for Perl CFLAGS)
			perl_cflags="`$perl -e 'use Config; print $Config{ccflags};'`"
                        AC_MSG_RESULT($perl_cflags)
        
                        # Check for Header files, fixed by PH

                        perl_tmp=$CPPFLAGS
                        CPPFLAGS="$CPPFLAGS -I$perl_includes"
                        AC_CHECK_HEADERS([EXTERN.h], [], 
		[AC_MSG_WARN(Could not find EXTERN.h) && build_perl="no"])
                        AC_CHECK_HEADERS([perl.h], [], 
	[AC_MSG_WARN(Could not find perl.h)  && build_perl="no" ],[
#ifdef HAVE_EXTERN_H
#include <EXTERN.h>
#endif
])
                        AC_CHECK_HEADERS(XSUB.h, [], 
		[AC_MSG_WARN(Could not find XSUB.h)  && build_perl="no"])
                        CPPFLAGS=$perl_tmp
        
                        # Substitute Makefile Vars. 
                        AC_SUBST(perl_installdir)
                        AC_SUBST(perl_includes)
			AC_SUBST(perl_cflags)
                fi

                dnl AC_SUBST(perl)
                        
                if test $build_perl = "no"; then
                        test
                        [$1]
			AC_MSG_WARN(Can not build perl module)
                else
                        test    
                        [$2]
                fi
        fi
 dnl since I didn't figure out to build a static perl module we have
 dnl to check wether shared objects are built by libtool
	if test $enable_shared = "yes"; then
		if test $build_perl = "yes"; then
			really_build_perl="yes"
		else
			really_build_perl="no"
		fi
	else
		if test $build_perl = "yes"; then
			AC_MSG_WARN(Can not build a static perl module)
			AC_MSG_WARN(Re-run configure with '--enable-shared' or '--disable-perl')
		fi
		really_build_perl="no"
	fi

	AM_CONDITIONAL(PERL, test $really_build_perl = "yes")
	dnl this funny fellow is computed by AC_PROG_LIBTOOL, hence the
	dnl AC_REQUIRE at the beginning. This we need for the install-hook
        dnl to work properly
	AC_SUBST(shrext_cmds)
	AC_SUBST(really_build_perl)	

]) # AC_SWIG_PERL

