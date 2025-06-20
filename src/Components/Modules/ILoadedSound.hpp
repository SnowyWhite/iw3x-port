#pragma once

namespace Components
{
	class ILoadedSound : public AssetHandler::AssetInterface
	{
	public:
		ILoadedSound();
		~ILoadedSound();

		const char* GetName() override { return "ILoadedSound"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_LOADED_SOUND; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override {
			
			char* data = GetLoadedSoundData(header.loadSnd);

			Game::IW4::XAssetHeader iw4Header{};
			iw4Header.loadSnd = LocalAllocator.Allocate<Game::IW3::LoadedSound>();
			std::memcpy(iw4Header.loadSnd, header.loadSnd, sizeof(Game::IW3::LoadedSound));

			iw4Header.loadSnd->sound.data = data;

			return iw4Header;
		};
		static void DuplicateSoundData(Game::IW3::LoadedSound* loadedSound);

	private:
		static std::map<std::string, char*> availableSoundDatas;

		static char* GetLoadedSoundData(const Game::IW3::LoadedSound* loadedSound);
	};
}
