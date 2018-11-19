from conan.packager import ConanMultiPackager

if __name__ == "__main__":
    builder = ConanMultiPackager()
    # just add a single build config, use all default values
    builder.add()
    builder.run()
