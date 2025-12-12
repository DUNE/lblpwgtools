## Building lbl-extrap-cafana as a Standalone Project Using CMake

Written December 2025, Alex B.

Before building `lbl-extrap-cafana`, make sure that you have followed the steps [here](https://github.com/DUNE/lblpwgtools/tree/main/CAFAna#cafana-readme) to set up `CAFAna` and have `source`-ed `CAFAnaEnv.sh`.

You've should already have a copy of `lblpwgtools` in some directory. In this `README`, that directory is assumed to be set as the environment variable `BASE_DIR`. For example:

```
export BASE_DIR=/exp/dune/app/users/abooth/ExtrapAna
```

`lbl-extrap-cafana` depends on the `dune_plot_style` package so we need to grab that and, build and install it.
```
cd ${BASE_DIR}
git clone https://github.com/DUNE/dune_plot_style.git

export SOURCE_DIR_DPS=${BASE_DIR}/dune_plot_style
export BUILD_DIR_DPS=${BASE_DIR}/dune_plot_style/build ; mkdir -p ${BUILD_DIR_DPS}
export INSTALL_DIR_DPS=${BASE_DIR}/dune_plot_style/install ; mkdir -p ${INSTALL_DIR_DPS}

cd ${BUILD_DIR_DPS}
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR_DPS} ${SOURCE_DIR_DPS}
make install
```

We then need to add this package to the `CMAKE_PREFIX_PATH` so that our build of `lbl-extrap-cafana` can see it.
```
export CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:${INSTALL_DIR_DPS}/share/cmake/DUNEPlotStyle
```

Finally, build and install `lbl-extrap-cafana`.
```
export SOURCE_DIR_EXTRAP=${BASE_DIR}/lblpwgtools/lbl-extrap-cafana
export BUILD_DIR_EXTRAP=${BASE_DIR}/lblpwgtools/lbl-extrap-cafana/build ; mkdir -p ${BUILD_DIR_EXTRAP}
export INSTALL_DIR_EXTRAP=${BASE_DIR}/lblpwgtools/lbl-extrap-cafana/install ; mkdir -p ${INSTALL_DIR_EXTRAP}

cd ${BUILD_DIR_EXTRAP}
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR_EXTRAP} ${SOURCE_DIR_EXTRAP}
make install
```
