1. make project dir
2. install curl library
3. make library directory (lib)
4. make library
  4.1 library object
    gcc -fPIC -c passportLib.c -lcurl
  4.2 library so
    gcc -shared -Wl,-soname,libpassport.so.0 -o libpassport.so.0.0.0 passportLib.o -lcurl
    ln -s libpassport.so.0.0.0 libpassport.so
    ln -s libpassport.so.0.0.0 libpassport.so.0
  4.3 so library 등록
    /usr/local/lib

5. program compile with so
  gcc -o http http.c -ldl
