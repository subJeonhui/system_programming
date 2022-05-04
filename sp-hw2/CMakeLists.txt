cmake_minimum_required(VERSION 3.22)
project(sp_hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(sp_hw2
        cmake-build-debug/account.dat
        account_backup.dat
        cmake-build-debug/operation.dat
        operation_backup.dat
        sp-hw2-201902763.c)
