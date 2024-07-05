@echo off


rem 1>NUL del    ..\soc\platform\inc_s\rtl876x.h
rem 1>NUL copy   ..\..\rom\inc_int\rtl876x.h    ..\soc\platform\inc_s
echo %TIME%

1>NUL copy   ..\..\rom\include\pcc_reg.h    ..\src\peripheral
1>NUL copy   ..\..\rom\include\rtl876x_aon_reg.h    ..\src\peripheral
1>NUL copy   ..\..\rom\include\mem_cfg_in.h    ..\src\peripheral
1>NUL copy   ..\..\rom\include\mem_config.h    ..\src\peripheral
1>NUL copy   ..\..\rom\include\address_map.h    ..\src\peripheral
1>NUL copy   ..\..\rom\include\vector_table.h    ..\src\system_init\inc
1>NUL copy   ..\..\rom\include\vector_table_auto_gen.h    ..\src\system_init\inc
REM 1>NUL copy   ..\..\rom\source\vector_table.c    ..\src\system_init
REM 1>NUL copy   ..\..\rom\source\vector_table_auto_gen.c    ..\src\system_init