Real-time signal to noise ratio,constellation plotting using HighCharts.

Reason for using POCO is that some embedded box running YOCTO on PPC does not have nodejs available (stable version). 

Server side is implemented using pure C++ POCO WebSocket & light weight http server.

Armadillo is used for MATLAB alternative for FFT computation.

Steps to test on OSX:

1. Run setup.sh (Builds & installs POCO & Armadillo headers & binaries)
2. open DAQWebSocketServer.xcodeproj
3. Build & run
4. open http://localhost:9980
