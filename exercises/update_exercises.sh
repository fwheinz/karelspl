cd ..
( for NAME in $(ls exercises/*/main.c | grep SPL); do N=$(echo $NAME|cut -d/ -f2); TEST=$(ls exercises/$N/test.c 2>/dev/null); echo "add_spl_target($N $TEST $NAME)"; done; ) > exercises/exercises.cmake
( for NAME in $(ls exercises/*/main.c | grep Karel); do N=$(echo $NAME|cut -d/ -f2); TEST=$(ls exercises/$N/test.c 2>/dev/null); echo "add_karel_target($N $TEST $NAME)"; done; ) >> exercises/exercises.cmake
