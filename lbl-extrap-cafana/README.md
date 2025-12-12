## Building lbl-extrap-cafana as a Standalone Project Using CMake

```
export BASE_DIR=/exp/dune/app/users/abooth/Postdoc/LBL/ExtrapAna ; mkdir -p ${BASE_DIR}
git clone https://github.com/DUNE/dune_plot_style.git

export SOURCE_DIR_DPS=/exp/dune/app/users/abooth/Postdoc/LBL/ExtrapAna/dune_plot_style
export BUILD_DIR_DPS=/exp/dune/app/users/abooth/Postdoc/LBL/ExtrapAna/dune_plot_style/build ; mkdir -p ${BUILD_DIR_DPS}
export INSTALL_DIR_DPS=/exp/dune/app/users/abooth/Postdoc/LBL/ExtrapAna/dune_plot_style/install ; mkdir -p ${INSTALL_DIR_DPS}

cd ${BUILD_DIR_DPS}
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR_DPS} ${SOURCE_DIR_DPS}

export CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:${INSTALL_DIR_DPS}/share/cmake/DUNEPlotStyle
```

```
export SOURCE_DIR_EXTRAP=/exp/dune/app/users/abooth/Postdoc/LBL/ExtrapAna/lblpwgtools/lbl-extrap-cafana ; mkdir -p ${SOURCE_DIR_EXTRAP}
export BUILD_DIR_EXTRAP=/exp/dune/app/users/abooth/Postdoc/LBL/ExtrapAna/lblpwgtools/lbl-extrap-cafana/build ; mkdir -p ${BUILD_DIR_EXTRAP}
export INSTALL_DIR_EXTRAP=/exp/dune/app/users/abooth/Postdoc/LBL/ExtrapAna/lblpwgtools/lbl-extrap-cafana/install ; mkdir -p ${INSTALL_DIR_EXTRAP}

cd ${BUILD_DIR_EXTRAP}
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR_EXTRAP} ${SOURCE_DIR_EXTRAP}
make install
```
