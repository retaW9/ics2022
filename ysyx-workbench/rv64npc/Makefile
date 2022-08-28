TOPNAME = ysyx_22040575_rvnpc
IMG ?=
INC_PATH ?= 
VERILATOR = verilator
# rules for verilator
VERILATOR_CFLAGS += -MMD --build --trace -cc \
				-O3 --x-assign fast --x-initial fast --noassert

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME) 

default: $(BIN)
$(shell mkdir -p $(BUILD_DIR))

#sim project source
VSRCS_SIM = $(shell find $(abspath ./vsrc/riscv_core) -name "*.v")
CSRCS_SIM = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")


INCFLAGS = $(addprefix -I, $(INC_PATH) )
CFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\"" 
LDFLAGS += -lreadline -I$(NEMU_HOME)/build/riscv64-nemu-interpreter-so 

	
run:
	$(call git_commit, "sim RTL")
	@rm -rf $(OBJ_DIR)   
	@echo "Verification based one Vrilator."  	
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		-top $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN)) $(CSRCS_SIM) $(VSRCS_SIM)
	$(BUILD_DIR)/$(TOPNAME) $(IMG) $(NEMU_HOME)/build/riscv64-nemu-interpreter-so
	gtkwave dump.vcd

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean run
