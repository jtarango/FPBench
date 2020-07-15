# git clone https://github.com/FPBench/FPBench.git
# cd FPBench
# git checkout aa7bacb06277f5e8e20ba33d36bd233a7244fb15
racket export.rkt benchmarks/fptaylor-extra.fpcore FPCBenchMarks/fptaylor-extra.c
racket export.rkt benchmarks/fptaylor-real2float.fpcore FPCBenchMarks/fptaylor-real2float.c
racket export.rkt benchmarks/fptaylor-tests.fpcore FPCBenchMarks/fptaylor-tests.c
racket export.rkt benchmarks/hamming-ch3.fpcore FPCBenchMarks/hamming-ch3.c
racket export.rkt benchmarks/herbie.fpcore FPCBenchMarks/herbie.c
racket export.rkt benchmarks/precimonious.fpcore FPCBenchMarks/precimonious.c
racket export.rkt benchmarks/rosa.fpcore FPCBenchMarks/rosa.c
racket export.rkt benchmarks/rump.fpcore FPCBenchMarks/rump.c
racket export.rkt benchmarks/salsa.fpcore FPCBenchMarks/salsa.c

