sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test -y
sudo add-apt-repository -y ppa:zoogie/sdl2-snapshots -y
sudo add-apt-repository -y ppa:nschloe/boost-nightly
sudo add-apt-repository -y ppa:mc3man/trusty-media
sudo add-apt-repository -y ppa:jonathonf/ffmpeg-3

sudo apt update -qq
sudo apt install -y -qq libsdl2-dev 
sudo apt install -y -qq libsdl2-mixer-dev
sudo apt install -y -qq libboost-all-dev
sudo apt install -y -qq ffmpeg