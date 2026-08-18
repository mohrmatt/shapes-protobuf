# shapes-protobuf

Demonstrates using Google Protocol Buffers (protobuf) to define the DDS ShapeType data model for use with RTI Connext DDS. Three build variants show how the same logical type can be expressed with and without DDS key annotations, and how that compares to a traditional IDL-only workflow.

## Repository Structure

| Directory | Description |
|-----------|-------------|
| `build_dds/` | Pure IDL example — type generated with `rtiddsgen` directly from `shape_type_key.idl`, no protobuf runtime |
| `build_no_key/` | Protobuf-based — `ShapeType.proto` defines a plain shape type with no DDS key field |
| `build_key/` | Protobuf-based — `ShapeType_Key.proto` defines a shape type with `color` annotated as a DDS key via `omg.dds.member` options |
| `omg/dds/` | RTI's DDS descriptor proto used for annotations in `ShapeType_Key.proto` |

The `.proto` files live at the repo root. Each `build_*/` directory contains the generated type-support code, publisher, subscriber, makefile, and QoS profile.

## Requirements

- **RTI Connext DDS Professional 7.6.0** — default install path assumed at `/home/matt/rti_connext_dds-7.6.0`; set `NDDSHOME` to override
- **protoc** (Google Protocol Buffers compiler) with RTI's plugins — `protoc-gen-idl4` and `protoc-gen-connext-cpp` are bundled with Connext under `$NDDSHOME/bin`
- **GCC 8.5.0+** — default architecture target is `x64Linux4gcc8.5.0`; set `ARCH` to override

## Generating Source Files Manually

Source the Connext environment and extend `PATH` for RTI's protoc plugins:

```bash
export NDDSHOME=/home/matt/rti_connext_dds-7.6.0
source "$NDDSHOME/resource/scripts/rtisetenv_x64Linux4gcc8.5.0.bash"
export PATH="$NDDSHOME/bin:$PATH"
```

### build_no_key — Protobuf without key

```bash
# Step 1: Generate .pb.cc/.pb.h and .idl from the proto file
protoc \
    -I . -I "$NDDSHOME/include" \
    --idl4_out=build_no_key \
    --cpp_out=build_no_key \
    --connext-cpp_out=build_no_key \
    ShapeType.proto

# Step 2: Generate DDS type-support and example code from the generated IDL
rtiddsgen -language C++11 -standard PROTOBUF_CPP \
    -example x64Linux4gcc8.5.0 -replace \
    -I build_no_key -d build_no_key \
    build_no_key/ShapeType.idl
```

Generated files: `ShapeType.pb.h`, `ShapeType.pb.cc`, `ShapeType.idl`, `ShapeType.hpp`, `ShapeType.cxx`, `ShapeTypePlugin.hpp`, `ShapeTypePlugin.cxx`, `ShapeType_publisher.cxx`, `ShapeType_subscriber.cxx`

### build_key — Protobuf with DDS key annotation

```bash
# Step 1: Generate .pb.cc/.pb.h and .idl from the proto file
mkdir -p build_key/omg/dds
protoc \
    -I . -I "$NDDSHOME/include" \
    --idl4_out=build_key \
    --cpp_out=build_key \
    --connext-cpp_out=build_key \
    ShapeType_Key.proto omg/dds/descriptor.proto

# Step 2: Generate DDS type-support and example code
rtiddsgen -language C++11 -standard PROTOBUF_CPP \
    -example x64Linux4gcc8.5.0 -replace \
    -I build_key -d build_key \
    build_key/ShapeType_Key.idl
```

Generated files: same as above, plus `omg/dds/descriptor.pb.cc` and `descriptor.pb.h` for the DDS annotation dependency.

### build_dds — Pure IDL (no protobuf runtime)

The IDL is already committed to the repo. Generate type-support and example code directly:

```bash
cd build_dds
rtiddsgen -language C++11 \
    -example x64Linux4gcc8.5.0 -replace \
    -d . \
    shape_type_key.idl
```

### Subscriber modification

The default `rtiddsgen` subscriber prints data with `std::cout << sample.data()`. The subscribers in this repo have been modified to print each field individually. For the protobuf variants the fields are accessed as getter methods (`data.color()`, `data.x()`, etc.); for the pure IDL variant they are struct members (`data.color`, `data.x`, etc.).

## Building

Source the Connext environment (if not already done), then run `make` inside each build directory:

```bash
# build_no_key
cd build_no_key && make -f makefile_ShapeType_x64Linux4gcc8.5.0

# build_key
cd build_key  && make -f makefile_ShapeType_Key_x64Linux4gcc8.5.0

# build_dds
cd build_dds  && make -f makefile_shape_type_key_x64Linux4gcc8.5.0
```

Add `DEBUG=1` for a debug build.

## Running

Make sure `USER_QOS_PROFILES.xml` is in the working directory (it is — run from the `build_*/` directory).

```bash
# Publisher
objs/x64Linux4gcc8.5.0/<AppName>_publisher -d <domain_id> -s <sample_count>

# Subscriber
objs/x64Linux4gcc8.5.0/<AppName>_subscriber -d <domain_id> -s <sample_count>
```

App name is `ShapeType_Key`, `ShapeType`, or `shape_type_key` depending on the build directory.

## Automated Script

[`generate_and_build.sh`](generate_and_build.sh) handles all of the above steps in one command:

```bash
# Default (uses NDDSHOME=/home/matt/rti_connext_dds-7.6.0 and ARCH=x64Linux4gcc8.5.0)
./generate_and_build.sh

# Override defaults
NDDSHOME=/opt/rti_connext_dds-7.6.0 ARCH=x64Linux4gcc8.5.0 ./generate_and_build.sh
```
