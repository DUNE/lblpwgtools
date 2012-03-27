
dnl Experimental support for making rpm's
dnl This is largely adapted from the rpm.m4 macro by
dnl Dale K. Hawkins <dhawkins@cdrgts.com>

dnl AM_RPM_INIT
dnl Figure out how to create rpms for this system and setup for an
dnl automake target

AC_DEFUN([AM_RPM_INIT],
[dnl
AC_REQUIRE([AC_CANONICAL_HOST])
dnl Find the RPM (rpmbuild) program
AC_ARG_WITH(rpm-prog,[  --with-rpm-prog=PROG   Which rpm to use (optional)],
            rpm_prog="$withval", rpm_prog="rpmbuild")

AC_ARG_ENABLE(rpm-rules, [  --enable-rpm-rules       Try to create rpm make rules [[default=no]]],
                enable_rpm_rules="$withval",enable_rpm_rules=no)

AC_ARG_WITH(rpm-extra-args, [  --with-rpm-extra-args=ARGS       Run rpm with extra arguments (defaults to none)],
                rpm_extra_args="$withval", rpm_extra_args="")

AC_ARG_WITH(rpm-targets, [  --with-rpm-targets=ARGS       Run rpm with targets (default is host_cpu)],
                rpm_targets="$withval", rpm_targets="$host_cpu")


dnl echo enable_rpm_rules is $enable_rpm_rules
dnl echo rpm_prog is $rpm_prog

RPM_ARCHS=$rpm_targets
AC_SUBST(RPM_ARCHS)

  RPM_TARGET=""

  if test x$enable_rpm_rules = xno ; then
     echo "Not trying to build rpms for your system (use --enable-rpm-rules to override) "
     no_rpm=yes
  else
    if test x$rpm_prog != x ; then
       if test x${RPM_PROG+set} != xset ; then
          RPM_PROG=$rpm_prog
       fi
    fi
dnl update to rpmbuild -- PH
dnl echo "RPM_PROG is $RPM_PROG"
dnl replace rpm with $rpm_prog in the next line -- PH
    AC_PATH_PROG(RPM_PROG, $rpm_prog, no)
    no_rpm=no
    if test "$RPM_PROG" = "no" ; then
echo "*** RPM Configuration Failed"
echo "*** Failed to find the rpmbuild program." 
echo "*** If you want to build rpm packages indicate the path to the rpmbuild"
echo "*** program using --with-rpm-prog=PROG"
echo "*** On older systems rpmbuild may be replaced by rpm itself."
echo "*** You can test this by calling rpm -ba, if rpm recognizes this option"
echo "*** rpm can be used instead of rpmbuild, just re-run configure using"
echo "*** --with-rpm-prog=rpm"
      no_rpm=yes
      RPM_MAKE_RULES=""
    else
      AC_MSG_CHECKING(how rpm sets %{_rpmdir})
      rpmdir=`rpm --eval %{_rpmdir}`
      if test x$rpmdir = x"%{_rpmdir}" ; then
        AC_MSG_RESULT([not set (cannot build rpms?)])
        echo *** Could not determine the value of %{_rpmdir}
        echo *** This could be because it is not set, or your version of rpm does not set it
        echo *** It must be set in order to generate the correct rpm generation commands
        echo ***
        echo *** You might still be able to create rpms, but I could not automate it for you
        echo *** BTW, if you know this is wrong, please help to improve the rpm.m4 module
        echo *** Send corrections, updates and fixes to dhawkins@cdrgts.com.  Thanks.
      else
        AC_MSG_RESULT([$rpmdir])
      fi
      AC_MSG_CHECKING(how rpm sets %{_rpmfilename})
	rpmfilename_list=""
	for arch in `echo $rpm_targets`
	do
      	rpmfilename_list="$rpmfilename_list $rpmdir/`rpm --eval %{_rpmfilename} | sed "s/%{ARCH}/${arch}/g" | sed "s/%{NAME}/$PACKAGE/g" | sed "s/%{VERSION}/${VERSION}/g" | sed "s/%{RELEASE}/${RPM_RELEASE}/g"`"
	done
      AC_MSG_RESULT([$rpmfilename_list])

dnl the filenames which are rpmbuilt without path
	rpmcleanfilename_list=""
	for arch in `echo $rpm_targets`
	do
      	rpmcleanfilename_list="$rpmcleanfilename_list `rpm --eval %{_rpmfilename} | sed "s/%{ARCH}\///" | sed "s/%{ARCH}/${arch}/g" | sed "s/%{NAME}/$PACKAGE/g" | sed "s/%{VERSION}/${VERSION}/g" | sed "s/%{RELEASE}/${RPM_RELEASE}/g"`"
	done
dnl not forgettint the srpm
	rpmcleanfilename_list="$rpmcleanfilename_list `rpm --eval %{_rpmfilename} | sed "s/%{ARCH}\///" | sed "s/%{ARCH}/src/g" | sed "s/%{NAME}/$PACKAGE/g" | sed "s/%{VERSION}/${VERSION}/g" | sed "s/%{RELEASE}/${RPM_RELEASE}/g"`"
	RPM_CLEAN_NAMES=$rpmcleanfilename_list

AC_MSG_CHECKING(handling different build archs)	

dnl checking the rpm source directory where the tar-ball will end up...
     	AC_MSG_CHECKING([how rpm sets %{_sourcedir}])
	rpmsourcedir=`rpm --eval %{_sourcedir}`
	AC_MSG_RESULT([$rpmsourcedir])	

	RPM_SOURCE_DIR=${rpmsourcedir}
dnl checking where the source rpm will end up
	AC_MSG_CHECKING([how rpm sets %{_srcrpmdir}])
	rpmsrcdir=`rpm --eval %{_srcrpmdir}`
	AC_MSG_RESULT([$rpmsrcdir])	
	RPM_SRC_DIR=${rpmsrcdir}

dnl figuring out the name of the source rpm
	AC_MSG_CHECKING([how rpm names the source rpm])
	srpmfilename="$rpmsrcdir/`rpm --eval %{_rpmfilename} | sed "s/%{ARCH}\///" | sed "s/%{ARCH}/src/g" | sed "s/%{NAME}/$PACKAGE/g" | sed "s/%{VERSION}/${VERSION}/g" | sed "s/%{RELEASE}/${RPM_RELEASE}/g"`"
	RPM_SRPM_NAME=$srpmfilename
        AC_MSG_RESULT([$srpmfilename])	
dnl checking the rpm build directory where the tar-ball will be build...
     	AC_MSG_CHECKING([how rpm sets %{_builddir}])
	rpmbuilddir=`rpm --eval %{_builddir}`
	AC_MSG_RESULT([$rpmbuilddir])	
	RPM_BUILD_DIR=${rpmbuilddir}
      RPM_DIR=${rpmdir}
      RPM_TARGET=$rpmfilename_list
      RPM_ARGS="-ba $rpm_extra_args"
      RPM_TARBALL=${PACKAGE}-${VERSION}.tar.gz
      RPM_VERSION=${PACKAGE}-${VERSION}
    fi
  fi

  case "${no_rpm}" in
    yes) make_rpms=false;;
    no) make_rpms=true;;
    *) AC_MSG_WARN([bad value ${no_rpm} for no_rpm (not making rpms)])
       make_rpms=false;;
  esac
dnl RPM_ARGS="$RPM_ARGS --target $rpm_targets"
AC_SUBST(RPM_CLEAN_NAMES)
AC_SUBST(RPM_SRC_DIR)
AC_SUBST(RPM_SRPM_NAME)
AC_SUBST(RPM_SOURCE_DIR)
AC_SUBST(RPM_BUILD_DIR)	
  AC_SUBST(RPM_DIR)
  AC_SUBST(RPM_TARGET)
  AC_SUBST(RPM_ARGS)
  AC_SUBST(RPM_TARBALL)
 AC_SUBST(RPM_VERSION)


  RPM_CONFIGURE_ARGS=${ac_configure_args}
  AC_SUBST(RPM_CONFIGURE_ARGS)
])

