setenv PDKPATH /farmshare/home/classes/ee/272/PDKs/sky130A

setenv PATH /cad/mentor/2019.11/Catapult_Synthesis_10.4b-841621/Mgc_home/bin:$PATH
setenv PATH /farmshare/home/classes/ee/272/python37/bin:$PATH
setenv MGLS_LICENSE_FILE 1717@cadlic0.stanford.edu
setenv PATH /cad/iverilog/bin:$PATH

source /cad/modules/tcl/init/tcsh
module load base
module load vcs/S-2021.09-SP1
module load dc_shell/S-2021.06-SP5-4
module load innovus/21.16.000
module load calibre/2019.1
module load prime/T-2022.03
module load magic
module load netgen

module load genus/20.11.000 

complete make \
    'n/-f/f/' \
    'c/*=/f/' \
    'n@*@`cat -s GNUmakefile Makefile makefile |& sed -n -e "/No such file/d" -e "/^[^     #].*:/s/:.*//p"`@'
