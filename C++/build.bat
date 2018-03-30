@IF EXIST "wordcheck.exe" @del /f wordcheck.exe
@IF EXIST "train.exe" @del /f train.exe
@IF EXIST "autocomplete.exe" @del /f autocomplete.exe

@ECHO Removed old .exe files

@ECHO.
@ECHO Build autocomplete.exe:
@g++ -o autocomplete.exe autocomplete.cpp source\AlphaSearch.cpp source\FileParse.cpp

@IF EXIST "autocomplete.exe" (  @ECHO autocomplete.exe built successfully.) ELSE (
  @ECHO.
  @ECHO Build Halted: error in autocomplete.exe
   GOTO:EOF
)

@ECHO.
@ECHO Build train.exe:
@g++ -o train.exe train.cpp source\AlphaSearch.cpp source\FileParse.cpp

@IF EXIST "train.exe" (  @ECHO train.exe built successfully.) ELSE (
  @ECHO.
  @ECHO Build Halted: error in train.exe
   GOTO:EOF
)

@ECHO.
@ECHO Build wordcheck.exe:
@g++ -o wordcheck.exe wordcheck.cpp source\FileParse.cpp source\AlphaSearch.cpp

@IF EXIST "wordcheck.exe" ( @ECHO wordcheck.exe built successfully. ) ELSE (
  @ECHO.
  @ECHO Build Halted: error in wordcheck.exe
  GOTO:EOF
)
