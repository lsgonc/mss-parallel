@echo off
echo Compilando o programa OpenMP...

gcc -fopenmp -o openmp.exe openmp.c
if %ERRORLEVEL% NEQ 0 (
    echo Erro na compilacao!
    pause
    exit /b %ERRORLEVEL%
)
echo Compilacao concluida com sucesso!
echo.

echo Testes para N fixo (5000), K variavel e Threads variavel

for %%t in (1 2 4 8 16 32) do (
    for %%k in (5 10 20 50 100) do (
        echo Testando N=10000, K=%%k, Threads=%%t
        for /L %%i in (1,1,5) do (
            openmp.exe 10000 %%k %%t
        )
        echo.
    )
)

echo Testes para K fixo (10), N variavel e Threads variavel

for %%t in (1 2 4 8 16 32) do (
    for %%n in (100 500 1000 5000) do (
        echo Testando N=%%n, K=10, Threads=%%t
        for /L %%i in (1,1,5) do (
            openmp.exe %%n 10 %%t
        )
        echo.
    )
)

echo Testes concluidos!
