Import("env")

# Generate map file
env.Append(LINKFLAGS=[
    "-Wl,-Map=$BUILD_DIR/${PROGNAME}.map"
])