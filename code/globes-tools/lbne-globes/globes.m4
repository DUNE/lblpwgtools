# Configure path for GLoBES following: "Configure path for the GNU
# Scientific Library Christopher R. Gabriel <cgabriel@linux.it>, April
# 2000". In contrast to the former GLoBES and its dependencies are
# appended to 'LIBS' and 'GLOBESINC' is defined and AC_SUBST'ed. Thus
# the only thing you have to do is to call 'AM_PATH_GLOBES' in
# 'configure.in' and to add 'GLOBESINC' to the 'INCLUDE' directive of
# your 'Makefile.am'. You also can require a certain minimum version
# of globes by calling 'AM_PATH_GLOBES(2.3.1)', in that case having
# any version less recent than '2.3.1' would be a configure error
#
# (C) The GLoBES Team 2006, <globes@ph.tum.de>

AC_DEFUN([AM_PATH_GLOBES],
[
AC_ARG_WITH(globes-prefix,[  --with-globes-prefix=PFX   Prefix where GLoBES is installed (optional)],
            globes_prefix="$withval", globes_prefix="")
AC_ARG_WITH(globes-exec-prefix,[  --with-globes-exec-prefix=PFX Exec prefix where GLoBES is installed (optional)],
            globes_exec_prefix="$withval", globes_exec_prefix="")
AC_REQUIRE([AC_HEADER_STDC])

GLOBES_PATH=$PATH  

     if test "x$globes_prefix" != x ; then
         GLOBES_PATH="$globes_prefix/bin"
     fi
     if test "x$globes_exec_prefix" != x ; then
        GLOBES_PATH="$gsl_exec_prefix/bin"
     fi
  
  AC_PATH_PROG(GLOBES_CONFIG, [globes-config], [no], [$GLOBES_PATH])

  no_globes=""
  if test "x$GLOBES_CONFIG" = "xno" ; then
    no_globes=yes
	AC_MSG_ERROR([globes-config not found])
  else

    globes_major_version=`$GLOBES_CONFIG --version | sed -n 's/^Package //p'| \ 
	sed 's/^\([[0-9]]*\).*/\1/'`	
	if test "x${globes_major_version}" = "x" ; then
       		globes_major_version=0
    	fi

    globes_minor_version=`$GLOBES_CONFIG --version | sed -n 's/^Package //p'| \
        sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`
    	if test "x${globes_minor_version}" = "x" ; then
       		globes_minor_version=0
    	fi

    globes_micro_version=`$GLOBES_CONFIG --version | sed -n 's/^Package //p'| \
         sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`
    	if test "x${globes_micro_version}" = "x" ; then
       		globes_micro_version=0
    	fi

req_version=$1	



    req_major_version=` echo $req_version | \ 
	sed 's/^\([[0-9]]*\).*/\1/'`	
	if test "x${req_major_version}" = "x" ; then
       		req_major_version=0
    	fi

    req_minor_version=` echo $req_version | \
        sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\2/'`
    	if test "x${req_minor_version}" = "x" ; then
       		req_minor_version=0
    	fi

    req_micro_version=` echo $req_version | \
         sed 's/^\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\)\.\{0,1\}\([[0-9]]*\).*/\3/'`
    	if test "x${rec_micro_version}" = "x" ; then
       		req_micro_version=0
    	fi

AC_MSG_CHECKING([for globes version >= $req_major_version.$req_minor_version.$req_micro_version])

	version_ok=no

	if ((req_major_version < globes_major_version)); then
 		version_ok=yes
        else 
		if ((req_major_version == globes_major_version)); then
			if  ((req_minor_version < globes_minor_version)); then
				version_ok=yes
			else 
				if ((req_minor_version == globes_minor_version)); then
					if ((req_micro_version < globes_micro_version)); then
						version_ok=yes
					else
						if ((req_micro_version == globes_micro_version)); then
							version_ok=yes
						fi
					fi
				fi
			fi
		fi
	fi



AC_MSG_RESULT($version_ok)
if test x$version_ok = xno; then
	AC_MSG_ERROR([globes version is less recent than $req_version])
fi

    GLOBES_CFLAGS=`$GLOBES_CONFIG --cflags`
   # this change seems necessary to properly handle cases where
   # GLoBES is installed in places where the linker can not find it at
   # run time
    globes_pref=`$GLOBES_CONFIG --prefix`	
    globes_use=`$GLOBES_CONFIG --libs`
    GLOBES_LIBS="-Wl,--rpath -Wl,$globes_pref/lib $globes_use"	
    GLOBESINC="-I$globes_pref/include"	 
  fi
  
  AC_SUBST(GLOBES_CFLAGS)
  AC_SUBST(GLOBES_LIBS)
  AC_SUBST(GLOBESINC)
  LIBS="$GLOBES_LIBS $LIBS"
  globes_cflags="$GFLAGS"
  # we need that to make sure globes headers are found even if globes
  # is installed in non-standard place
  CFLAGS="$GLOBESINC $CFLAGS"	
  AC_LINK_IFELSE(AC_LANG_PROGRAM([[
	#include <stdlib.h>
	#include <stdio.h>
	#include <globes/globes.h>
	]],[[
	glbInit("test");
	]])
  ,[],[
  AC_MSG_ERROR([cannot link libglobes])
  ])

# now we check whether globes and globes-config really have the same version
test_version="$globes_major_version.$globes_minor_version.$globes_micro_version"

 AC_RUN_IFELSE(AC_LANG_PROGRAM([[
	#include <stdlib.h>
	#include <stdio.h>
	#include <globes/globes.h>
 ]],[[
	glbInit("test");
	return glbTestReleaseVersion("$test_version");
 ]])
 ,[],[
 AC_MSG_ERROR([globes version returned by 'globes-config' does not
 match version of the library])
 ])

CFLAGS="$globes_cflags"

])


