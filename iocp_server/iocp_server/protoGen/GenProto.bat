@echo off

cd ./GetSource

for %%i in (../*.proto) do protoc -I=../ --cpp_out=./ ../"%%i" 

cd ..

cd ./Debug

for %%i in (../*.proto) do protoc -I=../ --cpp_out=./ ../"%%i" 

cd ..
cd ./Release

for %%i in (../*.proto) do protoc -I=../ --cpp_out=./ ../"%%i" 

cd ..
