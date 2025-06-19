#!/bin/bash

pushd grub                                  &&
./bootstrap                                 &&
./configure --prefix=/usr                   \
            --sysconfdir=/etc               \
            --with-platform=efi             \
            --target=x86_64                 &&
make -j$(nproc)                             &&
sudo make install                           &&
sudo grub-install /dev/sda                  &&
popd                                        &&
sudo mkdir -p /usr/local/etc/default        &&
sudo cp grubCfg /usr/local/etc/default/grub &&
sudo cp grubCfg /etc/default/grub           &&
sudo grub-mkconfig -o /boot/grub/grub.cfg   ;
