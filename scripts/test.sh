ROOT="$(dirname `dirname $0`)"
cd $ROOT

SCRIPTS="tests/scripts"
MALCO="build/malco"
RESULT=0

for filename in $SCRIPTS/*.rasm; do
    echo "[tests] Compiling test `basename $filename`"
    $MALCO -c $filename
    if [ "$?" == 0 ] ; then
        echo "[tests] Test `basename $filename` success"
    else
        echo "[tests] Test `basename $filename` error"
        RESULT=1
    fi
done

exit $RESULT
