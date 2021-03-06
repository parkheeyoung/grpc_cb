-- premake5.lua
--[[
Usage examples: 
	for windows: premake5.exe --os=windows vs2015
	fot linux:   premake5.exe --os=linux gmake
]]

grpc_root = "../third_party/grpc"
protobuf_root = grpc_root .. "/third_party/protobuf"

-- Todo: support non-vs2015, x64
grpc_vs_packages = grpc_root .. "/vsprojects/packages"
platform_dir = "v140/Win32"  -- $(PlatformToolset)\$(Platform)
zlib_libdir = grpc_vs_packages .. "/grpc.dependencies.zlib.1.2.8.10/build/native/lib/" .. platform_dir .. "/%{cfg.buildcfg}/static/cdecl"
openssl_libdir = grpc_vs_packages .. "/grpc.dependencies.openssl.1.0.204.1/build/native/lib/" .. platform_dir .. "/%{cfg.buildcfg}/static"

workspace "grpc_cb"
	configurations { "Debug", "Release" }
	language "C++"
	flags {
		"C++11",
		"StaticRuntime",
	}
	includedirs {
		"../include",
		grpc_root .. "/include",
		protobuf_root .. "/src",
	}
	libdirs {
		grpc_root .. "/vsprojects/%{cfg.buildcfg}",
		protobuf_root .. "/cmake/%{cfg.buildcfg}",
		zlib_libdir,
		openssl_libdir,
	}
	
	filter "configurations:Debug"
		flags { "Symbols" }
		links {
			"libprotobufd",
		}
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		links {
			"libprotobuf",
		}
	filter {}

	if os.is("windows") then
		defines {
			"_WIN32_WINNT=0x0600"  -- i.e. Windows 7 target
		}
		links {
			"ws2_32",
		}
	end

project "grpc_cpp_cb_plugin"
	kind "ConsoleApp"
	files {
		"../src/compiler/**",
	}
	includedirs {
		"..",
		grpc_root,
	}
	libdirs {
		grpc_root .. "/vsprojects/%{cfg.buildcfg}",  -- DEL
		protobuf_root .. "/cmake/%{cfg.buildcfg}",  -- DEL
	}
	links {
		"grpc_plugin_support",
	}

	filter "configurations:Debug"
		links {
			"libprotocd",
		}
	filter "configurations:Release"
		links {
			"libprotoc",
		}
	filter {}

project "grpc_cb"
	kind "StaticLib"
	includedirs {
		"../src/cpp_cb",
	}
	files {
		"../include/grpc_cb/**.h",
		"../src/cpp_cb/**",
	}

group "examples"

	examples_dep_libs = {
		"grpc_cb",
		"grpc",
		"gpr",
		"zlib",
		"ssleay32",
		"libeay32",
	}

	project "greeter_cb_client"
		kind "ConsoleApp"
		files {
			"../examples/cpp_cb/helloworld/**",
		}
		removefiles {
			"../examples/cpp_cb/helloworld/greeter_cb_server.cc",
		}
		links(examples_dep_libs)

	project "greeter_cb_server"
		kind "ConsoleApp"
		files {
			"../examples/cpp_cb/helloworld/**",
		}
		removefiles {
			"../examples/cpp_cb/helloworld/greeter_cb_client.cc",
		}
		links(examples_dep_libs)

	project "route_guide_cb_client"
		kind "ConsoleApp"
		files {
			"../examples/cpp_cb/route_guide/**",
		}
		removefiles {
			"../examples/cpp_cb/route_guide/route_guide_cb_server.cc",
		}
		links(examples_dep_libs)

	project "route_guide_cb_server"
		kind "ConsoleApp"
		files {
			"../examples/cpp_cb/route_guide/**",
		}
		removefiles {
			"../examples/cpp_cb/route_guide/route_guide_cb_client.cc",
		}
		links(examples_dep_libs)

group ""  -- End of group "examples"
