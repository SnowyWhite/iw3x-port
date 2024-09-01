#include "STDInclude.hpp"

namespace Components
{
	static std::string currentMap{};

		struct mpTypes
		{
			std::string sniper;
			std::string support;
			std::string assault;
			std::string recon;
			std::string specops;
			std::string riot;
		};

		struct teamDef
		{
			std::string originalName;
			std::string iw4name;
			std::string iw4fallback;
			std::string localizedName;
			std::string localizedShortName;
			std::string eliminatedName;
			std::string forfeitedName;
			std::string factionImage;
			std::string factionImageFade; // Duplicate normal image
			std::string voicePrefix;	  //
			std::string spawnTheme;
			std::string victoryTheme;
			std::string flagModel;
			std::string flagCarryModel;
			std::string objPointFlagMaterial;
			std::string flagFX;
			Game::vec3_t color;
			std::string headIcon;	// Absent from IW3
			std::string plasticCaseModel; // Absent from IW3
			
			std::string takeFromMap;
			mpTypes mptypes;
		};


	void TeamDumper::DumpMpTypes(std::ofstream& zoneSource, std::vector<std::string> mpTypes)
	{
		for (const std::string& mpType : mpTypes)
		{
			const auto& path = std::format("mptype/{}.gsc", mpType);

			Game::IW3::XAssetHeader mpTypeFile = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_RAWFILE, path.data());
			if (!mpTypeFile.data)
			{
				continue;
			}

			Components::Logger::Print("Parsing mptype %s\n", mpType.data());
			zoneSource << "rawfile," << path << "\n";
			Command::Execute(std::format("dumpRawFile {}", path), true);


			std::vector<std::string> characterFiles{};

			// Parse mptype
			{
				std::string contents = mpTypeFile.rawfile->buffer;
				static std::regex subscriptCatcher("((?:character|xmodelalias)\\\\(?:.+))::");
				{
					std::smatch matches;
					std::string::const_iterator searchStart(contents.cbegin());

					while (std::regex_search(searchStart, contents.cend(), matches, subscriptCatcher))
					{
						if (matches.size() > 1)
						{
							const auto& match = matches[1];
							auto scriptName = std::format("{}.gsc", match.str());
							std::replace(scriptName.begin(), scriptName.end(), '\\', '/');

							Game::IW3::XAssetHeader characterFile = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_RAWFILE, scriptName.data());

							if (characterFile.data)
							{
								characterFiles.push_back(characterFile.rawfile->buffer);

								Command::Execute(std::format("dumpRawFile {}", scriptName), true);
								zoneSource << "rawfile," << scriptName << "\n";
								Components::Logger::Print("Preparing to dump character %s\n", scriptName.data());
							}
						}

						searchStart = matches.suffix().first;
					}
				}
			}

			// Parse character files
			{
				for (const auto& contents : characterFiles)
				{
					static std::regex modelCatcher("precache.odel\\( ?\"(.*)\" ?\\);");
					{
						std::smatch matches;
						std::string::const_iterator searchStart(contents.cbegin());

						while (std::regex_search(searchStart, contents.cend(), matches, modelCatcher))
						{
							if (matches.size() > 1)
							{
								const auto& match = matches[1];
								auto modelName = match.str();
								zoneSource << "xmodel," << modelName << "\n";
								Command::Execute(std::format("dumpXModel {}", modelName), true);
							}

							searchStart = matches.suffix().first;
						}
					}
				}
			}
		}

		Components::Logger::Print("Done with mptypes\n");
	}

	TeamDumper::TeamDumper()
	{
		Command::Add("dumpTeams", [](const Command::Params&)
			{
				MapDumper::GetApi()->set_work_path(AssetHandler::GetExportPath());
				DumpTeams();
				Logger::Print("Teams successfully exported.\n");
			});
	}

	void TeamDumper::DumpTeams()
	{
		const teamDef finalTeams[] = {
			// ORIGINAL NAME			NAME			FALLBACK			LOC NAME			LOC SHORT				ELIMINATED					FORFEITED				MAT					MAT_FADE			VOICE	SPAWN_THEME		VICTORY_THEME		FLAG						FLAG_CARRY				OBJPOINT						FLAG_FX					COLOR					HEADICON			PLASTIC_CASE				// MAP			// MPTYPES
			{ "marines",	 "iw3_usmc_desert",			"us_army",			"MPUI_MARINES",		"MPUI_MARINES_SHORT",	"MP_MARINES_ELIMINATED",	"MP_MARINES_FORFEITED",	"faction_128_usmc", "faction_128_usmc_fade",	"US_",	"mp_spawn_usa",	"mp_victory_usa", "prop_flag_american",			"prop_flag_neutral",	"objpoint_flag_american",		"ui_flagbase_black",	{0.0f, 0.0f,  0.0f},	"headicon_american",	"com_plasticcase_beige_big", "mp_bog",		{"mptype_ally_sniper",				"mptype_ally_support",			"mptype_ally_rifleman",			"mptype_ally_engineer",				"mptype_ally_cqb",			"mptype_us_army_riot" }},
			{ "opfor",		 "iw3_opfor_desert",		"opforce_composite","MPUI_OPFOR",		"MPUI_OPFOR_SHORT",		"MP_OPFOR_ELIMINATED",		"MP_OPFOR_FORFEITED",		"faction_128_arab",	"faction_128_arab_fade",	"AB_",	"mp_spawn_opfor",	"prop_flag_opfor", "prop_flag_neutral",	"prop_flag_neutral",	"objpoint_flag_opfor",			"ui_flagbase_black", {0.6f,  0.57f, 0.41f},	"headicon_opfor",	"com_plasticcase_beige_big", "mp_bog",		{"mptype_axis_sniper",		"mptype_axis_support",	 "mptype_axis_rifleman",	"mptype_axis_engineer",		"mptype_axis_cqb",	"mptype_opforce_comp_riot" }},
			{ "sas",		 "iw3_sas_urban",			"seals_udt",	"MPUI_SAS",			"MPUI_SAS_SHORT",		"MP_SAS_ELIMINATED",		"MP_SAS_FORFEITED",		"faction_128_sas",	"faction_128_sas_fade",	"UK_",	"mp_spawn_sas",	"mp_victory_sas", "prop_flag_brit",			"prop_flag_neutral",	"objpoint_flag_british",		"ui_flagbase_black",	{0.6f,  0.64f, 0.69f},	"headicon_british",	"com_plasticcase_beige_big", "mp_carentan",		{"mptype_ally_urban_sniper",		"mptype_ally_urban_support",	"mptype_ally_urban_assault",	"mptype_ally_urban_recon",			"mptype_ally_urban_specops",	"mptype_seal_udt_riot" }},
			{ "russian",	 "iw3_spetsnaz_urban",		"opforce_airborne",	"MPUI_SPETSNAZ",	"MPUI_SPETSNAZ_SHORT",	"MP_SPETSNAZ_ELIMINATED",	"MP_SPETSNAZ_FORFEITED",	"faction_128_ussr",	"faction_128_ussr_fade",	"RU_",	"mp_spawn_soviet",	"prop_flag_russian", "prop_flag_neutral",	"prop_flag_neutral",	"objpoint_flag_russian",		"ui_flagbase_black", {0.58f,  0.28f, 0.28f},"headicon_russian",	"com_plasticcase_beige_big", "mp_carentan",		{"mptype_axis_urban_sniper",		"mptype_axis_urban_support",	 "mptype_axis_urban_assault",	"mptype_axis_urban_engineer",		"mptype_axis_urban_cqb",	"mptype_opforce_airborne_riot" }},
			{ "sas",		 "iw3_sas_woodland",		"socom_141_forest",	"MPUI_SAS",			"MPUI_SAS_SHORT",		"MP_SAS_ELIMINATED",		"MP_SAS_FORFEITED",		"faction_128_sas",	"faction_128_sas_fade",	"UK_",	"mp_spawn_sas",	"mp_victory_sas", "prop_flag_brit",			"prop_flag_neutral",	"objpoint_flag_british",		"ui_flagbase_black",	{0.6f,  0.64f, 0.69f},	"headicon_british",	"com_plasticcase_beige_big", "mp_pipeline",	{"mptype_ally_woodland_sniper",	"mptype_ally_woodland_support",	"mptype_ally_woodland_assault",	"mptype_ally_woodland_recon",		"mptype_ally_woodland_specops",	"mptype_tf141_forest_riot" }},
			{ "russian",	 "iw3_spetsnaz_woodland",	"opforce_airborne",	"MPUI_SPETSNAZ",	"MPUI_SPETSNAZ_SHORT",	"MP_SPETSNAZ_ELIMINATED",	"MP_SPETSNAZ_FORFEITED",	"faction_128_ussr",	"faction_128_ussr_fade",	"RU_",	"mp_spawn_soviet",	"prop_flag_russian", "prop_flag_neutral",	"prop_flag_neutral",	"objpoint_flag_russian",		"ui_flagbase_black", {0.58f,  0.28f, 0.28f},"headicon_russian",	"com_plasticcase_beige_big", "mp_pipeline",	{"mptype_axis_woodland_sniper",		"mptype_axis_woodland_support",	 "mptype_axis_woodland_rifleman",	"mptype_axis_woodland_engineer",		"mptype_axis_woodland_cqb",	"mptype_opforce_airborne_riot" }}
		};

		const auto& basePath = AssetHandler::GetExportPath();

		for (const auto& team : finalTeams)
		{
			const std::string& exportPath = std::format("{}/{}", basePath, team.iw4name);
			Utils::CreateDir(exportPath);

			AssetHandler::SetExportPath(exportPath.data());
			MapDumper::GetApi()->set_work_path(exportPath);
			MapDumper::GetApi()->clear_writes();

			std::string mapToDump = team.takeFromMap;

			if (currentMap != mapToDump)
			{
				currentMap = mapToDump;
				Logger::Print("Loading map '%s'...\n", mapToDump.data());
				Command::Execute(Utils::VA("loadzone %s", mapToDump.data()), true);
			}

			std::string lowerPrefix = team.voicePrefix;
			for (auto& c : lowerPrefix)
			{
				c = static_cast<char>(tolower(c));
			}

			const auto newPrefix = std::format("IW3_{}", team.voicePrefix);
			const auto newLowPrefix = std::format("iw3_{}", lowerPrefix);

			const auto& newVictoryTheme = std::format("IW3_{}", team.victoryTheme);
			const auto& newSpawnTheme = std::format("IW3_{}", team.spawnTheme);

			{
				std::ofstream zoneSource(std::format("{}/{}.csv", exportPath, team.iw4name));
				zoneSource << "# IW3xport - generated by Louve@Xlabs\n\n";

				DumpMpTypes(zoneSource, { team.mptypes.assault, team.mptypes.sniper, team.mptypes.support, team.mptypes.recon, team.mptypes.specops });

				zoneSource << "material," << team.factionImage << "\n";
				Command::Execute(Utils::VA("dumpMaterial %s", team.factionImage.data()), true);

				zoneSource << "material," << team.factionImageFade << "\n";
				// Copy the same image - IW3 doesnt have fade
				// Need to rename it inside too...
				{
					std::ifstream readFile(std::format("{}/materials/{}.iw4x.json", exportPath, team.factionImage));
					std::ofstream outFile(std::format("{}/materials/{}.iw4x.json", exportPath, team.factionImageFade));
					std::string readout;
					std::string search = std::format("    \"name\": \"{}\",", team.factionImage);
					std::string replace = std::format("    \"name\": \"{}\",", team.factionImageFade);
					while (getline(readFile, readout)) {
						if (readout == search) {
							outFile << replace << "\n";
						}
						else {
							outFile << readout << "\n";
						}
					}
				}

				// dump all voice sounds using prefix (easier than parsing GSC)
				{
					std::vector<std::string> soundsToDump{};

					Game::DB_EnumXAssetEntries(Game::IW3::XAssetType::ASSET_TYPE_SOUND, [&team, &soundsToDump, &lowerPrefix](Game::IW3::XAssetEntryPoolEntry* poolEntry) {
						if (poolEntry)
						{
							auto header = poolEntry->entry.asset.header;
							if (header.data && header.sound->count > 0 && header.sound->count < 32)
							{
								std::string name = header.sound->aliasName;

								if (name.starts_with(team.voicePrefix) || name.starts_with(lowerPrefix))
								{
									soundsToDump.push_back(name);
								}
							}
						}
						}, false);


					for (const auto& soundName : soundsToDump)
					{
						const auto newPrefixedName = std::format("{}{}", newPrefix, soundName.substr(3));
						zoneSource << "sound," << newPrefixedName << "\n";
						Command::Execute(Utils::VA("dumpSound %s", soundName.data()), true);

						// Rename it once dumped 
						const std::string outputPath = std::format("{}/sounds/{}.json", exportPath, soundName);
						const std::string newPath = std::format("{}/sounds/{}.json", exportPath, newPrefixedName);
						if (Utils::FileExists(outputPath))
						{
							if (Utils::FileExists(newPath))
							{
								std::filesystem::remove(newPath);
							}

							std::filesystem::rename(outputPath, newPath);

							// Update alias name in file
							auto contents = Utils::ReadFile(newPath);

							std::regex aliasNameCatcher(std::format("(aliasName\": \")({})(\")", soundName));
							contents = std::regex_replace(contents, aliasNameCatcher, std::format("$1{}$3", newPrefixedName));

							Utils::WriteFile(newPath, contents);
						}
					}
				}

				zoneSource << "sound," << newSpawnTheme << "\n";
				Command::Execute(Utils::VA("dumpSound %s", team.spawnTheme.data()), true);
				{
					// Rename it once dumped 
					const std::string outputPath = std::format("{}/sounds/{}.json", exportPath, team.spawnTheme);
					const std::string newPath = std::format("{}/sounds/{}.json", exportPath, newSpawnTheme);
					if (Utils::FileExists(outputPath))
					{
						if (Utils::FileExists(newPath))
						{
							std::filesystem::remove(newPath);
						}

						std::filesystem::rename(outputPath, newPath);

						const auto& iw4ExpectedPath = std::format("{}/sounds/{}{}.json", exportPath, newLowPrefix, "spawn_music");
						std::filesystem::remove(iw4ExpectedPath);
						std::filesystem::copy(newPath, iw4ExpectedPath);

						// Update alias name in file
						auto contents = Utils::ReadFile(newPath);

						std::regex aliasNameCatcher(std::format("(aliasName\": \")({})(\")", team.spawnTheme));
						contents = std::regex_replace(contents, aliasNameCatcher, std::format("$1{}$3", newSpawnTheme));

						Utils::WriteFile(newPath, contents);
					}
				}

				zoneSource << "sound," << newVictoryTheme << "\n";
				Command::Execute(Utils::VA("dumpSound %s", team.victoryTheme.data()), true);
				{
					// Rename it once dumped 
					const std::string outputPath = std::format("{}/sounds/{}.json", exportPath, team.victoryTheme);
					const std::string newPath = std::format("{}/sounds/{}.json", exportPath, newVictoryTheme);
					if (Utils::FileExists(outputPath))
					{
						if (Utils::FileExists(newPath))
						{
							std::filesystem::remove(newPath);
						}

						std::filesystem::rename(outputPath, newPath);
						const auto& iw4ExpectedPath = std::format("{}/sounds/{}{}.json", exportPath, newLowPrefix, "victory_music");
						std::filesystem::remove(iw4ExpectedPath);
						std::filesystem::copy(newPath, iw4ExpectedPath);

						auto contents = Utils::ReadFile(newPath);

						std::regex aliasNameCatcher(std::format("(aliasName\": \")({})(\")", team.victoryTheme));
						contents = std::regex_replace(contents, aliasNameCatcher, std::format("$1{}$3", newVictoryTheme));

						Utils::WriteFile(newPath, contents);
					}
				}

				zoneSource << "xmodel," << team.flagCarryModel << "\n";
				Command::Execute(Utils::VA("dumpXModel %s", team.flagCarryModel.data()), true);

				zoneSource << "fx,misc/" << team.flagFX << "\n";
				Command::Execute(Utils::VA("dumpFxEffectDef misc/%s", team.flagFX.data()), true);

				zoneSource << "material," << team.objPointFlagMaterial << "\n";
				Command::Execute(Utils::VA("dumpMaterial %s", team.objPointFlagMaterial.data()), true);

				zoneSource << "material," << team.headIcon << "\n";
				Command::Execute(Utils::VA("dumpMaterial %s", team.headIcon.data()), true);

				zoneSource << "xmodel," << team.plasticCaseModel << "\n";
				Command::Execute(Utils::VA("dumpXModel %s", team.plasticCaseModel.data()), true);

				zoneSource << "xmodel," << team.flagModel << "\n";
				Command::Execute(Utils::VA("dumpXModel %s", team.flagModel.data()), true);

			}

			Utils::WriteFile(std::format("{}/iw4_voice_fallback.txt", exportPath), lowerPrefix);

			Utils::WriteFile(std::format("{}/iw4_team_fallback.txt", exportPath), team.iw4fallback);

			// GSC
			Utils::WriteFile(
				std::format("{}/_teams.gsc", exportPath),
				std::format("\n\
							case \"{0}\":\n\
								mptype\\{1}::precache();\n\
								mptype\\{2}::precache();\n\
								mptype\\{3}::precache();\n\
								mptype\\{4}::precache();\n\
								mptype\\{5}::precache();\n\
								mptype\\{6}::precache();\n\
\n\
								game[team + \"_model\"][\"SNIPER\"] = mptype\\{1}::main;\n\
								game[team + \"_model\"][\"LMG\"] = mptype\\{2}::main;\n\
								game[team + \"_model\"][\"ASSAULT\"] = mptype\\{3}::main;\n\
								game[team + \"_model\"][\"SHOTGUN\"] = mptype\\{4}::main;\n\
								game[team + \"_model\"][\"SMG\"] = mptype\\{5}::main;\n\
								game[team + \"_model\"][\"RIOT\"] = mptype\\{6}::main;\n\
\n\
								break; \n\
				", team.iw4name, team.mptypes.sniper, team.mptypes.support, team.mptypes.assault, team.mptypes.specops, team.mptypes.recon, team.mptypes.riot)
			);


			// STR
			{
				std::ofstream strFile(std::format("{}/{}.str", exportPath, team.iw4name));
				if (strFile.is_open())
				{
					strFile << "REFERENCE			" << team.localizedName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.localizedName.data(), "", 0) << "\n\n";
					strFile << "REFERENCE			" << team.localizedShortName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.localizedShortName.data(), "", 0) << "\n\n";
					strFile << "REFERENCE			" << team.eliminatedName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.eliminatedName.data(), "", 0) << "\n\n";
					strFile << "REFERENCE			" << team.forfeitedName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.forfeitedName.data(), "", 0) << "\n\n";
				}
			}


			// Factiontable
			Utils::WriteFile(
				std::format("{}/factionTable.csv", exportPath),
				std::format
				(
					"{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}",
					team.iw4name,team.localizedName,team.localizedShortName,team.eliminatedName,team.forfeitedName,team.factionImage,team.factionImageFade,newPrefix,newSpawnTheme,newVictoryTheme,team.flagModel,team.flagCarryModel,team.objPointFlagMaterial,team.flagFX,team.color[0],team.color[1],team.color[2],team.headIcon,team.plasticCaseModel
				)
			);
		}
	}
}