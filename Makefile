# Attempt to load a config.make file.
# If none is found, project defaults in config.project.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

# make sure the the OF_ROOT location is defined
ifndef OF_ROOT
    OF_ROOT=$(realpath ../../../../../Downloads/of_v0.9.8_osx_release)
endif


#I DID THIS
#few diectories that might help
OF_CORE_LIBS="\"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoCrypto.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoData.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoDataSQLite.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoJSON.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoUtil.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoXML.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoNet.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoNetSSL.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoZip.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/lib/osx/PocoFoundation.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/tess2/lib/osx/tess2.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/glew/lib/osx/glew.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/cairo/lib/osx/cairo-script-interpreter.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/cairo/lib/osx/cairo.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/cairo/lib/osx/pixman-1.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/fmodex/lib/osx/libfmodex.dylib\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/rtAudio/lib/osx/rtAudio.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/openssl/lib/osx/crypto.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/openssl/lib/osx/ssl.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/glfw/lib/osx/glfw3.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/FreeImage/lib/osx/freeimage.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/freetype/lib/osx/freetype.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/boost/lib/osx/boost_filesystem.a\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/boost/lib/osx/boost_system.a\""
OF_CORE_FRAMEWORKS="-framework Accelerate -framework AGL -framework AppKit -framework ApplicationServices -framework AudioToolbox -framework AVFoundation -framework Cocoa -framework CoreAudio -framework CoreFoundation -framework CoreMedia -framework CoreServices -framework CoreVideo -framework IOKit -framework OpenGL -framework QuartzCore -framework QuickTime -framework QTKit -framework GLUT"
FRAMEWORK_SEARCH_PATHS +="/Users/jeronimo/Documents/dev/insertions/osx/webInsertion/bin  \"/Users/jeronimo/Documents/dev/insertions/osx/webInsertion/../../../../../Downloads/of_v0.9.8_osx_release/libs/glut/lib/osx\" ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxSyphon/libs/Syphon/lib/osx"
HEADER_SEARCH_PATHS += "/Users/jeronimo/Documents/dev/insertions/osx/webInsertion/bin/include \"../../../../../Downloads/of_v0.9.8_osx_release/libs/openFrameworks/**\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/poco/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/freetype/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/freetype/include/freetype2\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/fmodex/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/glew/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/FreeImage/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/tess2/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/cairo/include/cairo\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/rtAudio/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/glfw/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/boost/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/utf8cpp/include\" \"../../../../../Downloads/of_v0.9.8_osx_release/libs/openssl/include\" ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxAvFoundationHLSPlayer/src ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxGui/src ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxSyphon/libs ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxSyphon/libs/Syphon ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxSyphon/libs/Syphon/lib ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxSyphon/libs/Syphon/lib/osx ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxSyphon/libs/Syphon/src ../../../../../Downloads/of_v0.9.8_osx_release/addons/ofxSyphon/src"
LDFLAGS = -I/$(HEADER_SEARCH_PATHS) -I/$(OF_CORE_LIBS) $(OF_CORE_FRAMEWORKS) -F/$(FRAMEWORK_SEARCH_PATHS) -framework Syphon -arch i386

#sets flags for the compiler
#LDFLAGS = -I/$(HEADER_SEARCH_PATHS) -F$(OTHER_LDFLAGS)
#LDFLAGS = -I/$(HEADER_SEARCH_PATHS) -F/$(FRAMEWORK_SEARCH_PATHS) -framework Accelerate -framework AGL -framework AppKit -framework ApplicationServices -framework AudioToolbox -framework AVFoundation -framework Cocoa -framework CoreAudio -framework CoreFoundation -framework CoreMedia -framework CoreServices -framework CoreVideo -framework IOKit -framework OpenGL -framework QuartzCore -framework QuickTime -framework QTKit -framework GLUT -framework Syphon -arch i386

#LDFLAGS += -arch i386

# call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk
