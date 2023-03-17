// fakerustc.cpp : Defines the entry point for the application.
// Fake rustc wrapper to trick Cargo into building C++ Code

#include "fakerustc.h"

std::string fake_ver_str =
"rustc 1.68.0 (71bdb4ba4 2023-03-16) (fake)\n"
"binary: rustc\n"
"commit-hash: 71bdb4ba408a52016151479ce8962f110519e839\n"
"commit-date: 2023-03-16\n"
"host: x86_64-unknown-linux-gnu\n"
"release: 1.68.0\n"
"LLVM version: 15.0.6\n"
"fakerustc version: 1.0.0 (71bdb4ba4 2023-03-16)\n";

std::string fake_cfg_str =
"debug_assertions"
"panic=\"unwind\""
"target_arch=\"x86_64\""
"target_endian=\"little\""
"target_env=\"gnu\""
"target_family=\"unix\""
"target_feature=\"fxsr\""
"target_feature=\"sse\""
"target_feature=\"sse2\""
"target_has_atomic=\"16\""
"target_has_atomic=\"32\""
"target_has_atomic=\"64\""
"target_has_atomic=\"8\""
"target_has_atomic=\"ptr\""
"target_os=\"linux\""
"target_pointer_width=\"64\""
"target_vendor=\"unknown\""
"unix";

int main(int argc, char** argv)
{
	// If --crate-name is ___, pass everything directly to rustc as its a test crate used by cargo to verify rustc is working
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--crate-name") == 0)
		{
			if (strcmp(argv[i + 1], "___") == 0)
			{
				// pass all environment variables to rustc
				std::string env_str = "";
				char* env_var = std::getenv("RUSTC_WRAPPER");
				if (env_var != NULL)
				{
					env_str += "RUSTC_WRAPPER=";
					env_str += env_var;
					env_str += " ";
				}
				env_var = std::getenv("RUSTC");
				if (env_var != NULL)
				{
					env_str += "RUSTC=";
					env_str += env_var;
					env_str += " ";
				}
				env_var = std::getenv("RUSTDOC");
				if (env_var != NULL)
				{
					env_str += "RUSTDOC=";
					env_str += env_var;
					env_str += " ";
				}
				env_var = std::getenv("RUSTC_BOOTSTRAP");
				if (env_var != NULL)
				{
					env_str += "RUSTC_BOOTSTRAP=";
					env_str += env_var;
					env_str += " ";
				}
				env_var = std::getenv("RUSTC_WORKSPACE_WRAPPER");
				if (env_var != NULL)
				{
					env_str += "RUSTC_WORKSPACE_WRAPPER=";
					env_str += env_var;
					env_str += " ";
				}
				env_var = std::getenv("RUSTC_WORKSPACE_WRAPPER");
				if (env_var != NULL)
				{
					env_str += "RUSTC_WORKSPACE_WRAPPER=";
					env_str += env_var;
					env_str += " ";
				}
				env_var = std::getenv("RUSTC_WORKSPACE_WRAPPER");
				if (env_var != NULL)
				{
					env_str += "RUSTC_WORKSPACE_WRAPPER=";
					env_str += env_var;
					env_str += " ";
				}
				env_var = std::getenv("RUSTC_WORKSPACE_WRAPPER");
				if (env_var != NULL)
				{
					env_str += "RUSTC_WORKSPACE_WRAPPER=";
					env_str += env_var;
					env_str += " ";
				}
				// pass all this to rustc
				std::string rustc_str = " rustc ";
				for (int i = 1; i < argc; i++)
				{
					rustc_str += argv[i];
					rustc_str += " ";
				}
				env_str += rustc_str;
				system(env_str.c_str());
				return 0;
			}
		}
	}


	// intercept -vV and --version to print the fake version
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-vV") == 0 || strcmp(argv[i], "--version") == 0)
		{
			std::cout << fake_ver_str << std::endl;
			return 0;
		}
	}

	std::string crate_name;
	// search for passed --crate-name
	for (int j = 1; j < argc; j++)
	{
		if (strcmp(argv[j], "--crate-name") == 0)
		{
			crate_name = argv[j + 1];
			break;
		}
	}

	// get crate type, for now only support bin
	std::string crate_type;
	for (int j = 1; j < argc; j++)
	{
		if (strcmp(argv[j], "--crate-type") == 0)
		{
			crate_type = argv[j + 1];
			break;
		}
	}
	if (strcmp(crate_type.c_str(), "bin") != 0)
	{
		printf("error: only bin crate type is supported");
		return 1;
	}

	// intercept --print=file-names, --print=sysroot, --print=cfg, --print=crate-name, --print=split-debuginfo
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--print=file-names") == 0)
		{
			// search for file name which will start with src/... in simple cargo projects
			// print it out and strip the src/ and the .rs part
			for (int i = 1; i < argc; i++)
			{
				if (strncmp(argv[i], "src/", 4) == 0)
				{
					std::string file_name = argv[i];
					file_name = file_name.substr(4, file_name.length() - 3);
					std::cout << file_name << std::endl;
				}
			}

		}
		else if (strcmp(argv[i], "--print=sysroot") == 0)
		{
			std::cout << "/usr/local/sbin/.rustup/toolchains/stable-x86_64-unknown-linux-gnu" << std::endl;
		}
		else if (strcmp(argv[i], "--print=cfg") == 0)
		{
			std::cout << fake_cfg_str << std::endl;
		}
		else if (strcmp(argv[i], "--print=crate-name") == 0)
		{
			std::cout << crate_name << std::endl;
		}
		else if (strcmp(argv[i], "--print=split-debuginfo") == 0)
		{
			std::cout << "off\npacked\nunpacked" << std::endl;
		}
	}

	// get --out-dir for the output
	std::string out_dir = "";
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--out-dir") == 0)
		{
			out_dir = argv[i + 1];
		}
	}
	// if out_dir ends with /deps, remove it
	if (out_dir.length() > 5 && out_dir.substr(out_dir.length() - 5, 5) == "/deps")
	{
		out_dir = out_dir.substr(0, out_dir.length() - 5);
	}

	// build the fake output file using g++, outputting to the --out-dir with file-name
	std::string cmd = "g++ -x c++ src/main.rs -o " + out_dir + "/" + crate_name;
	int gpp_ret;
	gpp_ret = system(cmd.c_str());
	return gpp_ret;
}
