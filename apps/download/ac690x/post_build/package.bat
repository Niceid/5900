@echo off
cd /d %~dp0
set exefile="C:\Program Files\McuTool\package.exe"
set srcdir="%cd%"
set dstdir="%cd%"
echo %exefile%
if exist %exefile% (@echo package ... && %exefile% %srcdir% %dstdir% && @echo package finished.)