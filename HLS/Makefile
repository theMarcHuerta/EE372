CATAPULT = /cad/mentor/2019.11/Catapult_Synthesis_10.4b-841621/Mgc_home/bin/catapult

gui:
	$(CATAPULT) build.ccs

build/Renderer.v1/rtl.v:
	$(CATAPULT) -shell -file scripts/Renderer.tcl

rtl_test_no_gui: build/Renderer.v1/rtl.v
	$(CATAPULT) -shell -file scripts/run_rtl_test_no_gui.tcl

rtl_test: build/Renderer.v1/rtl.v
	$(CATAPULT) -shell -file scripts/run_rtl_test.tcl

.PHONY: clean gui c_test InputDoubleBuffer WeightDoubleBuffer SystolicArrayCore ProcessingElement
clean:
	rm -rf build.ccs
	rm -rf build
	rm -rf test*

