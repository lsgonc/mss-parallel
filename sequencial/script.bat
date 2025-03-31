@echo off
echo Compilando o programa sequencial...

gcc -o sequencial.exe sequencial.c
if %ERRORLEVEL% NEQ 0 (
    echo Erro na compilacao!
    pause
    exit /b %ERRORLEVEL%
)
echo Compilacao concluida com sucesso!
echo.

echo Testes para N fixo (10000) e K variavel

for %%k in (5 10 20 50 100) do (
    echo Testando N=10000, K=%%k
    for /L %%i in (1,1,5) do (
        sequencial.exe 10000 %%k
    )
)

echo Testes para K fixo (10) e N variavel

for %%n in (100 1000 5000 10000) do (
    echo Testando N=%%n, K=10
    for /L %%i in (1,1,5) do (
        sequencial.exe %%n 10
    )
)
