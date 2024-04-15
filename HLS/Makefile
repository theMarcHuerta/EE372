CATAPULT = /cad/mentor/2019.11/Catapult_Synthesis_10.4b-841621/Mgc_home/bin/catapult

gui:
	$(CATAPULT) build.ccs &

test/Renderer.v1: 
	$(CATAPULT) -shell -file scripts/run_c_test.tcl

build/Renderer.v1/rtl.v:
	$(CATAPULT) -shell -file scripts/Renderer.tcl

c_test: test/Renderer.v1
	cd test/Renderer.v1 && make -f scverify/Verify_orig_cxx_osci.mk sim

rtl_test_no_gui: build/Renderer.v1/rtl.v
	$(CATAPULT) -shell -file scripts/run_rtl_test_no_gui.tcl

rtl_test: build/Renderer.v1/rtl.v
	$(CATAPULT) -shell -file scripts/run_rtl_test.tcl

.PHONY: clean gui c_test InputDoubleBuffer WeightDoubleBuffer SystolicArrayCore ProcessingElement
clean:
	rm -rf build.ccs
	rm -rf build
	rm -rf test*
	rm -rf catapult.log
	rm -rf waves.shm/
	rm -rf .simvision/
	rm -rf design_checker_constraints.tcl
	rm -rf design_checker_pre_build.tcl
