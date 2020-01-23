rd /s /q "./.solution"
cmake -G "Visual Studio 16 2019" -Ax64 -B./.solution -H.
pause
cmake --open ./.solution