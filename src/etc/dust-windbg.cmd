@echo off
setlocal

for /f "delims=" %%i in ('dustc --print=sysroot') do set dustc_sysroot=%%i

set dust_etc=%dustc_sysroot%\lib\dustlib\etc

windbg -c ".nvload %dust_etc%\intrinsic.natvis; .nvload %dust_etc%\liballoc.natvis; .nvload %dust_etc%\libcore.natvis;" %*
