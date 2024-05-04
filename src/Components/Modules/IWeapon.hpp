#pragma once

namespace Components
{
	class IWeapon : public AssetHandler::AssetInterface
	{
	public:
		IWeapon();
		~IWeapon();

		const char* GetName() override { return "IWeapon"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_WEAPON; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.weapon) }; };

	private:
		static Game::IW4::WeaponCompleteDef* Convert(Game::IW3::WeaponDef* comWorld);
		static void ReTagSilencedFlash(Game::IW4::WeaponCompleteDef* iw4WeaponCompleteDef, unsigned short newString);
		static Game::IW4::TracerDef* GenerateTracerDef(Game::IW3::WeaponDef* weapon);
		static void DifferentiateWeapon(Game::IW4::WeaponCompleteDef* weapon);
		static void DifferentiateModel(Game::IW4::XModel* model);
		static std::string GetCacCategoryForWeapon(const Game::IW4::WeaponCompleteDef* weapon);
		static std::string GetPerkForWeapon(const Game::IW4::WeaponCompleteDef* weapon);
		static std::string GetAttributesLine(const Game::IW4::WeaponCompleteDef* weapon);
		static std::string GetStatsLine(const std::string& originalName, const Game::IW4::WeaponCompleteDef* weapon);
		static size_t GetMobilityForClass(Game::IW4::weapClass_t clss);
		static std::array<int, 5> GetStatsForWeapon(const Game::IW4::WeaponCompleteDef* weapon);
		static void DumpAllWeapons();
	};
}
