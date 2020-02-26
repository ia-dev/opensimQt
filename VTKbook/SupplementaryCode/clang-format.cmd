@echo off

rem Just invokes clang-format.bash.

if %1.==. goto help

set "src_dir=%~dp0\"
set "bash_terminal=%LOCALAPPDATA%\Programs\Git\bin\bash.exe"

if exist "%src_dir%clang-format.bash" (
  cd "%src_dir%"
  echo "Running clang-format.bash in a bash terminal."
  "%bash_terminal%" --login -m clang-format.bash %1
  cd  %src_dir%
) else (
  echo Error: clang-format.bash not found.
)
goto end:

:help
echo ----------------
echo usage: clang-format.cmd [--]
echo     --help                     Print usage plus more detailed help.
echo     --clang-format <tool>      Use given clang-format tool.
echo     --amend                    Filter files changed by HEAD.
echo     --cached                   Filter files locally staged for commit.
echo     --modified                 Filter files locally modified from HEAD.
echo     --tracked                  Filter files tracked by Git.
echo ----------------
echo Example to format locally modified files:
echo     Utilities/Scripts/clang-format.bash --modified
echo Example to format locally modified files staged for commit:
echo     Utilities/Scripts/clang-format.bash --cached
echo Example to format files modified by the most recent commit:
echo     Utilities/Scripts/clang-format.bash --amend
echo Example to format all files:
echo     Utilities/Scripts/clang-format.bash --tracked
echo ----------------
goto end

:end
