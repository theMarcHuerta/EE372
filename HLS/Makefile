MODULE ?= RTcore

CATAPULT = /cad/mentor/2019.11/Catapult_Synthesis_10.4b-841621/Mgc_home/bin/catapult

gui:
	$(CATAPULT) build.ccs &

test/$(MODULE).v1: 
	export MODULE=$(MODULE); \
	$(CATAPULT) -shell -file scripts/run_c_test.tcl

build/$(MODULE).v1/rtl.v:
	export MODULE=$(MODULE); \
	$(CATAPULT) -shell -file scripts/build_rtl.tcl

build/WorldHit.v1/rtl.v: src/WorldHit.h
	$(CATAPULT) -shell -file scripts/WorldHit.tcl

WorldHit: build/WorldHit.v1/rtl.v

c_test: test/$(MODULE).v1
	export MODULE=$(MODULE); \
	cd test/$(MODULE).v1 && make -f scverify/Verify_orig_cxx_osci.mk sim

rtl_test_no_gui: build/$(MODULE).v1/rtl.v
	export MODULE=$(MODULE); \
	$(CATAPULT) -shell -file scripts/run_rtl_test_no_gui.tcl

rtl_test: build/$(MODULE).v1/rtl.v
	export MODULE=$(MODULE); \
	$(CATAPULT) -shell -file scripts/run_rtl_test.tcl

.PHONY: clean gui c_test
clean:
	rm -rf build.ccs
	rm -rf build
	rm -rf test/
	rm -rf test.ccs
	rm -rf catapult.log
	rm -rf waves.shm/
	rm -rf .simvision/
	rm -rf design_checker_constraints.tcl
	rm -rf design_checker_pre_build.tcl
	rm -rf Catapult*