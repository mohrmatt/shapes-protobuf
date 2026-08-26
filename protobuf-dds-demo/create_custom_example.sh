#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ARCH="${ARCH:-x64Linux4gcc8.5.0}"
OUTPUT_ROOT="$SCRIPT_DIR/custom-examples"
PROTO_FILE=""
TYPE_NAME=""
EXAMPLE_NAME=""
USE_DDS_OPTIONS=0
PROTO_PATHS=()

usage() {
    cat <<'USAGE'
Usage:
  ./create_custom_example.sh --proto FILE --type TYPE --name NAME [options]

Required:
  --proto FILE       Proto file to generate from
  --type TYPE        Top-level protobuf message used by the example
  --name NAME        New directory name under custom-examples/

Options:
  --proto-path DIR   Additional protobuf import path (repeatable)
  --dds-options      Include RTI's bundled omg/dds/descriptor.proto options
  --output-root DIR  Parent output directory (default: custom-examples/)
  -h, --help         Show this help

The script generates source and an example makefile, but does not customize
publisher/subscriber behavior or compile the generated application.
USAGE
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --proto)
            PROTO_FILE="${2:-}"
            shift 2
            ;;
        --type)
            TYPE_NAME="${2:-}"
            shift 2
            ;;
        --name)
            EXAMPLE_NAME="${2:-}"
            shift 2
            ;;
        --proto-path)
            PROTO_PATHS+=("${2:-}")
            shift 2
            ;;
        --dds-options)
            USE_DDS_OPTIONS=1
            shift
            ;;
        --output-root)
            OUTPUT_ROOT="${2:-}"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "ERROR: unknown argument: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

: "${NDDSHOME:?NDDSHOME is not set. Set it to your RTI Connext DDS installation.}"

if [[ -z "$PROTO_FILE" || -z "$TYPE_NAME" || -z "$EXAMPLE_NAME" ]]; then
    echo "ERROR: --proto, --type, and --name are required." >&2
    usage >&2
    exit 2
fi

if [[ ! -f "$PROTO_FILE" ]]; then
    echo "ERROR: proto file not found: $PROTO_FILE" >&2
    exit 1
fi

if [[ ! "$EXAMPLE_NAME" =~ ^[A-Za-z0-9._-]+$ ]]; then
    echo "ERROR: --name may contain only letters, numbers, dots, underscores, and hyphens." >&2
    exit 2
fi

for proto_path in "${PROTO_PATHS[@]}"; do
    if [[ ! -d "$proto_path" ]]; then
        echo "ERROR: protobuf import path not found: $proto_path" >&2
        exit 1
    fi
done

OUTPUT_DIR="$(mkdir -p "$OUTPUT_ROOT" && cd "$OUTPUT_ROOT" && pwd)/$EXAMPLE_NAME"
if [[ -e "$OUTPUT_DIR" ]]; then
    echo "ERROR: output already exists: $OUTPUT_DIR" >&2
    echo "Choose another --name or remove the existing directory." >&2
    exit 1
fi

source "$NDDSHOME/resource/scripts/rtisetenv_${ARCH}.bash"
export PATH="$NDDSHOME/bin:$PATH"

for command in protoc rtiddsgen; do
    if ! command -v "$command" >/dev/null 2>&1; then
        echo "ERROR: required command not found: $command" >&2
        exit 1
    fi
done

PROTO_FILE="$(cd "$(dirname "$PROTO_FILE")" && pwd)/$(basename "$PROTO_FILE")"
PROTO_DIR="$(dirname "$PROTO_FILE")"
PROTO_BASENAME="$(basename "$PROTO_FILE")"
PROTO_STEM="${PROTO_BASENAME%.proto}"

mkdir -p "$OUTPUT_DIR/source"
cp "$PROTO_FILE" "$OUTPUT_DIR/source/$PROTO_BASENAME"

PROTOC_INCLUDES=(-I "$PROTO_DIR" -I "$NDDSHOME/include")
for proto_path in "${PROTO_PATHS[@]}"; do
    PROTOC_INCLUDES+=(-I "$proto_path")
done
if [[ "$USE_DDS_OPTIONS" -eq 1 ]]; then
    PROTOC_INCLUDES+=(-I "$SCRIPT_DIR/proto")
fi

echo "==> Generating protobuf, IDL, and Connext integration source"
protoc \
    "${PROTOC_INCLUDES[@]}" \
    --idl4_out="$OUTPUT_DIR" \
    --cpp_out="$OUTPUT_DIR" \
    --connext-cpp_out="$OUTPUT_DIR" \
    "$PROTO_FILE"

if [[ "$USE_DDS_OPTIONS" -eq 1 ]]; then
    echo "==> Generating RTI DDS options dependency"
    mkdir -p "$OUTPUT_DIR/omg/dds"
    protoc \
        -I "$SCRIPT_DIR/proto" \
        --cpp_out="$OUTPUT_DIR" \
        "$SCRIPT_DIR/proto/omg/dds/descriptor.proto"
fi

IDL_FILE="$OUTPUT_DIR/$PROTO_STEM.idl"
if [[ ! -f "$IDL_FILE" ]]; then
    echo "ERROR: expected generated IDL not found: $IDL_FILE" >&2
    exit 1
fi

echo "==> Generating DDS type support and publisher/subscriber templates"
rtiddsgen \
    -language C++11 \
    -standard PROTOBUF_CPP \
    -example "$ARCH" \
    -replace \
    -I "$OUTPUT_DIR" \
    -d "$OUTPUT_DIR" \
    "$IDL_FILE"

MAKEFILE="$OUTPUT_DIR/makefile_${PROTO_STEM}_${ARCH}"
if [[ ! -f "$MAKEFILE" ]]; then
    echo "ERROR: expected generated makefile not found: $MAKEFILE" >&2
    exit 1
fi

echo "==> Patching generated makefile for protobuf"
python3 - "$MAKEFILE" "$PROTO_STEM" "$USE_DDS_OPTIONS" <<'PY'
import pathlib
import sys

makefile = pathlib.Path(sys.argv[1])
proto_stem = sys.argv[2]
use_dds_options = sys.argv[3] == "1"
content = makefile.read_text()

def replace_once(old, new, description):
    global content
    if content.count(old) != 1:
        raise SystemExit(
            f"ERROR: could not uniquely patch {description} in {makefile.name}"
        )
    content = content.replace(old, new, 1)

replace_once(
    "$(STATIC_LIBRARIES) $(SYSLIBS)",
    "$(STATIC_LIBRARIES) -lprotobuf $(SYSLIBS)",
    "protobuf link library",
)

source_line = next(
    (line for line in content.splitlines() if line.startswith("SOURCES = ")),
    None,
)
if source_line is None:
    raise SystemExit(f"ERROR: SOURCES line not found in {makefile.name}")

protobuf_sources = [f"$(SOURCE_DIR){proto_stem}.pb.cc"]
if use_dds_options:
    protobuf_sources.append("$(SOURCE_DIR)omg/dds/descriptor.pb.cc")
replace_once(
    source_line,
    source_line + " " + " ".join(protobuf_sources),
    "protobuf sources",
)

common_objects = "COMMONOBJS    = $(COMMONSOURCES:%.cxx=objs/$(TARGET_ARCH)/%.o)"
replace_once(
    common_objects,
    "COMMONOBJS_CXX = $(filter %.cxx,$(COMMONSOURCES))\n"
    "COMMONOBJS_CC  = $(filter %.cc,$(COMMONSOURCES))\n"
    "COMMONOBJS     = $(COMMONOBJS_CXX:%.cxx=objs/$(TARGET_ARCH)/%.o) "
    "$(COMMONOBJS_CC:%.cc=objs/$(TARGET_ARCH)/%.o)",
    "mixed C++ object sources",
)

compile_rule_marker = "\n#\n# Uncomment these lines if you want the support files regenerated"
rules = [
    f"objs/$(TARGET_ARCH)/{proto_stem}.pb.o : "
    f"$(SOURCE_DIR){proto_stem}.pb.cc $(SOURCE_DIR){proto_stem}.pb.h\n"
    "\t$(COMPILER) $(COMPILER_FLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<\n"
]
if use_dds_options:
    rules.append(
        "objs/$(TARGET_ARCH)/descriptor.pb.o : "
        "$(SOURCE_DIR)omg/dds/descriptor.pb.cc "
        "$(SOURCE_DIR)omg/dds/descriptor.pb.h\n"
        "\t$(COMPILER) $(COMPILER_FLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<\n"
    )
replace_once(
    compile_rule_marker,
    "\n" + "\n".join(rules) + compile_rule_marker,
    "protobuf compile rules",
)

makefile.write_text(content)
PY

cat > "$OUTPUT_DIR/NEXT_STEPS.txt" <<NEXT_STEPS
Custom example generated from:
  $PROTO_FILE

Selected top-level message:
  $TYPE_NAME

DDS options enabled:
  $([[ "$USE_DDS_OPTIONS" -eq 1 ]] && echo yes || echo no)

Before building:
1. In ${PROTO_STEM}_publisher.cxx, populate required fields on sample.data()
    before writer.write(). Set DDS key fields before registering or writing.
2. In ${PROTO_STEM}_subscriber.cxx, replace generic sample output with the
    protobuf getters or application processing required for your fields.
3. If the proto imports other user-defined protos, generate their C++ sources
    and add each additional .pb.cc file and compile rule to the makefile.
4. Build from this directory:
    make -f $(basename "$MAKEFILE")

The reference build_dds, build_key, and build_no_key directories were not modified.
NEXT_STEPS

echo ""
echo "Generated custom example: $OUTPUT_DIR"
echo "Read $OUTPUT_DIR/NEXT_STEPS.txt before building."