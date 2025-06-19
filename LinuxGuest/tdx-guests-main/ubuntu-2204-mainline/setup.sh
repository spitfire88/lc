#!/bin/bash

set -ex

sudo apt-get install -y git build-essential autoconf automake      \
autopoint pkg-config autoconf-archive bison flex python-is-python3 \
libncurses-dev libssl-dev libelf-dev bc libtool libmspack-dev      \
libglib2.0-dev libpam0g-dev libxml2-dev libxmlsec1-dev libxext-dev \
libx11-dev libxinerama-dev libxi-dev libxrender-dev libxtst-dev    \
libgtk-3-dev libgtkmm-3.0-dev;

git config --global user.email "tim.merrifield@broadcom.com"
git config --global user.name  "Tim Merrifield"

if [ ! -d grub ]
then
	git clone https://git.savannah.gnu.org/git/grub.git   &&
        pushd grub                                            &&
        git checkout 04d2a50f31c4f29a451b507aefe02bf0798b534b &&
        popd;
fi

if [ ! -d linux ]
then
	git clone https://github.com/torvalds/linux.git       &&
        cp tdx.config linux/.config                           &&
        pushd linux                                           &&
        git checkout 39133352cbed6626956d38ed72012f49b0421e7b &&
	for i in ../vmw-patches/*; do git am "$i"; done       &&
	popd;
fi

if [ ! -d open-vm-tools ]
then
	git clone https://github.com/vmware/open-vm-tools.git &&
        pushd open-vm-tools  &&
	pushd open-vm-tools  &&
        for i in ../../tools-patches/*; do git am "$i"; done  &&
        autoreconf -i        &&
	./configure          &&
	make -j `nproc`      &&
	sudo make install    &&
	sudo ldconfig        &&
	popd                 &&
	popd;
fi
