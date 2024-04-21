#pragma once

namespace Components
{
	class TeamDumper : public Component
	{
	public:

		TeamDumper();

		const char* GetName() override { return "TeamDumper"; };

	private:
		static void DumpTeams();
		static void DumpMpTypes(std::ofstream& zoneSource, std::vector<std::string> mpTypes);
	};
}

