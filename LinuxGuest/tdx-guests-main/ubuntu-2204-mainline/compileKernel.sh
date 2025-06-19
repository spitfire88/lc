pushd linux &&
sudo make -j `getconf _NPROCESSORS_ONLN` &&
#sudo make -j `getconf _NPROCESSORS_ONLN` modules_install &&
#sudo make -j `getconf _NPROCESSORS_ONLN` install &&
popd
