@echo off
echo Testes para N fixo (5000), K variavel e Threads variavel

for %%t in (1 2 4 8) do (
    for %%k in (5 10 20 50 100) do (
        echo Testando N=5000, K=%%k, Threads=%%t
        for /L %%i in (1,1,5) do (
            openmp.exe 5000 %%k %%t
        )
        echo.
    )
)

echo Testes para K fixo (10), N variavel e Threads variavel

for %%t in (1 2 4 8) do (
    for %%n in (100 500 1000 5000) do (
        echo Testando N=%%n, K=10, Threads=%%t
        for /L %%i in (1,1,5) do (
            openmp.exe %%n 10 %%t
        )
        echo.
    )
)

pause