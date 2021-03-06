cmake_minimum_required(VERSION 3.22)
project(system_programming)

set(CMAKE_CXX_STANDARD 14)

include_directories(study/final/12_SysV-IPC)

add_executable(system_programming
        sp-hw1/sp-hw1-201902763.c
        sp-hw2/sp-hw2-201902763.c
        sp-hw3/sp-hw3-201902763-client.c
        sp-hw3/sp-hw3-201902763-server.c
        sp-hw4/c.c
        sp-hw4/s.c
        study/midterm/debug/loop.c
        study/midterm/debug/myProg.c
        study/midterm/debug/segfault.c
        study/final/09_mmap/code/mmcp.c
        study/final/09_mmap/code/nonblockw.c
        study/final/13_network/code/echo_client.c
        study/final/13_network/code/echo_server.c
        study/final/13_network/code/gethostbyaddr_ex.c
        study/final/13_network/code/gethostbyname_ex.c
        study/final/13_network/code/getservbyname_ex.c
        study/final/13_network/code/select_client.c
        study/final/13_network/code/select_server.c
        study/final/10_semaphore/code/mprod-cons.c
        study/final/10_semaphore/code/prod-cons.c
        study/final/10_semaphore/code/semaphore.c
        study/final/11_signal/code/abort.c
        study/final/11_signal/code/alarm-getchar.c
        study/final/11_signal/code/alarm-getchar2.c
        study/final/11_signal/code/alarm.c
        study/final/11_signal/code/itimer.c
        study/final/11_signal/code/pause.c
        study/final/11_signal/code/posix-timer.c
        study/final/11_signal/code/sighandler.c
        study/final/11_signal/code/sigsuspend.c
        study/final/11_signal/code/siguser1.c
        study/final/11_signal/code/sleep-sig.c
        study/final/12_SysV-IPC/sysV-sem/shm-consumer.c
        study/final/12_SysV-IPC/sysV-sem/shm-producer.c
        study/final/12_SysV-IPC/sysV-sem/shm-seminit.c
        study/final/12_SysV-IPC/code/crecv.c
        study/final/12_SysV-IPC/code/csend.c
        study/final/12_SysV-IPC/code/hello.c
        study/final/12_SysV-IPC/code/memlayer.c
        study/final/12_SysV-IPC/code/mymessage.h
        study/final/12_SysV-IPC/code/shm-con.c
        study/final/12_SysV-IPC/code/shm-pro.c
        study/final/14_time/code/setTime.c
        study/final/14_time/code/strFTime.c
        study/final/14_time/code/timeF.c
        study/final/14_time/code/timeToStr.c
        study/final/14_time/code/toUTC.c
        study/midterm/fileio/access-ex.c
        study/midterm/fileio/access.c
        study/midterm/fileio/chmod-ex.c
        study/midterm/fileio/chmod.c
        study/midterm/fileio/chown-ex.c
        study/midterm/fileio/create-ex.c
        study/midterm/fileio/dup-ex.c
        study/midterm/fileio/fcopy2-ex.c
        study/midterm/fileio/feof-ex.c
        study/midterm/fileio/ferror-ex.c
        study/midterm/fileio/filecopy.c
        study/midterm/fileio/fileio-ex.c
        study/midterm/fileio/frandom.c
        study/midterm/fileio/freopen-ex1.c
        study/midterm/fileio/freopen-ex2.c
        study/midterm/fileio/fstat-ex.c
        study/midterm/fileio/hlink-ex.c
        study/midterm/fileio/io-redir.c
        study/midterm/fileio/lineio-ex.c
        study/midterm/fileio/open-ex.c
        study/midterm/fileio/readlink-ex.c
        study/midterm/fileio/readlink.c
        study/midterm/fileio/stdio-ex.c
        study/midterm/fileio/symlink-ex.c
        study/midterm/fileio/termtest.c
        study/midterm/process/calc.c
        study/midterm/process/echoall.c
        study/midterm/process/fork.c
        study/midterm/process/fwait.c
        study/midterm/process/nexec.c
        study/midterm/process/pid-ex.c
        study/midterm/process/pipe.c
        study/midterm/process/race.c
        study/midterm/process/sleep5.c
        study/midterm/process/system-ex.c
        study/midterm/process/test.c
        study/midterm/process/time-ex.c
        study/midterm/reclock/rec-proc.c
        study/midterm/reclock/reclock.c
        sp-hw1/sp-hw1-sol/sp-hw1-sol.c
        sp-hw1/sp-hw1-sol/sp-hw1-sol2.c
        sp-hw2/sp-hw2-data/sp-hw2/account.dat
        sp-hw2/sp-hw2-data/sp-hw2/operation.dat
        study/midterm/thread/cond-var.c
        study/midterm/thread/dot_product.c
        study/midterm/thread/inc_dec_no_mtx.c
        study/midterm/thread/inc_dec_with_mtx.c
        study/midterm/thread/mutex_counter.c
        study/midterm/thread/prod-cons.c
        study/midterm/thread/th_cancel.c
        study/midterm/thread/th_cancel2.c
        study/midterm/thread/th_hello.c
        study/midterm/thread/th_hello2.c
        study/midterm/thread/thread-ex1.c
        study/midterm/thread/thread-ex2.c
        study/midterm/thread/thread-join.c study/final/11_signal/code/sigTest.c)
