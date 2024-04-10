
PROTOC = protoc

ifneq (, $(shell protoc --version 2> /dev/null))
	# Using protoc binary on path
	PROTOC = protoc
else ifneq (, $(shell python3 --version 2> /dev/null))
	# Using python3 (on Linux)
	PROTOC = python3 -m grpc_tools.protoc --plugin=protoc-gen-nanopb.exe
else
	# Using python (on Windows)
	PROTOC = python -m grpc_tools.protoc --plugin=protoc-gen-nanopb.exe
endif

PROTO_SRC_DIR = .
PROTO_SRCS = telemetry.proto

NANOPB_SRC_DIR = ../firmware/NanoPB
PYTHON_SOURCE_DIR = ../software

NANOPB_SRCS = $(addprefix $(NANOPB_SRC_DIR)/,$(notdir $(PROTO_SRCS:.proto=.pb.c)))
NANOPB_HDRS = $(NANOPB_SRCS:.pb.c=.pb.h)

PYTHON_SRCS = $(addprefix $(PYTHON_SOURCE_DIR)/,$(notdir $(PROTO_SRCS:.proto=_pb2.py)))
PYTHON_SRCS += $(PYTHON_SOURCE_DIR)/nanopb_pb2.py # Unused import but unfortunately necessary
PYTHON_STUBS = $(PYTHON_SRCS:.py=.pyi)

all: nanopb python

nanopb: $(NANOPB_SRCS) $(NANOPB_HDRS)

python: $(PYTHON_SRCS) $(PYTHON_STUBS)

# Rule for building .pb.c and .pb.h
$(NANOPB_SRC_DIR)/%.pb.c $(NANOPB_SRC_DIR)/%.pb.h: %.proto %.options
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=$(NANOPB_SRC_DIR)/. -I$(PROTO_SRC_DIR) $<

$(NANOPB_SRC_DIR)/%.pb.c $(NANOPB_SRC_DIR)/%.pb.h: %.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=$(NANOPB_SRC_DIR)/. -I$(PROTO_SRC_DIR) $<

$(PYTHON_SOURCE_DIR)/%_pb2.py: %.proto
	$(PROTOC) $(PROTOC_OPTS) --python_out=$(PYTHON_SOURCE_DIR) -I$(PROTO_SRC_DIR) $<

$(PYTHON_SOURCE_DIR)/%_pb2.pyi: %.proto
	$(PROTOC) $(PROTOC_OPTS) --pyi_out=$(PYTHON_SOURCE_DIR) -I$(PROTO_SRC_DIR) $<

.PHONY: all nanopb python