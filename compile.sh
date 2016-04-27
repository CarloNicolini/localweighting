mex CFLAGS="\$CFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" CXXFLAGS="\$CXXFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" -largeArrayDims -I../eigen localweighting_dense_symm_mx.cpp localweighting.cpp -o localweighting_dense_symm_mx.mexa64
mex CFLAGS="\$CFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" CXXFLAGS="\$CXXFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" -largeArrayDims -I../eigen localweighting_dense_asymm_mx.cpp localweighting.cpp -o localweighting_dense_asymm_mx.mexa64
mex CFLAGS="\$CFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" CXXFLAGS="\$CXXFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" -largeArrayDims -I../eigen localweighting_sparse_symm_mx.cpp localweighting.cpp -o localweighting_sparse_symm_mx.mexa64
mex CFLAGS="\$CFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" CXXFLAGS="\$CXXFLAGS -O3 -fopenmp" LDFLAGS="\$LDFLAGS -fopenmp" -largeArrayDims -I../eigen localweighting_sparse_asymm_mx.cpp localweighting.cpp -o localweighting_sparse_asymm_mx.mexa64

# This is the compilation command sent to generate the mex
#g++ -c  -I../eigen -I/home/carlo/MATHWORKS/extern/include  -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -fopenmp  -O3 -DNDEBUG  "localweighting_mx.cpp"
#g++ -c  -I../eigen -I/home/carlo/MATHWORKS/extern/include  -DMATLAB_MEX_FILE -ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -fopenmp  -O3 -DNDEBUG  "localweighting.cpp"
#g++ -O3 -pthread -shared -Wl,--version-script,/home/carlo/MATHWORKS/extern/lib/glnxa64/mexFunction.map -Wl,--no-undefined -fopenmp -fopenmp -o  "localweighting_mx.mexa64"  localweighting_mx.o localweighting.o -Wl,-rpath-link,/home/carlo/MATHWORKS/bin/glnxa64 -L/home/carlo/MATHWORKS/bin/glnxa64 -lmx -lmex -lmat -lm
