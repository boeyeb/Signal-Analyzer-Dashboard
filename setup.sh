#!/bin/bash -x
mkdir DAQ
cd DAQ
curl -O http://pocoproject.org/releases/poco-1.7.3/poco-1.7.3.tar.gz
tar zxvf poco-1.7.3.tar.gz
cd poco-1.7.3
./configure --omit=Data/ODBC,Data/MySQL,NetSSL_OpenSSL,Crypto
make
make install
cd ..
wget http://sourceforge.net/projects/arma/files/armadillo-6.700.6.tar.gz
tar zxvf armadillo-6.700.6.tar.gz
cd armadillo-6.700.6
cmake .
make
make install

