C:\Program` Files\Epic` Games\UE_4.27\Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="%CD%\CppSessions.uproject" -platform=Win64 -config=DebugGame -build -cook -compressed -stage -pak -stagingdirectory="%CD%\Build\CppSessions_$(Get-Date -Format yyyy-MM-dd_HH-mm)_$(git branch --show-current)_DebugGame"