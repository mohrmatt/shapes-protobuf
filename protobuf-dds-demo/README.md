# Protobuf DDS Demo

This demo shows how Google Protocol Buffers types integrate with RTI Connext
DDS. It supports two distinct workflows:

| Goal | Workflow |
|------|----------|
| Build and compare the committed ShapeType examples | [Workflow 1: Reference Examples](#workflow-1-reference-examples) |
| Generate an isolated example from your own proto | [Workflow 2: Custom Proto](#workflow-2-custom-proto) |

The generated source for the reference examples is already committed. You do
not need to run `protoc` or `rtiddsgen` to build and run them.

## Directory Structure

| Path | Description |
|------|-------------|
| `proto/` | Source `.proto` files and the `omg/dds/` DDS descriptor dependency |
| `idl/` | Source IDL file |
| `build_dds/` | Native DDS example. IDL (`shape_type_key.idl`) was generated from `ShapeType_Key.proto` via `protoc --idl4_out`, then used with `rtiddsgen` directly; illustrates how easily a protobuf definition converts to IDL with no protobuf runtime dependency |
| `build_no_key/` | Protobuf-based baseline. `ShapeType.proto` has no DDS key or extensibility annotations, showing what plain protobuf integration looks like. Without the key annotation the type is unkeyed, so DDS instance tracking and keyed topic benefits are unavailable - this highlights the value of the annotations shown in `build_key` |
| `build_key/` | Protobuf-based. `ShapeType_Key.proto` defines a shape type with `color` annotated as a DDS key via `omg.dds.member` options |
| `create_custom_example.sh` | Generates an isolated example from a user-provided proto without modifying the reference builds |
| `custom-examples/` | Default output location for user-generated examples |

`build_key` and `build_dds` define the same keyed, appendable DDS type and use
the same `Circle` topic. Their publishers and subscribers interoperate in both
directions, demonstrating that protobuf-backed and native DDS applications can
exchange the same data type.

## Requirements

- **RTI Connext DDS Professional 7.7.0**
- **GCC 8.5.0+**: default architecture target is `x64Linux4gcc8.5.0`; set `ARCH` to override
- **Custom generation only**: `protoc`, Python 3, and RTI's
  `protoc-gen-idl4` and `protoc-gen-connext-cpp` plugins from `$NDDSHOME/bin`

## Workflow 1: Reference Examples

All generated source files (`.pb.cc`, `.pb.h`, `.idl`, type-support code) are
already committed. To build the publisher and subscriber binaries, source the
Connext environment and run the makefile for each desired variant:

```bash
export NDDSHOME=/path/to/rti_connext_dds-7.7.0
source "$NDDSHOME/resource/scripts/rtisetenv_x64Linux4gcc8.5.0.bash"

make -C build_no_key \
    -f makefile_ShapeType_x64Linux4gcc8.5.0

make -C build_key \
    -f makefile_ShapeType_Key_x64Linux4gcc8.5.0

make -C build_dds \
    -f makefile_shape_type_key_x64Linux4gcc8.5.0
```

Add `DEBUG=1` for a debug build.

Run each application from its build directory so it finds the local
`USER_QOS_PROFILES.xml` file:

```bash
# Terminal 1
cd build_key
objs/x64Linux4gcc8.5.0/ShapeType_Key_subscriber \
    -d <domain_id> -s <sample_count>

# Terminal 2
cd build_key
objs/x64Linux4gcc8.5.0/ShapeType_Key_publisher \
    -d <domain_id> -s <sample_count>
```

## Workflow 2: Custom Proto

Use `create_custom_example.sh` for your own protobuf definition.

```bash
export NDDSHOME=/path/to/rti_connext_dds-7.7.0

./create_custom_example.sh \
    --proto /path/to/MyType.proto \
    --type MyType \
    --name my-type-example
```

Append repeatable `--proto-path /path/to/imports` arguments when your schema
uses additional protobuf import directories.

### Optional DDS protobuf options

Most existing protobuf definitions do not import or use RTI's DDS options and
do not need additional arguments. Generate them with the default command above.

Use `--dds-options` when the proto imports `omg/dds/descriptor.proto` and uses
options such as `(omg.dds.type)` or `(omg.dds.member).key`:

```protobuf
import "omg/dds/descriptor.proto";

message SensorReading {
    option (omg.dds.type) = { extensibility: APPENDABLE };
    string sensor_id = 1 [(omg.dds.member).key = true];
    double value = 2;
}
```

```bash
./create_custom_example.sh \
    --proto /path/to/SensorReading.proto \
    --type SensorReading \
    --name sensor-reading \
    --dds-options
```

This flag adds the repository's `proto/` directory to the protobuf import path
and generates the C++ dependency for `omg/dds/descriptor.proto` in the custom
example. It does not add DDS annotations to the user's proto automatically.

### Required changes after generation

The script generates source, an RTI example template, and patches the generated
makefile to link protobuf and compile the generated `.pb.cc` files. It does not
customize application behavior or compile the result. It writes a checklist to
`custom-examples/<name>/NEXT_STEPS.txt`. Before building, complete these steps:

1. **Publisher** - in the write loop, populate every required field on
    `sample.data()` using the generated protobuf setter methods before the call
    to `writer.write()`.

2. **Subscriber** - replace the generated generic sample output with the
    protobuf getter calls or application processing required for your fields.

3. **Makefile review** - the script adds the primary generated `.pb.cc` file
    and, with `--dds-options`, `omg/dds/descriptor.pb.cc`. If the proto imports
    other user-defined protos, generate their C++ sources and add each additional
    `.pb.cc` file and compile rule to the makefile. The standard modifications
    are documented in
    [`makefile_protobuf_changes.md`](makefile_protobuf_changes.md).

4. **Build** - run the generated makefile only after completing the changes
    above.

## Appendix: Reference Generation Commands

The following commands document how the committed `build_*` reference examples
were generated. They are not used by `create_custom_example.sh`. Running them
with `-replace` will overwrite customizations in those directories.

Source the Connext environment and extend `PATH` for RTI's protoc plugins:

```bash
export NDDSHOME=/path/to/rti_connext_dds-7.7.0
source "$NDDSHOME/resource/scripts/rtisetenv_x64Linux4gcc8.5.0.bash"
export PATH="$NDDSHOME/bin:$PATH"
```

### build_no_key: Protobuf without DDS annotations (baseline)

```bash
# Step 1: Generate .pb.cc/.pb.h and .idl from the proto file
protoc \
    -I proto -I "$NDDSHOME/include" \
    --idl4_out=build_no_key \
    --cpp_out=build_no_key \
    --connext-cpp_out=build_no_key \
    proto/ShapeType.proto

# Step 2: Generate DDS type-support and example code from the generated IDL
rtiddsgen -language C++11 -standard PROTOBUF_CPP \
    -example x64Linux4gcc8.5.0 -replace \
    -I build_no_key -d build_no_key \
    build_no_key/ShapeType.idl
```

Generated files: `ShapeType.pb.h`, `ShapeType.pb.cc`, `ShapeType.idl`, `ShapeType.hpp`, `ShapeType.cxx`, `ShapeTypePlugin.hpp`, `ShapeTypePlugin.cxx`, `ShapeType_publisher.cxx`, `ShapeType_subscriber.cxx`

### build_key: Protobuf with DDS key annotation

```bash
# Step 1: Generate .idl, .pb.cc/.pb.h, and Connext-augmented pb files from the proto
protoc \
    -I proto -I "$NDDSHOME/include" \
    --idl4_out=build_key \
    --cpp_out=build_key \
    --connext-cpp_out=build_key \
    proto/ShapeType_Key.proto

# Step 2: Compile the DDS annotation dependency separately to avoid generating unwanted files
mkdir -p build_key/omg/dds
protoc \
    -I proto \
    --cpp_out=build_key \
    proto/omg/dds/descriptor.proto

# Step 3: Generate DDS type-support and example code
rtiddsgen -language C++11 -standard PROTOBUF_CPP \
    -example x64Linux4gcc8.5.0 -replace \
    -d build_key \
    build_key/ShapeType_Key.idl
```

Generated files: same as above, plus `omg/dds/descriptor.pb.cc` and `descriptor.pb.h` for the DDS annotation dependency.

### build_dds: Pure IDL (no protobuf runtime)

This variant shows how easily a protobuf definition can be converted to IDL. A single `protoc` command with RTI's `--idl4_out` plugin translates the `.proto` file directly to `.idl`, after which the standard `rtiddsgen` workflow takes over with no protobuf runtime involved.

```bash
# Step 1: Convert the proto definition to IDL 
protoc \
    -I proto \
    -I "$NDDSHOME/include" \
    --idl4_out=build_dds \
    proto/ShapeType_Key.proto

# Step 2: Generate DDS type-support and example code from the generated IDL
cd build_dds
rtiddsgen -language C++11 \
    -example x64Linux4gcc8.5.0 -replace \
    -d . \
    shape_type_key.idl
```
