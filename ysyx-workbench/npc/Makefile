TOPNAME = ysyx_22040575_RVNPC
INC_PATH ?=

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build --trace -cc  \
				-O3 --x-assign fast --x-initial fast --noassert

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)

default: $(BIN)
$(shell mkdir -p $(BUILD_DIR))

#sim project source
VSRCS_SIM = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS_SIM = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""
LDFLAGS += -lSDL2 -lSDL2_image

$(BIN): $(VSRCS) $(CSRCS) 
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		-top $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))
	
sim:
	$(call git_commit, "sim RTL")   
	@echo "Verification based one Vrilator."  	
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		-top $(TOPNAME) $^ \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN)) $(CSRCS_SIM) $(VSRCS_SIM)
	$(BUILD_DIR)/$(TOPNAME)
	gtkwave dump.vcd
	
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean run
