#!/bin/bash -x
mkdir DAQ
cd DAQ
curl -O https://pocoproject.org/releases/poco-1.7.3/poco-1.7.3.tar.gz
tar zxvf poco-1.7.3.tar.gz
cd poco-1.7.3
./configure --omit=Data/ODBC,Data/MySQL,NetSSL_OpenSSL,Crypto
make
make install
cd ..

wget http://sourceforge.net/projects/arma/files/armadillo-7.900.1.tar.xz
tar zxvf armadillo-7.900.1.tar.xz
cd armadillo-7.900.1
cmake .
make
make install

