# Compila y ejecuta el proyecto con g++ (MSYS2 UCRT64).
# Uso: .\build.ps1        -> compila y ejecuta
#      .\build.ps1 -NoRun -> solo compila
param([switch]$NoRun)

$ErrorActionPreference = "Stop"
New-Item -ItemType Directory -Force build | Out-Null

g++ -std=c++17 -Wall -Wextra -g src/main.cpp -o build/planificador.exe -pthread
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

if (-not $NoRun) { & .\build\planificador.exe }
