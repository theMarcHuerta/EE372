#=========================================================================
# Makefile
#=========================================================================
# Generator : /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/backends/make_backend.py

SHELL=/usr/bin/env bash -euo pipefail

# $1 -- $dst
# $2 -- $src
# $3 -- $stamp

define cpdir
	rm -rf ./$1
	cp -aL $2 $1 || true
	chmod -R +w $1
	touch $3
endef

# $1 -- $dst
# $2 -- $src
# $3 -- $stamp

define cpdir-and-parameterize
	rm -rf ./$1
	cp -aL $2 $1 || true
	chmod -R +w $1
	cp .mflowgen/$1/configure.yml $1
	touch $3
endef

# $1 -- $dst
# $2 -- $src
# $3 -- $stamp

define mkdir-and-symlink
	rm -rf ./$1
	mkdir -p $1
	cd $1 && ln -sf ../$2/* . && cd ..
	rm $1/configure.yml && cp .mflowgen/$1/configure.yml $1
	touch $3
endef

# $1 -- $dst_dir
# $2 -- $dst
# $3 -- $src
# $4 -- $stamp

define symlink
	mkdir -p $1
	cd $1 && ln -sf $3 $2 && touch $4
endef

# $1 -- $stamp

define stamp
	touch $1
endef

define make-subgraph-dir
       mkdir -p $1
       chmod -R +w $1
       cd $1 && mflowgen run --subgraph --design $2
       touch $3
endef

# Default

default: constraints hls info skywater-130nm gen-saif-rtl synopsys-dc-synthesis-clock_period-8

# ------------------------------------------------------------------------
# constraints
# ------------------------------------------------------------------------

# build dir

ifeq ("$(wildcard 0-constraints/.prebuilt)","")
0-constraints/.stamp:
	$(call cpdir-and-parameterize,0-constraints,../design/constraints,0-constraints/.stamp)
endif

# collect inputs

# execute

ifeq ("$(wildcard 0-constraints/.prebuilt)","")

define 0_constraints_commands_rule
	mkdir -p 0-constraints/outputs && set -o pipefail && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t constraints && chmod +x .mflowgen/0-constraints/mflowgen-run && cp -f .mflowgen/0-constraints/mflowgen-run 0-constraints && if [[ -e .mflowgen/0-constraints/mflowgen-debug ]]; then chmod +x .mflowgen/0-constraints/mflowgen-debug && cp -f .mflowgen/0-constraints/mflowgen-debug 0-constraints; fi && if [[ -e .mflowgen/0-constraints/mflowgen-check-preconditions.py ]]; then chmod +x .mflowgen/0-constraints/mflowgen-check-preconditions.py && cp -f .mflowgen/0-constraints/mflowgen-check-preconditions.py 0-constraints; fi && if [[ -e .mflowgen/0-constraints/mflowgen-check-postconditions.py ]]; then chmod +x .mflowgen/0-constraints/mflowgen-check-postconditions.py && cp -f .mflowgen/0-constraints/mflowgen-check-postconditions.py 0-constraints; fi && cd 0-constraints && if [[ -e mflowgen-check-preconditions.py ]]; then ./mflowgen-check-preconditions.py || exit 1; fi && ./mflowgen-run 2>&1 | tee mflowgen-run.log || exit 1 && cd .. && touch -c 0-constraints/outputs/*
endef

0-constraints/.execstamp: 0-constraints/.stamp
	$(call 0_constraints_commands_rule)
	touch $@

0-constraints/outputs/.execstamp.constraints.tcl: 0-constraints/.execstamp
	touch $@

endif

# collect outputs

0-constraints/outputs/.stamp.constraints.tcl: 0-constraints/.execstamp 0-constraints/outputs/.execstamp.constraints.tcl
	$(call stamp,0-constraints/outputs/.stamp.constraints.tcl)

# post-conditions

define 0_constraints_post_conditions_commands_rule
	cd 0-constraints && if [[ -e mflowgen-check-postconditions.py ]]; then ./mflowgen-check-postconditions.py || exit 1; fi && cd ..
endef

0-constraints/.postconditions.stamp: 0-constraints/.execstamp 0-constraints/outputs/.execstamp.constraints.tcl 0-constraints/outputs/.stamp.constraints.tcl
	$(call 0_constraints_post_conditions_commands_rule)
	touch $@

# alias

.PHONY: constraints

constraints: 0-constraints/.execstamp 0-constraints/.postconditions.stamp 0-constraints/outputs/.execstamp.constraints.tcl 0-constraints/outputs/.stamp.constraints.tcl

.PHONY: 0

0: 0-constraints/.execstamp 0-constraints/.postconditions.stamp 0-constraints/outputs/.execstamp.constraints.tcl 0-constraints/outputs/.stamp.constraints.tcl

# debug

# ------------------------------------------------------------------------
# hls
# ------------------------------------------------------------------------

# build dir

ifeq ("$(wildcard 1-hls/.prebuilt)","")
1-hls/.stamp:
	$(call cpdir-and-parameterize,1-hls,../design/hls,1-hls/.stamp)
endif

# collect inputs

# execute

ifeq ("$(wildcard 1-hls/.prebuilt)","")

define 1_hls_commands_rule
	mkdir -p 1-hls/outputs && set -o pipefail && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t hls && chmod +x .mflowgen/1-hls/mflowgen-run && cp -f .mflowgen/1-hls/mflowgen-run 1-hls && if [[ -e .mflowgen/1-hls/mflowgen-debug ]]; then chmod +x .mflowgen/1-hls/mflowgen-debug && cp -f .mflowgen/1-hls/mflowgen-debug 1-hls; fi && if [[ -e .mflowgen/1-hls/mflowgen-check-preconditions.py ]]; then chmod +x .mflowgen/1-hls/mflowgen-check-preconditions.py && cp -f .mflowgen/1-hls/mflowgen-check-preconditions.py 1-hls; fi && if [[ -e .mflowgen/1-hls/mflowgen-check-postconditions.py ]]; then chmod +x .mflowgen/1-hls/mflowgen-check-postconditions.py && cp -f .mflowgen/1-hls/mflowgen-check-postconditions.py 1-hls; fi && cd 1-hls && if [[ -e mflowgen-check-preconditions.py ]]; then ./mflowgen-check-preconditions.py || exit 1; fi && ./mflowgen-run 2>&1 | tee mflowgen-run.log || exit 1 && cd .. && touch -c 1-hls/outputs/*
endef

1-hls/.execstamp: 1-hls/.stamp
	$(call 1_hls_commands_rule)
	touch $@

1-hls/outputs/.execstamp.design.v: 1-hls/.execstamp
	touch $@
1-hls/outputs/.execstamp.run.vcd: 1-hls/.execstamp
	touch $@

endif

# collect outputs

1-hls/outputs/.stamp.design.v: 1-hls/.execstamp 1-hls/outputs/.execstamp.design.v 1-hls/outputs/.execstamp.run.vcd
	$(call stamp,1-hls/outputs/.stamp.design.v)

1-hls/outputs/.stamp.run.vcd: 1-hls/.execstamp 1-hls/outputs/.execstamp.design.v 1-hls/outputs/.execstamp.run.vcd
	$(call stamp,1-hls/outputs/.stamp.run.vcd)

# post-conditions

define 1_hls_post_conditions_commands_rule
	cd 1-hls && if [[ -e mflowgen-check-postconditions.py ]]; then ./mflowgen-check-postconditions.py || exit 1; fi && cd ..
endef

1-hls/.postconditions.stamp: 1-hls/.execstamp 1-hls/outputs/.execstamp.design.v 1-hls/outputs/.execstamp.run.vcd 1-hls/outputs/.stamp.design.v 1-hls/outputs/.stamp.run.vcd
	$(call 1_hls_post_conditions_commands_rule)
	touch $@

# alias

.PHONY: hls

hls: 1-hls/.execstamp 1-hls/.postconditions.stamp 1-hls/outputs/.execstamp.design.v 1-hls/outputs/.execstamp.run.vcd 1-hls/outputs/.stamp.design.v 1-hls/outputs/.stamp.run.vcd

.PHONY: 1

1: 1-hls/.execstamp 1-hls/.postconditions.stamp 1-hls/outputs/.execstamp.design.v 1-hls/outputs/.execstamp.run.vcd 1-hls/outputs/.stamp.design.v 1-hls/outputs/.stamp.run.vcd

# debug

# ------------------------------------------------------------------------
# info
# ------------------------------------------------------------------------

# build dir

ifeq ("$(wildcard 2-info/.prebuilt)","")
2-info/.stamp:
	$(call cpdir-and-parameterize,2-info,../../venv/lib/python3.7/site-packages/steps/info,2-info/.stamp)
endif

# collect inputs

# execute

ifeq ("$(wildcard 2-info/.prebuilt)","")

define 2_info_commands_rule
	mkdir -p 2-info/outputs && set -o pipefail && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t info && chmod +x .mflowgen/2-info/mflowgen-run && cp -f .mflowgen/2-info/mflowgen-run 2-info && if [[ -e .mflowgen/2-info/mflowgen-debug ]]; then chmod +x .mflowgen/2-info/mflowgen-debug && cp -f .mflowgen/2-info/mflowgen-debug 2-info; fi && if [[ -e .mflowgen/2-info/mflowgen-check-preconditions.py ]]; then chmod +x .mflowgen/2-info/mflowgen-check-preconditions.py && cp -f .mflowgen/2-info/mflowgen-check-preconditions.py 2-info; fi && if [[ -e .mflowgen/2-info/mflowgen-check-postconditions.py ]]; then chmod +x .mflowgen/2-info/mflowgen-check-postconditions.py && cp -f .mflowgen/2-info/mflowgen-check-postconditions.py 2-info; fi && cd 2-info && if [[ -e mflowgen-check-preconditions.py ]]; then ./mflowgen-check-preconditions.py || exit 1; fi && ./mflowgen-run 2>&1 | tee mflowgen-run.log || exit 1 && cd .. && touch -c 2-info/outputs/*
endef

2-info/.execstamp: 2-info/.stamp
	$(call 2_info_commands_rule)

2-info/.execstamp.execute-phony: 2-info/.execstamp
	touch $@

endif

# collect outputs

# post-conditions

define 2_info_post_conditions_commands_rule
	cd 2-info && if [[ -e mflowgen-check-postconditions.py ]]; then ./mflowgen-check-postconditions.py || exit 1; fi && cd ..
endef

2-info/.postconditions.stamp: 2-info/.execstamp 2-info/.execstamp.execute-phony
	$(call 2_info_post_conditions_commands_rule)
	touch $@

# alias

.PHONY: info

info: 2-info/.execstamp 2-info/.execstamp.execute-phony 2-info/.postconditions.stamp

.PHONY: 2

2: 2-info/.execstamp 2-info/.execstamp.execute-phony 2-info/.postconditions.stamp

# debug

# ------------------------------------------------------------------------
# skywater-130nm
# ------------------------------------------------------------------------

# build dir

ifeq ("$(wildcard 3-skywater-130nm/.prebuilt)","")
3-skywater-130nm/.stamp:
	$(call cpdir-and-parameterize,3-skywater-130nm,../../../../../../../../farmshare/home/classes/ee/272/skywater-130nm-adk.v2021,3-skywater-130nm/.stamp)
endif

# collect inputs

# execute

ifeq ("$(wildcard 3-skywater-130nm/.prebuilt)","")

define 3_skywater_130nm_commands_rule
	mkdir -p 3-skywater-130nm/outputs && set -o pipefail && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t skywater-130nm && chmod +x .mflowgen/3-skywater-130nm/mflowgen-run && cp -f .mflowgen/3-skywater-130nm/mflowgen-run 3-skywater-130nm && if [[ -e .mflowgen/3-skywater-130nm/mflowgen-debug ]]; then chmod +x .mflowgen/3-skywater-130nm/mflowgen-debug && cp -f .mflowgen/3-skywater-130nm/mflowgen-debug 3-skywater-130nm; fi && if [[ -e .mflowgen/3-skywater-130nm/mflowgen-check-preconditions.py ]]; then chmod +x .mflowgen/3-skywater-130nm/mflowgen-check-preconditions.py && cp -f .mflowgen/3-skywater-130nm/mflowgen-check-preconditions.py 3-skywater-130nm; fi && if [[ -e .mflowgen/3-skywater-130nm/mflowgen-check-postconditions.py ]]; then chmod +x .mflowgen/3-skywater-130nm/mflowgen-check-postconditions.py && cp -f .mflowgen/3-skywater-130nm/mflowgen-check-postconditions.py 3-skywater-130nm; fi && cd 3-skywater-130nm && if [[ -e mflowgen-check-preconditions.py ]]; then ./mflowgen-check-preconditions.py || exit 1; fi && ./mflowgen-run 2>&1 | tee mflowgen-run.log || exit 1 && cd .. && touch -c 3-skywater-130nm/outputs/*
endef

3-skywater-130nm/.execstamp: 3-skywater-130nm/.stamp
	$(call 3_skywater_130nm_commands_rule)
	touch $@

3-skywater-130nm/outputs/.execstamp.adk: 3-skywater-130nm/.execstamp
	touch $@

endif

# collect outputs

3-skywater-130nm/outputs/.stamp.adk: 3-skywater-130nm/.execstamp 3-skywater-130nm/outputs/.execstamp.adk
	$(call stamp,3-skywater-130nm/outputs/.stamp.adk)

# post-conditions

define 3_skywater_130nm_post_conditions_commands_rule
	cd 3-skywater-130nm && if [[ -e mflowgen-check-postconditions.py ]]; then ./mflowgen-check-postconditions.py || exit 1; fi && cd ..
endef

3-skywater-130nm/.postconditions.stamp: 3-skywater-130nm/.execstamp 3-skywater-130nm/outputs/.execstamp.adk 3-skywater-130nm/outputs/.stamp.adk
	$(call 3_skywater_130nm_post_conditions_commands_rule)
	touch $@

# alias

.PHONY: skywater-130nm

skywater-130nm: 3-skywater-130nm/.execstamp 3-skywater-130nm/.postconditions.stamp 3-skywater-130nm/outputs/.execstamp.adk 3-skywater-130nm/outputs/.stamp.adk

.PHONY: 3

3: 3-skywater-130nm/.execstamp 3-skywater-130nm/.postconditions.stamp 3-skywater-130nm/outputs/.execstamp.adk 3-skywater-130nm/outputs/.stamp.adk

# debug

# ------------------------------------------------------------------------
# gen-saif-rtl
# ------------------------------------------------------------------------

# build dir

ifeq ("$(wildcard 4-gen-saif-rtl/.prebuilt)","")
4-gen-saif-rtl/.stamp: 1-hls/.execstamp 1-hls/.postconditions.stamp 1-hls/outputs/.execstamp.design.v 1-hls/outputs/.execstamp.run.vcd 1-hls/outputs/.stamp.design.v 1-hls/outputs/.stamp.run.vcd
4-gen-saif-rtl/.stamp:
	$(call cpdir-and-parameterize,4-gen-saif-rtl,../../venv/lib/python3.7/site-packages/steps/synopsys-vcd2saif-convert,4-gen-saif-rtl/.stamp)
endif

# collect inputs

ifeq ("$(wildcard 4-gen-saif-rtl/.prebuilt)","")
4-gen-saif-rtl/inputs/.stamp.run.vcd: 4-gen-saif-rtl/.stamp 1-hls/outputs/.stamp.run.vcd
	$(call symlink,4-gen-saif-rtl/inputs,run.vcd,../../1-hls/outputs/run.vcd,.stamp.run.vcd)
endif

# execute

ifeq ("$(wildcard 4-gen-saif-rtl/.prebuilt)","")

define 4_gen_saif_rtl_commands_rule
	mkdir -p 4-gen-saif-rtl/outputs && set -o pipefail && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t gen-saif-rtl && chmod +x .mflowgen/4-gen-saif-rtl/mflowgen-run && cp -f .mflowgen/4-gen-saif-rtl/mflowgen-run 4-gen-saif-rtl && if [[ -e .mflowgen/4-gen-saif-rtl/mflowgen-debug ]]; then chmod +x .mflowgen/4-gen-saif-rtl/mflowgen-debug && cp -f .mflowgen/4-gen-saif-rtl/mflowgen-debug 4-gen-saif-rtl; fi && if [[ -e .mflowgen/4-gen-saif-rtl/mflowgen-check-preconditions.py ]]; then chmod +x .mflowgen/4-gen-saif-rtl/mflowgen-check-preconditions.py && cp -f .mflowgen/4-gen-saif-rtl/mflowgen-check-preconditions.py 4-gen-saif-rtl; fi && if [[ -e .mflowgen/4-gen-saif-rtl/mflowgen-check-postconditions.py ]]; then chmod +x .mflowgen/4-gen-saif-rtl/mflowgen-check-postconditions.py && cp -f .mflowgen/4-gen-saif-rtl/mflowgen-check-postconditions.py 4-gen-saif-rtl; fi && cd 4-gen-saif-rtl && if [[ -e mflowgen-check-preconditions.py ]]; then ./mflowgen-check-preconditions.py || exit 1; fi && ./mflowgen-run 2>&1 | tee mflowgen-run.log || exit 1 && cd .. && touch -c 4-gen-saif-rtl/outputs/*
endef

4-gen-saif-rtl/.execstamp: 4-gen-saif-rtl/.stamp 4-gen-saif-rtl/inputs/.stamp.run.vcd
	$(call 4_gen_saif_rtl_commands_rule)
	touch $@

4-gen-saif-rtl/outputs/.execstamp.run.saif: 4-gen-saif-rtl/.execstamp
	touch $@

endif

# collect outputs

4-gen-saif-rtl/outputs/.stamp.run.saif: 4-gen-saif-rtl/.execstamp 4-gen-saif-rtl/outputs/.execstamp.run.saif
	$(call stamp,4-gen-saif-rtl/outputs/.stamp.run.saif)

# post-conditions

define 4_gen_saif_rtl_post_conditions_commands_rule
	cd 4-gen-saif-rtl && if [[ -e mflowgen-check-postconditions.py ]]; then ./mflowgen-check-postconditions.py || exit 1; fi && cd ..
endef

4-gen-saif-rtl/.postconditions.stamp: 4-gen-saif-rtl/.execstamp 4-gen-saif-rtl/outputs/.execstamp.run.saif 4-gen-saif-rtl/outputs/.stamp.run.saif
	$(call 4_gen_saif_rtl_post_conditions_commands_rule)
	touch $@

# alias

.PHONY: gen-saif-rtl

gen-saif-rtl: 4-gen-saif-rtl/.execstamp 4-gen-saif-rtl/.postconditions.stamp 4-gen-saif-rtl/outputs/.execstamp.run.saif 4-gen-saif-rtl/outputs/.stamp.run.saif

.PHONY: 4

4: 4-gen-saif-rtl/.execstamp 4-gen-saif-rtl/.postconditions.stamp 4-gen-saif-rtl/outputs/.execstamp.run.saif 4-gen-saif-rtl/outputs/.stamp.run.saif

# debug

# ------------------------------------------------------------------------
# synopsys-dc-synthesis-clock_period-8
# ------------------------------------------------------------------------

# build dir

ifeq ("$(wildcard 5-synopsys-dc-synthesis-clock_period-8/.prebuilt)","")
5-synopsys-dc-synthesis-clock_period-8/.stamp: 0-constraints/.execstamp 0-constraints/.postconditions.stamp 0-constraints/outputs/.execstamp.constraints.tcl 0-constraints/outputs/.stamp.constraints.tcl
5-synopsys-dc-synthesis-clock_period-8/.stamp: 1-hls/.execstamp 1-hls/.postconditions.stamp 1-hls/outputs/.execstamp.design.v 1-hls/outputs/.execstamp.run.vcd 1-hls/outputs/.stamp.design.v 1-hls/outputs/.stamp.run.vcd
5-synopsys-dc-synthesis-clock_period-8/.stamp: 3-skywater-130nm/.execstamp 3-skywater-130nm/.postconditions.stamp 3-skywater-130nm/outputs/.execstamp.adk 3-skywater-130nm/outputs/.stamp.adk
5-synopsys-dc-synthesis-clock_period-8/.stamp: 4-gen-saif-rtl/.execstamp 4-gen-saif-rtl/.postconditions.stamp 4-gen-saif-rtl/outputs/.execstamp.run.saif 4-gen-saif-rtl/outputs/.stamp.run.saif
5-synopsys-dc-synthesis-clock_period-8/.stamp:
	$(call cpdir-and-parameterize,5-synopsys-dc-synthesis-clock_period-8,../../venv/lib/python3.7/site-packages/steps/synopsys-dc-synthesis,5-synopsys-dc-synthesis-clock_period-8/.stamp)
endif

# collect inputs

ifeq ("$(wildcard 5-synopsys-dc-synthesis-clock_period-8/.prebuilt)","")
5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.adk: 5-synopsys-dc-synthesis-clock_period-8/.stamp 3-skywater-130nm/outputs/.stamp.adk
	$(call symlink,5-synopsys-dc-synthesis-clock_period-8/inputs,adk,../../3-skywater-130nm/outputs/adk,.stamp.adk)
endif

ifeq ("$(wildcard 5-synopsys-dc-synthesis-clock_period-8/.prebuilt)","")
5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.constraints.tcl: 5-synopsys-dc-synthesis-clock_period-8/.stamp 0-constraints/outputs/.stamp.constraints.tcl
	$(call symlink,5-synopsys-dc-synthesis-clock_period-8/inputs,constraints.tcl,../../0-constraints/outputs/constraints.tcl,.stamp.constraints.tcl)
endif

ifeq ("$(wildcard 5-synopsys-dc-synthesis-clock_period-8/.prebuilt)","")
5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.design.v: 5-synopsys-dc-synthesis-clock_period-8/.stamp 1-hls/outputs/.stamp.design.v
	$(call symlink,5-synopsys-dc-synthesis-clock_period-8/inputs,design.v,../../1-hls/outputs/design.v,.stamp.design.v)
endif

ifeq ("$(wildcard 5-synopsys-dc-synthesis-clock_period-8/.prebuilt)","")
5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.run.saif: 5-synopsys-dc-synthesis-clock_period-8/.stamp 4-gen-saif-rtl/outputs/.stamp.run.saif
	$(call symlink,5-synopsys-dc-synthesis-clock_period-8/inputs,run.saif,../../4-gen-saif-rtl/outputs/run.saif,.stamp.run.saif)
endif

# execute

ifeq ("$(wildcard 5-synopsys-dc-synthesis-clock_period-8/.prebuilt)","")

define 5_synopsys_dc_synthesis_clock_period_8_commands_rule
	mkdir -p 5-synopsys-dc-synthesis-clock_period-8/outputs && set -o pipefail && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t synopsys-dc-synthesis-clock_period-8 && chmod +x .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-run && cp -f .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-run 5-synopsys-dc-synthesis-clock_period-8 && if [[ -e .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-debug ]]; then chmod +x .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-debug && cp -f .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-debug 5-synopsys-dc-synthesis-clock_period-8; fi && if [[ -e .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-check-preconditions.py ]]; then chmod +x .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-check-preconditions.py && cp -f .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-check-preconditions.py 5-synopsys-dc-synthesis-clock_period-8; fi && if [[ -e .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-check-postconditions.py ]]; then chmod +x .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-check-postconditions.py && cp -f .mflowgen/5-synopsys-dc-synthesis-clock_period-8/mflowgen-check-postconditions.py 5-synopsys-dc-synthesis-clock_period-8; fi && cd 5-synopsys-dc-synthesis-clock_period-8 && if [[ -e mflowgen-check-preconditions.py ]]; then ./mflowgen-check-preconditions.py || exit 1; fi && ./mflowgen-run 2>&1 | tee mflowgen-run.log || exit 1 && cd .. && touch -c 5-synopsys-dc-synthesis-clock_period-8/outputs/*
endef

5-synopsys-dc-synthesis-clock_period-8/.execstamp: 5-synopsys-dc-synthesis-clock_period-8/.stamp 5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.adk 5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.constraints.tcl 5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.design.v 5-synopsys-dc-synthesis-clock_period-8/inputs/.stamp.run.saif
	$(call 5_synopsys_dc_synthesis_clock_period_8_commands_rule)
	touch $@

5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v: 5-synopsys-dc-synthesis-clock_period-8/.execstamp
	touch $@
5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc: 5-synopsys-dc-synthesis-clock_period-8/.execstamp
	touch $@
5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap: 5-synopsys-dc-synthesis-clock_period-8/.execstamp
	touch $@
5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf: 5-synopsys-dc-synthesis-clock_period-8/.execstamp
	touch $@
5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf: 5-synopsys-dc-synthesis-clock_period-8/.execstamp
	touch $@

endif

# collect outputs

5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.v: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v
	$(call stamp,5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.v)

5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.sdc: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v
	$(call stamp,5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.sdc)

5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.namemap: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v
	$(call stamp,5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.namemap)

5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.svf: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v
	$(call stamp,5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.svf)

5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.upf: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v
	$(call stamp,5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.upf)

# post-conditions

define 5_synopsys_dc_synthesis_clock_period_8_post_conditions_commands_rule
	cd 5-synopsys-dc-synthesis-clock_period-8 && if [[ -e mflowgen-check-postconditions.py ]]; then ./mflowgen-check-postconditions.py || exit 1; fi && cd ..
endef

5-synopsys-dc-synthesis-clock_period-8/.postconditions.stamp: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.v
	$(call 5_synopsys_dc_synthesis_clock_period_8_post_conditions_commands_rule)
	touch $@

# alias

.PHONY: synopsys-dc-synthesis-clock_period-8

synopsys-dc-synthesis-clock_period-8: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/.postconditions.stamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.v

.PHONY: 5

5: 5-synopsys-dc-synthesis-clock_period-8/.execstamp 5-synopsys-dc-synthesis-clock_period-8/.postconditions.stamp 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.execstamp.design.v 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.namemap 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.sdc 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.svf 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.upf 5-synopsys-dc-synthesis-clock_period-8/outputs/.stamp.design.v

# debug

define 5_debug_rule
	cd 5-synopsys-dc-synthesis-clock_period-8 && ./mflowgen-debug 2>&1 | tee mflowgen-debug.log
endef

debug-synopsys-dc-synthesis-clock_period-8: 
	$(call 5_debug_rule)

.PHONY: debug-5

debug-5: debug-synopsys-dc-synthesis-clock_period-8

# ------------------------------------------------------------------------
# Misc
# ------------------------------------------------------------------------

# Clean

.PHONY: clean-all

clean-all:
	@find . -maxdepth 1 ! -name Makefile ! -name .mflowgen\* ! -name \. ! -name \.\. -exec rm -rf {} +

.PHONY: clean-0

clean-0:
	rm -rf ./0-constraints

.PHONY: clean-constraints

clean-constraints: clean-0

.PHONY: clean-1

clean-1:
	rm -rf ./1-hls

.PHONY: clean-hls

clean-hls: clean-1

.PHONY: clean-2

clean-2:
	rm -rf ./2-info

.PHONY: clean-info

clean-info: clean-2

.PHONY: clean-3

clean-3:
	rm -rf ./3-skywater-130nm

.PHONY: clean-skywater-130nm

clean-skywater-130nm: clean-3

.PHONY: clean-4

clean-4:
	rm -rf ./4-gen-saif-rtl

.PHONY: clean-gen-saif-rtl

clean-gen-saif-rtl: clean-4

.PHONY: clean-5

clean-5:
	rm -rf ./5-synopsys-dc-synthesis-clock_period-8

.PHONY: clean-synopsys-dc-synthesis-clock_period-8

clean-synopsys-dc-synthesis-clock_period-8: clean-5

# Diff

.PHONY: diff-0

diff-0:
	@echo && diff -r -u --minimal --exclude={configure.yml,.time_end,.time_start,mflowgen-run*,mflowgen-debug,.stamp,inputs,outputs} ../design/constraints 0-constraints | grep --color=always -e '^-.*' -e '$$' -e 'Only in ../design/constraints.*' | GREP_COLOR='01;32' grep --color=always -e '^+.*' -e '$$' -e 'Only in 0-constraints.*' && echo || true

.PHONY: diff-1

diff-1:
	@echo && diff -r -u --minimal --exclude={configure.yml,.time_end,.time_start,mflowgen-run*,mflowgen-debug,.stamp,inputs,outputs} ../design/hls 1-hls | grep --color=always -e '^-.*' -e '$$' -e 'Only in ../design/hls.*' | GREP_COLOR='01;32' grep --color=always -e '^+.*' -e '$$' -e 'Only in 1-hls.*' && echo || true

.PHONY: diff-2

diff-2:
	@echo && diff -r -u --minimal --exclude={configure.yml,.time_end,.time_start,mflowgen-run*,mflowgen-debug,.stamp,inputs,outputs} ../../venv/lib/python3.7/site-packages/steps/info 2-info | grep --color=always -e '^-.*' -e '$$' -e 'Only in ../../venv/lib/python3.7/site-packages/steps/info.*' | GREP_COLOR='01;32' grep --color=always -e '^+.*' -e '$$' -e 'Only in 2-info.*' && echo || true

.PHONY: diff-3

diff-3:
	@echo && diff -r -u --minimal --exclude={configure.yml,.time_end,.time_start,mflowgen-run*,mflowgen-debug,.stamp,inputs,outputs} ../../../../../../../../farmshare/home/classes/ee/272/skywater-130nm-adk.v2021 3-skywater-130nm | grep --color=always -e '^-.*' -e '$$' -e 'Only in ../../../../../../../../farmshare/home/classes/ee/272/skywater-130nm-adk.v2021.*' | GREP_COLOR='01;32' grep --color=always -e '^+.*' -e '$$' -e 'Only in 3-skywater-130nm.*' && echo || true

.PHONY: diff-4

diff-4:
	@echo && diff -r -u --minimal --exclude={configure.yml,.time_end,.time_start,mflowgen-run*,mflowgen-debug,.stamp,inputs,outputs} ../../venv/lib/python3.7/site-packages/steps/synopsys-vcd2saif-convert 4-gen-saif-rtl | grep --color=always -e '^-.*' -e '$$' -e 'Only in ../../venv/lib/python3.7/site-packages/steps/synopsys-vcd2saif-convert.*' | GREP_COLOR='01;32' grep --color=always -e '^+.*' -e '$$' -e 'Only in 4-gen-saif-rtl.*' && echo || true

.PHONY: diff-5

diff-5:
	@echo && diff -r -u --minimal --exclude={configure.yml,.time_end,.time_start,mflowgen-run*,mflowgen-debug,.stamp,inputs,outputs} ../../venv/lib/python3.7/site-packages/steps/synopsys-dc-synthesis 5-synopsys-dc-synthesis-clock_period-8 | grep --color=always -e '^-.*' -e '$$' -e 'Only in ../../venv/lib/python3.7/site-packages/steps/synopsys-dc-synthesis.*' | GREP_COLOR='01;32' grep --color=always -e '^+.*' -e '$$' -e 'Only in 5-synopsys-dc-synthesis-clock_period-8.*' && echo || true

# Info

.PHONY: info-0

info-0:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t constraints && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-info -y .mflowgen/0-constraints/configure.yml

.PHONY: info-1

info-1:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t hls && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-info -y .mflowgen/1-hls/configure.yml

.PHONY: info-2

info-2:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t info && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-info -y .mflowgen/2-info/configure.yml

.PHONY: info-3

info-3:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t skywater-130nm && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-info -y .mflowgen/3-skywater-130nm/configure.yml

.PHONY: info-4

info-4:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t gen-saif-rtl && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-info -y .mflowgen/4-gen-saif-rtl/configure.yml

.PHONY: info-5

info-5:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-letters -c -t synopsys-dc-synthesis-clock_period-8 && /home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-info -y .mflowgen/5-synopsys-dc-synthesis-clock_period-8/configure.yml

# Runtimes

.PHONY: runtimes

runtimes:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-runtimes

# List

.PHONY: list

list:
	@echo && echo Generic Targets: && echo && printf " - %s\\n" "list      -- List all steps" "status    -- Print build status for each step" "runtimes  -- Print runtimes for each step" "graph     -- Generate a PDF of the step dependency graph" "clean-all -- Remove all build directories" "clean-N   -- Clean target N" "info-N    -- Print configured info for step N" "diff-N    -- Diff target N" && echo && echo Targets: && echo && printf " - %s\\n" "  0 : constraints" "  1 : hls" "  2 : info" "  3 : skywater-130nm" "  4 : gen-saif-rtl" "  5 : synopsys-dc-synthesis-clock_period-8" && echo && echo Debug Targets: && echo && printf " - %s\\n" "debug-5  : debug-synopsys-dc-synthesis-clock_period-8" && echo

# Graph

.PHONY: graph

graph:
	dot -Tpdf .mflowgen/graph.dot > graph.pdf

# Status

.PHONY: status

status:
	@/home/users/mah2001/ee372/EE372/flowgen/venv/lib/python3.7/site-packages/mflowgen/scripts/mflowgen-status --backend make -s 0-constraints,1-hls,2-info,3-skywater-130nm,4-gen-saif-rtl,5-synopsys-dc-synthesis-clock_period-8

