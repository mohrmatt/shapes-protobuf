#!/bin/bash
# Generates all DDS type-support and example code from .proto/.idl sources,
# patches the subscriber to print individual fields, then compiles all three
# build variants.
set -euo pipefail

ARCH="${ARCH:-x64Linux4gcc8.5.0}"
NDDSHOME="${NDDSHOME:-/home/matt/rti_connext_dds-7.6.0}"
REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ---------------------------------------------------------------------------
# Environment
# ---------------------------------------------------------------------------
echo "==> Sourcing Connext environment ($ARCH)"
source "$NDDSHOME/resource/scripts/rtisetenv_${ARCH}.bash"
export PATH="$NDDSHOME/bin:$PATH"

# ---------------------------------------------------------------------------
# build_no_key — ShapeType without a DDS key field
# ---------------------------------------------------------------------------
echo "==> Generating build_no_key"
cd "$REPO_DIR"
protoc \
    -I . \
    -I "$NDDSHOME/include" \
    --idl4_out=build_no_key \
    --cpp_out=build_no_key \
    --connext-cpp_out=build_no_key \
    ShapeType.proto

cd "$REPO_DIR/build_no_key"
rtiddsgen -language C++11 -standard PROTOBUF_CPP \
    -example "$ARCH" -replace \
    -I . -d . \
    ShapeType.idl

# ---------------------------------------------------------------------------
# build_key — ShapeType with color annotated as a DDS key
# ---------------------------------------------------------------------------
echo "==> Generating build_key"
cd "$REPO_DIR"
mkdir -p build_key/omg/dds
protoc \
    -I . \
    -I "$NDDSHOME/include" \
    --idl4_out=build_key \
    --cpp_out=build_key \
    --connext-cpp_out=build_key \
    ShapeType_Key.proto omg/dds/descriptor.proto

cd "$REPO_DIR/build_key"
rtiddsgen -language C++11 -standard PROTOBUF_CPP \
    -example "$ARCH" -replace \
    -I . -d . \
    ShapeType_Key.idl

# ---------------------------------------------------------------------------
# build_dds — pure IDL example (no protobuf runtime)
# ---------------------------------------------------------------------------
echo "==> Generating build_dds"
cd "$REPO_DIR/build_dds"
rtiddsgen -language C++11 \
    -example "$ARCH" -replace \
    -d . \
    shape_type_key.idl

# ---------------------------------------------------------------------------
# Patch subscribers to print individual fields
# rtiddsgen generates: std::cout << sample.data() << std::endl;
# We replace it with per-field output using the appropriate access style.
# ---------------------------------------------------------------------------
echo "==> Patching subscribers"
python3 - <<'EOF'
import os, sys

REPO = os.path.dirname(os.path.abspath(__file__)) if '__file__' in dir() else os.getcwd()

PATCHES = [
    # (filepath, old_line, new_lines)
    (
        os.path.join(REPO, "build_no_key", "ShapeType_subscriber.cxx"),
        "std::cout << sample.data() << std::endl;",
        ('const ::ShapeType& data = sample.data();\n'
         '            std::cout << "Received ShapeType: "\n'
         '                      << "color=" << data.color()\n'
         '                      << ", x=" << data.x()\n'
         '                      << ", y=" << data.y()\n'
         '                      << ", shapesize=" << data.shapesize() << std::endl;'),
    ),
    (
        os.path.join(REPO, "build_key", "ShapeType_Key_subscriber.cxx"),
        "std::cout << sample.data() << std::endl;",
        ('const ::ShapeType& data = sample.data();\n'
         '            std::cout << "Received ShapeType: "\n'
         '                      << "color=" << data.color()\n'
         '                      << ", x=" << data.x()\n'
         '                      << ", y=" << data.y()\n'
         '                      << ", shapesize=" << data.shapesize() << std::endl;'),
    ),
    (
        os.path.join(REPO, "build_dds", "shape_type_key_subscriber.cxx"),
        "std::cout << sample.data() << std::endl;",
        ('const ::ShapeType& data = sample.data();\n'
         '            std::cout << "Received ShapeType: "\n'
         '                      << "color=" << data.color\n'
         '                      << ", x=" << data.x\n'
         '                      << ", y=" << data.y\n'
         '                      << ", shapesize=" << data.shapesize << std::endl;'),
    ),
]

all_ok = True
for filepath, old, new in PATCHES:
    with open(filepath, 'r') as f:
        content = f.read()
    if old in content:
        with open(filepath, 'w') as f:
            f.write(content.replace(old, new, 1))
        print(f"  Patched {os.path.relpath(filepath)}")
    else:
        print(f"  WARNING: pattern not found in {os.path.relpath(filepath)} — may already be patched or template changed")
        all_ok = False

sys.exit(0 if all_ok else 1)
EOF

# ---------------------------------------------------------------------------
# Build
# ---------------------------------------------------------------------------
echo "==> Building build_no_key"
cd "$REPO_DIR/build_no_key"
make -f "makefile_ShapeType_${ARCH}"

echo "==> Building build_key"
cd "$REPO_DIR/build_key"
make -f "makefile_ShapeType_Key_${ARCH}"

echo "==> Building build_dds"
cd "$REPO_DIR/build_dds"
make -f "makefile_shape_type_key_${ARCH}"

echo ""
echo "Done. Binaries are in each build directory under objs/${ARCH}/"
EOF
