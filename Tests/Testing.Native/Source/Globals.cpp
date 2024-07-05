#include "Globals.hpp"

namespace Testing
{
	Coral::HostInstance Globals::HostInstance;
	Coral::ManagedAssembly* Globals::TestAssembly;
	Coral::AssemblyLoadContext Globals::AssemblyLoadContext;
	Coral::ManagedObject Globals::TestInstance;
	Coral::Type Globals::TestType;
	Coral::HostSettings Globals::HostSettings;
	std::filesystem::path Globals::AssemblyPath;
}