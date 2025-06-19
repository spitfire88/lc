

pushd open-vm-tools  &&
pushd open-vm-tools  &&
make -j `nproc`      &&
sudo make install    &&
sudo ldconfig        &&
popd                 &&
popd
