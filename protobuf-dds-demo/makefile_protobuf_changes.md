# Makefile Changes for Protobuf Integration

This document describes the changes required to an `rtiddsgen -example`-generated
makefile in order to build a Connext DDS application that uses Protobuf types.

The diff below compares the unmodified `rtiddsgen` output (`build_dds/`) against
the protobuf-enabled makefile (`build_key/`). Type-name renaming (e.g.
`shape_type_key` → `ShapeType_Key`) is omitted - those are just a consequence of
the IDL file name and are not protobuf-specific.

---

## Diff

```diff
--- makefile_shape_type_key_x64Linux4gcc8.5.0   (rtiddsgen output, no protobuf)
+++ makefile_ShapeType_Key_x64Linux4gcc8.5.0    (protobuf-enabled)

@@ LIBS - link the protobuf runtime @@

-       $(STATIC_LIBRARIES) $(SYSLIBS)
+       $(STATIC_LIBRARIES) -lprotobuf $(SYSLIBS)

@@ SOURCES - add the two protobuf-generated .pb.cc files @@

-SOURCES = $(SOURCE_DIR)ShapeType_KeyPlugin.cxx $(SOURCE_DIR)ShapeType_Key.cxx
+SOURCES = $(SOURCE_DIR)ShapeType_KeyPlugin.cxx $(SOURCE_DIR)ShapeType_Key.cxx \
+          $(SOURCE_DIR)ShapeType_Key.pb.cc \
+          $(SOURCE_DIR)omg/dds/descriptor.pb.cc

@@ COMMONOBJS - split pattern substitution to handle both .cxx and .cc extensions @@

-COMMONOBJS    = $(COMMONSOURCES:%.cxx=objs/$(TARGET_ARCH)/%.o)
+COMMONOBJS_CXX = $(filter %.cxx,$(COMMONSOURCES))
+COMMONOBJS_CC  = $(filter %.cc,$(COMMONSOURCES))
+COMMONOBJS     = $(COMMONOBJS_CXX:%.cxx=objs/$(TARGET_ARCH)/%.o) \
+                 $(COMMONOBJS_CC:%.cc=objs/$(TARGET_ARCH)/%.o)

@@ Build rules - add explicit rules for the two .pb.cc files @@

+objs/$(TARGET_ARCH)/ShapeType_Key.pb.o : $(SOURCE_DIR)ShapeType_Key.pb.cc \
+        $(SOURCE_DIR)ShapeType_Key.pb.h
+	$(COMPILER) $(COMPILER_FLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<
+
+objs/$(TARGET_ARCH)/descriptor.pb.o : $(SOURCE_DIR)omg/dds/descriptor.pb.cc \
+        $(SOURCE_DIR)omg/dds/descriptor.pb.h
+	$(COMPILER) $(COMPILER_FLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<
```

---

## Explanation of Each Change

### 1. Link `-lprotobuf`

```makefile
$(STATIC_LIBRARIES) -lprotobuf $(SYSLIBS)
```

The protobuf runtime library must be added to the link line. The protobuf-generated
`.pb.cc` files call into this library at run time (serialization, reflection, etc.).

---

### 2. Add protobuf-generated source files to `SOURCES`

```makefile
$(SOURCE_DIR)ShapeType_Key.pb.cc
$(SOURCE_DIR)omg/dds/descriptor.pb.cc
```

`rtiddsgen` generates these two files when run with `-protobuf`:

| File | Purpose |
|---|---|
| `ShapeType_Key.pb.cc` | Protobuf message implementation for the type |
| `omg/dds/descriptor.pb.cc` | OMG DDS protobuf file descriptor (shared across all types) |

Both must be compiled and linked into every application that uses the protobuf type.

---

### 3. Split `COMMONOBJS` to handle mixed `.cxx` / `.cc` extensions

```makefile
COMMONOBJS_CXX = $(filter %.cxx,$(COMMONSOURCES))
COMMONOBJS_CC  = $(filter %.cc,$(COMMONSOURCES))
COMMONOBJS     = $(COMMONOBJS_CXX:%.cxx=objs/$(TARGET_ARCH)/%.o) \
                 $(COMMONOBJS_CC:%.cc=objs/$(TARGET_ARCH)/%.o)
```

The original single-pattern substitution `$(COMMONSOURCES:%.cxx=objs/…/%.o)` only
strips `.cxx` suffixes. Protobuf-generated files use the `.cc` extension, so they
would be silently ignored. Splitting into two variables with `filter` ensures both
extensions are transformed to the correct `.o` paths.

---

### 4. Add explicit build rules for each `.pb.cc` file

```makefile
objs/$(TARGET_ARCH)/ShapeType_Key.pb.o : $(SOURCE_DIR)ShapeType_Key.pb.cc \
        $(SOURCE_DIR)ShapeType_Key.pb.h
	$(COMPILER) $(COMPILER_FLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<

objs/$(TARGET_ARCH)/descriptor.pb.o : $(SOURCE_DIR)omg/dds/descriptor.pb.cc \
        $(SOURCE_DIR)omg/dds/descriptor.pb.h
	$(COMPILER) $(COMPILER_FLAGS) -o $@ $(DEFINES) $(INCLUDES) -c $<
```

Make's generic pattern rule `%.o : %.cxx` does not match `.cc` files, so without
these explicit rules make would not know how to compile the protobuf sources.
A separate rule is required for each `.pb.cc` file.
