#include "stdafx.h"
#include "SpriteFontLoader.h"
#include <bitset>

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
		//See BMFont Documentation for Binary Layout
	// pReader->MoveBufferPosition(0);
		
	if (pReader->Read<char>() != 'B' || pReader->Read<char>() != 'M' || pReader->Read<char>() != 'F')
	{
		Logger::LogError(L"SprintFontLoader::LoadContent > not a valid .fnt font");
		return nullptr;
	}

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,

	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...

	//Parse the version (version 3 required)
	if (static_cast<int>(pReader->Read<BYTE>()) != 3)
	{
		Logger::LogError(L"SprintFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	 }
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	auto blockId = pReader->Read<BYTE>();
	auto blockSize = pReader->Read<INT32>();
	
	
	//Retrieve the FontSize [fontDesc.fontSize]
	auto fontSize = pReader->Read<short>();
	fontDesc.fontSize = fontSize;

	//was 14 but it is 2 step down
	pReader->MoveBufferPosition(12);


	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	auto fontName = pReader->ReadNullString();
	fontDesc.fontName = fontName;
	//Retrieve the FontName [fontDesc.fontName]
	//...

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	 blockId = pReader->Read<BYTE>();
	 blockSize = pReader->Read<INT32>();
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	 //MOVE TO POS 4
	 pReader->MoveBufferPosition(4);
	
	 //READ THE CONTNT
	auto scaleW = pReader->Read<UINT16>();
	auto scaleH = pReader->Read<UINT16>();
	fontDesc.textureWidth = scaleW;
	fontDesc.textureHeight = scaleH;
	//Retrieve PageCount
	
	if (static_cast<int>(pReader->Read<UINT16>()) > 1)
	{
		Logger::LogError(L"SprintFontLoader::Log Error (Only one texture per font is allowed!)");
		return nullptr;
	}
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	//Advance to Block2 (Move Reader)
	pReader->MoveBufferPosition(5);
	//...

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<BYTE>();
	blockSize = pReader->Read<INT32>();
	//Retrieve the PageName (BinaryReader::ReadNullString)
	auto pageName = pReader->ReadNullString();
	if (pageName.empty())
	{
		Logger::LogError(L"SprintFontLoader::Log Error Empty");
		return nullptr;
	}
	//Construct the full path to the page texture file
	std::wstring path = loadInfo.assetFullPath;
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	std::filesystem::path p;
	p /= loadInfo.assetFullPath.parent_path();
	p /= pageName;
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	fontDesc.pTexture = ContentManager::Load<TextureData>(p);
	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pReader->Read<BYTE>();
	blockSize = pReader->Read<INT32>();
	//Retrieve Character Count (see documentation)
	auto charSize = blockSize / 20;
   
	//Create loop for Character Count, and:
	for (int i = 0; i < charSize; ++i)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		auto id = static_cast<wchar_t>(pReader->Read<UINT32>());
	//> Create instance of FontMetric (struct)
		auto& metric = fontDesc.metrics;
	//	> Set Character (CharacterId) [FontMetric::character]
		metric[id].character = id;
	//	> Retrieve Xposition (store Local)
		const auto xPos = pReader->Read<unsigned short>();
	//	> Retrieve Yposition (store Local)
		const auto yPos = pReader->Read<unsigned short>();
	//	> Retrieve & Set Width [FontMetric::width]
		metric[id].width = pReader->Read<unsigned short>();
	//	> Retrieve & Set Height [FontMetric::height]
		metric[id].height = pReader->Read<unsigned short>();
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
		metric[id].offsetX = pReader->Read<short>();
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
		metric[id].offsetY = pReader->Read<short>();
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
		metric[id].advanceX = pReader->Read<short>();
	//	> Retrieve & Set Page [FontMetric::page]
		metric[id].page = pReader->Read<BYTE>();
	//	> Retrieve Channel (BITFIELD!!!) 
		std::bitset<4> channel = pReader->Read<unsigned char>();
		metric[id].channel = channel[2] * 0 + channel[1] * 1 + channel[0] * 2 + channel[3] * 3;
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
		metric[id].texCoord = DirectX::XMFLOAT2(xPos /static_cast<float>(fontDesc.textureWidth), yPos / static_cast<float>(fontDesc.textureHeight));
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
		fontDesc.metrics = metric;
		
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)

	}
	

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
