# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt" AND EXISTS "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitinfo.txt" AND
  "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt" IS_NEWER_THAN "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/doctest/doctest.git" "doctest-src"
    WORKING_DIRECTORY "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/doctest/doctest.git'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe"
          checkout "ae7a13539fb71f270b87eb2e874fbac80bc8dda2" --
  WORKING_DIRECTORY "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'ae7a13539fb71f270b87eb2e874fbac80bc8dda2'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitinfo.txt" "C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'C:/Users/Max Brenner/Documents/elen3009a project/2023-project-2108438-Brenner-2176006-Kienle/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt'")
endif()
